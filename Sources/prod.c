#include "../Headers/prod.h"

int try_shm_palloc(int shm_id, int mem_size, int proc_id, int p_amount){

    int cell_amount = mem_size/MEMSPACE_SIZE;
    int free_cells = get_free_cell_amount(shm_id, cell_amount);

    if (free_cells < p_amount)
        return -1;
    else
        set_free_cell_amount(shm_id, cell_amount, free_cells - p_amount);

    int current_page = 1;

    for (int i = 1; i <= cell_amount; i++){

        if (current_page <= p_amount) {

            MCell * cell = read_shm_cell(shm_id, i);
            if (cell->held_proc_num == -1) {

                write_to_shm(shm_id, i, proc_id, current_page);
                write_to_log(ALLOCATION, 1, proc_id, i, current_page, 0);
                current_page += 1;
            }
            free(cell);
        }
        else
            break;
    }

    return 1;
}

int try_shm_salloc(int shm_id, int mem_size, int proc_id, int s_amount, int parts_per_seg){

    int cell_amount = mem_size/MEMSPACE_SIZE;
    int needed_cells = s_amount*parts_per_seg;
    int free_cells = get_free_cell_amount(shm_id, cell_amount);

    if (free_cells < needed_cells)
        return -1;
    else
        set_free_cell_amount(shm_id, cell_amount, free_cells - needed_cells);

    int current_segment = 1;
    int current_seg_part = 1;

    for (int i = 1; i <= cell_amount; i++){

        if (current_segment <= s_amount) {

            MCell * cell = read_shm_cell(shm_id, i);
            if (cell->held_proc_num == -1) {

                write_to_shm(shm_id, i, proc_id, current_segment);
                write_to_log(ALLOCATION, 0, proc_id, i, current_segment, current_seg_part);
                if (current_seg_part == parts_per_seg) {
                    current_segment += 1;
                    current_seg_part = 1;
                } else
                    current_seg_part += 1;
            }
            free(cell);
        }
        else
            break;
    }

    return 1;
}

int try_shm_dealloc(int shm_id, int mem_size, int proc_id, int ps_amount){

    int cell_amount = mem_size/MEMSPACE_SIZE;
    int free_cells = get_free_cell_amount(shm_id, cell_amount);

    /* Implica un error en la cantidad de espacios a liberar */
    if (free_cells + ps_amount > cell_amount)
        return -1;
    else
        set_free_cell_amount(shm_id, cell_amount, free_cells + ps_amount);

    int current_ps = 1;

    for (int i = 1; i <= cell_amount; i++){

        if (current_ps <= ps_amount) {

            MCell * cell = read_shm_cell(shm_id, i);
            if (cell->held_proc_num == proc_id) {

                write_to_shm(shm_id, i, -1, -1);
                write_to_log(DEALLOCATION, 0, proc_id, i, 0, 0);
                current_ps += 1;
            }
            free(cell);
        }
    }

    return 1;
}

pthread_mutex_t bp_mutex = PTHREAD_MUTEX_INITIALIZER;

void * run_proc(t_args * args){

    /* Ejecucion de un proceso!
    1. Pedir semáforo de memoria
    2. Buscar su ubicación
    3. Escribir en Bitácora
    4. Devolver semáforo de memoria
    5. Sleep
    6. Pedir Semáforo de memoria
    7. Liberar memoria
    8. Escribir en Bitácora
    9. Devolver semáforo de memoria
    */

    pthread_t id = pthread_self();
    pthread_detach(id);

    /* Variables de memoria compartida */
    key_t shm_key;
    int shm_id; /* Shared memory ID */
    int mem_size = read_file_int(MEMSIZE_FILENAME);

    /* Obtener llave del archivo */
    if ((shm_key = ftok(KEY_FILENAME, 'R')) == -1)
    {
        perror("Error de generacion de la clave. \n");
        exit(-1);
    }

    /* Obtener el ID de la memoria compartida */
    if ((shm_id = shmget(shm_key, mem_size, 0644)) == -1) {
        perror("No se encontro la referencia a la memoria compartida. \n");
        exit(-1);
    }

    /* Variable semaforo */
    sem_t * memory_sem = sem_open(SHM_SEM_NAME, 0);

    if(memory_sem == (void*) -1){
        perror("Error de referencia al semaforo.\n");
        exit(-1);
    }

    int alloc_success = 0;

    /* Agregar a la lista de procesos bloqueados de ser posible */
    int available_bp = -1;

    while (available_bp == -1) {
        pthread_mutex_lock(&bp_mutex);
        available_bp = add_to_bp_list(args->bp_id, args->p_id);
        pthread_mutex_unlock(&bp_mutex);
    }

    /* Punto 1 */
    if(!sem_wait(memory_sem)) {

        getchar();
        /* Borrar de la lista de procesos bloqueados */
        while (available_bp == 1) {
            pthread_mutex_lock(&bp_mutex);
            del_from_bp_list(args->bp_id, args->p_id);
            pthread_mutex_unlock(&bp_mutex);
            available_bp = -1;
        }

        /* Colocar como actual proceso en busqueda de memoria */
        set_searching_pid(shm_id, mem_size / MEMSPACE_SIZE, args->p_id);

        if(args->p_id == 1){
            sleep(300);
        }

        /* Punto 2 */
        if (args->is_paging){
            /* Punto 3 dentro de try_shm_palloc */
            alloc_success = try_shm_palloc(shm_id, mem_size, args->p_id, args->ps_amount);
            if (alloc_success == -1) {
                /* Punto 3 en caso de memoria llena */
                write_to_log(FAIL, 1, args->p_id, args->ps_amount, 0, 0);
                append_to_proc_file(args->p_id, OFMPROC_FILENAME);
            }
        }
        else {
            /* Punto 3 dentro de try_shm_salloc */
            alloc_success = try_shm_salloc(shm_id, mem_size, args->p_id, args->ps_amount,
                                           args->spaces_per_seg);
            if (alloc_success == -1) {
                /* Punto 3 en caso de memoria llena */
                write_to_log(FAIL, 0, args->p_id, args->ps_amount * args->spaces_per_seg, 0, 0);
                append_to_proc_file(args->p_id, OFMPROC_FILENAME);
            }
        }
    }

    /* Quitar P_ID de actual proceso en busqueda de memoria */
    set_searching_pid(shm_id, mem_size / MEMSPACE_SIZE, -1);

    /* Punto 4 */
    sem_post(memory_sem);

    if (alloc_success != -1) {

        /* Punto 5 */
        sleep((unsigned int) args->run_time);

        /* Vuelve a la lista de procesos bloqueados */
        while (available_bp == -1) {
            pthread_mutex_lock(&bp_mutex);
            available_bp = add_to_bp_list(args->bp_id, args->p_id);
            pthread_mutex_unlock(&bp_mutex);
        }

        /* Punto 6 */
        if (!sem_wait(memory_sem)) {

            /* Borrar de la lista de procesos bloqueados */
            while (available_bp == 1) {
                //pthread_mutex_lock(&bp_mutex);
                del_from_bp_list(args->bp_id, args->p_id);
                //pthread_mutex_unlock(&bp_mutex);
                available_bp = -1;
            }

            int dealloc_success;
            /* Punto 7 */
            if (args->is_paging) {
                /* Punto 8 dentro de try_shm_dealloc */
                dealloc_success = try_shm_dealloc(shm_id, mem_size, args->p_id, args->ps_amount);
                if (dealloc_success == -1)
                    /* Error de deallocation */
                    printf("Proceso %d fallo en liberar memoria.\n", args->p_id);
            } else {
                /* Punto 8 dentro de try_shm_dealloc */
                dealloc_success = try_shm_dealloc(shm_id, mem_size, args->p_id, args->ps_amount * args->spaces_per_seg);
                if (dealloc_success == -1)
                    /* Error de deallocation */
                    printf("Proceso %d fallo en liberar memoria.\n", args->p_id);
            }
        }

        /* Agregar a lista de procesos que terminaron */
        append_to_proc_file(args->p_id, ENDPROC_FILENAME);

        /* Punto 9 */
        sem_post(memory_sem);
    }
    /* Liberar el puntero de parametros */
    free(args);
}

int prod_main(int argc, char *argv[]) {

    int min_runtime = 20, max_runtime = 60, min_creation_lapse = 30, max_creation_lapse = 60;
    printf("Programa Productor \n");
    printf("------------------ \n");

    if (argc < 2){
        printf("Cantidad de argumentos inválida. \n");
        exit(-1);
    }

    /* Obtener llave del archivo de proc bloqueados */
    key_t bp_key = ftok(BLCKKEY_FILENAME, 'R');
    if (bp_key == -1)
        exit_failure("Error de generacion de la clave de bloqueados. \n");

    /* Obtener el ID de la memoria de blockd procs */
    int bp_id = shmget(bp_key, (size_t) sizeof(int)*(MAX_BLOCKED_P+1), 0644);
    if (bp_id == -1)
        exit_failure("Error de acceso a procesos bloqueados. \n");

    /* Parametros opcionales de tiempos de ejecucion y lapso de creacion */
    if (argc == 6){
        int aux;

        min_runtime = (int) strtol(argv[2], 0, 10);
        max_runtime = (int) strtol(argv[3], 0, 10);
        if (min_runtime > max_runtime){
            aux = min_runtime; min_runtime = max_runtime; max_runtime = aux;
        }

        min_creation_lapse = (int) strtol(argv[4], 0, 10);
        max_creation_lapse = (int) strtol(argv[5], 0, 10);
        if (min_creation_lapse > max_creation_lapse){
            aux = min_creation_lapse; min_creation_lapse = max_creation_lapse;
            max_creation_lapse = aux;
        }
    }

    /* Para que en cada ejecución existan randoms diferentes */
    srand((unsigned int) getpid());

    int process_id = 1;

    /* Guarda el algoritmo utilizado */
    write_new_file(ALGORITHM_FILENAME, argv[1], 0);

    if (strcmp(argv[1], "pag") == 0){
        while (process_id <= 4){

            t_args * args = malloc(sizeof(t_args));

            args->is_paging = 1;
            args->p_id = process_id;
            args->ps_amount = get_random_int(1, 10);
            args->run_time = get_random_int(min_runtime, max_runtime);
            args->bp_id = bp_id;

            pthread_t thread;
            if (pthread_create(&thread, 0, (void *) run_proc, args) < 0) {
                printf("\nError de inicio del proceso #%d.\n", process_id);
            }

            sleep((unsigned int) get_random_int(min_creation_lapse, max_creation_lapse));
            process_id += 1;
            // printf("gets"); getchar();
        }
    }
    else if (strcmp(argv[1], "seg") == 0) {
        while (1){

            t_args * args = malloc(sizeof(t_args));

            args->is_paging = 0;
            args->p_id = process_id;
            args->ps_amount = get_random_int(1, 5);
            args->spaces_per_seg = get_random_int(1, 3);
            args->run_time = get_random_int(min_runtime, max_runtime);
            args->bp_id = bp_id;

            pthread_t thread;
            if (pthread_create(&thread, 0, (void *) run_proc, args) < 0) {
                printf("\nError de inicio del proceso #%d.\n", process_id);
            }

            sleep((unsigned int) get_random_int(min_creation_lapse, max_creation_lapse));
            process_id += 1;
        }
    }
    else {
        printf("Argumento invalido. \n");
    }
    getchar();
    return EXIT_SUCCESS;
}