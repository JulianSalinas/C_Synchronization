#include "../Headers/init.h"

/* Kb de memoria compartida */
#define SHM_SIZE 1024
/* Nombre del semaforo para la llave */
#define KEY_SEM_NAME "key_semaphore"
/* Nombre del archivo con la llave */
#define KEY_FILENAME "key.txt"
/* Llave para la memoria compartida */
#define SH_KEY "V40BUW3K53L"


int init_main(int argc, char *argv[]) {

    printf("Programa Inicializador \n");
    printf("---------------------- \n");

    if (argc < 2){
        printf("Cantidad de argumentos inválida. \n");
        exit(1);
    }

    /* Cantidad de espacios de memoria */
    long mem_sp_amount = (int) strtol(argv[1], 0, 10);

    /* Variables de memoria compartida */
    key_t key;
    int shmid; /* Shared memory ID */
    char *data; /* Puntero al espacio de memoria */

    /* Variables de semaforo */
    sem_t * key_sem;

    /* Crear el archivo con la llave */
    write_new_file(KEY_FILENAME, SH_KEY);

    while(1){
        getchar();
    }

    /* Obtener llave del archivo */
    if ((key = ftok(KEY_FILENAME, 'R')) == -1)
    {
        perror("Error de generacion de la clave. \n");
        exit(1);
    }

    /* Reservar la memoria compartida */
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
    key_sem = sem_open(KEY_SEM_NAME, O_CREAT, 0644, 0);

    if(key_sem == (void*)-1){
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
    sem_post(key_sem);

    /* detach from the segment: */
    if (shmdt(data) == -1) {
        perror("shmdt");
        exit(1);
    }

    /* close semaphore */

    sem_close(key_sem);
    getchar();

    if ((shmctl (shmid, IPC_RMID, (struct shmid_ds *) 0)) == -1)
    {
        perror ("shmctl");
        exit (-1);
    }
}