#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef char cell;

static const cell EMPTY     =  ' ';
static const cell WILL_BORN =  'B';

static const cell ALIVE     =  'X';
static const cell WILL_DIE  =  'D';
static const cell WILL_STAY =  'S';

typedef struct {
  int   WIDTH;
  int   HEIGHT;
  cell* BOARD;
  int   ALIVE; //number of alive cells
  int   GLIDERS; //number of gliders that went overboard
} board;

cell* access (board * b , int X, int Y) {
  int W = b-> WIDTH;
  cell* ptr = &(b->BOARD[W*(X-1)+Y-1]);
  return ptr;
}

board* initialize (char* filename) {
  FILE * fd = fopen (filename, "r");
  int X,Y;
  fscanf (fd, "%d", &X);
  fscanf (fd, "%d", &Y);
  int W = 3 - X + 8;
  int H = 5 + Y;
  cell* input_board = malloc(W*H*sizeof(cell));
  for (int i = 0; i < W*H; i++) {
    input_board[i] = EMPTY;
  }
  //Putting glider
  input_board[W+2] = ALIVE;
  input_board[2*W+3] = ALIVE;
  input_board[3*W+1] = ALIVE;
  input_board[3*W+2] = ALIVE;
  input_board[3*W+3] = ALIVE;
  //Putting acorn
  input_board[W*(H-1)-2] = ALIVE;
  input_board[W*(H-1)-3] = ALIVE;
  input_board[W*(H-1)-4] = ALIVE;
  input_board[W*(H-1)-7] = ALIVE;
  input_board[W*(H-1)-8] = ALIVE;
  input_board[W*(H-2)-5] = ALIVE;
  input_board[W*(H-3)-7] = ALIVE;
  board * b = (board *) malloc(sizeof(board));
  b-> WIDTH = W;
  b-> HEIGHT = H;
  b-> BOARD = input_board;
  b-> ALIVE = 12;
  b-> GLIDERS = 0;
  fclose(fd);
  return b;
}

void enlargeup(board* b) {//Enlarging UP by 5 squares
  printf("Enlarging UP UP UP UP UP UP UP UP UP UP UP UP UP UP UP the board..\n");
  int inputW = b-> WIDTH;
  int inputH = b-> HEIGHT;
  cell* initial = b-> BOARD;
  int W = inputW, H = inputH + 5;
  cell* new_board = malloc(sizeof(cell)*W*H);
  int pos;
  for (pos = 0; pos < 5*W; pos++) {
    new_board[pos] = EMPTY;
  }
  for (int i = 0; i < inputH; i++) {
    for (int j = 0; j < inputW; j++) {
      new_board[pos++] = initial[j + i*inputW];
    }
  }
  b-> WIDTH = W;
  b-> HEIGHT = H;
  b-> BOARD = new_board;
}

void enlargedown(board* b) {//Enlarging DOWN by 5 squares
  printf("Enlarging DOWN DOWN DOWN DOWN DOWN DOWN DOWN DOWN DOWN DOWN the board..\n");
  int inputW = b-> WIDTH;
  int inputH = b-> HEIGHT;
  cell* initial = b-> BOARD;
  int W = inputW, H = inputH + 5;
  cell* new_board = malloc(sizeof(cell)*W*H);
  int pos = 0;
  for (int i = 0; i < inputH; i++) {
    for (int j = 0; j < inputW; j++) {
      new_board[pos++] = initial[j + i*inputW];
    }
  }
  for (; pos < 5*W; pos++) {
    new_board[pos] = EMPTY;
  }
  b-> WIDTH = W;
  b-> HEIGHT = H;
  b-> BOARD = new_board;
}

void enlargeright(board* b) {//Enlarging RIGHT by 5 squares
  printf("Enlarging RIGHT RIGHT RIGHT RIGHT RIGHT RIGHT RIGHT the board..\n");
  int inputW = b-> WIDTH;
  int inputH = b-> HEIGHT;
  cell* initial = b-> BOARD;
  int W = inputW + 5, H = inputH;
  cell* new_board = malloc(sizeof(cell)*W*H);
  int pos = 0;
  for (int i = 0; i < inputH; i++) {
    for (int j = 0; j < inputW; j++) {
      new_board[pos++] = initial[j + i*inputW];
    }
    for (int j = 0; j < 5; j++) {
      new_board[pos++] = EMPTY;
    }
  }
  b-> WIDTH = W;
  b-> HEIGHT = H;
  b-> BOARD = new_board;
}

void enlargeleft(board* b) {//Enlarging LEFT by 5 squares
  printf("Enlarging LEFT LEFT LEFT LEFT LEFT LEFT LEFT LEFT the board..\n");
  int inputW = b-> WIDTH;
  int inputH = b-> HEIGHT;
  cell* initial = b-> BOARD;
  int W = inputW + 5, H = inputH;
  cell* new_board = malloc(sizeof(cell)*W*H);
  int pos = 0;
  for (int i = 0; i < inputH; i++) {
    for (int j = 0; j < 5; j++) {
      new_board[pos++] = EMPTY;
    }
    for (int j = 0; j < inputW; j++) {
      new_board[pos++] = initial[j + i*inputW];
    }
  }
  b-> WIDTH = W;
  b-> HEIGHT = H;
  b-> BOARD = new_board;
}

int doesexist (int W, int H, int X, int Y) {
  if (X < 1 || Y < 1) {return 0;}
  else if (X > H || Y > W) {return 0;}
  else {return 1;}
}

void alivecheck (board * b , int X, int Y) {
  int W = b-> WIDTH;
  int H = b-> HEIGHT;
  int nalive = -1;
  for(int i=-1; i <=1; i++) {
    for(int j=-1; j <=1; j++) {
      if(doesexist(W,H,X+i,Y+j)) {
        cell elem = *access (b,X+i,Y+j);
        if(elem == ALIVE || elem == WILL_STAY || elem == WILL_DIE){
          nalive++;
        }
      }
    }
  }
  if(nalive < 2) {
    *access (b,X,Y) = WILL_DIE;
  }
  else if(nalive <= 3) {
    *access (b,X,Y) = WILL_STAY;
  }
  else {
    *access (b,X,Y) = WILL_DIE;
  }
}

void deadcheck (board * b , int X, int Y) {
  int W = b-> WIDTH;
  int H = b-> HEIGHT;
  int nalive = 0;
  for(int i=-1; i <=1; i++) {
    for(int j=-1; j <=1; j++) {
      if(doesexist(W,H,X+i,Y+j)) {
        cell elem = *access (b,X+i,Y+j);
        if(elem == ALIVE || elem == WILL_STAY || elem == WILL_DIE){
          nalive++;
        }
      }
    }
  }
  if(nalive == 3) {
    *access (b,X,Y) = WILL_BORN;
  }
}

void print (board *b) {
  assert(b);
  for (int i = 0; i < b->WIDTH * b->HEIGHT; i++) {
    printf("|%c", b->BOARD[i]);
    if ( ( i + 1 )% b->WIDTH == 0)  {
      printf("|\n");
    }
  }
}

int glidercheckup(board* b, int X, int Y) {//Checks glider on the upper edge
  int W = b-> WIDTH;
  int H = b-> HEIGHT;
  int nalive = 0,nstay=0,nborn=0,ndie=0;
  for(int i=0; i <=4; i++) {
    for(int j=-4; j <=4; j++) {
      if(doesexist(W,H,X+i,Y+j)) {
        cell elem = *access (b,X+i,Y+j);
        if(elem == ALIVE){nalive++;}
        else if(elem == WILL_STAY){nstay++;}
        else if(elem == WILL_BORN){nborn++;}
        else if(elem == WILL_DIE){ndie++;}
      }
    }
  }
  if (nalive == 1 && nstay ==3 && ndie ==2 && nborn ==1) {
    return(1);
  }
  return(0);
}

int glidercheckdown(board* b, int X, int Y) {//Checks glider on the bottom edge
  int W = b-> WIDTH;
  int H = b-> HEIGHT;
  int nalive = 0;
  for(int i=-4; i <=0; i++) {
    for(int j=-4; j <=4; j++) {
      if(doesexist(W,H,X+i,Y+j)) {
        cell elem = *access (b,X+i,Y+j);
        if(elem == ALIVE){nalive++;}
      }
    }
  }
  if (nalive == 5) {
    return(1);
  }
  return(0);
}

int glidercheckright(board* b, int X, int Y) { //Checks glider on the right edge
  int W = b-> WIDTH;
  int H = b-> HEIGHT;
  int nalive = 0,nstay=0,nborn=0,ndie=0;
  for(int i=-4; i <=4; i++) {
    for(int j=-4; j <=4; j++) {
      if(doesexist(W,H,X+i,Y+j)) {
        cell elem = *access (b,X+i,Y+j);
        if(elem == ALIVE){nalive++;}
        else if(elem == WILL_STAY){nstay++;}
        else if(elem == WILL_BORN){nborn++;}
        else if(elem == WILL_DIE){ndie++;}
      }
    }
  }
  if (nalive == 3 && nstay == 2 && nborn == 0 && ndie == 0) {
    return(1);
  }
  else if (nalive == 4 && nstay == 0 && nborn == 1 && ndie == 1) {
    return(1);
  }
  return(0);
}

int glidercheckleft(board* b, int X, int Y) { //Checks glider on the left edge
  int W = b-> WIDTH;
  int H = b-> HEIGHT;
  int nalive = 0,nstay=0,ndie=0,nborn=0;
  for(int i=-4; i <=4; i++) {
    for(int j=-4; j <=4; j++) {
      if(doesexist(W,H,X+i,Y+j)) {
        cell elem = *access (b,X+i,Y+j);
        if(elem == ALIVE){nalive++;}
        else if(elem == WILL_STAY){nstay++;}
        else if(elem == WILL_BORN){nborn++;}
        else if(elem == WILL_DIE){ndie++;}
      }
    }
  }
  if (nalive == 3 && nstay == 1 && ndie ==2 && nborn==1) {
    return(1);
  }
  else if (nalive == 2 && nstay == 3 && ndie ==1 && nborn==0) {
    return(1);
  }
  else {return(0);}
}

int deleteglider(board* b, int X, int Y) {
  int W = b-> WIDTH;
  int H = b-> HEIGHT;
  int addalive = 0;
  for(int i=-2; i <=2; i++) {
    for(int j=-2; j <=2; j++) {
      if(doesexist(W,H,X+i,Y+j)) {
        cell pos = *access (b,X+i,Y+j);
        if (pos == WILL_STAY) {
          *access (b,X+i,Y+j) = EMPTY; addalive++;
        }
        else if (pos == WILL_BORN) {
          *access (b,X+i,Y+j) = EMPTY; addalive++;
        }
        else {
          *access (b,X+i,Y+j) = EMPTY;
        }
      }
    }
  }
  b->GLIDERS++;
  return(addalive);
}

void process (board *b) {
  int W = b-> WIDTH;
  int H = b-> HEIGHT;
  for (int i = 0; i < W*H; i++) {
    cell value = b->BOARD[i];
    int row = (i / W) + 1;
    int col = ((i+1) % W)==0 ? W : (i+1) % W;
    if (value == ALIVE) {
      alivecheck (b , row, col);
    }
    else {
      deadcheck (b , row, col);
    }
  }
  int alive = 0, enlarge[4] = {0,0,0,0};
  for(int i = 0; i < W; i++) {//Checking the first row
    cell value = b->BOARD[i];
    if (value == WILL_STAY || value == WILL_BORN) {
      * access (b, 1 , i+1) = ALIVE; alive++;
      if (glidercheckup(b,1 , i+1)) {
        printf("\n------\nGlider found!!\n------TOP\n");
        //print(b);
        alive += deleteglider(b,1 , i+1);
        printf("Number of gliders deleted is %d\n", b->GLIDERS);
      }
      else {enlarge[0] = 1;}
    }
    else {* access(b,1,i+1) = EMPTY;
    }
  }
  for (int i = 1; i < H-1; i++) {//Checking the middle
    //First row
    cell value = b->BOARD[i*W];
    int row = i + 1;
    if (value == WILL_STAY || value == WILL_BORN) {
      * access (b,row,1) = ALIVE; alive++;
      if (glidercheckleft(b,row,1)) {
        printf("\n------\nGlider found!!\n------LEFT\n");
        //print(b);
        alive += deleteglider(b,row,1);
        printf("Number of gliders deleted is %d\n", b->GLIDERS);
      }
      else {enlarge[1] = 1;}
    }
    else {* access(b,row,1) = EMPTY;}
    //Middle rows
    for (int j = 1; j < W-1; j++) {
      value = b->BOARD[i*W + j];
      if (value == WILL_STAY || value == WILL_BORN) {
        * access (b, row, j+1) = ALIVE;
        alive++;
      }
      else {
        * access (b, row, j+1) = EMPTY;
      }
    }
    //Last row
    value = b->BOARD[(i+1)*W - 1];
    if (value == WILL_STAY || value == WILL_BORN) {
      * access (b, row, W) = ALIVE; alive++;
      if (glidercheckright(b,row, W)) {
        printf("\n------\nGlider found!!\n------RIGHT\n");
        //print(b);
        alive += deleteglider(b,row,W);
        printf("Number of gliders deleted is %d\n", b->GLIDERS);
      }
      else {enlarge[2] = 1;}
    }
    else {* access(b,row,W) = EMPTY;}
  }
  for(int i = W*(H-1); i < W*H; i++) {//Checking the last row
    cell value = b->BOARD[i];
    int col = ((i+1) % W)==0 ? W : (i+1) % W;
    if (value == WILL_STAY || value == WILL_BORN) {
      * access (b, H , col) = ALIVE; alive++;
      if (glidercheckdown(b,H,col)) {
        printf("\n------\nGlider found!!\n------DOWN\n");
        //print(b);
        alive += deleteglider(b,H,col);
        printf("Number of gliders deleted is %d\n", b->GLIDERS);
      }
      else {enlarge[3] = 1;}
    }
    else {* access(b,H,col) = EMPTY;}
  }
  b->ALIVE = alive;
  if (enlarge[0]) {enlargeup(b);}
  if (enlarge[1]) {enlargeleft(b);}
  if (enlarge[2]) {enlargeright(b);}
  if (enlarge[3]) {enlargedown(b);}
}

board* readfile (char* filename) {//Makes sure that there are 5 empty levels
  FILE * fd = fopen (filename, "r");
  cell* input_board = malloc(1000*sizeof(cell));
  cell* initial = input_board;
  char C;
  int i = 0, lines = 0, alive = 0;
  fscanf (fd, "%c", &C);
  while (feof (fd) == 0) {
    if (C == '\n' || C == '\0') { lines++; }
    else {
      if (C == 'X') {
        *input_board = ALIVE;
        alive++;
        input_board++; i++;
      }
      else if (C == '-') {
        *input_board = EMPTY;
        input_board++; i++;
      }
    }
    fscanf (fd, "%c", &C);
  }
  int inputW = (i+1)/lines;
  int inputH = lines;
  int W = inputW + 10, H = inputH + 10;
  cell* new_board = malloc(sizeof(cell)*W*H);
  int pos;
  for (pos = 0; pos < 5*W; pos++) {
    new_board[pos] = EMPTY;
  }
  for (int i = 0; i < inputH; i++) {
    for (int j = 0; j < 5; j++) {
      new_board[pos++] = EMPTY;
    }
    for (int j = 0; j < inputW; j++) {
      new_board[pos++] = initial[j + i*inputW];
    }
    for (int j = 0; j < 5; j++) {
      new_board[pos++] = EMPTY;
    }
  }
  for (int i = 0; i < 5*W; i++) {
    new_board[pos++] = EMPTY;
  }
  board * b = (board *) malloc(sizeof(board));
  b-> WIDTH = W;
  b-> HEIGHT = H;
  b-> BOARD = new_board;
  b-> ALIVE = alive;
  b-> GLIDERS = 0;
  fclose(fd);
  return b;
}

int main (int argc, char ** argv) {
  board *b = NULL;
  //b = readfile(argv[1]);
  b = initialize(argv[1]);
  FILE * fo = fopen (argv [2] , "wb");
  /*printf("Initial\n");
  print(b);
  printf("Alivecount is %d\n", b->ALIVE);*/
  int i = 0, alive[5] = {0,0,0,0,0}, cont = 1;
  while (cont){
    i++;
    process(b);
    //print(b);
    for (int j = 1; j < 5; j++) {
      alive[j-1] = alive[j];
    }
    alive[4] = b->ALIVE + b->GLIDERS * 5;
    if (alive[0]==alive[1] && alive[1]==alive[2] && alive[2]==alive[3] && alive[3]==alive[4]) {
      cont = 0;
    }
  }
  fprintf(fo,"%d %d",i-4, alive[4]);
   /* 
  for (char c ; c = getc(stdin) != 'q';){
    printf("After turn %d\n", i++);
    process(b);
    print(b);
    for (int i = 1; i < 5; i++) {
      alive[i-1] = alive[i];
    }
    alive[4] = b->ALIVE + b->GLIDERS * 5;
    printf("Alivecount is %d\n", alive[4]);
  } */
  fclose (fo);
  return(0);
}
