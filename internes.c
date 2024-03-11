#include "internes.h"

int cd(w_index *index) {
	char *nouv;

	if (index->words[1] == NULL) {
		nouv = getenv("HOME");
		if (nouv == NULL) {
			fprintf(stderr, "Erreur : $HOME n'est pas défini.\n");
			return 1;
		}
	} else if (strcmp(index->words[1], "-") == 0) {
		nouv = getenv("OLDPWD");
		if (nouv == NULL) {
			fprintf(stderr, "Erreur : le répertoire précédent n'est pas défini.\n");
			return 1;
		}
	} else {
		nouv = index->words[1];
	}

	if (chdir(nouv) == 0) {
		setenv("OLDPWD", getenv("PWD"), 1);
		setenv("PWD", nouv, 1);
		return 0;
	} else {
		perror("chdir");
		return 1;
	}
}

int p_jobs(w_index *index) {
	return print_jobs(jobs);
} 

int pwd(w_index *index) {
	char poss[2028];
	if (getcwd(poss, sizeof(poss)) != NULL) {
		printf("%s\n", poss);
		return 0;
	} else {
		perror("getcwd");
		return 1;
	}
}

int return_code() {
	char *ret;
	ret = getenv("?");
	if(ret == NULL)
		return 1;
	printf("%s\n", ret);
	return 0;
}


int send_signal(int signal, int target, int job_or_not){
	if (job_or_not==1) {
		if(jobs[target-1]!=NULL){
			if (kill(-jobs[target-1]->pgid, signal) == -1) {
				perror("kill");
				return 1;
			}
		}
	} else if (job_or_not==0){
		if(kill(target, signal) == -1) {
			perror("kill");
			return 2;
		}
	} else {
		fprintf(stderr, "La cible est invalide : %d\n", target);
		return 3;
	}
	return 0;
}

int exit_shell(w_index *index) {
	// Vérifier si des jobs sont en cours d'exécution ou suspendus
	if (are_jobs_running(jobs)) {
		fprintf(stderr, "There is %d job.\n", count_jobs(jobs));
		return 1;
	}

	if (index->size == 1) {
		// Pas d'argument pour exit, terminer le shell
		char *last = getenv("?");
		int value = atoi(last);
		free_index(index);
		free_index(current_index);
		exit(value);
	} else if (index->size >= 2) {
		// terminer le shell avec la valeur de retour spécifiée
		int value = atoi(index->words[1]);
		free_index(index);
		free_index(current_index);
		exit(value);
	} 
	return 0;
}

int kill_job(w_index *index){
	int signal;
	int target;
	int job_or_not=1;

	if (index->size<2 || index->size>3){
		return 1;
	}
	if(index->size==2){
		signal=SIGTERM;
		if(index->words[1][0]!='%'){
			target=atoi(index->words[1]);
			job_or_not=0;
		} else {
			target=atoi(&index->words[1][1]);
		}
	} else {
		signal= atoi(&index->words[1][1]);
		if(index->words[2][0]!='%'){
			target=atoi(index->words[2]);
			job_or_not=0;
		} else {
			target=atoi(&index->words[2][1]);
		}
	}
	if(send_signal(signal, target, job_or_not)==1) return 1;
	return 0;

}

int bg_cmd(w_index *index) {
	int target = 0;
	if(index->size < 2) {
		return 1;
	}
	if(index->words[1][0] != '%') {
		return 1;
	}
	target = atoi(&index->words[1][1]);
	if(send_signal(SIGCONT, target, 1) == 1) return 1;
	return 0;
}

int fg_cmd(w_index *index) {
	int target = 0;
	int status;
	if(index->size < 2) {
		return 1;
	}
	if(index->words[1][0] != '%') {
		return 1;
	}
	target = atoi(&index->words[1][1]);
	job *j = jobs[target - 1];
	if(send_signal(SIGCONT, target, 1) == 1) return 1;
	j->fg = 1;
	j->state = RUNNING;
	tcsetpgrp(STDERR_FILENO, j->pgid);
	
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
	return 0;
}


