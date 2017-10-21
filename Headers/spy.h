#ifndef C_SYNCHRONIZATION_SPY_H
#define C_SYNCHRONIZATION_SPY_H

#include "main.h"

/* Function definitions */
int spy_main(int argc, char *argv[]);

void spy_shell();

void show_options();

void exec_option(char option);

void spy_log_file();

void spy_memory_state();

void spy_processes_state();

#endif //C_SYNCHRONIZATION_SPY_H
