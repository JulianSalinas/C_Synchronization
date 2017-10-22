#include "../Headers/prod.h"

int try_shm_palloc(int shm_id, int mem_size, int proc_id, int p_amount){

    int cell_amount = mem_size/MEMSPACE_SIZE;
    int free_cells = get_free_cell_amount(shm_id, cell_amount);

    if (free_cells < p_amount)
        return -1;
    else
        set_free_cell_amount(shm_id, cell_amount, free_cells - p_amount);

    for (int i = 1; i <= cell_amount; i++){

        int current_part = 1;

        if (current_part <= p_amount) {

            MCell * cell = read_shm_cell(shm_id, i);
            if (cell->held_proc_num == -1) {
                printf("Celda %d se encuentra vacia. \n", i);
                write_to_shm(shm_id, i, proc_id, current_part);
                current_part += 1;
            }
            free(cell);
        }
        else
            break;
    }

    return 1;
}

int try_shm_salloc(int shm_id, int mem_size, int proc_id, int s_amount, int part_per_seg){

}

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

    /* Punto 1 */
    if(!sem_wait(memory_sem)) {

        int alloc_success;
        /* Punto 2 */
        if (args->is_paging)
            /* Punto 3 dentro de try_shm_palloc */
            alloc_success = try_shm_palloc(shm_id, mem_size, args->p_id, args->ps_amount);
        else
            /* Punto 3 dentro de try_shm_salloc */
            alloc_success = try_shm_salloc(shm_id, mem_size, args->p_id, args->ps_amount,
                                           args->spaces_per_seg);
    }

    /* Punto 4 */
    sem_post(memory_sem);

    /* Punto 5 */
    sleep((unsigned int) args->run_time);

    /* Punto 6 */
    if(!sem_wait(memory_sem)) {

        /* Punto 7 */

        /* Punto 8 */

    }

    /* Punto 9 */
    sem_post(memory_sem);

    /* Liberar el puntero de parametros */
    free(args);
}

int prod_main(int argc, char *argv[]) {

    printf("Programa Productor \n");
    printf("------------------ \n");

    if (argc < 2){
        printf("Cantidad de argumentos inválida. \n");
        exit(-1);
    }

    /* Para que en cada ejecución existan randoms diferentes */
    srand((unsigned int) getpid());

    int process_id = 1;

    if (strcmp(argv[1], "pag") == 0){
        while (1){

            t_args * args = malloc(sizeof(t_args));

            args->is_paging = 1;
            args->p_id = process_id;
            args->ps_amount = get_random_int(1, 10);
            args->run_time = get_random_int(20, 60);

            pthread_t thread;
            if (pthread_create(&thread, 0, run_proc, args) < 0) {
                printf("\nError de inicio del proceso #%d.\n", process_id);
            }

            //sleep((unsigned int) get_random_int(30, 60));
            process_id += 1;
            getchar();
        }
    }
    else if (strcmp(argv[1], "seg") == 0) {
        while (1){

            t_args * args = malloc(sizeof(t_args));

            args->is_paging = 0;
            args->p_id = process_id;
            args->ps_amount = get_random_int(1, 5);
            args->spaces_per_seg = get_random_int(1, 3);
            args->run_time = get_random_int(20, 60);

            pthread_t thread;
            if (pthread_create(&thread, 0, run_proc, args) < 0) {
                printf("\nError de inicio del proceso #%d.\n", process_id);
            }

            sleep((unsigned int) get_random_int(30, 60));
            process_id += 1;
        }
    }
    else {
        printf("Argumento invalido. \n");
    }

    return EXIT_SUCCESS;
}