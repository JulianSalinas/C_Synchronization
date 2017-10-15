#ifndef C_SERVERS_MAIN_H
#define C_SERVERS_MAIN_H

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <libgen.h>
#include <stdlib.h>
#include <pthread.h>

#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>

#include "Util/list.h"
#include "Util/util.h"

/* Shared memory */
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

/* Main references */
#include "init.h"
#include "prod.h"

#endif //C_SERVERS_MAIN_H
