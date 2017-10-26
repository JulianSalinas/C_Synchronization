#include "../Headers/prod.h"

pthread_mutex_t bp_mutex = PTHREAD_MUTEX_INITIALIZER;

int prod_main(int argc, char * argv[]) {

    srand((unsigned int) getpid());
    if (argc < 2) exit_failure("Cantidad de argumentos inválida. \n");

    /* Guarda el algoritmo utilizado */
    write_new_file(ALGORITHM_FILENAME, argv[1], 0);

    /* Obteniendo semáforo */
    sem_t * shm_sem = get_shm_sem(SHM_SEM_NAME);

    /* Obteniendo tamaño de la memoria */
    int mem_size = read_file_int(MEMSIZE_FILENAME);

    /* Obteniendo memorias compartidas */
    int shm_id = get_shm_id(KEY_FILENAME, mem_size, 0);
    int bp_id = get_shm_id(BLCKKEY_FILENAME, sizeof(int) * (MAX_BLOCKED_P + 1), 0);

    /* Obteniendo algoritmo */
    int is_paging = strcmp(argv[1], "pag") == 0 ? 1 :
                    strcmp(argv[1], "seg") == 0 ? 0 :
                    exit_failure("Agoritmo no reconocido");

    for (int process_id = 1; process_id; process_id++) {

        t_args * args = malloc(sizeof(t_args));

        /* Colocando parámetros para el proceso */
        args->shm_id = shm_id;
        args->bp_id = bp_id;

        args->p_id = process_id;
        args->shm_sem = shm_sem;
        args->mem_size = mem_size;
        args->is_paging = is_paging;

        args->ps_amount = get_random_int(1, args->is_paging ? 10 : 5);
        args->run_time = get_random_int(MIN_RUNTIME, MAX_RUNTIME);
        args->spaces_per_seg = !args->is_paging ? get_random_int(1, 3) : 0;

        /* Imprimir informacíon del proceso creado */
        PRINTLINE
        printf("Proceso creado: %d \n", args->p_id);
        printf(args->is_paging ? "Páginas: %d\n" : "Segmentos: %d\n", args->ps_amount);
        printf(args->is_paging ? "" : "Espacios por segmento: %d\n", args->spaces_per_seg);
        printf("Tiempo programado: %d \n", args->run_time);
        PRINTLINE

        /* Nuevo hilo que representa el proceso */
        pthread_t thread;
        if (pthread_create(&thread, 0, (void *) run_proc, args) < 0)
            printf("\nError de inicio del proceso #%d.\n", process_id);

        sleep((unsigned int) get_random_int(MIN_CREATION_LAPSE, MAX_CREATION_LAPSE));

    }

    return EXIT_SUCCESS;
}

void * run_proc(t_args * args){

    PRINTLINE
    pthread_detach(pthread_self());

    int shm_id = args->shm_id;
    int mem_size = args->mem_size;
    sem_t * memory_sem = args->shm_sem;

    /* El proceso se agrega a la lista de bloqueados */
    pthread_mutex_lock(&bp_mutex);
    add_to_bp_list(args->bp_id, args->p_id);
    pthread_mutex_unlock(&bp_mutex);

    int alloc_success = 1;

    /* Punto 1: Pedir semáforo de memoria*/
    printf("Punto 1: Proceso %d pidiendo semáforo\n", args->p_id);
    if(!sem_wait(memory_sem)) {

        /* Colocar como actual proceso en busqueda de memoria */
        set_searching_pid(shm_id, mem_size, args->p_id);
        
        /* Borrar de la lista de procesos bloqueados */
        pthread_mutex_lock(&bp_mutex);
        del_from_bp_list(args->bp_id, args->p_id);
        pthread_mutex_unlock(&bp_mutex);
        
        if (args->p_id == 5) getchar();
        
        /* Punto 2: Buscar su ubicación */ /* Punto 3: Escribir en Bitácora, dentro de try_shm_palloc */
        printf("Punto 2: Proceso %d buscando ubicación \n", args->p_id);
        alloc_success = args->is_paging ?
                            try_shm_palloc(shm_id, mem_size, args->p_id, args->ps_amount):
                            try_shm_salloc(shm_id, mem_size, args->p_id, args->ps_amount, args->spaces_per_seg);
        printf("Punto 3: Proceso %d escribiendo en bítacora \n", args->p_id);

        /* Quitar P_ID de actual proceso en busqueda de memoria */
        set_searching_pid(shm_id, mem_size, -1);

    }

    /* Punto 4: Devolver semáforo de memoria */
    printf("Punto 4: Proceso %d soltando semáforo \n", args->p_id);
    sem_post(memory_sem);

    if (alloc_success != -1) {

        /* Punto 5: Sleep */
        printf("Punto 5: Proceso %d actualmente en memoria \n", args->p_id);
        sleep((unsigned int) args->run_time);

        /* Vuelve a la lista de procesos bloqueados */
        pthread_mutex_lock(&bp_mutex);
        add_to_bp_list(args->bp_id, args->p_id);
        pthread_mutex_unlock(&bp_mutex);

        /* Punto 6: Pedir Semáforo de memoria */
        if (!sem_wait(memory_sem)) {

            /* Borrar de la lista de procesos bloqueados */
            printf("Punto 6: Proceso %d pidiendo semáforo\n", args->p_id);
            pthread_mutex_lock(&bp_mutex);
            del_from_bp_list(args->bp_id, args->p_id);
            pthread_mutex_unlock(&bp_mutex);

            /* Punto 7: Liberar memoria */ /* Punto 8: Escribir en Bitácora */
            printf("Punto 7: Proceso %d soltando la memoria \n", args->p_id);
            int dealloc_success = args->is_paging ?
                                  try_shm_dealloc(shm_id, mem_size, args->p_id, args->ps_amount):
                                  try_shm_dealloc(shm_id, mem_size, args->p_id, args->ps_amount * args->spaces_per_seg);
            printf("Punto 8: Proceso %d escribiendo en bítacora \n", args->p_id);

            if (dealloc_success == -1)
                printf("Proceso %d fallo en liberar memoria.\n", args->p_id);

        }

        /* Punto 9: Devolver semáforo de memoria*/
        printf("Punto 9: Proceso %d soltando semáforo \n", args->p_id);
        sem_post(memory_sem);

        /* Agregar a lista de procesos que terminaron */
        append_to_proc_file(args->p_id, ENDPROC_FILENAME);

    }
    PRINTLINE
}

int try_shm_palloc(int shm_id, int cell_amount, int proc_id, int p_amount){

    int free_cells = get_free_cell_amount(shm_id, cell_amount);

    if (free_cells < p_amount) {
        write_to_log(FAIL, 1, proc_id, p_amount, 0, 0);
        append_to_proc_file(proc_id, OFMPROC_FILENAME);
        return -1;
    }
    else
        set_free_cell_amount(shm_id, cell_amount, free_cells - p_amount);

    int current_page = 1;

    for (int i = 1; i <= cell_amount; i++){

        if (current_page <= p_amount) {

            MCell * cell = read_shm_cell(shm_id, i);
            if (cell->held_proc_num == -1) {

                write_to_shm(shm_id, i, proc_id, current_page, -1);
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

int try_shm_salloc(int shm_id, int cell_amount, int proc_id, int s_amount, int parts_per_seg){
    
    int needed_cells = s_amount*parts_per_seg;
    int free_cells = get_free_cell_amount(shm_id, cell_amount);
    int base_registers[5] = {};
    
    if (free_cells < needed_cells){
        append_to_proc_file(proc_id, OFMPROC_FILENAME);
        write_to_log(FAIL, 0, proc_id, s_amount * parts_per_seg, 0, 0);
        return -1;
    }
    
    set_free_cell_amount(shm_id, cell_amount, free_cells - needed_cells);
    
    int available_blocks_found = 0;
    
    for (int j = 1; (j <= cell_amount) && (available_blocks_found < s_amount);) {
        
        int adjacent_free_cells = 0;
        
        for (int i = j; (adjacent_free_cells < parts_per_seg) && (i <= cell_amount); ++i) {
            
            MCell * current_cell = read_shm_cell(shm_id, i);
            
            if(current_cell->held_proc_num == -1) {
                adjacent_free_cells++;
                free(current_cell);
            }
            else
            {
                free(current_cell); break;
            }
        }

        if(adjacent_free_cells == parts_per_seg) {
            base_registers[available_blocks_found] = j;
            j = j + parts_per_seg;
            available_blocks_found++;
        }
        else
            j++;
    }

    if(available_blocks_found == s_amount){
        
        for (int current_seg = 1; current_seg <= s_amount; current_seg++){
            
            for (int current_part = 1; current_part <= parts_per_seg; current_part++){
                int cell_id = base_registers[current_seg-1] + current_part - 1;
                write_to_shm(shm_id, cell_id , proc_id, current_seg, current_part);
                write_to_log(ALLOCATION, 0, proc_id, cell_id, current_seg, current_part);
            }
        }
    }
    
    return 1;
}

int try_shm_dealloc(int shm_id, int cell_amount, int proc_id, int ps_amount){

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

                write_to_shm(shm_id, i, -1, -1, -1);
                write_to_log(DEALLOCATION, 0, proc_id, i, 0, 0);
                current_ps += 1;
            }
            free(cell);
        }
    }

    return 1;
}