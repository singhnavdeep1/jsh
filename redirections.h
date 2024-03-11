#ifndef REDIRECTIONS_H
#define REDIRECTIONS_H
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include "parser.h"

enum redir {
	INPUT,
	NO_OVERWRITE,
	OVERWRITE,
	CONCAT,
	ERR_NO_OVERWRITE,
	ERR_OVERWRITE,
	ERR_CONCAT,
	PIPE,
	SUBSTITUTION
};

struct redir_index {
	int redir;
	int indice;
};
	
extern redir_index is_redirected(w_index *);
extern int redirect(int,char *);
extern int is_chevron(char *);
extern int check_redirection(w_index *);
extern 

#endif
