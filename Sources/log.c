#include <time.h>
#include "../Headers/log.h"

void write_to_log(int type, int algoritm, __pid_t process_id, int cells_amount) {

    /* Obtenemos la hora del sistema */
    time_t current_time;
    time(&current_time);
    struct tm * time_info = localtime(&current_time);

    /* Si es la primera vez que se llama la función, se tiene que crear un nuevo log */
    char * mode = "a";
    static int first_exec = 1;
    if (first_exec) { mode = "w"; first_exec--; };
    FILE * file = fopen(LOG_FILENAME, mode);

    /* Buffer para colocar el mensaje que se va a escribir en el archivo */
    char * msg = malloc(FILENAME_MAX);
    memset(msg, 0, FILENAME_MAX);

    /* Se coloca la hora y el proceso en el mensaje */
    fprintf(file, "[%d:%d:%d] ", time_info->tm_hour, time_info->tm_min, time_info->tm_sec);
    fprintf(file, "Proceso %i: ", process_id);

    /* Se escribe el mensaje en la bitacora*/
    if(type == ALLOCATION)
        fprintf(file, "Se han asignado %d celdas de memoria\n", cells_amount);
    else if(type == DEALLOCATION)
        fprintf(file, "Se han de-asignado %d celdas de memoria\n", cells_amount);
    else if(type == FAIL)
        fprintf(file, "No se ha podido ubicar en memoria, necesita %d celdas de memoria\n", cells_amount);

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
