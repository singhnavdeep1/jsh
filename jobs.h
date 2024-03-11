#ifndef JOBS_H
#define JOBS_H

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>

#include "parser.h"

#define MAX_JOBS 500

typedef enum state {
	DONE,
	KILLED,
	DETACHED,
	STOPPED,
	RUNNING,
	unknown
} state;

typedef struct process {
	struct process *next;
	pid_t pid;
	int status; //-1 tant que il a pas été initialisé
	char *cmd;
	w_index *cmd_index;
} process;   


typedef struct job {
	int pgid;
	int id;
	int fg;
	process *pipeline;
	state state;
	char *cmd;
} job;

int update_jobs(FILE *, job **);
int update_job(FILE*, job **, job *, int);
int print_jobs(job **);
int exist(job **jobs, job *j);
job *exec_command(char *, w_index *, int, job **);
void free_process(process *);
void free_job(job *);
void free_jobs(job **);
void remove_job(job **, job *);
int count_jobs(job **);
int are_jobs_running(job **);

#endif
