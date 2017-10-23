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
    printf("D. Algoritmo utilizado \n");
    printf("X. Salir del programa \n");
    printf("Ingrese una opción: ");

}

void exec_option(char option){

    if (option == 'A' || option == 'B' || option == 'C' || option == 'D'){
        option == 'A' ? spy_memory_state() :
        option == 'B' ? spy_processes_state() :
        option == 'C' ? spy_log_file():
        spy_algorithm();
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
        if (cell->held_proc_num != -1)
            printf("Espacio: #%d\t Proceso: %d\t Parte: %d\t \n",
                   cell->cell_number,
                   cell->held_proc_num,
                   cell->held_proc_part);
        else
            printf("Espacio: #%d\t Sin asignar \n", cell->cell_number);

    }

}

void spy_algorithm(){
    PRINTLINE
    char * algorithm = read_file_string(ALGORITHM_FILENAME);
    printf("Algoritmo utilizado: %s\n", algorithm);
}

void spy_processes_state(){

    int mem_size = read_file_int(MEMSIZE_FILENAME);
    int cell_amount = mem_size / MEMSPACE_SIZE;

    /* Obtener llave del archivo */
    key_t key = ftok(KEY_FILENAME, 'R');
    if (key == -1)
        exit_failure("Error de generacion de la clave. \n");

    /* Obtener el ID de la memoria compartida */
    int shm_id = shmget(key, (size_t) mem_size, 0644);
    if (shm_id == -1)
        exit_failure("Error de acceso a memoria. \n");

    PRINTLINE
    printf("Estado de los procesos:\n\n");

    /*
     *
     * El PID de los procesos que estén bloqueados (esperando por la región critica) (punto 1 o 6 del
proceso)
 El PID de los procesos que han muerto por no haber espacio suficiente.
 El PID de los procesos que ya terminaron su ejecución*/

    /* Procesos en memoria actualmente */
    List * procs_in_shm = new_list();

    printf("1. Procesos en memoria actualmente: ");
    for (int i = 1; i <= cell_amount; i++){
        MCell * current_cell = read_shm_cell(shm_id, i);

        /* Revisar que no esta vacia */
        if (current_cell->held_proc_num != -1){
            /* Revisar que no se haya impreso ya */
            if (list_contains_int(procs_in_shm, current_cell->held_proc_num) == 0){
                printf("#%d ", current_cell->held_proc_num);
                /* Agregar a la lista de encontrados */
                push(procs_in_shm, (void *) (long) current_cell->held_proc_num);
            }
        }
    }
    if (procs_in_shm->start == NULL) printf("Ninguno.");
    /* Liberar la lista */
    free(procs_in_shm);

    /* Proceso unico que busca espacio para asignar */
    printf("\n");
    printf("2. Unico proceso buscando espacio para asignacion: ");
    int uniq_pid = get_searching_pid(shm_id, cell_amount);
    if (uniq_pid != -1 && uniq_pid != 0)
        printf("#%d", uniq_pid);
    else
        printf("Ninguno.");

    /* Procesos que han muerto por no haber espacio */
    char * f_content;
    printf("\n");
    printf("3. Procesos muertos por falta de espacio:");
    f_content = read_file_string(OFMPROC_FILENAME);
    if (strcmp(f_content, " ") != 0)
        printf("%s", f_content);
    else
        printf(" Ninguno.");

    /* Procesos que terminaron su ejecucion satisfactoriamente */
    printf("\n");
    printf("4. Procesos ejecutados correctamente:");
    f_content = read_file_string(ENDPROC_FILENAME);
    if (strcmp(f_content, " ") != 0)
        printf("%s", f_content);
    else
        printf(" Ninguno.");

    printf("\n\n");
}

void spy_log_file(){
    PRINTLINE
    printf("Bitácora: \n");
    printf("%s", read_file_string(LOG_FILENAME));
}