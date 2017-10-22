# Proyecto II: Sistemas Operativos
__________________________________


##### Existen 4 ejecutables, cada uno se puede usar de la siguiente manera:

Inicializa la memoria y los demás recursos compartidos
> ./Init cant_espacios_memoria (int)

Produce procesos emulados por medio de hilos
> ./Prod tipo_algoritmo (-seg, -pag)

Consulta estado de procesos y memoria
> ./Spy (Shell interactivo)            

Finaliza bitácora y los demás recursos
> ./Fin                            

Nota: A pesar de que se trata de la misma función para todos, cada proceso aparece con
nombre distinto. Se puede comprobar usando el comando > ps -a -T. Si se corre desde el
editor los procesos no van a aparecer.     