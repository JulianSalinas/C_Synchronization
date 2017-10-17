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
/* KB asignados para cada espacio de memoria */
#define MEMSPACE_SIZE 1
/* Nombre de la carpeta de configs */
#define CONFIG_FOLDER "config"
/* Nombre del archivo con la llave */
#define KEY_FILENAME "config/key"
/* Nombre del archivo con el tamano de la memoria */
#define MEMSIZE_FILENAME "config/memsize"

#endif //C_SERVERS_MAIN_H
