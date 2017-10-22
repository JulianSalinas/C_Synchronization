#include "../Headers/spy.h"


int spy_main(int argc, char **argv) {

    show_options();

    char option = 0;
    while(option != 'X'){
        option = (char) getchar();
        option = (char) toupper(option);
        exec_option(option);
    }

    return EXIT_SUCCESS;

}

void show_options(){

    PRINTLINE
    printf("Opciones: \n");
    printf("A. Estado de la memoria \n");
    printf("B. Estado de los procesos \n");
    printf("C. Mostrar bitácora \n");
    printf("X. Salir del programa \n");
    printf("Ingrese una opción: ");

}

void exec_option(char option){

    if (option == 'A' || option == 'B' || option == 'C'){
        option == 'A' ? spy_memory_state() :
        option == 'B' ? spy_processes_state() :
        spy_log_file();
        show_options();
    }

}

void spy_memory_state(){

    PRINTLINE
    printf("Estado de la memoria: \n");

    int mem_size = read_file_int(MEMSIZE_FILENAME);

    /* Obtener llave del archivo */
    key_t key = ftok(KEY_FILENAME, 'R');
    if (key == -1)
        exit_failure("Error de generacion de la clave. \n");

    /* Obtener el ID de la memoria compartida */
    int shm_id = shmget(key, (size_t) mem_size, 0644);
    if (shm_id == -1)
        exit_failure("Error de acceso a memoria. \n");

    for(int i = 1; i < mem_size+1; i++){
        MCell * cell = read_shm_cell(shm_id, i);
        printf("Espacio: #%d\t Proceso: %d\t Parte: %d\t \n",
               cell->cell_number,
               cell->held_proc_num,
               cell->held_proc_part);
    }

}

void spy_processes_state(){
    printf("------------------ \n");
    printf("Ver estado de los procesos no implementado\n");
}

void spy_log_file(){
    printf("------------------ \n");
    printf("Bitácora: \n");
    printf("%s", read_log());
}