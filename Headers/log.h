#ifndef C_SYNCHRONIZATION_LOG_H
#define C_SYNCHRONIZATION_LOG_H

#include "main.h"
#include <time.h>

#define ALLOCATION 1
#define DEALLOCATION 2
#define FAIL 0

/**
 * @brief Escribe una entrada en la bitacora de actividad de procesos
 * @param type: ALLOCATION, DEALLOCATION, FAIL tipo de entrada
 * @param is_paging: bandera para el tipo de algoritmo
 * @param process_id: proceso que genera la entrada
 * @param cell_number: celda afectada
 * @param ps: page or segment number
 * @param part: (aplica para segmentos) numero de parte del segmento
 */
void write_to_log(int type, int is_paging, int process_id, int cell_number, int ps, int part);

/**
 * @brief Agrega un proceso a un archivo con una lista de procesos
 * terminados por diferente razon
 * @param process_id: proceso que murio
 * @param filename: OFMPROC_FILENAME or ENDPROC_FILENAME
 */
void append_to_proc_file(int process_id, char * filename);

#endif //C_SYNCHRONIZATION_LOG_H
