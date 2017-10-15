#include "../Headers/main.h"
#include "../Headers/init.h"

/**
 *
 * Exiten 4 ejecutables, cada uno de ellos esta configurado en el archivo CMakeLists.txt
 * Cada uno de los ejecutables usa la misma función main. La función main ejecuta la función
 * apropiada según el nombre del ejecutable invocado.Estos se pueden usar como a continuación:
 *
 * >> ./Init cant_espacios_memoria (int)    ->      # Inicializa la memoria
 * >> ./Prod tipo_algoritmo (-seg, -pag)    ->      # Produce procesos (threads)
 * >> ./Spy (Shell interactivo)             ->      # Consulta estado de procesos y memoria
 * >> ./Fin                                 ->      # Finaliza bitacora y recursos
 *
 * Nota: A pesar de que se trata de la misma función para todos, cada proceso aparece con
 * nombre distinto. Se puede comprobar usando el comando > ps -a -T. Si se corre desde el
 * editor los procesos no van a aparecer.
 *
 */

int main(int argc, char *argv[]) {

    char * program_name = basename(argv[0]);

    if(strcmp(program_name, "Init") == 0)
        init_main(argc, argv);

    else if(strcmp(program_name, "Prod") == 0)
        printf("Prod sin implementar \n");

    else if(strcmp(program_name, "Spy") == 0)
        printf("Spy sin implementar \n");

    else if(strcmp(program_name, "Fin") == 0)
        printf("Fin sin implementar \n");

    printf("Presione ENTER para finalizar: ");
    getchar();
    return EXIT_SUCCESS;

}