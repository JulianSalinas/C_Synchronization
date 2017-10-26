#include "../Headers/log.h"

void append_to_proc_file(int process_id, char * filename) {

    FILE * file = fopen(filename, "a");
    fprintf(file, "%d ", process_id);
    fclose(file);

}

void write_to_log(int type, int is_paging, int process_id, int cell_number, int ps, int part) {

    /* Obtenemos la hora del sistema */
    time_t current_time;
    time(&current_time);
    struct tm * time_info = localtime(&current_time);

    /* Si es la primera vez que se llama la función, se tiene que crear un nuevo log */
    char * mode = "a";
    static int first_exec = 1;
    if (first_exec) { mode = "w"; first_exec--; };
    FILE * file = fopen(LOG_FILENAME, mode);

    /* Se coloca la hora y el proceso en el mensaje */
    fprintf(file, "[%-2d:%-2d:%-2d] ", time_info->tm_hour, time_info->tm_min, time_info->tm_sec);
    fprintf(file, "Proceso %i: ", process_id);

    /* Se escribe el mensaje en la bitacora*/
    if(type == ALLOCATION) {
        if (is_paging)
            fprintf(file, "Se asigna el espacio de memoria #%d para la pagina %d\n", cell_number, ps);
        else
            fprintf(file, "Se asigna el espacio de memoria #%d para el segmento %d parte %d\n", cell_number, ps, part);
    }
    else if(type == DEALLOCATION)
        fprintf(file, "Se libera el espacio de memoria #%d \n", cell_number);
    else if(type == FAIL)
        if (is_paging)
            fprintf(file, "No se ha podido ubicar en memoria, no hay disponibles %d espacios de memoria \n", cell_number);
        else
            fprintf(file, "No se ha podido ubicar en memoria, no hay disponibles %d espacios de memoria o no pueden acomodarse los segmentos \n", cell_number);

    fclose(file);

}