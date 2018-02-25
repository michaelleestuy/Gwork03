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



void add_entry(struct matrix * b, int x0, int y0, int z0, int b0, int x1, int y1, int z1, int b1){
  b->array[0][b->entries] = x0;
  b->array[1][b->entries] = y0;
  b->array[2][b->entries] = z0;
  b->array[3][b->entries] = b0;
  b->array[0][b->entries + 1] = x1;
  b->array[1][b->entries + 1] = y1;
  b->array[2][b->entries + 1] = z1;
  b->array[3][b->entries + 1] = b1;
  b->entries += 2;
}

struct matrix* make_identity(){
  struct matrix *b = make_new();
  add_entry(b, 1, 0, 0, 0, 0, 1, 0, 0);
  add_entry(b, 0, 0, 1, 0, 0, 0, 0, 1);
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

void multiply(struct matrix * a, struct matrix * b){
  int counter = b->entries;
  int w = 0;
  int x = 0;
  int y = 0;
  int z = 0;

  int i;
  for(i = 0; i < counter; i++){
    w = a->array[0][0] * b->array[0][i] + a->array[0][1] * b->array[1][i] + a->array[0][2] * b->array[2][i] + a->array[0][3] * b->array[3][i];
    x = a->array[1][0] * b->array[0][i] + a->array[1][1] * b->array[1][i] + a->array[1][2] * b->array[2][i] + a->array[1][3] * b->array[3][i];
    y = a->array[2][0] * b->array[0][i] + a->array[2][1] * b->array[1][i] + a->array[2][2] * b->array[2][i] + a->array[2][3] * b->array[3][i];
    z = a->array[3][0] * b->array[0][i] + a->array[3][1] * b->array[1][i] + a->array[3][2] * b->array[2][i] + a->array[3][3] * b->array[3][i];
    b->array[0][i] = w;
    b->array[1][i] = x;
    b->array[2][i] = y;
    b->array[3][i] = z;    
  }
}

void linemaker(char image[500][500], int x, int y, int ex, int ey){
  int dx = ex - x;
  int dy = ey - y;
  if( (dx < 0 && dy < 0) || (dx > 0 && dy < 0) || (dx == 0 && dy < 0) || (dy == 0 && dx < 0)){
    int tx = x;
    int ty = y;
    x = ex;
    y = ey;
    ex = tx;
    ey = ty;
    dx = ex - x;
    dy = ey - y;
  }
  int c = dx * y - dy * x;
  if(dy > 0 && dx < 0){
    int d =  2 * dy * (x + 1) - dx * (2 * y + 1) + 2 * c;
    if(dx + dy <= 0){
       while(x >= ex){
	 image[x][y] = 'r';
	 if(d < 0){
	   d -= 2 * dx;
	   y++;
	 }
	 d -= 2 * dy;
	 x--;
       }
    }
    else{
      while(y <= ey){
	 image[x][y] = 'r';
	 if(d > 0){
	   d -= 2 * dy;
	   x--;
	 }
	 d -= 2 * dx;
	 y++;
       }
    }
  } 
  else if(dx >= dy){
    int d =  2 * dy * (x + 1) - dx * (2 * y + 1) + 2 * c;   
    while(x <= ex){
      image[x][y] = 'r';
      if(d > 0){
	d -= 2 * dx;
	y++;
      }
      d += 2 * dy;
      x++;
    }
  }  
  else if(dy > dx){
    int d =  dy * (2 * x + 1) - dx * (2 * y) + 2 * c;   
    while(y <= ey){
      image[x][y] = 'r';
      if(d < 0){
	d += 2 * dy;
	x++;
      }
      d -= 2 * dx;
      y++;
    }
  }
}

void edgemaker(struct matrix * a, char image[500][500]){
  int counter = a->entries;
  int i;
  for(i = 0; i < counter; i+=2){
    linemaker(image, a->array[0][i], a->array[1][i], a->array[0][i+1], a->array[1][i+1]);
  }
}

void main(){
  int b = open("basic.ppm", O_CREAT | O_WRONLY | O_APPEND, 888);
  char image[500][500];
  int i;
  int j;
  for(i = 0; i < 500; i++){
    for(j = 0; j < 500; j++){
      image[i][j] = 'g';
    }
  }

  printf("Original Edge Matrix: \n");

  struct matrix * a = make_new();
  add_entry(a, 250, 250, 0, 1, 250, 300, 0, 1);  
  print_matrix(a);

  printf("Adding edge [(150, 150),(250,250)]: \n");

  add_entry(a, 150, 150, 0, 1, 250, 250, 0, 1);
  print_matrix(a);

  printf("Making Identity: \n");

  struct matrix * c = make_identity();
  print_matrix(c);

  printf("Making Transformation (39, 39): \n");
  c->array[0][3] = 39;
  c->array[1][3] = 39;

  print_matrix(c);
  printf("Multiplying Edge Matrix: \n");

  multiply(c, a);
  print_matrix(a);

  printf("Making Image: \n");
  edgemaker(a, image);

  

  write(b, "P3 500 500 255", sizeof("P3 500 500 255"));
  for(i = 0; i < 500; i++){
    for(j = 0; j < 500; j++){
      if(image[i][j] == 'g')
	write(b, "0 255 0", sizeof("0 255 0"));
      if(image[i][j] == 'r')
	write(b, "255 0 0", sizeof("255 0 0"));
    }
  }
  
}
