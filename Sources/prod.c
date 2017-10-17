#include "../Headers/prod.h"

#define SEM_NAME "smf"

int prod_main(int argc, char *argv[]) {

    printf("Programa Productor \n");
    printf("------------------ \n");

    /* shared memory vars */
    key_t key;
    int shmid;
    char *data;
    int mem_size = read_file_int(MEMSIZE_FILENAME);

    /* semaphore vars */
    sem_t *sem_des;

    /* Key */
    if ((key = ftok("key.txt", 'R')) == -1)
    {
        perror("Error de generacion de la clave. \n");
        exit(1);
    }

    /* Obtener el ID de la memoria compartida */
    if ((shmid = shmget(key, mem_size, 0644)) == -1) {
        perror("Error de acceso a memoria. \n");
        exit(1);
    }

    /****************************************************************************/


    while(1) {
        getchar();

        /* Open the Semaphore */

        sem_des = sem_open(SEM_NAME, 0);

        if(sem_des == (void*) -1){
            perror("sem_open failure");
            exit(1);
        }

        /* Lock the semaphore */

        if(!sem_wait(sem_des)) {

            /* Access to the shared memory area */
            /* attach to the segment to get a pointer to it: */
            data = shmat(shmid, NULL, 0);
            if (data == (char *) (-1)) {
                perror("shmat");
                exit(1);
            }

            printf("segment contains: \"%s\"\n", data);

            /* detach from the segment: */
            if (shmdt(data) == -1) {
                perror("shmdt");
                exit(1);
            }
        }
        /* Release the semaphore lock */

        sem_post(sem_des);
        sem_close(sem_des);

    }

}