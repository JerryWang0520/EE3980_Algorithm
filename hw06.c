/* EE3980 HW06 Power Ranking
 * 105061212, Chia-Chun Wang
 * 2019/04/11
 * Revised at 2019/06/05
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

typedef struct listnode						// adjacency list structure
{
	int adj;								// adjacency vertex
	struct listnode* next;					// next element of list
} Node;

// read input data and construct adjacency list
void readInput(void);
double GetTime(void);						// get local CPU time
void topsort_call(int number);				// topological sort function call
void top_sort(int start);					// topological sort function
void printResult(void);						// print out results

int Nrepeat = 100000;						// repeat times
int V;										// number of vertices (players)
int E;										// number of edges (games)
char** name_arr;							// array to store names
int* visit;									// record if vertex is visited
int* result;								// record the result
int result_top;								// result array index
Node* list;									// adjacency list

int main(void)
{
    int i;									// loop index
	double t0, t1, t_avg;					// CPU time

	// read input data and construct adjacency list
    readInput();

	t0 = GetTime();							// get CPU time

	for (i = 0; i < Nrepeat; i++) {			// repeat Nrepeat times
		topsort_call(i);					// topological sort
	}

	t1 = GetTime();							// get CPU time
	t_avg = (t1 - t0) / Nrepeat;			// calculate average CPU time

	// print out CPU time result
	printf("%d players %d games CPU time = %e sec\n", V, E, t_avg);

	free(list);								// free dynamic memories

    return 0;
}

// read input data and construct adjacency list
void readInput(void)
{
	int i, j;								// loop index
	char win[10];							// string of winner's name
	char lose[10];							// string of loser's name
	char t[1];								// store ">"
	int win_i;								// winner's array index
	int lose_i;								// loser's array index
	int done;								// flag during finding
	Node* tp;								// new node for list

	// player and game numbers
	scanf("%d", &V);
	scanf("%d", &E);

	// allocate dynamic memories for each vertex and initialize
	list = (Node*)malloc(sizeof(Node) * V);
	name_arr = (char**)malloc(sizeof(char*) * V);
	for (i = 0; i < V; i++) {
		list[i].next = NULL;
		name_arr[i] = (char*)malloc(sizeof(char) * 10);
	}

	for (i = 0; i < V; i++) {
		scanf("%s", name_arr[i]);			// store players' name
	}

	for (i = 0; i < E; i++) {				// for every games
		// store winner's and loser's names
		scanf("%s", win);
		scanf("%s", t);
		scanf("%s", lose);
		
		done = 0;							// reset flag

		// find winner's array index
		for (j = 0; j < V && done == 0; j++) {
			if (strcmp(name_arr[j], win) == 0) {
				win_i = j;
				done = 1;
			}
		}
		
		done = 0;							// reset flag

		// find loser's array index
		for (j = 0; j < V && done == 0; j++) {
			if (strcmp(name_arr[j], lose) == 0) {
				lose_i = j;
				done = 1;
			}
		}

		// set the loser's index in the adjacency list of winner's
		tp = malloc(sizeof(Node));
		tp->adj = lose_i;
		tp->next = list[win_i].next;
		list[win_i].next = tp;
	}
}

double GetTime(void)						// get local time in seconds
{
    struct timeval tv;						// time interval structure

    gettimeofday(&tv, NULL);				// write local time into tv

	// return time with microsecond
    return tv.tv_sec + tv.tv_usec * 0.000001;
}

void topsort_call(int number)				// topological sort function call
{
	int i;									// loop index

	// allocate dynamic memories
	visit = (int*)malloc(sizeof(int) * V);
	result = (int*)malloc(sizeof(int) * V);

	result_top = V - 1;						// initialize

	for (i = 0; i < V; i++) {				// initialize
		visit[i] = 0;
		result[i] = 0;
	}

	for (i = 0; i < V; i++) {				// for all vertices
		if (visit[i] == 0){					// if not be visited
			top_sort(i);					// topological sort
		}
	}

	if (number == 0){						// if first execution
		printResult();						// print out the results
	}

	// free dynamic memories
	free(visit);
	free(result);
}

void top_sort(int start)					// topological sort function
{
	int adj;								// adjacent vertex
	Node* p;								// pointer for traversal

	visit[start] = 1;						// record visited

	p = &list[start];						// initialize to start
	while (p->next != NULL) {				// if not the end
		p = p->next;						// go to next
		adj = p->adj;
		if (visit[adj] == 0) {				// if not be visited
			top_sort(adj);					// keep topological sort
		}
	}

	// record the vertex to result
	result[result_top] = start;
	result_top--;
}

void printResult(void)						// print out results
{
	int i;									// loop index

	for (i = 0; i < V; i++) {				// for all vertices
		printf("%d ", i + 1);				// print label
		printf("%s\n", name_arr[result[i]]);// print name
	}
}
