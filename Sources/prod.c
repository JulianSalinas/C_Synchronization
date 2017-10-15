#include "../Headers/prod.h"

#define SHM_SIZE 1024  /* Kb de memoria compartida */

int prod_main(int argc, char *argv[]) {
    printf("Programa Productor \n");
    printf("------------------ \n");

    key_t key;
    int shmid;
    char *data;
    int mode;

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

        /* attach to the segment to get a pointer to it: */
        data = shmat(shmid, NULL, 0);
        if (data == (char *)(-1)) {
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

}