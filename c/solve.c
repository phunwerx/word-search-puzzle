#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "solve.h"

const char * dirptrs[][3] = {
  {"Southeast" ,"South",   "Southwest"},
  {"East"      ,"neutral", "West"},
  {"Northeast" ,"North",   "Northwest"}
};

typedef const char *(*_dp)[3];

_dp direction = (_dp) &dirptrs[1][1];

void strrev(char *p)
{
  char *q = p;
  while(q && *q) ++q;
  for(--q; p < q; ++p, --q)
    *p = *p ^ *q,
    *q = *p ^ *q,
    *p = *p ^ *q;
}

void init(Matrix ** m, Views ** v)
{
  int i,j;

  *m = (Matrix *) malloc(ROWSIZE*COLSIZE*sizeof(Letter));
  *v = (Views *) malloc(NUMVIEWS*NUMORIENT*sizeof(View));

  for (i = 0; i < NUMVIEWS; i++){
    for (j = 0; j < NUMORIENT; j++){
      (*v)[i][j].s = (char *) malloc(ROWSIZE*COLSIZE*sizeof(char)+1);
      (*v)[i][j].l = (Letter *) malloc(ROWSIZE*COLSIZE*sizeof(Letter));
    }
  }
}

void load_matrix(Matrix * m)
{
  FILE * f;
  char buf[80];
  char * s;
  int row,column;

  if ((f = fopen ("puzzle.matrix","r")) == NULL){
    exit(-1);
  }

  row = 0;

  while (fgets(buf,sizeof(buf),f) != NULL){
    s = buf; column=0;
    while (*s){
      if(*s == '\n'){
        s++;
        continue;
      }
      m[row][column].row = row;
      m[row][column].column = column;
      m[row][column].letter = *s++;
      column++;
    }
    row++;
  }

  if(feof(f))
    printf("end of file reached\n");
}

void load_views(Matrix * m, Views * v)
{
  int i,j,k=0;

  for (i = 0; i < NUMROWS; i++){
    for (j = 0; j < NUMCOLS; j++){
      v[rows][forwards].s[k] = m[i][j].letter;
      v[rows][forwards].l[k] = m[i][j];
      k++;
    }
    v[rows][forwards].s[k] = 0;
  }

  strcpy(v[rows][backwards].s,v[rows][forwards].s);
  strrev(v[rows][backwards].s);
}

void release(Matrix * m, Views * v)
{
  int i,j;

  for (i = 0; i < NUMVIEWS; i++){
    for (j = 0; j < NUMORIENT; j++){
      free(v[i][j].s);
      free(v[i][j].l);
    }
  }

  free(v);
  free(m);
}

int main (int argc, char ** argv)
{
  ViewID view_id[] = {rows, columns, lr_diagonal, rl_diagonal};
  Orientation orientation[] = {forwards, backwards};
  Views * v;
  Matrix * m;
  init(&m,&v);
  load_matrix(m);
  load_views(m,v);
  release(m,v);
  return 0;
}
