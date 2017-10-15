#include "../Headers/init.h"

#define SHM_NAME "/shm"
#define SIZE 10


void * init_main(int argc, char **argv) {

    /* Información del proceso */
    __pid_t program_id = getpid();
    char * program_name = basename(argv[0]);
    printf("PID #%i \n", program_id);
    printf("Nombre: %s \n", program_name);

    /* Revisamos que se haya especficado la cantidad de memoria */
    if(argc != 2){
        printf("El proceso #%i ha terminado porque ", program_id);
        printf("falta como argumento el número de espacios de memoria \n");
        exit(EXIT_FAILURE);
    }

    /* Obtenemos desde el argumento los espacios de memoria que se usarán */
    size_t mem_space = (size_t) strtol(argv[1], 0, 10) * SIZE;
    printf("Espacios de memoria solicitados: %lu \n", mem_space / SIZE);

    /* Creamos el espacio compartido como si fuera un archivo */
    printf("Inicializando memoria \n");
    int shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);

    /* Revisamos que no haya error obteniendo el fd */
    if (shm_fd == -1) {
        printf("Error al obtener el file descriptor para la memoria compartida: %s\n", strerror(errno));
        exit(1);
    }

    /* Se configura el tamaño de la memoria compartida */
    ftruncate(shm_fd, mem_space);

    /* Se mapea el segmento de memoria compartido a la dirección del proceso */
    char * shm_base = mmap(0, mem_space, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

    /* Se testea que en la memoria este el "HOLA" = 48 4F 4C 41 */
    sprintf(shm_base, "%s", "HOLA");
    printf("Estado de la memoria: \n");
    print_memory(shm_base, mem_space);

    /* Revisamos que no haya un error al realizar el mapeo */
    if (shm_base == MAP_FAILED) {
        printf("Error al mapear el segmento de memoria compartido al ");
        printf("espacio de direcciones del proceso: %s\n", strerror(errno));
        exit(1);
    }

    /* Se remueve el segmento de memoria compartido de espacio de direcciones del proceso */
    if (munmap(shm_base, SIZE) == -1) {
        printf("Error al remover el segmento de memoria compartido del ");
        printf("espacio de direcciones del proceso: %s\n", strerror(errno));
        exit(1);
    }

    /* Cerramos el espacio compartido como si fuera un archivo */
    if (close(shm_fd) == -1) {
        printf("Error al cerrar el file descriptor para la memoria compartida: %s\n", strerror(errno));
        exit(1);
    }

    /* Desvinculamos la memoria compartida */
    if (shm_unlink(SHM_NAME) == -1) {
        printf("Error al desvincular la memoria compartida: %s\n", strerror(errno));
        exit(1);
    }

    return EXIT_SUCCESS;

}
