#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "parser.h"
#include "internes.h"
#include "prompt.h"
#include "jobs.h"
#include "shell.h"
#include "redirections.h"
#include "substitutions.h"
#include "pipe.h"

int main() {

	int in=dup(0);
	int out=dup(1);
	int err_out=dup(2);
	char *tmp_inp;
	char input[2000];
	w_index *index;
	char prompt[256];
	int res=update_prompt(prompt, 0);
	rl_initialize();
	rl_outstream=stderr;
	ignore_signals();
	extern job *jobs[MAX_JOBS];
	memset(jobs, 0, MAX_JOBS * sizeof(job *));
	int fg = 1;
	extern int ret_code;
	ret_code = 0;

	setenv("?", "0", 1);

	while((tmp_inp = readline(prompt)) != NULL) {
		strcpy(input, tmp_inp);
		free(tmp_inp);
		if(res) {
			exit(10);
		}
		add_history(input);
		index = split_space(input);
		current_index=index;

		if(index->size != 0) {
			int n=is_substituted(index);
			if(n==-1 || n==-2){ 
				if(n==-2) fprintf(stderr,"Syntax error : substitutions\n");
				goto end_loop;
			}
			if(n>0) {
				w_index *ti=substitute(index);
				if(ti==NULL) goto end_loop;
				free_index(index);
				index=ti;
			}


			if(!strcmp(index->words[index->size - 1], "&")) {
				fg = 0;
				w_index *tmp=index;
				index = sub_index(index, 0, index->size - 1);
				free_index(tmp);
				current_index=index;
			}
			int status;
			job *j = exec_command(input, index, fg, jobs);
			if(j==NULL){
				ret_code=1;
				goto end_loop;
			}
			if(fg) {
				if(exist(jobs, j)) {
					do {
						update_job(stderr, jobs, j, j->id);
					} while(j->state == RUNNING && j->fg);
					if(j->fg) {
						status = j->pipeline->status;
						if(WIFEXITED(status)) {
							ret_code = WEXITSTATUS(status);
						}

						remove_job(jobs, j);
					}
					tcsetpgrp(STDERR_FILENO,getpid());
				}
			}
		}
end_loop:
		char buff[8];
		sprintf(buff, "%d", ret_code);
		setenv("?", buff, 1);
		dup2(in,0);
		dup2(out,1);
		dup2(err_out,2);
		update_jobs(stderr, jobs);
		update_prompt(prompt, count_jobs(jobs));
		fg = 1;
		}
char *last = getenv("?");
return last == NULL ? 0 : atoi(last);
}



