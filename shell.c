#include "shell.h"

job *jobs[MAX_JOBS];
int ret_code=0;
w_index *current_index = NULL;
int fds[1024];

void ignore_signals(){
	sigset_t set;
	sigemptyset(&set);
	sigaddset(&set,SIGINT);
	sigaddset(&set,SIGTERM);
	sigaddset(&set,SIGTTIN);
	sigaddset(&set,SIGQUIT);
	sigaddset(&set,SIGTTOU);
	sigaddset(&set,SIGTSTP);
	sigprocmask(SIG_BLOCK,&set,NULL);
}


