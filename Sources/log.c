#include "../Headers/log.h"

void append_to_proc_file(int process_id, char * filename) {

    static int i = 1;
    FILE * file = fopen(filename, "a");
    fprintf(file, "%d%c", process_id, ((i++)%16) ? ' ': '\n');
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
    fprintf(file, "[%d:%d:%d] ", time_info->tm_hour, time_info->tm_min, time_info->tm_sec);
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
        fprintf(file, "No se ha podido ubicar en memoria, no hay disponibles %d espacios de memoria \n", cell_number);

    fclose(file);

}

char * read_log() {

    /* Abrimos el archivo */
    FILE * file = fopen(LOG_FILENAME, "r");
    size_t file_size = (size_t) get_fd_size(fileno(file));

    /* Se copia el contenido del archivo en memoria */
    char * log_string = malloc(file_size);
    memset(log_string, 0, file_size);

    char * buffer = malloc(FILENAME_MAX);
    while(fgets(buffer, FILENAME_MAX, file) != 0)
        strcat(log_string, buffer);

    fclose(file);
    return log_string;

}
