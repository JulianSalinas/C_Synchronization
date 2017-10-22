#include "../../Headers/Util/util.h"

void print_memory(char * begin, size_t offset) {
    for (int i = 0; i < offset; i++){
        printf("%02x%c", begin[i], ((i+1)%16) ? ' ' : '\n');
    }
    printf("\n");
}

void copy_content(int from_fd, int to_fd){

    char buffer[BUFFER_SIZE] = {0};
    memset(buffer, 0, BUFFER_SIZE);

    ssize_t written_bytes = read(from_fd, buffer, BUFFER_SIZE);
    while(written_bytes) {
        write(to_fd, buffer, (size_t) written_bytes);
        written_bytes = read(from_fd, buffer, BUFFER_SIZE);
    }

}

int is_valid_name(char * name, char bad_chars[]){

    for (int i = 0; i < strlen(bad_chars); ++i)
        if(strchr(name, bad_chars[i]) != 0) return 0;

    return 1;

}

int is_valid_filename(char * filename){

    char bad_chars[] = "!@%^*~|";
    return is_valid_name(filename, bad_chars);

}

int is_valid_folder(char * foldername){

    char bad_chars[] = "!@%^*~|\\#.";
    return is_valid_name(foldername, bad_chars);

}

char * get_homedir(){

    char * homedir = getenv("HOME");
    if(homedir == 0 ) homedir = getpwuid(getuid())->pw_dir;
    return homedir;

}

char * map_folderpath_to_homedir(char * folderpath){

    char * homedir = get_homedir();

    if(!strstr(folderpath, "home/")){
        strcat(homedir, "/");
        strcat(homedir, folderpath);
        strcat(homedir, "/");
    }

    return homedir;
}

int mkdir_folder(char * foldername){

    if(!is_valid_folder(foldername))
        return 0;

    struct stat st = {0};
    if(stat(foldername, &st) < 0)
        return mkdir(foldername, 0700) == 0;
    return 1;

}

char * get_absolute_filename(char *base_folder, char *filename){

    char * absolute_path = malloc(FILENAME_MAX);
    strcpy(absolute_path, base_folder);
    strcat(absolute_path, filename);
    return absolute_path;

}

char * get_temporal_filename(char *filepath){

    char * tmp_path = malloc(FILENAME_MAX);
    strcat(tmp_path, filepath);
    strcat(tmp_path, ".tmp-XXXXXX");
    return tmp_path;

}

void * tokenizer(char * string, char * delim){

    void * tokens = new_list();
    char * current_token = strtok(string, delim);
    while(current_token){
        add(tokens, current_token);
        current_token = strtok(NULL, delim);
    }
    return tokens;

}

off_t get_fd_size(int file_fd){

    struct stat st;
    fstat(file_fd, &st);
    return st.st_size;

}

void show_file(char * filename){

    char * command = malloc(FILENAME_MAX + 10);
    sprintf(command, "xdg-open %s", filename);
    system(command);

}

void write_new_file(char * filename, void * content, int contentType){

    FILE * file_pointer = fopen(filename, "w");

    if(file_pointer == NULL){
        printf("Fallo la creacion del archivo %s", filename);
        exit(-1);
    }

    if (contentType == 0)
        fprintf(file_pointer, "%s", content);
    else if (contentType == 1)
        fprintf(file_pointer, "%d", content);

    fclose(file_pointer);
}

int read_file_int(char * filename){

    int value;
    FILE * file_pointer = fopen (filename, "r");

    if(file_pointer == NULL){
        printf("Fallo la lectura del archivo %s", filename);
        exit(-1);
    }

    fscanf(file_pointer, "%d", &value);

    fclose(file_pointer);

    if (value == NULL){
        printf("El archivo %s se encuentra vacio.", filename);
        exit(-1);
    }

    return value;
}

char * read_file_string(char * filename) {

    /* Abrimos el archivo */
    FILE * file = fopen(filename, "r");

    if(file == 0)
        return 0;

    size_t file_size = (size_t) get_fd_size(fileno(file));

    /* Se copia el contenido del archivo en memoria */
    char * log_string = malloc(file_size);
    memset(log_string, 0, file_size);

    char * buffer = malloc(FILENAME_MAX);
    while(fgets(buffer, FILENAME_MAX, file) != 0)
        strcat(log_string, buffer);

    fclose(file);
    return log_string;

}


void exit_failure(char * format, ...) {
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
    printf("---------------------- \n");
    exit(EXIT_FAILURE);
}
