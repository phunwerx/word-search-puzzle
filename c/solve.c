#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "solve.h"

Direction _direction[] = {
  {"Southeast" ,"South",   "Southwest"},
  {"East"      ,0      ,   "West"},
  {"Northeast" ,"North",   "Northwest"}
};

Direction * direction = (Direction *) &_direction[1][1];

int compare(int left, int right)
{
  if (left < right)
    return -1;
  if (left == right)
    return 0;
  return 1;
}

char * upcase(char * s)
{
  char * p = s;
  while (*p = toupper(*p))
    p++;
  return s;
}

void reverse(char s[], Letter l[])
{
      int length = strlen(s);
      int c, i, j;
      Letter ltr;

      for (i = 0, j = length - 1; i < j; i++, j--){
         c = s[i];
         s[i] = s[j];
         s[j] = c;

         ltr = l[i];
         l[i] = l[j];
         l[j] = ltr;
      }
}

void init(Matrix ** m, Views ** v)
{
  int i,j;

  *m = malloc(ROWSIZE*COLSIZE*sizeof(Letter));
  *v = malloc(NUMVIEWS*NUMORIENT*sizeof(View));

  for (i = 0; i < NUMVIEWS; i++){
    for (j = 0; j < NUMORIENT; j++){
      (*v)[i][j].s = malloc(ROWSIZE*COLSIZE*sizeof(char)+1);
      (*v)[i][j].l = malloc(ROWSIZE*COLSIZE*sizeof(Letter));
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
}

void load_rows_and_columns(Matrix * m, Views * vw)
{
  int i,j,k=0;
  char * s;
  View
    * rf =  &vw[rows][forwards],
    * rb =  &vw[rows][backwards],
    * cf =  &vw[columns][forwards],
    * cb =  &vw[columns][backwards];

  for (i = 0; i < NUMROWS; i++){
    for (j = 0; j < NUMCOLS; j++){
      rf->s[k] = m[i][j].letter;
      rf->l[k] = m[i][j];
      rb->l[k] = m[i][j];
      cf->s[k] = m[j][i].letter;
      cf->l[k] = m[j][i];
      cb->l[k] = m[j][i];
      k++;
    }
  }

  rf->s[k] = 0;
  cf->s[k] = 0;

  strcpy(rb->s,rf->s);
  strcpy(cb->s,cf->s);
  reverse(rb->s,rb->l);
  reverse(cb->s,cb->l);
}

void load_lr_diagonal(Matrix * m, Views * vw)
{
  int i,j,k,l=0;

  View
    * df =  &vw[lr_diagonal][forwards],
    * db =  &vw[lr_diagonal][backwards];

  /*

    first half looks like this:

    0,0
    1,0 0,1
    2,0 1,1, 0,2
    3,0 2,1, 1,2 2,0
    ...
    21,0 ... 0,21

  */

  for (i = 0; i < NUMROWS; i++){
    for (j = i, k=0; j >= 0; j--,k++){
      df->s[l] = m[j][k].letter;
      df->l[l] = m[j][k];
      db->l[l] = m[j][k];
      l++;
    }
  }

  /*
  
    second half looks like this:

    21,0 20,1 19,2 ... 2,17 3,18 2,19 1,20 0,21  (this sequence was generated on last iteration above, so start with next sequence below)
    21,1 20,2 19,3 ... 2,17 3,18 2,19 1,20
    21,2 20,3 19,4 ... 2,17 3,18 2,19
    21,3 20,4 19,5 ... 2,17 3,18
    ...
    21,18 20,19 19,20, 18,21
    21,19 20,20 19,21
    21,20 20,21
    21,21

  */

  
  for (i = 1; i < NUMCOLS; i++){
    for (j = NUMCOLS-1, k=i; j >=i; j--,k++){
      df->s[l] = m[j][k].letter;
      df->l[l] = m[j][k];
      db->l[l] = m[j][k];
      l++;
    }
  }

  df->s[l] = 0;

  strcpy(db->s,df->s);
  reverse(db->s,db->l);
}


void load_rl_diagonal(Matrix * m, Views * vw)
{
  int i,j,k,l=0;

  View
    * df =  &vw[rl_diagonal][forwards],
    * db =  &vw[rl_diagonal][backwards];

  /*
  
    first half looks like this:

    0,21
    0,20 1,21
    0,19 1,20 2,21
    0,18 1,19 2,20 3,21
    ...
    0,0 1,1 2,2 3,3 ... 18,18 19,19 20,20 21,21

  */

  for (i = 0; i < NUMCOLS; i++){
    for (j = 0, k=NUMCOLS-i-1; j <= i; j++,k++){
      df->s[l] = m[j][k].letter;
      df->l[l] = m[j][k];
      db->l[l] = m[j][k];
      l++;
    }
  }

  /*
  
    second half looks like this:

    0,0 1,1 2,2 3,3 ... 18,18 19,19 20,20 21,21 (this sequence was generated on last iteration above, so start with next sequence below)
    1,0 2,1 3,2 4,3 ... 19,18 20,19 21,20
    2,0 3,1 4,2 5,3 ... 20,18 21,19
    ...
    21,0

  */
  
  for (i = 1; i < NUMCOLS; i++){
    for (j = i, k=0; k < NUMCOLS-i; j++,k++){
      df->s[l] = m[j][k].letter;
      df->l[l] = m[j][k];
      db->l[l] = m[j][k];
      l++;
    }
  }

  df->s[l] = 0;

  strcpy(db->s,df->s);
  reverse(db->s,db->l);
}

void load_views(Matrix * m, Views * v)
{
  load_rows_and_columns(m,v);
  load_lr_diagonal(m,v);
  load_rl_diagonal(m,v);
}

void solve(Views * v)
{
  FILE * f;
  char w[80], *s;
  size_t ln;
  int i,j,where;
  Letter * l;

  if ((f = fopen ("/usr/share/dict/words","r")) == NULL){
    exit(-1);
  }

 next_word:
  while (fgets(w,sizeof(w),f) != NULL){
    ln = strlen(w) - 1;
    if (ln < 5)
      continue;
    if (w[ln] == '\n')
      w[ln] = '\0';
    upcase(w);
    for(i = 0; i < NUMVIEWS; i++){
      for(j = 0; j < NUMORIENT; j++){
        if ((s = strstr(v[i][j].s,w)) == NULL)
          continue;
        where = s - v[i][j].s;
        l = &v[i][j].l[where];
        printf("%s at [%d,%d] %s\n",w,l[0].row,l[0].column,direction[compare(l[0].row,l[1].row)][compare(l[0].column,l[1].column)]);
        goto next_word;
      }
    }
  }
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
  Views * v;
  Matrix * m;
  init(&m,&v);
  load_matrix(m);
  load_views(m,v);
  solve(v);
  release(m,v);
  return 0;
}
