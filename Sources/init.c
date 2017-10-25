#include "../Headers/init.h"

int init_main(int argc, char *argv[]) {

    if (argc < 2)
        exit_failure("Cantidad de argumentos inválida. \n");

    /* Cantidad de espacios de memoria */
    int mem_space_amount = (int) strtol(argv[1], 0, 10);

    /* Crear carpeta de configuraciones */
    mkdir_folder(CONFIG_FOLDER);

    /* Crear el archivo con la llave de la memoria compartida*/
    write_new_file(KEY_FILENAME, KEY_FILE_VALUE, 0);

    /* Crear el archivo con la llave de los procesos bloqueados */
    write_new_file(BLCKKEY_FILENAME, BLCKPROC_KEY, 0);

    /* Crear bitacora de actividad de procesos */
    write_new_file(LOG_FILENAME, " ", 0);

    /* Crear el archivo para procesos que no encontraron memoria */
    write_new_file(OFMPROC_FILENAME, " ", 0);

    /* Crear el archivo para procesos terminados */
    write_new_file(ENDPROC_FILENAME, " ", 0);

    /* Crear el archivo con la cantidad de espacios de memoria */
    write_new_file(MEMSIZE_FILENAME, (void * ) mem_space_amount, 1);

    /* Inicializacion de la memoria compartida ----------------------------------- */

    /* Reservar la memoria compartida */
    int shm_id = get_shm_id(KEY_FILENAME, mem_space_amount, IPC_CREAT);

    /* Alistar memoria para referenciarse */
    instance_memory_simulation(shm_id, mem_space_amount);

    /* Inicializacion de la lista de procesos bloqueados ------------------------ */

    /* Reservar la memoria compartida */
    int bp_id = get_shm_id(BLCKKEY_FILENAME, (size_t )(MAX_BLOCKED_P + 1) * sizeof(int), IPC_CREAT);

    /* Alistar memoria para referenciarse */
    instance_bp_list(bp_id, MAX_BLOCKED_P);

    /* Instanciar el semaforo de acceso a memoria ------------------------------- */
    sem_t * shm_sem = sem_open(SHM_SEM_NAME, O_CREAT, 0644, 1);

    if(shm_sem == SEM_FAILED)
        exit_failure("Error de inicializacion del semaforo de SHM.\n");

    printf("Id memoria: %d \n", shm_id);
    printf("Id memoria para procesos bloqueados: %d \n", bp_id);
    printf("Direccion del semáforo: %ld\n", (long) &shm_sem);

}