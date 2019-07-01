#include "minishell.h"


char **copy_tab(const char **tab){
  size_t i=0;
  char **res;
  
  for(; tab[i]; i++);
  res=malloc(sizeof(char *) * (i+1));
  if(!res)
    return NULL;
  res[i]=NULL;
  for(i=0; tab[i]; i++){
    
    res[i]=strdup(tab[i]);
  }
  return res;
}

char* get_env(const char**env, char*key){
  char *k;
  char * token;
  if (strlen(key) <= 1)
    return "$";
  k = &key[1];
  for(size_t i=0; env[i]; i++){
    token=strtok((char*)env[i],"=");
    if(strcmp(token,k)==0){
      return strtok(NULL,"=");
    }
  }
  return "";
}

void set_env(char **env, char * key, char * value){
  char *k;
  char * token;
  if (strlen(key) <= 1)
    return ;
  k = &key[1];
  for(size_t i=0; env[i]; i++){
    token=strtok((char*)env[i],"=");
    if(strcmp(token,k)==0){
      env[i]=realloc(env[i],sizeof(char)*(strlen(key)+strlen(value)+2));
      if(!env[i])
	return;
      sprintf(env[i],"%s=%s",key,value);
    }
  }
}

void print_tab(char **tab){
  for(size_t i=0; tab[i]; i++){
    printf("%s\n",tab[i]);
  }
}

int main(int argc, char ** argv, const char **envp){
  char buffer[SIZE_MAX+1];
  char ** tokens;
  char ** env = copy_tab(envp);
  char *ret;
  int link[2];
  pid_t pid;
  char foo[SIZE_MAX];
  int  p[2];
  int nbytes=0;
  char *path;
  char * prk;
  pipe(p);

  (void)argc;
  (void)argv;
  (void)ret;
  (void)envp;
  while (42){
    if (isatty(0))
      printf("minishell$ ");
    ret = fgets(buffer,SIZE_MAX, stdin);
    if (ret == NULL){        
      if (isatty(0)){
	printf("exit");
      }	
      exit(0);
    }
    tokens=my_tokenize(buffer,(const char* *) env);
    
    if(tokens[0]!=NULL && strcmp(tokens[0],"env")==0)
      print_tab(env);
    else if(tokens[0]!=NULL && strcmp(tokens[0],"echo")==0)
      cmd_echo(tokens);
    else if(tokens[0]!=NULL && strcmp(tokens[0],"exit")==0){
      cmd_exit(tokens);
    }
    else if(tokens[0]!=NULL && strcmp(tokens[0],"cd")==0){
      cmd_cd(tokens,env);
    }
    else if(tokens[0]!=NULL && strcmp(tokens[0],"kill")==0){
      cmd_kill(tokens);
    }

    else{

            if (pipe(link)==-1)
	      exit(1);

      if ((pid = fork()) == -1)
	exit(1);

      if(pid == 0) {

		dup2 (link[1], STDOUT_FILENO);
	close(link[0]);
	close(link[1]);
	path=get_env((const char **)env,"$PATH");
	if(access(tokens[0],X_OK|F_OK) && execve(tokens[0], tokens, env)==0){
	  exit(0);
	}
	for(char *bin=strtok_r(path,":",&prk);bin;bin=strtok_r(NULL,":",&prk)){
	  char * tmp=malloc(sizeof(char)*(strlen(bin)+strlen(tokens[0]+2)));
	  if(!tmp)
	    exit(1);
	  sprintf(tmp,"%s/%s",bin,tokens[0]);
	  
	  if(access(tmp,X_OK|F_OK)==0 && execve(tmp, tokens, env)==0){
	 
	    exit(0);
	  }
	}
	fprintf(stderr,"%s: command not found\n",tokens[0]);
	exit(1);
      }
      else {

		close(link[1]);
		nbytes=read(link[0], foo, sizeof(foo));
	printf("%.*s", nbytes, foo);
	wait(NULL);

      }
    }
  
  }
}


