#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

typedef struct NOD  // structura pentru arbore, vom tine in data
{                   //valoarea unui literal, iar in literal
  int data;         //reprezentarea acestuia, chiar daca e un operator
  char literal;
  struct NOD *NOD_stanga;
  struct NOD *NOD_dreapta;
}NOD;

typedef struct nod  // structura pentru stiva, unde vom tine nodurile din arbore
{
    NOD *val;
    struct nod* next;
} nod;

// Variabile globale

int literal[26] = {0}; // vector pentru a retine ce literali avem (A,B..)
char cnf_form[100000]; // vector pentru a retine forma cnf a expresiei
int contor[26] = {0}; // contor pentru a putea sa imi generez toate variantele de true/false pentru variabile


int isEmpty(nod* cap) // stiva e goala
{
    return cap == NULL;
}

NOD * pop(nod** cap) // scoate un element din stiva
{
    if(isEmpty(*cap))
      return NULL;
    NOD* valueFirst;
    nod *current = *cap;
    valueFirst = current -> val;
    *cap = current -> next;
    free(current);
    return valueFirst;
}

NOD *creare_nod(char literal) // creare nod arbore
{
  NOD *nod_nou;
  nod_nou=(NOD *)malloc(sizeof(NOD));
  if (nod_nou==NULL)
  {
    printf("Eroare: Memoria nu a putut fi alocata! \n");
    return NULL;
  }

  nod_nou->literal=literal;
  return nod_nou;
}

void push(nod** cap, NOD* val) // adaugare element stiva
{
    nod *current = (nod *)malloc(sizeof(nod));
    current -> val = val;
    current -> next = (*cap);
    (*cap) = current;
}


void printStack(nod* cap) // scrie stiva
{
    nod* p = cap;
    while(p != NULL)
    {
        printf("%c ", p->val->literal);
        p = p->next;
    }
    printf("\n");
}


void reading( char *v, int *i) // Citirea inputului caracter cu caracter
{
  FILE *f = fopen("./tests/input9.txt","r");
  char c;
  while( (c = fgetc(f)) != EOF )
  {
    if(c == '-')
    {
      continue;
    }
    if(c == '\n')
    {
      continue;
    }

    v[*i] = c;
    (*i)++;

  }
  fclose(f);
}

void afisare_preordine(struct NOD *prim) // afisare preordine
{
  if (prim!=NULL)
  {
  printf("%d \n", prim->data);
  afisare_preordine(prim->NOD_stanga);
  afisare_preordine(prim->NOD_dreapta);
  }
}

void afisare_inordine(struct NOD *prim) // afisare inordine
{
    if (prim!=NULL)
    {
  afisare_inordine(prim->NOD_stanga);
  printf("%c \n", prim->literal);
  afisare_inordine(prim->NOD_dreapta);
  }
}

void buildTree(char *v, nod **cap, NOD **root) // construirea arborelui
{
    int poz=strlen(v)-1;
    NOD *nod_nou;
    while (poz>=0){
        switch (v[poz]){
            case 'v':
                nod_nou = creare_nod(v[poz]);
                nod_nou->NOD_stanga = pop(cap);
                nod_nou->NOD_dreapta = pop(cap);
                push(cap,nod_nou);
                break;

            case '^':
                nod_nou = creare_nod(v[poz]);
                nod_nou->NOD_stanga = pop(cap);
                nod_nou->NOD_dreapta = pop(cap);
                push(cap,nod_nou);
                break;

            case '>':
                nod_nou = creare_nod(v[poz]);
                nod_nou->NOD_stanga = pop(cap);
                nod_nou->NOD_dreapta = pop(cap);
                push(cap,nod_nou);
                break;

            case '~':
                nod_nou = creare_nod(v[poz]);
                nod_nou->NOD_stanga = NULL;
                nod_nou->NOD_dreapta = pop(cap);
                push(cap,nod_nou);
                break;

            default:
                nod_nou = creare_nod(v[poz]);
                literal[nod_nou->literal-65] = 1;
                nod_nou->NOD_stanga=NULL;
                nod_nou->NOD_dreapta=NULL;
                push(cap,nod_nou);
        }
        poz--;
    }
   *root = pop(cap);
}

NOD* echivalent_cnf(NOD *sub_arbore1,NOD *sub_arbore2) // algoritm pentru a transforma PvQ
{
    NOD *nod_nou;
    if(sub_arbore1->literal == '^'){

      nod_nou = creare_nod('^');
      nod_nou->NOD_stanga = echivalent_cnf(sub_arbore1->NOD_stanga,sub_arbore2);
      nod_nou->NOD_dreapta = echivalent_cnf(sub_arbore1->NOD_dreapta,sub_arbore2);
      return nod_nou;
   }

    if(sub_arbore2->literal == '^'){

      nod_nou = creare_nod('^');
      nod_nou->NOD_stanga = echivalent_cnf(sub_arbore1,sub_arbore2->NOD_stanga);
      nod_nou->NOD_dreapta = echivalent_cnf(sub_arbore1,sub_arbore2->NOD_dreapta);
      return nod_nou;
    }

    if(sub_arbore1->literal != '^' && sub_arbore2->literal != '^'){

      nod_nou = creare_nod('v');
      nod_nou->NOD_stanga = sub_arbore1;
      nod_nou->NOD_dreapta = sub_arbore2;
      return nod_nou;
    }

}

NOD* convert( NOD *root) // convertirea la forma CNF conform algoritmului oferit ca suport
{
  NOD *nod_nou;
  if(root->NOD_stanga==NULL && root->NOD_dreapta==NULL){
          return root;}

  if(root->literal == '^'){

    nod_nou = creare_nod('^');
    nod_nou->NOD_stanga = convert(root->NOD_stanga);
    nod_nou->NOD_dreapta = convert(root->NOD_dreapta);
    return nod_nou;
  }

  if(root->literal == 'v'){
    return echivalent_cnf( convert(root->NOD_stanga) , convert(root->NOD_dreapta) );

  }
  if(root->literal=='~'){

    nod_nou = creare_nod('~');
    nod_nou->NOD_dreapta = convert(root->NOD_dreapta);
    nod_nou->NOD_stanga = NULL;
    return nod_nou;
  }

}


void formeaza_cnf(struct NOD *a)
{ // pun in vectorul cnf_form forma expresiei in cnf
    if (a == NULL)
      return;
    char literal[2];
    literal[0]=a->literal;
    literal[1]='\0';
    formeaza_cnf(a->NOD_stanga);
    strcat(cnf_form,literal);
    formeaza_cnf(a->NOD_dreapta);

}

char *pune_paranteze(char *v) // pun paranteze in acea forma pentru vizibilitate buna
{
  char aux[1000];
  int i,j = 0;
  aux[0] = '(';
  j++;
  for(i=0;i<strlen(v);i++)
  {
    if(v[i] == '^')
    {
      aux[j] = ')';
      j++;
      aux[j] = '^';
      j++;
      aux[j] = '(';
      j++;
    }
    else
    {
      aux[j] = v[i];
      j++;
    }

  }
  aux[j] = ')';
  aux[j+1] = '\0';

  for(i=0;i<strlen(aux);i++)
    v[i] = aux[i];

  v[i] = '\0';
  return v;
}

void inc(int v[], int n) // incrementez contorul
{
  int i;
  i = 0;
  while(v[i] == 1 && i < n)
  {
    v[i] = 0;
    i++;
  }
  v[i]=1;
}

void print_v(int v[], int n) // functie de printare a contorului
{
  int i;
  for(i=0;i<n;i++)
  {
    printf("%d ",v[i]);
  }
  printf("\n");
}

void asignare_valori(struct NOD *root) // asignez valori frunzelor arborelui
{
      if (root == NULL)
        return;
      asignare_valori(root->NOD_stanga);
      if( (root->NOD_stanga==NULL && root->NOD_dreapta==NULL))
      {
	      root->data=contor[root->literal-65];
      }
     asignare_valori(root->NOD_dreapta);

}

int verifica(struct NOD *nod) // verificam functia daca este satisfiabila sau nu
{
    int left_val;
    int right_val;
    int val = 0;
     if(nod->NOD_stanga!=NULL || nod->NOD_dreapta!=NULL )
     {
          if(nod->NOD_stanga!=NULL)
          {
		          left_val = verifica(nod->NOD_stanga);
		      }
          else
          {                             \
              left_val = 0;
          }
          right_val = verifica(nod->NOD_dreapta);

          if(nod->literal == '~')
          {

            val = ~(right_val)+2;
          }
          if(nod->literal == '^')
          {
            val = left_val & right_val;

          }
          if(nod->literal == 'v')
          {
            val = left_val | right_val;
          }

          return val;
      }
      else
      {
          return nod->data;
     	}
}
