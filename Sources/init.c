#include "../Headers/init.h"

/* Llave para la memoria compartida */
#define KEY_FOR_FILE "V40BUW3K53L"


int init_main(int argc, char *argv[]) {

    printf("Programa Inicializador \n");
    printf("---------------------- \n");

    if (argc < 2){
        printf("Cantidad de argumentos inválida. \n");
        exit(-1);
    }

    /* Cantidad de espacios de memoria */
    long mem_sp_amount = (int) strtol(argv[1], 0, 10);

    /* Variables de memoria compartida */
    key_t shm_key;
    int shm_id; /* Shared memory ID */
    char *data; /* Puntero al espacio de memoria */

    /* Variables de semaforo */
    sem_t * key_sem;

    /* Crear el archivo con la llave */
    write_new_file(KEY_FILENAME, KEY_FOR_FILE);

    /* Obtener llave del archivo */
    if ((shm_key = ftok(KEY_FILENAME, 'R')) == -1)
    {
        perror("Error de generacion de la clave. \n");
        exit(-1);
    }

    /* Reservar la memoria compartida */
    if ((shm_id = shmget(shm_key, SHM_SIZE, 0644 | IPC_CREAT)) == -1) {
        perror("Error de reserva de memoria. \n");
        exit(-1);
    }

    printf("Id memoria: %d", shm_id);

    /* Memoria lista para referenciarse */
}