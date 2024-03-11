#include "prompt.h"


int update_prompt(char *prompt, int jobs) {
	char *path=malloc(256);
	if(getcwd(path,256)==NULL) {
		return -1;
	}
	
	int diff=0;
	int len_jobs=1;
	if(jobs>0) len_jobs=floor(log10((double)jobs))+1;
	int count=len_jobs+strlen(path)+4;

	if(count>30) {
		diff=count-30+3;
		sprintf(prompt,"\001\033[33m\002[%d]\001\033[36m\002...%s\001\033[00m\002$ ",jobs,path+diff);
		
	}else{
		sprintf(prompt,"\001\033[33m\002[%d]\001\033[36m\002%s\001\033[00m\002$ ",jobs,path);
	}
	free(path);
	return 0;
}

