#include "../../Headers/Util/smemory.h"

void instance_MemorySimulation(int shm_id, int cell_amount){

    MCell * empty_cell = malloc(sizeof(MCell));

    /* Inicializar celda vacia */
    empty_cell->cell_number = -1;
    empty_cell->held_proc_num = -1;
    empty_cell->held_proc_part = -1;

    /* Obtener puntero a memoria compartida */
    void * shm_address;
    shm_address = shmat(shm_id, NULL, 0);
    printf("add: %d\n", shm_address);

    if (shm_address < 0) {
        perror("shmat");
        exit(-1);
    }

    write_new_file(MEMADDRESS_FILENAME, (int) shm_address, 1);

    /* Copiar las celdas vacias a memoria compartida */
    for (int i = 0; i < cell_amount; i++){
        memcpy(shm_address, empty_cell, sizeof(MCell));
        shm_address += sizeof(MCell);
    }

    /* Liberar punteros */
    shmdt(shm_address);
    free(empty_cell);

    fin_main(0, NULL);
}