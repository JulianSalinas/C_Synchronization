#include "../Headers/mem.h"

int get_shm_id(char * mem_key, int size, int flags) {

    /* Obtener llave del archivo */
    key_t shm_key = ftok(mem_key, 'R');
    if (shm_key  == -1)
        exit_failure("Error en generacion de la clave \n");

    /* Reservar la memoria compartida */
    int shm_id = shmget(shm_key, (size_t) size, 0644 | flags);
    if (shm_id == -1)
        exit_failure("Error al obtener la memoria compartida \n");

    return shm_id;

}

void * get_shm_sem(char * sem_name) {

    sem_t * memory_sem = sem_open(sem_name, 0);

    if(memory_sem == (void*) -1)
        exit_failure("Error de referencia al semáforo \n");

    return memory_sem;

}
