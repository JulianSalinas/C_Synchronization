#ifndef C_SYNCHRONIZATION_PROD_H
#define C_SYNCHRONIZATION_PROD_H

#include "main.h"

typedef struct t_args t_args;

struct t_args {
    int is_paging;
    int p_id;
    int ps_amount;
    int spaces_per_seg;
    int run_time;
};

/* Function definitions */
int prod_main(int argc, char *argv[]);

/**
 * Funcion ejecutada por los hilos de paginacion
 * @param args: (paginacion) is_paging = 1, process_id, page_amount, run_time
 *              (segmentacion) is_paging = 0, process_id, segment_amount, spaces_per_seg, run_time
 * @return: none
 */
void * run_proc(t_args * args);

/**
 * Intenta mapear el proceso en la memoria de haber espacio
 * @param shm_id: shm id
 * @param mem_size: memory size
 * @param proc_id: proc id
 * @param p_amount: cantidad de paginas del proceso
 * @return: 1 en éxito, -1 en memoria llena
 */
int try_shm_palloc(int shm_id, int mem_size, int proc_id, int p_amount);

int try_shm_salloc(int shm_id, int mem_size, int proc_id, int s_amount, int parts_per_seg);

int try_shm_dealloc(int shm_id, int mem_size, int proc_id, int ps_amount);
#endif //C_SYNCHRONIZATION_PROD_H
