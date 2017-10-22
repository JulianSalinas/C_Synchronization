#include "../Headers/fin.h"

int fin_main(int argc, char *argv[]){

    size_t mem_size = (size_t) read_file_int(MEMSIZE_FILENAME);

    /* Obtener llave del archivo */
    key_t shm_key = ftok(KEY_FILENAME, 'R');
    if (shm_key == -1) exit_failure("Error obteniendo la clave. \n");

    /* Obtener el ID de la memoria compartida */
    int shm_id = shmget(shm_key, mem_size, 0644);
    if (shm_id == -1)
        exit_failure("No se encontro la referencia a la memoria compartida. \n");

    /* Liberar la memoria compartida */
    if ((shmctl (shm_id, IPC_RMID, (struct shmid_ds *) 0)) == -1)
        exit_failure("Error liberando la memoria compartida. \n");

    /* Referenciar el semaforo de SHM */
    sem_t * shm_sem = sem_open(SHM_SEM_NAME, 0);

    /* Eliminar o liberar el semaforo */
    sem_close(shm_sem);

    printf("Id memoria: %d \n", shm_id);

}