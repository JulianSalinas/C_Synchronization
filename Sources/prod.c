#include "../Headers/prod.h"

#define SEM_NAME "smf"

void * run_pag_proc(paging_args * args){

    pthread_t id = pthread_self();
    pthread_detach(id);

    printf("Proceso %d sirvio.\n", args->p_id);
    sleep(args->run_time);
    printf("Proceso %d murio.\n", args->p_id);

    free(args);
}

void * run_seg_proc(segm_args * args){

    pthread_t id = pthread_self();
    pthread_detach(id);


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

            paging_args * args = malloc(sizeof(paging_args));

            args->p_id = process_id;
            args->page_amount = get_random_int(1, 10);
            args->run_time = get_random_int(20, 60);

            pthread_t thread;
            if (pthread_create(&thread, 0, run_pag_proc, args) < 0) {
                printf("\nError de inicio del proceso #%d.\n", process_id);
            }

            sleep((unsigned int) get_random_int(30, 60));
            process_id += 1;
        }
    }
    else if (strcmp(argv[1], "seg") == 0) {
        while (1){

            segm_args * args = malloc(sizeof(segm_args));

            args->p_id = process_id;
            args->seg_amount = get_random_int(1, 5);
            args->spaces_per_seg = get_random_int(1, 3);
            args->run_time = get_random_int(20, 60);

            pthread_t thread;
            if (pthread_create(&thread, 0, run_seg_proc, args) < 0) {
                printf("\nError de inicio del proceso #%d.\n", process_id);
            }

            sleep((unsigned int) get_random_int(30, 60));
            process_id += 1;
        }
    }
    else {
        printf("Argumento invalido. \n");
    }
    exit(1);

    /* shared memory vars */
    key_t key;
    int shm_id;
    void * mem_cell;
    int mem_size = read_file_int(MEMSIZE_FILENAME);

    /* semaphore vars */
    sem_t * sem_des;

    /* Key */
    if ((key = ftok(KEY_FILENAME, 'R')) == -1)
    {
        perror("Error de generacion de la clave. \n");
        exit(-1);
    }

    /* Obtener el ID de la memoria compartida */
    if ((shm_id = shmget(key, mem_size, 0644)) == -1) {
        perror("Error de acceso a memoria. \n");
        exit(-1);
    }

    /****************************************************************************/


    //while(1) {

        /* Open the Semaphore */

        //sem_des = sem_open(SEM_NAME, 0);

        /*if(sem_des == (void*) -1){
            perror("sem_open failure");
            exit(1);
        }

        /* Lock the semaphore */

        //if(!sem_wait(sem_des)) {

            /* Access to the shared memory area */
            /* attach to the segment to get a pointer to it: */
            /*mem_cell = shmat(shm_id, NULL, 0);
            if (mem_cell == (void *) (-1)) {
                perror("Error de referencia a memoria.");
                exit(-1);
            }*/
    MCell * prueba = read_shm_cell(shm_id, 4);

    printf("Valor encontrado: %d \n", prueba->cell_number);

    free(prueba);
    /*MCell * asd = mem_cell+sizeof(MCell);*/

            /* detach from the segment: */
            /*if (shmdt(mem_cell) == -1) {
                perror("Error eliminando referencia a memoria.");
                exit(-1);
            }*/
        //}
        /* Release the semaphore lock */

        //sem_post(sem_des);
        //sem_close(sem_des);

    //}

}