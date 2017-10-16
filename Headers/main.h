#ifndef C_SERVERS_MAIN_H
#define C_SERVERS_MAIN_H

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <libgen.h>
#include <fcntl.h>
#include <stdlib.h>
#include <pthread.h>

#include <sys/stat.h>
#include <sys/mman.h>

#include "Util/list.h"
#include "Util/util.h"

/* Semaphores */
#include <semaphore.h>

/* Shared memory */
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

/* Main references */
#include "init.h"
#include "prod.h"
#include "fin.h"

/* Macros de configuraciones */
/* Kb de memoria compartida */
#define SHM_SIZE 1024
/* Nombre del archivo con la llave */
#define KEY_FILENAME "key"

#endif //C_SERVERS_MAIN_H
