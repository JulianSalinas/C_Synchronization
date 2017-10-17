#include "../../Headers/Util/smemory.h"

void instance_MemorySimulation(int shm_id, int cell_amount){

    MCell * MemorySimulation[cell_amount];

    MCell * empty_cell = malloc(sizeof(MCell));

    /* Inicializar celda vacia */
    empty_cell->cell_number = -1;
    empty_cell->held_proc_num = -2;
    empty_cell->held_proc_part = -3;

    /* Copiar la estructura a la memoria compartida */

    for(int i = 0; i < cell_amount; i++) {
        /* Obtener puntero a direccion sin uso */
        MemorySimulation[0] = shmat(shm_id, NULL, 0);
        /* Copiar una celda vacia */
        memcpy(MemorySimulation[0], empty_cell, sizeof(empty_cell));
        /* Liberar puntero */
        /* detach from the segment: */
        //shmdt(MemorySimulation[0]);

    }

    printf("%d", MemorySimulation[0]->held_proc_part);
    printf("%d", MemorySimulation[0]->held_proc_num);
    printf("%d", MemorySimulation[0]->cell_number);

}