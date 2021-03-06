#ifndef SOLVE_H
#define SOLVE_H

#define NELEMS(x)  (sizeof(x) / sizeof(x[0]))

#define ROWSIZE 22
#define COLSIZE 22
#define NUMROWS ROWSIZE
#define NUMCOLS COLSIZE

typedef const char * Direction[3];

typedef enum{
  rows,
  columns,
  lr_diagonal,
  rl_diagonal,
  NUMVIEWS
} ViewID;

typedef enum{
  forwards,
  backwards,
  NUMORIENT
} Orientation;

typedef struct{
  char letter;
  int row,column;
} Letter;

typedef Letter Matrix[COLSIZE];
 
typedef struct{
  char * s;
  Letter * l;
} View;

typedef View Views[NUMORIENT];

int compare(int left, int right);
char * upcase(char * s);
void reverse(char [], Letter []);
void init(Matrix **, Views **);
void load_rows_and_columns(Matrix *, Views *);
void load_rl_diagonal(Matrix *, Views *);
void load_lr_diagonal(Matrix *, Views *);
void load_matrix(Matrix *);
void load_views(Matrix *, Views *);
void solve(Views *);
void release(Matrix *, Views *);

#endif /* SOLVE_H */
