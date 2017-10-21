#include "../Headers/spy.h"


int spy_main(int argc, char **argv) {

    /* Información del proceso */
    __pid_t program_id = getpid();
    char * program_name = basename(argv[0]);

    printf("------------------ \n");
    printf("Programa Espía \n");
    printf("PID #%i \n", program_id);
    printf("Ejecutable: %s \n", program_name);

    /* Prueba para escribir en el log */
    write_to_log(ALLOCATION, PAGING, getpid(), 41);
    write_to_log(FAIL, PAGING, 345, 42);
    write_to_log(DEALLOCATION, PAGING, 321, 12);
    write_to_log(ALLOCATION, SEGMENTATION, 4345, 32);

    spy_shell();
    printf("------------------ \n");

    return EXIT_SUCCESS;

}

void spy_shell(){

    show_options();

    char option = 0;
    while(option != 'X'){
        option = (char) getchar();
        option = (char) toupper(option);
        exec_option(option);
    }

}

void show_options(){

    printf("------------------ \n");
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

    printf("------------------ \n");
    printf("Estado de la memoria: \n");

    /* shared memory vars */
    key_t key;
    int shm_id;
    int mem_size = read_file_int(MEMSIZE_FILENAME);

    /* Key */
    if ((key = ftok(KEY_FILENAME, 'R')) == -1)
    {
        perror("Error de generacion de la clave. \n");
        exit(-1);
    }

    /* Obtener el ID de la memoria compartida */
    if ((shm_id = shmget(key, mem_size, 0644)) == -1) {
        perror("Error de acceso a memoria. \n");
        exit(-1);
    }

    /* Cambiar esto si el indice cambia a empezar desde 0 */
    printf("Celda\tProceso\tParte\t\n");

    for(int i = 1; i < mem_size+1; i++){
        MCell * cell = read_shm_cell(shm_id, i);
        printf("%d\t\t%d\t\t%d\t\t\n", cell->cell_number, cell->held_proc_num, cell->held_proc_part);
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