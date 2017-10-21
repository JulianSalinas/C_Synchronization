#ifndef C_SERVERS_MAIN_H
#define C_SERVERS_MAIN_H

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <libgen.h>
#include <fcntl.h>
#include <stdlib.h>
#include <pthread.h>
#include <ctype.h>

#include <sys/stat.h>
#include <sys/mman.h>

#include "Util/list.h"
#include "Util/util.h"
#include "Util/random.h"
#include "Util/smemory.h"

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
#include "spy.h"
#include "log.h"

/* Macros de configuraciones */
/* KB asignados para cada espacio de memoria */
#define MEMSPACE_SIZE 1
/* Nombre de la carpeta de configs */
#define CONFIG_FOLDER "config"
/* Nombre del archivo con la llave */
#define KEY_FILENAME "config/key"
/* Nombre del archivo con el tamano de la memoria */
#define MEMSIZE_FILENAME "config/memsize"
/* Nombre del archivo con la direccion de memoria compartida */
#define MEMADDRESS_FILENAME "config/memaddress"
/* Nombre del archivo donde se registran las acciones */
#define LOG_FILENAME "config/log"

#endif //C_SERVERS_MAIN_H
