#include "../Headers/spy.h"

int spy_main(int argc, char **argv) {

    /* Información del proceso */
    __pid_t program_id = getpid();
    char * program_name = basename(argv[0]);

    printf("------------------ \n");
    printf("Programa Espía \n");
    printf("PID #%i \n", program_id);
    printf("Ejecutable: %s \n", program_name);
    printf("------------------ \n");

    /* Prueba para escribir en el log */
    write_to_log(ALLOCATION, PAGING, getpid(), 41);
    write_to_log(FAIL, PAGING, 345, 42);
    write_to_log(DEALLOCATION, PAGING, 321, 12);
    write_to_log(ALLOCATION, SEGMENTATION, 4345, 32);

    /* Prueba para leer del log */
    printf("%s", read_log());

}
