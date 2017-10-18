#include "../../Headers/Util/smemory.h"

void instance_MemorySimulation(int shm_id, int cell_amount){

    MCell * MemorySimulation[cell_amount];

    MCell * empty_cell = malloc(sizeof(MCell));

    /* Inicializar celda vacia */

    empty_cell->held_proc_num = -1;
    empty_cell->held_proc_part = -1;

    /* Inicializar la estructura de memoria local */
    for (int i = 0; i < cell_amount; i++){

        empty_cell->cell_number = -1*i;

        MemorySimulation[i] = malloc(sizeof(MCell));
        memcpy(MemorySimulation[i], empty_cell, sizeof(MCell));
    }

    /* Obtener puntero a memoria compartida */
    void * shm_address = shmat(shm_id, NULL, 0);

    /* Copiar la estructura a la memoria compartida */
    memcpy(MemorySimulation, shm_address, sizeof(MCell)*cell_amount);

    /* Liberar punteros */
    shmdt(shm_address);
    for (int i = 0; i < cell_amount; i++) {
        free(MemorySimulation[i]);
    }
    free(empty_cell);
}