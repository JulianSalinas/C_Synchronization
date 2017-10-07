#include "../../Headers/Util/util.h"

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
