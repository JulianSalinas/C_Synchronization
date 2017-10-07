#ifndef LIST_H
#define LIST_H

#include "../main.h"

typedef struct List List;
typedef struct Node Node;

/**
 * @brief The Node struct, unidad de la lista
 */
struct Node {
    Node * next, * prev;
    void * content;
};

/**
 * @brief The List struct, lista doblemente enlazada, permite guardar
 * datos de cualquier tipo por medio de un long que indica
 * la dirección de memoria.
 */
struct List {
    Node * start, * end;
    long size;
};

/**
 * @brief new_node, constructor del nodo
 * @return puntero al nodo creado
 */
Node * new_node(void *);

/**
 * @brief new_list, constructor de la lista
 * @return puntero a la lista creada
 */
List * new_list();

/**
 * @brief add, añade un nodo al final de la lista
 * @param list, lista a la que se va agregar el nodo
 * @param info, informacion que contendra el nodo a crear
 */
void add(List * list, void * info);

/**
 * @brief add, añade un nodo al inicio de la lista
 * @param list, lista a la que se va agregar el nodo
 * @param info, informacion que contendra el nodo a crear
 */
void push(List * list, void * info);

/**
 * @brief peek, retorna el último elemento agregado pero si desvincularlo de la lista
 * @param list, lista del nodo a ver
 * @return
 */
void * peek(List * list);

/**
 * @brief pop, borra el elemento en el tope
 * @param list, lista donde se borrara el nodo
 * @return
 */
void * pop(List * list);

/**
 * @brief print_list, convierte la lista en un string
 * @param list, lista a convertir
 * @param func, funcion para imprimir el nodo
 */

int print_list(List * list, int (* func)(const char *, ...));


#endif // LIST_H
