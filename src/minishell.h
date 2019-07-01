
#ifndef _MINISHELL_H
#define _MINISHELL_H
#define _GNU_SOURCE
#define SIZE_MAX 256
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

char ** my_tokenize(char *s, const char **env);
int cmd_echo(char ** tokens);
int cmd_exit(char ** tokens);
char* get_env(const char**env, char*key);
void set_env(char **env, char * key, char * value);
int cmd_cd(char ** tokens, char ** env);
int cmd_kill(char ** tokens);

#endif
