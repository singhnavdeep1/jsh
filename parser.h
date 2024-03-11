#ifndef PARSER_H
#define PARSER_H

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>


struct w_index {
	size_t size;
	char **words;
};
typedef struct w_index w_index;
typedef struct redir_index redir_index;

extern void free_index(w_index *);
extern void print_index(w_index *);
extern void print_index_in_line(w_index *);
extern w_index *split_space(char *);
extern w_index *split_slash(char *);
extern w_index *split_semicolon(char *);
extern w_index *reverse(w_index *);
extern w_index *sub_index(w_index *, size_t, size_t);
extern w_index *copy_index(w_index *);
extern char * concat(w_index *);
extern void add_word(w_index *, char *);

#endif
