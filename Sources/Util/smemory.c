#include "../../Headers/Util/smemory.h"

void instance_MemorySimulation(int shm_id, int cell_amount){

    MCell * empty_cell = malloc(sizeof(MCell));

    /* Dar valores a celda, son negativos para saber que son
     * de inicializacion y no ejecucion */ 
    empty_cell->held_proc_num = -1;
    empty_cell->held_proc_part = -1;

    /* Obtener puntero a memoria compartida */
    void * shm_address;
    shm_address = shmat(shm_id, NULL, 0);

    /* Copiar las celdas vacias a memoria compartida */
    for (int i = 0; i < cell_amount; i++){
        
        /* Identificador del numero de celda */
        empty_cell->cell_number = i+1;
        
        /* Copiar la celda a la memoria compartida */
        memcpy(shm_address, empty_cell, sizeof(MCell));
        shm_address += sizeof(MCell);
    }

    /* Liberar punteros */
    shmdt(shm_address);
    free(empty_cell);

}