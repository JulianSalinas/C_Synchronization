#ifndef C_SYNCHRONIZATION_PROD_H
#define C_SYNCHRONIZATION_PROD_H

#include "main.h"

typedef struct paging_args paging_args;

struct paging_args {
    int p_id;
    int page_amount;
    int run_time;
};

typedef struct segm_args segm_args;

struct segm_args {
    int p_id;
    int seg_amount;
    int spaces_per_seg;
    int run_time;
};

/* Function definitions */
int prod_main(int argc, char *argv[]);

/**
 * Funcion ejecutada por los hilos de paginacion
 * @param args: process_id, page_amount, run_time
 * @return: none
 */
void * run_pag_proc(paging_args * args);

/**
 * Funcion ejecutada por los hilos de segmentacion
 * @param args: process_id, segment_amount, spaces_per_seg, run_time
 * @return: none
 */
void * run_seg_proc(segm_args * args);

#endif //C_SYNCHRONIZATION_PROD_H
