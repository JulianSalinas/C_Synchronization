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
 * @return: Un puntero al arreglo de celdas.
 */
void instance_MemorySimulation(int shm_id, int cell_amount);

/**
 * Escribe en memoria compartida
 * @param shm_id: ID de la region de memoria
 * @param cell_num: numero de celda MAYOR O IGUAL A 1
 * @param proc_num: ID del proceso ficticio
 * @param proc_part: numero de segmento o pagina del proceso
 * @return: 1 para exito, -1 para error
 */
int write_to_shm(int shm_id, int cell_num, int proc_num, int proc_part);

/**
 * Busca una celda en memoria compartida
 * @param shm_id: ID de la region de memoria
 * @param cell_num: numero de celda MAYOR O IGUAL A 1
 * @return: un puntero a una copia local de la celda
 * en memoria compartida
 */
MCell * read_shm_cell(int shm_id, int cell_num);

#endif // SMEMORY_H
