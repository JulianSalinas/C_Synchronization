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

    int is_paging_ = strcmp(read_file_string(ALGORITHM_FILENAME), "pag") == 0 ? 1 : 0;

    int shm_id = get_shm_id(KEY_FILENAME, mem_size, 0);

    if (is_paging_) {
        printf("%-8s %-8s %-8s\n", "Espacio", "Proceso", "Página");
        for (int i = 1; i <= mem_size; i++) {
            MCell *cell = read_shm_cell(shm_id, i);
            if (cell->held_proc_num != -1)
                printf("%-8d %-8d %-8d \n",
                       cell->cell_number,
                       cell->held_proc_num,
                       cell->held_proc_ps);
            else printf("%-8d %-8s %-8s \n", cell->cell_number, "-", "-");

        }
    }
    else {
        printf("%-8s %-8s %-8s %-8s\n", "Espacio", "Proceso", "Segmento", "Parte");
        for (int i = 1; i <= mem_size; i++) {
            MCell *cell = read_shm_cell(shm_id, i);
            if (cell->held_proc_num != -1)
                printf("%-8d %-8d %-8d %-8d\n",
                       cell->cell_number,
                       cell->held_proc_num,
                       cell->held_proc_ps,
                       cell->held_proc_subpart);
            else printf("%-8d %-8s %-8s %-8s\n", cell->cell_number, "-", "-", "-");
        }
    }
}

void spy_algorithm(){
    PRINTLINE
    char * algorithm = read_file_string(ALGORITHM_FILENAME);
    printf("Algoritmo utilizado: %s\n", algorithm);
}

void spy_processes_state(){

    int cell_amount = read_file_int(MEMSIZE_FILENAME);
    int shm_id = get_shm_id(KEY_FILENAME, cell_amount, 0);
    int bp_id = get_shm_id(BLCKKEY_FILENAME, sizeof(int) * (MAX_BLOCKED_P+1), 0);

    PRINTLINE
    printf("Estado de los procesos:\n\n");

    /* Procesos en memoria actualmente --------------------------------------------- */
    List * procs_in_shm = new_list();

    printf("1. Procesos en memoria actualmente: \n");
    for (int i = 1; i <= cell_amount; i++){
        MCell * current_cell = read_shm_cell(shm_id, i);

        /* Revisar que no esta vacia */
        if (current_cell->held_proc_num != -1){
            /* Revisar que no se haya impreso ya */
            if (list_contains_int(procs_in_shm, current_cell->held_proc_num) == 0){
                printf("%-4d%s", current_cell->held_proc_num, ((i+1)%12) ? "": "\n");
                /* Agregar a la lista de encontrados */
                push(procs_in_shm, (void *) (long) current_cell->held_proc_num);
            }
        }
    }

    if (procs_in_shm->start == NULL) printf("Ninguno.");
    printf("\n");

    /* Liberar la lista */
    free(procs_in_shm);

    /* Proceso unico que busca espacio para asignar -------------------------------- */
    printf("\n");
    printf("2. Unico proceso buscando espacio para asignacion: \n");
    int uniq_pid = get_searching_pid(shm_id, cell_amount);
    if (uniq_pid != -1 && uniq_pid != 0)
        printf("%-4d", uniq_pid);
    else
        printf("Ninguno.");

    printf("\n");

    /* Procesos actualmente bloqueados esperando por semaforo ---------------------- */
    int * bp_address = shmat(bp_id, NULL, 0);

    printf("\n");
    printf("3. Procesos actualmente bloqueados: \n");

    if(bp_address[0] == MAX_BLOCKED_P)
        printf("Ninguno.");
    else {
        int column = 0;
        for (int i = 1; i < MAX_BLOCKED_P; i++){
            if(bp_address[i] != -1)
                printf("%-4d%s", bp_address[i], (((column++)+1) % 12) ? "" : "\n");
        }
    }

    shmdt(bp_address);

    printf("\n");

    /* Procesos que han muerto por no haber espacio -------------------------------- */
    char * f_content;
    printf("\n");
    printf("4. Procesos muertos por falta de espacio: \n");
    f_content = read_file_string(OFMPROC_FILENAME);

    List * dead_procs = tokenizer(f_content, " ");

    if(dead_procs->size == 0)
        printf("Ninguno.");
    else{
        int column = 0;
        Node * tmp_deap_proc = dead_procs->start;
        while(tmp_deap_proc != NULL){
            printf("%-4s%s", (char*) tmp_deap_proc->content, (((column++)+1) % 12) ? "" : "\n");
            tmp_deap_proc = tmp_deap_proc->next;
        }
    }

    printf("\n");

    /* Procesos que terminaron su ejecucion satisfactoriamente --------------------- */
    printf("\n");
    printf("5. Procesos ejecutados correctamente: \n");
    f_content = read_file_string(ENDPROC_FILENAME);

    List * finished_procs = tokenizer(f_content, " ");

    if(finished_procs->size == 0)
        printf("Ninguno.");
    else{
        int column = 0;
        Node * tmp_finished_proc = finished_procs->start;
        while(tmp_finished_proc != NULL){
            printf("%-4s%s", (char*) tmp_finished_proc->content, (((column++)+1) % 12) ? "" : "\n");
            tmp_finished_proc = tmp_finished_proc->next;
        }
    }

    printf("\n\n");
}

void spy_log_file(){
    PRINTLINE
    printf("Bitácora: \n");
    printf("%s", read_file_string(LOG_FILENAME));
}