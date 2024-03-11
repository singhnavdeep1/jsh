#ifndef INTERNES_H
#define INTERNES_H

#include "jobs.h"
#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

extern int cd(w_index *);
extern int pwd();
extern int return_code();
extern int p_jobs(w_index *);
extern int send_signal(int signal, int target, int job_or_not);
extern int exit_shell(w_index *);
extern int kill_job(w_index *);
extern int bg_cmd(w_index *);
extern int fg_cmd(w_index *);

#endif
