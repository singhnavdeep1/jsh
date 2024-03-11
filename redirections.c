#include "redirections.h"
#include "parser.h"

redir_index is_redirected(w_index *pi){
	redir_index ri={.redir=-1,.indice=-1};
	for (int i=0; i<pi->size; ++i){
		ri.indice=i;
		if(strcmp(pi->words[i],"<")==0){
			ri.redir=INPUT;
			break;
		}
		else if (strcmp(pi->words[i],">")==0){
			ri.redir=NO_OVERWRITE;
			break;
		}
		else if (strcmp(pi->words[i],">|")==0){
			ri.redir=OVERWRITE;
			break;
		}
		else if (strcmp(pi->words[i],">>")==0){
			ri.redir=CONCAT;
			break;
		}
		else if (strcmp(pi->words[i],"2>")==0){
			ri.redir=ERR_NO_OVERWRITE;
			break;
		}
		else if (strcmp(pi->words[i],"2>|")==0){
			ri.redir=ERR_OVERWRITE;
			break;
		}
		else if (strcmp(pi->words[i],"2>>")==0){
			ri.redir=ERR_CONCAT;
			break;
		}
		
	}
	return ri;
}

int redirect(int redir_type, char * path){
	int fd;
	switch(redir_type){
		case INPUT : 
			fd=open(path,O_RDONLY);
			if(fd!=-1) dup2(fd,STDIN_FILENO);
			break;
		case NO_OVERWRITE : 
			fd=open(path,O_WRONLY | O_CREAT | O_EXCL,0644);
			if(fd!=-1)dup2(fd,STDOUT_FILENO);
			break;
		case OVERWRITE :
			fd=open(path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if(fd!=-1) dup2(fd,STDOUT_FILENO);
			break;
		case CONCAT : 
			fd=open(path, O_WRONLY | O_CREAT | O_APPEND, 0644);
			if(fd!=1) dup2(fd,STDOUT_FILENO);
			break;
		case ERR_NO_OVERWRITE : 
			fd=open(path,O_WRONLY | O_CREAT | O_EXCL, 0644);
			if(fd!=-1) dup2(fd,STDERR_FILENO);
			break;
		case ERR_OVERWRITE :
			fd=open(path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if(fd!=-1) dup2(fd,STDERR_FILENO);
			break;
		case ERR_CONCAT : 
			fd=open(path,O_WRONLY | O_CREAT | O_APPEND, 0644);
			if(fd!=-1) dup2(fd, STDERR_FILENO);
			break;
		default : return -1;
			
	}
	if(fd!=-1) close(fd);
	return fd;
}

int is_chevron(char *s){
	if(strcmp(s,"<")==0){
		return INPUT;
	}
	else if (strcmp(s,">")==0){
		return NO_OVERWRITE;
	}
	else if (strcmp(s,">|")==0){
		return OVERWRITE;
	}
	else if (strcmp(s,">>")==0){
		return CONCAT;
	}
	else if (strcmp(s,"2>")==0){
		return ERR_NO_OVERWRITE;
	}
	else if (strcmp(s,"2>|")==0){
		return ERR_OVERWRITE;
	}
	else if (strcmp(s,"2>>")==0){
		return ERR_CONCAT;
	}
	return -1;
	
}

int check_redirection(w_index *pi){
	int i,j,res;
	if(pi->size==0) return -3;
	for(i=0; i<pi->size; ++i){
		res=is_chevron(pi->words[i]);
		if(res!=-1) break;
	}

	if(i==0 || (i==pi->size-1 && res!=-1)) return -2;
	if(i==pi->size && res==-1) return -3;
	for(j=i; j<pi->size; ++j){
		int res=is_chevron(pi->words[j]);
		if(res!=-1){
			if(j==pi->size-1) return -2;
			if(is_chevron(pi->words[j+1])!=-1) return -2;
			int nb=redirect(res,pi->words[j+1]);
			if(nb==-1) return -1;
		}
	}
	return i;

}









