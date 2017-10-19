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

int write_to_shm(int shm_id, int cell_num, int proc_num, int proc_part){

    if (cell_num < 1)
        return -1;

    /* Obtener referencia a memoria compartida */
    void * shm_addr = shmat(shm_id, NULL, 0);

    if (shm_addr == (void *) (-1)) {
        perror("Error de referencia a memoria.");
        exit(-1);
    }

    /* Ajustar el offset de la celda */
    shm_addr += (cell_num-1) * sizeof(MCell);

    /* Llenar una celda temporal */
    MCell * temp_cell = malloc(sizeof(MCell));
    temp_cell->cell_number = cell_num;
    temp_cell->held_proc_num = proc_num;
    temp_cell->held_proc_part = proc_part;

    /* Copiar la celda a la direccion compartida con offset */
    memcpy(shm_addr, temp_cell, sizeof(MCell));

    /* Liberar punteros */
    shmdt(shm_addr);
    free(temp_cell);

    return 1;
}

MCell * read_shm_cell(int shm_id, int cell_num){

    if (cell_num < 1)
        return NULL;

    /* Obtener referencia a memoria compartida */
    void * shm_addr = shmat(shm_id, NULL, 0);

    if (shm_addr == (void *) (-1)) {
        perror("Error de referencia a memoria.");
        exit(-1);
    }

    /* Ajustar el offset de la celda */
    shm_addr += (cell_num-1) * sizeof(MCell);

    /* Llenar una celda temporal */
    MCell * result_cell = malloc(sizeof(MCell));

    result_cell->cell_number = ((MCell *) shm_addr)->cell_number;
    result_cell->held_proc_num = ((MCell *) shm_addr)->held_proc_num;
    result_cell->held_proc_part = ((MCell *) shm_addr)->held_proc_part;

    /* Liberar referencia */
    shmdt(shm_addr);

    return result_cell;
}