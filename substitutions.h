#ifndef SUBSTITUTIONS_H
#define SUBSTITUTIONS_H

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "parser.h"

int count_substitutions(w_index *);
void pos_open_sub(w_index *,int *);
void pos_close_sub(w_index *,int *);
void print_tab(int *,int);
void get_cmds_sub(w_index **, w_index *, int *, int *, int );
int launch_cmd(w_index *);
int launch_test(w_index *);
int is_substituted(w_index *);
w_index * substitute(w_index *);

#endif


