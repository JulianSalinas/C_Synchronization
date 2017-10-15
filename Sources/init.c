#include "../Headers/init.h"

#define SHM_SIZE 1024  /* Kb de memoria compartida */
#define SEM_NAME "smf"


int init_main(int argc, char *argv[]) {

    printf("Programa Inicializador \n");
    printf("---------------------- \n");

    /* Cantidad de espacios de memoria */
    long mem_sp_amount = (int) strtol(argv[1], 0, 10);

    /* shared memory vars */
    key_t key;
    int shmid;
    char *data;
    int mode;

    /* semaphore vars */
    int index;
    sem_t *sem_des;

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

    /* after getting the pointer, create the semaphore */
    /* Create a semaphore in locked state */
    sem_des = sem_open(SEM_NAME, O_CREAT, 0644, 0);

    if(sem_des == (void*)-1){
        perror("sem_open failure");
        exit(1);
    }

    /* Access to the shared memory area */
    /* read or modify the segment, based on the command line: */
    printf("writing to segment: \"%s\"\n", "xdd");
    strncpy(data, "xdd", SHM_SIZE);
    printf("segment contains: \"%s\"\n Enter para liberar semaphore \n", data);

    getchar();
    /* Release the semaphore lock */
    sem_post(sem_des);

    /* detach from the segment: */
    if (shmdt(data) == -1) {
        perror("shmdt");
        exit(1);
    }

    /* close semaphore */

    sem_close(sem_des);
    getchar();

    if ((shmctl (shmid, IPC_RMID, (struct shmid_ds *) 0)) == -1)
    {
        perror ("shmctl");
        exit (-1);
    }
}