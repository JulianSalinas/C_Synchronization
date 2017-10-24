#include "../Headers/fin.h"

int fin_main(int argc, char *argv[]){

    int mem_size = read_file_int(MEMSIZE_FILENAME);

    /* Obtener el ID de la memoria compartida */
    int shm_id = get_shm_id(KEY_FILENAME, mem_size, 0);

    int bp_id = get_shm_id(BLCKKEY_FILENAME, (size_t )(MAX_BLOCKED_P + 1) * sizeof(int), IPC_CREAT);

    /* Liberar la memoria compartida */
    if ((shmctl (shm_id, IPC_RMID, (struct shmid_ds *) 0)) == -1)
        exit_failure("Error liberando la memoria compartida. \n");

    /* Liberar la memoria compartida */
    if ((shmctl (bp_id, IPC_RMID, (struct shmid_ds *) 0)) == -1)
        exit_failure("Error liberando la memoria compartida. \n");

    /* Referenciar el semaforo de SHM */
    sem_t * shm_sem = sem_open(SHM_SEM_NAME, 0);

    /* Eliminar o liberar el semaforo */
    sem_close(shm_sem);

    printf("Id memoria: %d \n", shm_id);
    printf("Id memoria para procesos bloqueados: %d \n", bp_id);
    printf("Direccion del semáforo: %ld\n", (long) &shm_sem);

}