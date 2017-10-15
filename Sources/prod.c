#include "../Headers/prod.h"

#define SHM_SIZE 1024  /* Kb de memoria compartida */
#define SEM_NAME "smf"


int prod_main(int argc, char *argv[]) {

    printf("Programa Productor \n");
    printf("------------------ \n");

    /* shared memory vars */
    key_t key;
    int shmid;
    char *data;
    int mode;

    /* semaphore vars */
    sem_t *sem_des;

    /* Key */
    if ((key = ftok("key.txt", 'R')) == -1)
    {
        perror("Error de generacion de la clave. \n");
        exit(1);
    }

    /* Reservar la memoria compartida */
    /* shmid -> shared memory id */
    if ((shmid = shmget(key, SHM_SIZE, 0644)) == -1) {
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