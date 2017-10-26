#ifndef C_SERVERS_MAIN_H
#define C_SERVERS_MAIN_H

#include <time.h>
#include <stdarg.h>
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
#include "Util/blckdplist.h"

/* Semaphores */
#include <semaphore.h>

/* Shared memory */
#include <sys/user.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>


/* Main references */
#include "mem.h"
#include "init.h"
#include "prod.h"
#include "fin.h"
#include "spy.h"
#include "log.h"

/* Macros de configuraciones */

/* Nombre de la carpeta de configs */
#define CONFIG_FOLDER "config"

/* Nombre del archivo con la llave de memoria compartida */
#define KEY_FILENAME "config/key"

/* Nombre del archivo con la llave de procesos bloqueados */
#define BLCKKEY_FILENAME "config/bpkey"

/* Nombre del archivo con el tamano de la memoria */
#define MEMSIZE_FILENAME "config/memsize"

/* Nombre del archivo donde se registran las acciones */
#define LOG_FILENAME "config/log"

/* Nombre del archivo de procesos que no encontraron memoria :out of memory: */
#define OFMPROC_FILENAME "config/ofmprocs"

/* Nombre del archivo de procesos que terminaron satisfactoriamente */
#define ENDPROC_FILENAME "config/endprocs"

/* Se guarda si el usuario escogió paginación o segmentación */
#define ALGORITHM_FILENAME "config/algorithm"

/* Identificador del semaforo para acceder a memoria compartida */
#define SHM_SEM_NAME "memory_access_semaphore"

/* Maximo de procesos bloqueados */
#define MAX_BLOCKED_P 25

/* Llave para la memoria compartida */
#define KEY_FILE_VALUE "V40BUW3K53L"

#define BLCKPROC_KEY "L35K3WUB04V"

#define PRINTLINE printf("------------------------------------------------- \n");

#endif //C_SERVERS_MAIN_H
