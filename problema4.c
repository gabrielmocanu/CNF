#include "functions.h"

int main(int argc, char const *argv[]) {

//  clock_t begin = clock();
  nod *stack = NULL;
  NOD *root = NULL;
  int nr_var = 0;

  int ok = 0;

  char v[10000];
  int i = 0;

  reading(v,&i);
  v[i] = '\0';

  buildTree(v,&stack,&root);


  root = convert(root);

  //formeaza_cnf(root);
  //puts(pune_paranteze(cnf_form));

  for(int j = 0;j<26;j++)
  {
    if(literal[j] != 0)
      nr_var++;
  }



  for(int k=0;k<pow(2,nr_var);k++)
  {
      asignare_valori(root);

      if( verifica(root) )
      {
        for(int j = 0; j<nr_var;j++)
        {
          printf("%c %d\n",(65+j),contor[j]);
        }
        ok = 1;
        break;
      }
      inc(contor,nr_var);

  }
  if(!ok)
  {
    printf("IMPOSSIBLE\n");
  }

 //clock_t end = clock();
 //double time_spent = (double)(end - begin)/CLOCKS_PER_SEC;
 //printf("Time = %f\n",time_spent);

  return 0;
}
