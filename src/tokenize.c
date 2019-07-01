#include "minishell.h"

char ** my_tokenize(char * s, const char **env){
  char ** res= malloc(sizeof(char *)* (strlen(s)+1));
  res[0]=NULL;
  size_t i=0;
  //  size_t j=0;
  size_t k=0;
  size_t index=0;
  while(s[i] && s[i]!='\n'){
    res[index]=malloc(sizeof(char *) * strlen(s)+ 1);
    while(s[i] && s[i]==' ')
      i++;
    k=0;
    if(s[i]=='\"'){
      i++;
      while(s[i] && s[i]!='\"' && s[i]!='\n'){
	res[index][k]=s[i];
	//printf("%c",res[index][k]);
	i++;
	k++;
      }
      i++;
      res[index][k]=0;
      k=0;
      index++;

    }
    else{
      while(s[i] && s[i]!=' ' && s[i]!='\n'){
	res[index][k]=s[i];
	//	printf("%c",res[index][k]);
	i++;
	k++;
      }
      res[index][k]=0;
      if (res[index][0]=='$'){
	res[index]=strdup(get_env(env,res[index]));
      }
      k=0;
      index++;
    }
  }
  res[index]=NULL;
  return res;
}
