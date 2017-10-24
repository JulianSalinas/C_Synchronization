#ifndef C_SYNCHRONIZATION_MEM_H
#define C_SYNCHRONIZATION_MEM_H

#include "main.h"

int get_shm_id(char * mem_key, int size, int flags);

void * get_shm_sem(char * sem_name);

#endif //C_SYNCHRONIZATION_MEM_H
