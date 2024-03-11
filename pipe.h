#ifndef PIPE_H
#define PIPE_H

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include "parser.h"

int count_pipe(w_index *);
void pos_pipe(w_index *, int *);
void get_cmds_pipe(w_index *, w_index **, int);


#endif

