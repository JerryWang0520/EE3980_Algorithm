/* EE3980 HW11 Sudoku
 * 105061212, Chia-Chun Wang
 * 2019/05/23
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

int** data;									// input data
int** A;									// copied array
int** space;								// row and column of blank space
int space_size = 0;							// number of blank space
int R = 100;								// repeat times
int sol_num = 0;							// number of solutions

void readInput(void);						// read input data
double GetTime(void);						// get local CPU time
void copyArray(void);						// copy array data
void backtracking(int repeat);				// backtracking algorithm
int placeable(int r, int c, int value);		// check if the number is placeable
void printResult(void);						// print out the result
void freeMemory(void);						// free dynamic memories

int main(void)
{
	int i;									// loop index
	double t0, t1;							// CPU time
	
	readInput();							// read input data
	
	t0 = GetTime();							// get local CPU time
	
	for (i = 1; i <= R; i++) {				// repeat R times
		copyArray();						// copy array data
		backtracking(i);					// solve Sudoku
	}
	
	t1 = GetTime();							// get local CPU time
	
	// print out results
	printf("Total number of solutions found: %d.\n", sol_num);
	printf("CPU time = %e sec\n", (t1 - t0) / R);
	
	freeMemory();							// free dynamic memories
	
    return 0;
}

void readInput(void)						// read input data
{
	int i, j;								// loop index
	char t;									// temporary input
	
	// allocate dynamic memories
	data = (int**)malloc(sizeof(int*) * 10);
	A = (int**)malloc(sizeof(int*) * 10);
	space = (int**)malloc(sizeof(int*) * 82);
	for (i = 0; i <= 9; i++) {
		data[i] = (int*)malloc(sizeof(int) * 10);
		A[i] = (int*)malloc(sizeof(int) * 10);
	}
	for (i = 0; i <= 81; i++) {
		space[i] = (int*)malloc(sizeof(int) * 2);
	}
	
	// read input data
	for (i = 1; i <= 9; i++) {
		for (j = 1; j <= 9; j++) {
			scanf("%c", &t);
			if (t == '.') {					// blank space
				data[i][j] = 0;
				space_size++;				// record the row and column
				space[space_size][0] = i;
				space[space_size][1] = j;
			}
			else {							// number defined
				data[i][j] = t - 48;
			}
			scanf("%c", &t);				// space or new line character
		}
	}

}

double GetTime(void)						// get local time in seconds
{
    struct timeval tv;						// time interval structure

    gettimeofday(&tv, NULL);				// write local time into tv

	// return time with microsecond
    return tv.tv_sec + tv.tv_usec * 0.000001;
}

void copyArray(void)						// copy array data
{
	int i, j;								// loop index
	
	for (i = 1; i <= 9; i++) {
		for (j = 1; j <= 9; j++) {
			A[i][j] = data[i][j];			// copy every data
		}
	}
}

void backtracking(int repeat)				// backtracking algorithm
{
	int r = space[1][0];					// started row index
	int c = space[1][1];					// started column index
	int k = 1;								// blank space index
	
	while (k > 0) {							// if there is still blank space
		A[r][c]++;							// try new value
		while (A[r][c] <= 9) {				// can find value
			if (placeable(r, c, A[r][c]) == 1) {
				if (k == space_size && repeat == 1) {
					printResult();			// one solution found
				}
				else {						// keep finding next element
					k++;
					r = space[k][0];
					c = space[k][1];
					A[r][c] = 0;			// initialize to 0
				}
			}
			A[r][c]++;						// try new value
		}
		k--;								// backtrack the previous
		r = space[k][0];
		c = space[k][1]; 
	}
}

int placeable(int r, int c, int value)		// check if the number is placeable
{
	int i, j;								// loop index
	int row_start, col_start;				// started row/column index
	
	for (i = 1; i <= 9; i++) {				// check the row
		if (A[r][i] == value && i != c) {
			return 0;
		}
	}
	
	for (i = 1; i <= 9; i++) {				// check the column
		if (A[i][c] == value && i != r) {
			return 0;
		}
	}
	
	// calculating the starting point of the 3x3 submatrix
	row_start = ((r - 1) / 3) * 3 + 1;
	col_start = ((c - 1) / 3) * 3 + 1;
	
	// check the submatrix
	for (i = row_start; i <= row_start + 2; i++) {
		for (j = col_start; j <= col_start + 2; j++) {
			if (A[i][j] == value && !(i == r && j == c)) {
				return 0;
			}
		}
	}
	
	return 1;								// placeable
}

void printResult(void)						// print out the results
{
	int i, j;								// loop index
	
	sol_num++;								// number of solution + 1
	printf("Solution %d:\n", sol_num);
	
	// print out the matrix
	for (i = 1; i <= 9; i++) {
		printf("  ");
		for (j = 1; j <= 9; j++) {
			if (j == 9) {
				printf("%d\n", A[i][j]);
			}
			else if (j == 3 || j == 6) {
				printf("%d | ", A[i][j]);
			}
			else {
				printf("%d ", A[i][j]);
			}
		}
		if (i == 3 || i == 6) {
			printf("  ------|-------|------\n");
		}
	}
}

void freeMemory(void)						// free dymanic memories
{
	int i;									// loop index
	
	// free the allocated memories
	for (i = 0; i <= 9; i++) {
		free(data[i]);
	}
	for (i = 0; i <= 81; i++) {
		free(space[i]);
	}
	free(data);
	free(space);
}


