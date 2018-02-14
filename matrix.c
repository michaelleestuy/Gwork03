#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>


struct matrix {
  int entries;
  int array[4][40];
};

struct matrix* make_new(){
  struct matrix * b = malloc(sizeof(struct matrix));
  (*b).entries = 0;
  return b;
}

void add_entry(struct matrix * b, int x0, int y0, int z0, int x1, int y1, int z1){
  b->array[0][b->entries] = x0;
  b->array[1][b->entries] = y0;
  b->array[2][b->entries] = z0;
  b->array[3][b->entries] = 1;
  b->array[0][b->entries + 1] = x1;
  b->array[1][b->entries + 1] = y1;
  b->array[2][b->entries + 1] = z1;
  b->array[3][b->entries + 1] = 1;
  b->entries += 2;
}

void print_matrix(struct matrix * b){
  int i, j;
  for(i = 0; i < 4; i++){
    for(j = 0; j < b->entries; j++){
      printf("%d ", b->array[i][j]);      
    }
    printf("\n");
  }
}

void main(){
  struct matrix * b = make_new();
  add_entry(b, 1, 1, 1, 2, 2, 2);
  add_entry(b, 1, 1, 1, 2, 2, 2);
  print_matrix(b);


}
