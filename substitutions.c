#include "substitutions.h"
#include "parser.h"
#include "jobs.h"
#include "shell.h"


int count_substitutions(w_index* pi){
	int n=0;
	int m=0;
	int acc=0;
	for(int i=0; i<pi->size; ++i){
		if(strcmp("<(",pi->words[i])==0){
			if(acc==0) ++n;
			++acc;
			
		}
		if(strcmp(")",pi->words[i])==0) {
			if(acc==1) ++m;
			--acc;
		}
	}
	if(n!=m) return -1;
	else return n;
}

void pos_open_sub(w_index *pi, int *t){
	int acc_t=0;	
	int acc=0;
	for(int i=0; i<pi->size; ++i){
		if(strcmp("<(",pi->words[i])==0){
			if(acc==0) {
				t[acc_t]=i;	
				++acc_t;
			}
			++acc;
		}
		if(strcmp(")",pi->words[i])==0) {
			--acc;
		}
	}
}
void pos_close_sub(w_index *pi,int *t){
	int acc_t=0;	
	int acc=0; 
	for(int i=0; i<pi->size; ++i){
		if(strcmp("<(",pi->words[i])==0){
			++acc;
		}
		if(strcmp(")",pi->words[i])==0) {
			if(acc==1) {
				t[acc_t]=i;
				++acc_t;
			}
			--acc;
		}
	}

}
void get_cmds_sub(w_index **cmds, w_index *pi, int *t, int *p, int n){
	for(int i=0; i<n; ++i){
		cmds[i]=sub_index(pi,t[i]+1,p[i]);
	}

}
int launch_cmd(w_index *pi){
	w_index *tmp=NULL;
	if(is_substituted(pi)>0){
		tmp=substitute(pi);
	}
	else tmp=pi;

	int out=dup(1);
	int fd[2];
	if(pipe(fd)==-1){
		perror("pipe failed");
		exit(1);
	}
	

	int pid=fork();
	switch(pid){
		case -1 : 
			perror("fork");
			exit(1);
		case 0 : 
			close(fd[0]);
			dup2(fd[1],1);
			close(fd[1]);
			close(out);
			char *cmd=concat(tmp);
			exec_command(cmd, tmp, 1, jobs);
			free(cmd);
			exit(1);
		default : 
			waitpid(pid,NULL,0);
			dup2(out,1);
			close(fd[1]);
			close(out);
			return fd[0];


	}

}

//-1 : FAILED
//-2 : SYNTAX ERROR
// 0 : NO SUBSTITUTION
int is_substituted(w_index *pi){
	int n=count_substitutions(pi);
	if(n==-1) return -2;
	if(n==0) return 0;
	return n;
}

w_index *substitute(w_index *pi){
	const int n=is_substituted(pi);
	if(n==-1 || n==-2) return NULL;
	if(n==0) return pi;
	int open[n];
	int close[n];
	pos_open_sub(pi,open);
	pos_close_sub(pi,close);
	w_index *first=sub_index(pi,0,open[0]);
	w_index **cmds=malloc(n*sizeof(w_index *));
	get_cmds_sub(cmds,pi,open,close,n);
	for(int i=0; i<n; ++i){
		fds[i]=launch_cmd(cmds[i]);
	}
	fds[n]=-1;
	int acc=0;
	int i=open[0];
	while(i<pi->size){
		if(strcmp("<(",pi->words[i])==0){
			char buff[50];
			sprintf(buff, "/dev/fd/%d", fds[acc]);
			add_word(first,buff);
			i=close[acc]+1;
			++acc;
		}else{
			add_word(first,pi->words[i]);
			++i;
		}
	}
	for(int i=0; i<n; ++i){
		free_index(cmds[i]);
	}
	free(cmds);
	return first;
}
