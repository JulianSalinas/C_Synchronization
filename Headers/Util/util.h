
#ifndef C_SERVERS_TESTS_H
#define C_SERVERS_TESTS_H

#include "../main.h"
#include <pwd.h>
#include <sys/stat.h>

#define BUFFER_SIZE 2048

/**
 * Imprime un trozo de memoria de forma hexadecimal
 * @param begin: puntero al inicio de donde se debe imprimir
 * @param offset: cantidad de bytes a imprimir
 */
void print_memory(char * begin, size_t offset);

/**
 * Función que copia el contenido de un stream a otro stream
 * por medio de chuncks (fragmentos). Puede ser de un archivo
 * a otro, de socket a socket, de socket a archivo o viceversa
 * @param from_fd: Descriptor de donde vienen los datos
 * @param to_fd: Descriptor para donde se copian los datos
 * @return
 */
void copy_content(int from_fd, int to_fd);

/**
 * Determina si un nombre es válido con base a un conjunto
 * de carácteres que se consideren invalidos
 * @param name: Nombre que se desea validar
 * @param bad_chars: Carácteres invalidos
 * @return 1 si es nombre es válido, 0 sino
 */
int is_valid_name(char * name, char bad_chars[]);

/**
 * Determina si el nombre de un archivo es válido por medio
 * de la función is_valid_name(2) y los caracteres !@%^*~|
 * @param filename: Nombre del archivo que se desea validar
 * @return 1 si es nombre es válido, 0 sino.
 */
int is_valid_filename(char * filename);

/**
 * Determina si el nombre de un archivo es válido por medio
 * de la función is_valid_name(2) y los caracteres !@%^*~|\#.
 * @param filename: Nombre del archivo que se desea validar
 * @return 1 si es nombre es válido, 0 sino.
 */
int is_valid_folder(char * foldername);

/**
 * Obtiene la dirección de la carpeta del usuario que este en
 * sesión. Por ejemplo: /home/julian/
 * @return ruta de la carpeta home del usuario
 */
char * get_homedir();

/**
 * Concatena la dirección home con la del folder que se
 * especifique. Si ya la posee no realiza ninguna acción
 * Ejemplo: /home/julian/ * /folder/test/ = /home/julian/folder/test/
 * @return concatenación de homedir con folderpath
 */
char * map_folderpath_to_homedir(char * folderpath);

/**
 * Crear una carpeta con el nombre especificado
 * @param foldername: ruta absoluta del folder
 * @return 1 si se creó, 0 sino.
 */
int mkdir_folder(char * foldername);

/**
 * Concatena el nombre del folder con un nombre de archivo
 * para asi obtener la ruta absoluta de dicho archivo
 * @return ruta absoluta del archivo
 */
char * get_absolute_filename(char * base_folder, char *filename);

/**
 * Con base a una ruta absoluta de un archivo, crea un nombre
 * único para un archivo temporal.
 * Ejemplo: get_temporal_filename("/home/test1.txt) -> /home/test1.txt.tmp-456fer
 * @return ruta absoluta del archivo
 */
char * get_temporal_filename(char * filepath);

/**
 * Separa una cadena en varios tokens
 * @param string: cadena a separar
 * @param delim: delimitador
 * @return List con punteros a los tokens
 */
void * tokenizer(char * string, char * delim);

/**
 * Dado un archivo descriptor, retorna el tamaño del contenido
 * @param file_fd: archivo descriptor del stream
 * @return off_t que representa el tamaño del contenido
 */
off_t get_fd_size(int file_fd);

/**
 * Ejecuta una función del sistema para abrir el archivo
 * con el programa predeterminado según el usuario en sesión
 * @param filename: Nombre del archivo para abrir
 */
void show_file(char * filename);

/**
 * Escribe o sobreescribe en caso de existir, un nuevo archivo
 * con el contenido especificado.
 * @param filename: Nombre del archivo
 * @param content: Contenido a escribir
 */
void write_new_file(char * filename, char * content);

#endif //C_SERVERS_TESTS_H
