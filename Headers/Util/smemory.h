#ifndef SMEMORY_H
#define SMEMORY_H

#include "../main.h"

typedef struct MCell MCell;

struct MCell {
    int cell_number;
    int held_proc_num;
    int held_proc_part;
};

/**
 * Genera un arreglo de celdas en un sector de memoria compartida.
 * @param shm_id: ID de la region de memoria compartida.
 * @param cell_amount: Cantidad de celdas a generar.
 * @return : Un puntero al arreglo de celdas.
 */
void instance_MemorySimulation(int shm_id, int cell_amount);

#endif // SMEMORY_H
