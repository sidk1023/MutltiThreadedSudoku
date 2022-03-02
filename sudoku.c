#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

/**
 * NxN sudoku will create 3 threads.
 * 1 for each N rows
 * 1 for each N cols
 * 1 for each N sub squares
 */

/**
 * Each thread will perform a verification task to verify weather or not 
 * the element input already exists 
 * 1. In the same Row
 * 2. In the same column
 * 3. In the same sub box
 */

/**
 * Structure that holds the parameters passed to a thread.
 * This specifies where the thread should start checking.
 * A struct is initialised each for the N rows, N columns, and N squares.
 */
typedef struct 
{
    // The starting row.
    int row;
    // The sint solve(parameters** param, int row, int col)tarting column.
    int col;
    // The pointer to the board.
    int (* board)[36];
    // Size of the board size = N.
    int size;
    // Number to check
    int num;
} parameters;

// Prototype for the check_rows function.
void * check_row(void * params);

// Prototype for the check_cols function.
void * check_col(void * params);

// Prototype for n x n square function.
void * check_square(void * params);

// Prototype for box Checker *
int checkBox (parameters **param,int row, int col, int input);

//prototype for solver
int solve(parameters** param, int (* board)[36],int row, int col);

void read_grid_from_file(int size, char *ip_file, int grid[36][36]) {
	FILE *fp;
	int i, j;
	fp = fopen(ip_file, "r");
	for (i=0; i<size; i++) 
		for (j=0; j<size; j++) {
			fscanf(fp, "%d", &grid[i][j]);
	}
} 
void print_grid(int size, int grid[36][36]) {
	int i, j;
	/* The segment below prints the grid in a standard format. Do not change */
	for (i=0; i<size; i++) {
		for (j=0; j<size; j++)
			printf("%d\t", grid[i][j]);
		printf("\n");
	}
}

int main(int argc, char *argv[]){
    int grid[36][36], size, i, j;
	if (argc != 3) {
		printf("Usage: ./sudoku.out grid_size inputfile");
		exit(-1);
	}
	size = atoi(argv[1]);
	read_grid_from_file(size, argv[2], grid);
    int N = size;
    int n = sqrt(N);
 // Create the parameter structs, 1 for each row, column, and square
 parameters **param;
 param = malloc(3*N*(sizeof(parameters)));


// First N params are for each Row
// Next N params are for each Column
//Third N params are for each Square
 
//Row parameters
for (int i=0,k=0; i<N;i++,k++){
    param[k] = malloc(sizeof(parameters));
    param[k]->row = i;
    param[k]->col = 0;
    param[k]->size = N;
    param[k]->board = grid;
    param[k]->num = 0;
    //printf("row%d\n",k);
}

//Col parameters
for (int i=0,k=N; i<N;i++,k++){
    param[k] = malloc(sizeof(parameters));
    param[k]->row = 0;
    param[k]->col = i;
    param[k]->size = N;
    param[k]->board = grid;
    param[k]->num = 0;
    //printf("col%d\n",k);
    
}

//Square parameters
int k = 2*N;
for (int i=0; i<N;i+=n){
    for (int j=0;j<N;j+=n){
    param[k] = malloc(sizeof(parameters));
    param[k]->row = i;
    param[k]->col = j;
    param[k]->size = N;
    param[k]->board = grid;
    param[k]->num = 0;
    //printf("sq%d\n",k);
    k+=1;
    }
}

// if(solve(param,grid,0,0)==1)
// print_grid(N,grid);
// else{
//     printf("Invalid");
// }
solve(param,grid,0,0);
print_grid(N,grid);
return 0;
}






//Input to function is the struct for the particular row.
//Return 1 if number is safe to enter into the row
void * check_row (void * params){
    parameters * input = (parameters *) params;
    int startI = input->row;
    int startJ = input->col;
    int N = input->size;
    int num = input->num;
    //printf("\n%d numrow\n",num);
    for (int j=0;j<N;j++){
        //printf("%d",input->board[startI][j]);
        if((input->board[startI][j]!=0)&&(input->board[startI][j]==num)){
            return (void *) 0;
        }
    }
    return (void *) 1;
}

//Input to the function is the struct for the particular column
//Return 1 if number is safe to enter into the column
void * check_col (void * params){
     parameters * input = (parameters *) params;
    int startI = input->row;
    int startJ = input->col;
    int N = input->size;
    int num = input->num;
    //printf("\n%d numcol\n",num);
    for (int i=0;i<N;i++){
        if((input->board[i][startJ]!=0)&&(input->board[i][startJ]==num)){
            return (void *) 0;
        }
    }
    return (void *) 1;
}

//Input to the function is the struct for the particular square
//Return 1 if number is safe to enter into the square.
void * check_square (void * params){
  parameters * input = (parameters *) params;
    int startI = input->row;
    int startJ = input->col;
    int N = input->size;
    int num = input->num;
    int n = sqrt(N);
    //printf("\n%d,%d %d\n",startI,startJ,n);
    long flag = 1;
    //printf("\n%d numSquare\n",num);
    for (int i = startI; i < startI + n; ++i) {
        for (int j = startJ; j < startJ + n; ++j) {
           //printf("\n i%d j%d\n",i,j);
            if((input->board[i][j]!=0)&&(input->board[i][j]==num)){
              flag = 0;
              break;
        }
    }
   
}
 return (void *) flag;
}

//This function creates three threads to check the validity of an input number
//at a particular box in the puzzle
int checkBox(parameters ** param, int row, int col, int input){
    //parameters** param = *params;
    int N = param[0]->size;
    int n = sqrt(N);
    int square = col/n  + row - row % n;
    param[row]->num = input;
    param[col+N]->num = input;
    param[square+2*N]->num = input;
    
//Create 3 threads
    pthread_t thread_rows, thread_cols, thread_square;

//Create return value for thread
 void * rowReturn;
 void * colReturn;
 void * squareReturn;


// Initialize the threads
   pthread_create(&thread_rows, NULL, check_row, (void*) param[row]);
   pthread_create(&thread_cols, NULL, check_col, (void*) param[col+N]);
   pthread_create(&thread_square,NULL, check_square,(void*) param[square+2*N]);

//Wait till threads finish the task
    pthread_join(thread_rows,&rowReturn);
    pthread_join(thread_cols,&colReturn);
    pthread_join(thread_square,&squareReturn);

//Check if the input is valid in the box.
   if(((long) rowReturn==1)&&((long) colReturn==1)&&((long)squareReturn==1)){
       return 1;
   }
   return 0;
}


//This function recursively calls itself to traverse the entire grid and try inputs from 1 to N
//At each box. It is an implementation of the backtracking algorithm that uses the checkBox function 
//to incorperate multithreading.
int solve(parameters** param,int (* board)[36],int row, int col){
    //get data of grid
    int N = param[0]->size;
    int n = sqrt(N);
    
    //If we reached the end of the sudoku, return
     if((row==N-1)&&(col==N)){
         return 1;
     }

     //If We have reached the 9th column, 
     //move to the start of the next row
        if (col == N) {
        col = 0;
        row++;
    }

    //If the value present in the current 
    //selected box is non 0, move to the next column
    if (board[row][col]>0){
       return solve(param,board,row,col+1);
    }

    //Check all digits at a box
    for (int input = 1; input<=N;input++){
        //if an input is valid, assign to the grid box
        if (checkBox(param,row,col,input)==1){
            board[row][col] = input;
            //check for next column
            if (solve(param,board,row,col+1)){
                return 1;
            }
        }

        //remove the assigned number if assumption was wrong
        board[row][col] = 0;
        
    }
return 0;
}