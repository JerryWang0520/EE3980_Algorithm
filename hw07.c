/* EE3980 HW07 Linear Sort
 * 105061212, 王家駿
 * 2019/04/20
 * Revised at 2019/07/28
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

int N;										// data size
int max_size;								// maximum string length
int R = 500;								// repeat times
char** data;								// input data
char** A;									// array to be sorted

void readInput(void);						// read all input
double GetTime(void);						// get local time in seconds
void copyArray(char** data, char** A);		// copy data to array A
void radixSort(char** A);					// linear radix sort
void countingSort(char** A, int d);			// stable counting sort
void HeapSort(char** list, int n);			// in-place heap sort
void Heapify(char** list, int root, int n);	// rearrange to form max heap
void printResult();							// print out the result

int main(void)
{
	int i;									// loop index
	double t0, t1, t2, t_radix, t_heap;		// CPU time
	
	readInput();							// read all input
	
	t0 = GetTime();							// get current CPU time
	
	for (i = 1; i <= R; i++) {				// repeat R times
		copyArray(data, A);
		radixSort(A);						// linear sort
		if (i == 1) printResult();			// print out the result
	}
	
	t1 = GetTime();							// get current CPU time
	
	for (i = 1; i <= R; i++) {				// repeat R times
		copyArray(data, A);
		HeapSort(A, N);						// in-place heap sort
	}
	
	t2 = GetTime();							// get current CPU time
	
	// calculate average time consumed
	t_radix = (t1 - t0) / R;
	t_heap = (t2 - t1) / R;
	
	// print out time results
	printf("N = %d\n", N);
	printf("  Linear Sort CPU time = %e seconds\n", t_radix);
	printf("  Heap Sort CPU time = %e seconds\n", t_heap);
	
	// free dynamic memories
	free(A);
	free(data);
	
	return 0;
}

void readInput(void)						// read all input
{
	int i, j;								// loop index
	int check;								// check for the end of string
	char s[15];								// temporary string

	scanf("%d", &N);						// data size

	// allocate dynamic memories
	data = (char**)malloc(sizeof(char*) * (N + 1));
	A = (char**)malloc(sizeof(char*) * (N + 1));
	
	max_size = 0;							// initialize maximum string size
	
	for (i = 1; i <= N; i++) {				// for all data
		// allocate dynamic memories
		data[i] = (char*)malloc(sizeof(char) * 15);
		A[i] = (char*)malloc(sizeof(char) * 15);
		
		scanf("%s", s);						// input string
		
		// find for the maximum string length
		if (strlen(s) > max_size) {
			max_size = strlen(s);
		}
		
		check = 0;
		for (j = 0; j <= 14; j++) {
			// mark characters after the string to be '`'
			if (s[j] == '\0') {
				check = 1;
			}
			if (check == 1) {
				data[i][j] = '`';
			}
			else {
				data[i][j] = s[j];
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

void copyArray(char** data, char** A)		// copy data to array A
{
    int i;									// loop index

    for (i = 1; i <= N; i++) {
        A[i] = data[i];				
		// assign the pointer A[i] to point to the data array
    }
}

void radixSort(char** A)					// linear radix sort
{
	int i;									// loop index
	
	for (i = max_size - 1; i >= 0; i--) {	// for all characters
		countingSort(A, i);					// stable counting sort
	}
}

void countingSort(char** A, int d)			// stable counting sort
{
	int i;									// loop index
	char** B;								// array to store result
	int* C;									// array to count the number
	int t;									// temporary index
	
	// allocate dynamic memories
	B = (char**)malloc(sizeof(char*) * (N + 1));
	for (i = 1; i <= N; i++) {
		B[i] = (char*)malloc(sizeof(char) * 15);
	}
	
	// allocate dynamic memories
	C = (int*)malloc(sizeof(int) * 130);
	
	// initialize C
	for (i = 96; i <= 122; i++) {
		C[i] = 0;
	}
	
	// count number of elements in C[A[i]]
	for (i = 1; i <= N; i++) {
		t = A[i][d];
		C[t]++;
	}
	
	// C[i] is the accumulate number of elements
	for (i = 97; i <= 122; i++) {
		C[i] = C[i] + C[i-1];
	}
	
	// store sorted order in B
	for (i = N; i >= 1; i--) {
		t = A[i][d];
		B[C[t]] = A[i];
		C[t]--;
	}
	
	// assign B to A
	for (i = 1; i <= N; i++) {
		A[i] = B[i];
	}
	
	// free dynamic memories
	free(B);
	free(C);
}

void HeapSort(char** list,int n)			// in-place heap sort
{
    int i;									// loop index
    char* tp;								// temporary pointer for swap

    // heapify all the subtrees and be a max heap
    for (i = n / 2; i >= 1; i--) {
        Heapify(list, i, n);
    }

    for (i = n; i >= 2; i--) {				// repeat n-1 times
		//swap the first node and the last node
        tp = list[i];
        list[i] = list[1];
        list[1] = tp;

		// make list[1:i-1] be a max heap
        Heapify(list, 1, i-1);
    }
}

void Heapify(char** list, int root, int n)	// rearrange to form max heap
{
    char* tp = list[root];					// assign root value to tp
    int j;									// loop index
	int done = 0;							// check if heapify ends

	// j is the leftchild of root and keeps finding its leftchild
    for (j = root * 2; j <= n && done == 0; j = j * 2) {
        if (j < n && strcmp(list[j], list[j+1]) < 0) {
			// j is the rchild if rchild > lchild
            j++;
        }
        if (strcmp(tp, list[j]) > 0) {
            done = 1;						// done if root > children
        }
        else{
            list[j/2] = list[j];			// place child node to parent
        }
    }

    list[j/2] = tp;							// put root to the proper place
}

void printResult()							// print out the result
{
	int i, j;								// loop index
	
	for (i = 1; i <= N; i++) {				// for all data
		for (j = 0; j < 15 && A[i][j] != '`'; j++) {
			// print out results
			printf("%c", A[i][j]);
		}
		printf("\n");
	}
}
