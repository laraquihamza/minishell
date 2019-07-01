#include "minishell.h"
static char *home=NULL;
int is_an_integer(const char *s){
  size_t i=0;
  if(s==NULL)
    return 0;
  else if(s[0]=='+' && s[0]=='-'){
    i++;
  }
  for(; s[i]; i++){
    if(!(s[i]>='0' && s[i]<='9')){
      return 0;
    }
  }
  return 1;
}
int cmd_echo(char ** tokens){
  if(tokens==NULL || tokens[0]==NULL)
    return 1;
  size_t n=1;
  size_t beg=1;
  if((tokens[1]) && (strcmp(tokens[1],"-n")==0)){
      n=0;
      beg=2;
  }
  for(size_t i=beg; tokens[i]; i++){
    if(i>beg)
      printf(" ");
    printf("%s",tokens[i]);
  }
  for (size_t i=0; i<n; i++)
    printf("\n");
  return 0;
   
}

int cmd_exit(char ** tokens){
  if(tokens==NULL || tokens[0]==NULL){
    return 1;
  }
  if(!tokens[1]){
    exit(1);
  }
  if(tokens[1]!=NULL && tokens[2]!=NULL){
    fprintf(stderr,"too many arguments");
    return 1;
  }
  exit(atoi(tokens[1]));

  return 0;
   
}
int cmd_cd(char ** tokens,  char ** env){
  char * path;
  (void) env;

  if(!home){
    home=get_env((const char **)env,"$HOME");
  }
  if (tokens[1]==NULL || strcmp(tokens[1],"")==0){
    path=home;

  }
  else{
    path=tokens[1];
  }
  if(path){
    if(chdir(path)==-1){
      fprintf(stderr,"%s: No such file or directory\n",tokens[1]);
      //      set_env(env,"$PWD",path);
      return -1;
    }
    else {
      return 0;
    }
  }
  return -1;
}

int cmd_kill(char ** tokens){
  if(tokens[1]!=NULL && strcmp(tokens[1],"-signal")==0 &&
     tokens[2]!=NULL && tokens[3]!=NULL){
    if(is_an_integer(tokens[2])==0){
      fprintf(stderr,"signal:not an integer\n");
      return -1;
    }
    if(is_an_integer(tokens[3])==0){
      fprintf(stderr,"pid:not an integer\n");
      return -1;
    }

    int signal=atoi(tokens[2]);
    int pid=atoi(tokens[3]);
    int res=kill(pid ,signal);
    if(res<0){
      fprintf(stderr,"Failed to execute kill\n");
      return -1;
    }
    return 0;
  }
  return -1;
}
