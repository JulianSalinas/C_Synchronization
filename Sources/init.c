#include "../Headers/init.h"

/* Llave para la memoria compartida */
#define KEY_FILE_VALUE "V40BUW3K53L"


int init_main(int argc, char *argv[]) {

    if (argc < 2)
        exit_failure("Cantidad de argumentos inválida. \n");

    /* Cantidad de espacios de memoria */
    int mem_space_amount = (int) strtol(argv[1], 0, 10);

    /* Crear carpeta de configuraciones */
    mkdir_folder(CONFIG_FOLDER);

    /* Crear el archivo con la llave */
    write_new_file(KEY_FILENAME, KEY_FILE_VALUE, 0);

    /* Crear el archivo con la cantidad de espacios de memoria */
    write_new_file(MEMSIZE_FILENAME, (void * )(MEMSPACE_SIZE * mem_space_amount), 1);

    /* Obtener llave del archivo */
    key_t shm_key = ftok(KEY_FILENAME, 'R');
    if (shm_key  == -1)
        exit_failure("Error de generacion de la clave. \n");

    /* Reservar la memoria compartida */
    int shm_id = shmget(shm_key, (size_t )(MEMSPACE_SIZE * mem_space_amount), 0644 | IPC_CREAT);
    if (shm_id == -1)
        exit_failure("Error de reserva de memoria. \n");

    /* Alistar memoria para referenciarse */
    instance_memory_simulation(shm_id, mem_space_amount);

    /* Instanciar el semaforo de acceso a memoria */
    sem_t * shm_sem = sem_open(SHM_SEM_NAME, O_CREAT, 0644, 1);

    if(shm_sem == SEM_FAILED){
        perror("Error de inicializacion del semaforo de SHM.\n");
        exit(EXIT_FAILURE);
    }

    printf("Id memoria: %d \n", shm_id);

}