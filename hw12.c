/* EE3980 HW12 Travelling Salesperson Problem
 * 105061212, Chia-Chun Wang
 * 2019/05/31
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#define INT_MAX 10000						// maximum distance

// Structure for each branch and bound step
typedef struct Step
{
	int** arr;								// 2d distance data array
	int* path_arr;							// path of visited cities
	int path_size;							// number of cities visited
	int cost;								// cumulated cost so far
} step;

int N;										// input data size
char** name_arr;							// array to store city names
int** data;									// distance array
step* s;									// branch and bound steps
int s_size;									// number of steps done

void readInput(void);						// read input data
double GetTime(void);						// get local CPU time
void copyCostArray(int** in, int** out);	// copy the 2d distance array
void copyPathArray(step in, step out);		// copy the visited cities data
void initNewStep(void);						// initialize new step
void costReduce(void);						// cost reduction on distance array
void adjustArray(int start, int end);		// adjust cost array by constraints
void TSP(void);								// TSP algorithm
void printResult(step ans);					// print out result
void freeMemory(void);						// free dynamic memories

int main(void)
{
	double t0, t1;							// CPU time data
	
	readInput();							// read input data
	
	t0 = GetTime();							// get current CPU time
	
	TSP();									// TSP algorithm
	
	t1 = GetTime();							// get current CPU time
	
	// calculate CPU time used (if needed)
	printf("CPU time used: %e seconds\n", t1 - t0);
	
    return 0;
}

void readInput(void)						// read input data
{
	int i, j;								// loop index
	char t;									// temporary storage
	int check;								// flag for end of line
	
	scanf("%d", &N);						// read data size
	
	// allocate dynamic memories
	name_arr = (char**)malloc(sizeof(char*) * (N + 1));
	data = (int**)malloc(sizeof(int*) * (N + 1));
	for (i = 1; i <= N; i++) {
		name_arr[i] = (char*)malloc(sizeof(char) * 20);
		data[i] = (int*)malloc(sizeof(int) * (N + 1));
	}
	
	scanf("%c", &t);						// read newline character
	
	for (i = 1; i <= N; i++) {				// for N cities
		check = 0;							// reset flag
		for (j = 0; check == 0; j++) {
			scanf("%c", &t);				// read and store in temporary
			if (t != '\n') {				// if not end of line
				name_arr[i][j] = t;			// store data
			}
			else {							// end of line
				name_arr[i][j] = '\0';		// end of string
				check = 1;					// set flag for new line
			}
		}
	}
	
	for (i = 1; i <= N; i++) {
		for (j = 1; j <= N; j++) {
			scanf("%d", &data[i][j]);		// read distance array
			if (data[i][j] == 0) {			// if distance = 0
				data[i][j] = INT_MAX;		// set to maximum
			}
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

void copyCostArray(int** in, int** out)		// copy the 2d distance array
{
	int i, j;								// loop index
	
	for (i = 1; i <= N; i++) {
		for (j = 1; j <= N; j++) {
			out[i][j] = in[i][j];			// copy each integer
		}
	}
}

void copyPathArray(step in, step out)		// copy the visited cities data
{
	int i;									// loop index
	
	for (i = 1; i <= in.path_size; i++) {
		out.path_arr[i] = in.path_arr[i];	// copy each integer
	}
	out.path_size = in.path_size;			// copy size data
}

void initNewStep(void)						// initialize new step
{
	int i;									// loop index
	
	s_size++;								// number of steps + 1
	
	// allocate dynamic memories
	s[s_size].arr = (int**)malloc(sizeof(int*) * (N + 1));
	for (i = 1; i <= N; i++) {
		s[s_size].arr[i] = (int*)malloc(sizeof(int) * (N + 1));
	}
	s[s_size].path_arr = (int*)malloc(sizeof(int) * (N + 1));
	
	s[s_size].cost = 0;						// reset cost data
}

void costReduce(void)						// cost reduction on distance array
{
	int i, j;								// loop index
	int mini;								// minimum number
	
	// reduce by row
	for (i = 1; i <= N; i++) {
		mini = INT_MAX;						// initialize
		for (j = 1; j <= N; j++) {
			if (s[s_size].arr[i][j] < mini) {
				mini = s[s_size].arr[i][j]; // find minimum
			}
		}
		for (j = 1; j <= N; j++) {
			if (s[s_size].arr[i][j] != INT_MAX) {
				s[s_size].arr[i][j] -= mini;// reduction to each element
			}
		}
		if (mini != INT_MAX) {
			s[s_size].cost += mini;			// add to cost
		}
	}
	
	// reduce by column
	for (i = 1; i <= N; i++) {
		mini = INT_MAX;						// initialize
		for (j = 1; j <= N; j++) {
			if (s[s_size].arr[j][i] < mini) {
				mini = s[s_size].arr[j][i];	// find minimum
			}
		}
		for (j = 1; j <= N; j++) {
			if (s[s_size].arr[j][i] != INT_MAX) {
				s[s_size].arr[j][i] -= mini;// reduction to each element
			}
		}
		if (mini != INT_MAX) {
			s[s_size].cost += mini;			// add to cost
		}
	}
}

void adjustArray(int start, int end)		// adjust cost array by constraints
{
	int i;									// loop index
	
	// adjust row
	for (i = 1; i <= N; i++) {
		s[s_size].arr[start][i] = INT_MAX;
	}
	
	// adjust column
	for (i = 1; i <= N; i++) {
		s[s_size].arr[i][end] = INT_MAX;
	}
	
	// adjust the path to the starting point if not the end
	if (s[s_size].path_size < N) {
		s[s_size].arr[end][1] = INT_MAX;
	}
	
	costReduce();							// cost reduction
}

void TSP(void)								// TSP algorithm
{
	int i;									// loop index
	int mini;								// minimum cost
	int s_idx;								// starting point index
	int arr_len;							// number of visited cities
	int start;								// starting city
	
	// initialize
	s = (step*)malloc(sizeof(step) * (10000000));
	s_size = 0;
	
	// add the first element
	initNewStep();
	copyCostArray(data, s[1].arr);
	s[1].path_arr[1] = 1;
	s[1].path_size = 1;
	
	// cost reduce
	costReduce();
	
	while (1) {								// loop until end
		// find min element in the undone steps
		mini = INT_MAX;						// initialize
		for (i = 1; i <= s_size; i++) {
			if (s[i].cost < mini) {
				mini = s[i].cost;			// update minimum
				s_idx = i;					// record index
			}
		}
		
		// check if it is end
		if (s[s_idx].path_size == N) {		// if end
			printResult(s[s_idx]);			// print out result
			return;							// terminate algorithm
		}
		
		// for all child, find out cost
		for (i = 1; i <= N; i++) {			// for all child
			arr_len = s[s_idx].path_size;	// number of visited cities
			start = s[s_idx].path_arr[arr_len];
			// feasible path
			if (s[s_idx].arr[start][i] != INT_MAX) {
				initNewStep();				// initialize new step
				// copy array data
				copyCostArray(s[s_idx].arr, s[s_size].arr);
				copyPathArray(s[s_idx], s[s_size]);
				// update path array data
				s[s_size].path_size = s[s_idx].path_size + 1;
				s[s_size].path_arr[s[s_size].path_size] = i;
				// add the edge cost
				s[s_size].cost = s[s_idx].cost + s[s_idx].arr[start][i];
				adjustArray(start, i);		// adjust cost array by constraints
			}
		}
		
		// delete visited item
		s[s_idx].cost = INT_MAX;
	}
}

void printResult(step ans)					// print out result
{
	int i;									// loop index
	int s, t;								// index of starting and terminal
	
	printf("Minimum distance route:\n");
	
	// print out city names
	for (i = 1; i <= N - 1; i++) {
		s = ans.path_arr[i];
		t = ans.path_arr[i+1];
		printf(" %s -> %s\n", name_arr[s], name_arr[t]);
	}
	s = ans.path_arr[N];
	t = ans.path_arr[1];
	printf(" %s -> %s\n", name_arr[s], name_arr[t]);
	
	// print out minimum cost
	printf("Total travelling distance: %d\n", ans.cost);
}

void freeMemory(void)						// free dynamic memories
{
	int i;									// loop index
	
	free(s);
	for (i = 1; i <= N; i++) {
		free(data[i]);
		free(name_arr[i]);
	}
	free(data);
	free(name_arr);
}