#include "../Headers/fin.h"

int fin_main(int argc, char *argv[]){

    printf("Programa Finalizador \n");
    printf("---------------------- \n");

    /* Variables de memoria compartida */
    key_t shm_key;
    int shm_id; /* Shared memory ID */
    int mem_size = read_file_int(MEMSIZE_FILENAME);

    /* Obtener llave del archivo */
    if ((shm_key = ftok(KEY_FILENAME, 'R')) == -1)
    {
        perror("Error obteniendo la clave. \n");
        exit(-1);
    }

    /* Obtener el ID de la memoria compartida */
    if ((shm_id = shmget(shm_key, mem_size, 0644)) == -1) {
        perror("No se encontro la referencia a la memoria compartida. \n");
        exit(-1);
    }

    printf("Id memoria: %d \n", shm_id);

    /* Liberar la memoria compartida */
    if ((shmctl (shm_id, IPC_RMID, (struct shmid_ds *) 0)) == -1)
    {
        perror ("Error liberando la memoria compartida.");
        exit (-1);
    }

    /* Referenciar el semaforo de SHM */
    sem_t * shm_sem = sem_open(SHM_SEM_NAME, 0);

    /* Eliminar o liberar el semaforo */
    sem_close(shm_sem);
}