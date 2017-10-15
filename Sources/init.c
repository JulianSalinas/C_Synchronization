#include "../Headers/init.h"

#define SHM_SIZE 1024  /* Kb de memoria compartida */

int init_main(int argc, char *argv[]) {
    printf("Programa Inicializador \n");
    printf("---------------------- \n");

    /* Cantidad de espacios de memoria */
    long mem_sp_amount = (int) strtol(argv[1], 0, 10);

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
    if ((shmid = shmget(key, SHM_SIZE, 0644 | IPC_CREAT)) == -1) {
        perror("Error de reserva de memoria. \n");
        exit(1);
    }

    /****************************************************************************/

    /* attach to the segment to get a pointer to it: */
    data = shmat(shmid, NULL, 0);
    if (data == (char *)(-1)) {
        perror("shmat");
        exit(1);
    }

    /* read or modify the segment, based on the command line: */
    printf("writing to segment: \"%s\"\n", "xdd");
    strncpy(data, "xdd", SHM_SIZE);

    printf("segment contains: \"%s\"\n", data);

    /* detach from the segment: */
    if (shmdt(data) == -1) {
        perror("shmdt");
        exit(1);
    }

    getchar();

    if ((shmctl (shmid, IPC_RMID, (struct shmid_ds *) 0)) == -1)
    {
        perror ("shmctl");
        exit (-1);
    }
}