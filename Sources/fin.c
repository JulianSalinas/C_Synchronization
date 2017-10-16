#include "../Headers/fin.h"

int fin_main(int argc, char *argv[]){

    /* Variables de memoria compartida */
    key_t shm_key;
    int shm_id; /* Shared memory ID */

    /* Obtener llave del archivo */
    if ((shm_key = ftok(KEY_FILENAME, 'R')) == -1)
    {
        perror("Error obteniendo la clave. \n");
        exit(-1);
    }

    /* Obtener el ID de la memoria compartida */
    if ((shm_id = shmget(shm_key, SHM_SIZE, 0644)) == -1) {
        perror("No se encontro la referencia a la memoria compartida. \n");
        exit(-1);
    }

    printf("Id memoria: %d", shm_id);

    /* Liberar la memoria compartida */
    if ((shmctl (shm_id, IPC_RMID, (struct shmid_ds *) 0)) == -1)
    {
        perror ("Error liberando la memoria compartida.");
        exit (-1);
    }

}