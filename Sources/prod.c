#include "../Headers/prod.h"

#define SEM_NAME "smf"

int prod_main(int argc, char *argv[]) {

    printf("Programa Productor \n");
    printf("------------------ \n");

    /* shared memory vars */
    key_t key;
    int shm_id;
    MCell * mem_cell;
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
        getchar();

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
            mem_cell = (MCell *) shmat(shm_id, NULL, 0);
            if (mem_cell == (MCell *) (-1)) {
                perror("Error de referencia a memoria.");
                exit(-1);
            }

            printf("Valor encontrado: %d \n", mem_cell->cell_number);

            /* detach from the segment: */
            if (shmdt(mem_cell) == -1) {
                perror("Error eliminando referencia a memoria.");
                exit(-1);
            }
        //}
        /* Release the semaphore lock */

        //sem_post(sem_des);
        //sem_close(sem_des);

    //}

}