/* EE3980 Project Comparing O(n) and O(nlgn) Sorts
 * 105061212, Chia-Chun Wang
 * 2019/06/07
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

int N;										// input data size
int** data;									// input data
int** arr;									// copied data array
int Nrepeat = 10;							// repeat times

void readInput(void);						// read input data
void copyArray(void);						// copy array values
double GetTime(void);						// get local CPU time
void reverseNegative(void);					// reverse order of negative part
void radixSort(int** A);					// linear radix sort
void countingSort(int** A, int d);			// stable counting sort
void HeapSort(int** list, int n);			// in-place heap sort
void Heapify(int** list, int root, int n);	// rearrange to form max heap
void mergeSort(int** A, int low, int high);	// stable merge sort
// merge two arrays
void merge(int** A, int low, int mid, int high);
int numberComparison(int* a, int* b);		// compare value of two numbers
void printResult(void);						// print out results
void freeMemory(void);						// free dynamic memories

int main(void)
{
	int i;									// loop indxe
	double t0, t1, t2, t3;					// local CPU time
	
    readInput();							// read input data
	
	t0 = GetTime();							// get current CPU time
	
	for (i = 1; i <= Nrepeat; i++) {		// repeat Nrepeat times
		copyArray();						// copied data array
		radixSort(arr);						// linear radix sort
		if (i == 1) {
			printResult();					// print out results
		}
	}
	
	t1 = GetTime();							// get current CPU time
	printf("Average CPU time -- Radix Sort: %e seconds\n",
			(t1 - t0) / Nrepeat);
	
	for (i = 1; i <= Nrepeat; i++) {		// repeat Nrepeat times
		copyArray();						// copied data array
		HeapSort(arr, N);					// in-place heap sort
	}
	
	t2 = GetTime();							// get current CPU time
	printf("Average CPU time -- Heap Sort:  %e seconds\n",
			(t2 - t1) / Nrepeat);
			
	for (i = 1; i <= Nrepeat; i++) {		// repeat Nrepeat times
		copyArray();						// copied data array
		mergeSort(arr, 1, N);				// stable merge sort
		reverseNegative();					// reverse order of negative part
	}
	
	t3 = GetTime();							// get current CPU time
	printf("Average CPU time -- Merge Sort: %e seconds\n",
			(t3 - t2) / Nrepeat);
			
	freeMemory();							// free dynamic memories
	
    return 0;
}

void readInput(void)						// read input data
{
	int i, j, k;							// loop index
	int t;									// temporary
	char in;								// character input
	int done;								// flag at the end of number
	int state;								// state of integer / decimal part
	int exp;								// flag when exponential happens
	
	scanf("%d", &N);						// input data size
	
	// allocate dynamic memories
	data = (int**)malloc(sizeof(int*) * (N + 1));
	arr = (int**)malloc(sizeof(int*) * (N + 1));
	
	for (i = 1; i <= N; i++) {				// for each number
		// allocate dynamic memories
		data[i] = (int*)malloc(sizeof(int) * 21);
		arr[i] = (int*)malloc(sizeof(int) * 21);
		
		// initialize
		for (j = 0; j <= 20; j++) {
			data[i][j] = 0;
		}
		
		scanf("%d", &t);					// integer label
		scanf("%c", &in);					// space
		
		// initialize
		j = 0;
		done = 0;
		state = 0;
		
		while (done == 0) {					// if not the end
			scanf("%c", &in);				// input digit
			if (j == 0) {					// first digit
				if (in == '-') {			// negative
					data[i][0] = 0;
					j++;
				}
				else {						// positive
					data[i][0] = 1;
					data[i][1] = in - 48;	// store in the second digit
					j += 2;
				}
			}
			else if (in == '\n') {			// end of a number
				done = 1;					// set flag
			}
			else if (in == '.') {			// decimal digit
				for (k = 11; j >= 1; k--) {	// shift to fit the format
					data[i][k] = data[i][j];
					j--;
				}
				while (k >= 1) {			// fill zeroes in the blank
					data[i][k] = 0;
					k--;
				}
				j = 11;						// continue from decimal part
				state = 1;
			}
			else if (in == 'e') {			// exponential number
				scanf("%c", &in);			// + / -
				scanf("%d", &exp);			// exponent
				if (in == '+') {			// exponent > 0
					// digits shifting
					for (k = exp + 1; k <= 20; k++) {
						data[i][k-exp] = data[i][k];
					}
					// fill zeroes in the blank
					for (k = 20 - exp + 1; k <= 20; k++) {
						data[i][k] = 0;
					}
				}
				else {						// exponent < 0
					// digits shifting
					for (k = 20 - exp; k >= 1; k--) {
						data[i][k+exp] = data[i][k];
					}
					// fill zeroes in the blank
					for (k = exp; k >= 1; k--) {
						data[i][k] = 0;
					}
				}
				done = 1;					// set flag
			}
			else {							// normal digit
				data[i][j] = in - 48;
				j++;
			}
		}
		
		if (state == 0) {					// only integer part
			for (k = 11; j >= 1; k--) {		// shift to fit the format
				data[i][k] = data[i][j];
				j--;
			}
			while (k >= 1) {				// fill zeroes in the blank
				data[i][k] = 0;
				k--;
			}
		}
	}
}

void copyArray(void)						// copy array values
{
	int i, j;								// loop index
	
	for (i = 1; i <= N; i++) {				// for every number
		for (j = 0; j <= 20; j++) {			// for every digit
			arr[i][j] = data[i][j];			// copy value
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

void reverseNegative(void)					// reverse order of negative part
{
	int i, j;								// loop index
	int neg;								// largest negative number position
	int done;								// flag when found successfully
	int* t;									// temporary
	
	if (arr[1][0] == 1) {					// all positive, nothing to do
		return;
	}
	
	// find the position of the largest negative number
	done = 0;								// reset flag
	for (i = 1; i <= N && done == 0; i++) {	// for every number
		if (arr[i][0] == 1) {				// find positive
			neg = i - 1;					// largest negative number index
			done = 1;						// set flag
		}
	}
	if (done == 0) {						// if didn't find
		neg = N;							// all negative
	}
	
	t = (int*)malloc(sizeof(int) * 21);		// allocate dynamic memories
	
	for (i = 1; i <= neg / 2; i++) {		// iterate until half of the index
		for (j = 0; j <= 20; j++) {			// swap the values
			t[j] = arr[i][j];
			arr[i][j] = arr[neg+1-i][j];
			arr[neg+1-i][j] = t[j];
		}
	}
	
	free(t);								// free dynamic memories
}

void radixSort(int** A)						// linear radix sort
{
	int i;									// loop index
	
	for (i = 20; i >= 0; i--) {				// for all digit
		countingSort(A, i);					// stable counting sort
	}
	
	reverseNegative();						// reverse order of negative part
}

void countingSort(int** A, int d)			// stable counting sort
{
	int i, j;								// loop index
	int** B;								// array to store result
	int* C;									// array to count the number
	int t;									// temporary index
	
	// allocate dynamic memories
	B = (int**)malloc(sizeof(int*) * (N + 1));
	for (i = 1; i <= N; i++) {
		B[i] = (int*)malloc(sizeof(int) * 21);
	}
	
	// allocate dynamic memories
	C = (int*)malloc(sizeof(int) * 10);
	
	// initialize C
	for (i = 0; i <= 9; i++) {
		C[i] = 0;
	}
	
	// count number of elements in C[A[i]]
	for (i = 1; i <= N; i++) {
		t = A[i][d];
		C[t]++;
	}
	
	// C[i] is the accumulate number of elements
	for (i = 1; i <= 9; i++) {
		C[i] = C[i] + C[i-1];
	}
	
	// store sorted order in B
	for (i = N; i >= 1; i--) {
		t = A[i][d];
		for (j = 0; j <= 20; j++) {
			B[C[t]][j] = A[i][j];
		}
		C[t]--;
	}
	
	// assign B to A
	for (i = 1; i <= N; i++) {
		for (j = 0; j <= 20; j++) {
			A[i][j] = B[i][j];
		}
	}
	
	// free dynamic memories
	free(B);
	free(C);
}

void HeapSort(int** list,int n)				// in-place heap sort
{
    int i, j;								// loop index
    int* t;									// temporary for swap
	
	t = (int*)malloc(sizeof(int) * 21);		// allocate dynamic memories

    // heapify all the subtrees and be a max heap
    for (i = n/2; i >= 1; i--) {
        Heapify(list, i, n);
    }

    for (i = n; i >= 2; i--) {				// repeat n-1 times
		//swap the first node and the last node
		for (j = 0; j <= 20; j++) {
			t[j] = list[i][j];
			list[i][j] = list[1][j];
			list[1][j] = t[j];
		}

		// make list[1:i-1] be a max heap
        Heapify(list, 1, i-1);
    }
	
	reverseNegative();						// reverse order of negative part
	
	free(t);								// free dynamic memories
}

void Heapify(int** list, int root, int n)	// rearrange to form max heap
{
    int* t = list[root];					// temporary array
    int j, k;								// loop index
	int done = 0;							// check if heapify ends
	
	t = (int*)malloc(sizeof(int) * 21);		// allocate dynamic memories
	for (j = 0; j <= 20; j++) {
		t[j] = list[root][j];				// assign root value to t
	}

	// j is the leftchild of root and keeps finding its leftchild
    for (j = root * 2; j <= n && done == 0;) {
        if (j < n && numberComparison(list[j], list[j+1]) == 0) {
			// j is the rchild if rchild > lchild
            j++;
        }
        if (numberComparison(t, list[j]) == 1) {
            done = 1;						// done if root > children
        }
        else {
			for (k = 0; k <= 20; k++) {
				list[j/2][k] = list[j][k];	// place child node to parent
			}
			j *= 2;
        }
    }

	for (k = 0; k <= 20; k++) {
		list[j/2][k] = t[k];				// put root to the proper place
	}
	
	free(t);								// free dynamic memories
}

void mergeSort(int** A, int low, int high)	// stable merge sort
{
	int mid;								// medium of the index
	
	if (low < high) {						// unarranged item
		mid = (low + high) / 2;				// find mid
		mergeSort(A, low, mid);				// merge sort for first half part
		mergeSort(A, mid + 1, high);		// merge sort for second half part
		merge(A, low, mid, high);			// merge two parts
	}
}

// merge two arrays
void merge(int** A, int low, int mid, int high)
{
	int g, h, i, j, k;						// loop index
	int** B;								// copied array
	
	// allocate dynamic memories
	B = (int**)malloc(sizeof(int*) * (N + 1));
	for (i = 1; i <= N; i++) {
		B[i] = (int*)malloc(sizeof(int) * 21);
	}
	
	// initialize
	h = low;
	i = low;
	j = mid + 1;
	
	while (h <= mid && j <= high) {			// store smaller one to B
		// A[h] is smaller
		if (numberComparison(A[h], A[j]) != 1) {
			for (g = 0; g <= 20; g++) {
				B[i][g] = A[h][g];
			}
			h++;
		}
		// A[j] is smaller
		else {
			for (g = 0; g <= 20; g++) {
				B[i][g] = A[j][g];
			}
			j++;
		}
		i++;
	}
	
	if (h > mid) {							// second part remaining
		for (k = j; k <= high; k++) {
			for (g = 0; g <= 20; g++) {
				B[i][g] = A[k][g];
			}
			i++;
		}
	}
	else {									// first part remaining
		for (k = h; k <= mid; k++) {
			for (g = 0; g <= 20; g++) {
				B[i][g] = A[k][g];
			}
			i++;
		}
	}
	
	for (k = low; k <= high; k++) {
		for (g = 0; g <= 20; g++) {
			A[k][g] = B[k][g];				// copy B to A
		}
	}
	
	// free dynamic memories
	for (i = 1; i <= N; i++) {
		free(B[i]);
	}
	free(B);
}

int numberComparison(int* a, int* b)		// compare value of two numbers
{
	int i;									// loop index
	
	for (i = 0; i <= 20; i++) {				// start from the first digit
		if (a[i] > b[i]) {
			return 1;						// a number > b number
		}
		if (a[i] < b[i]) {
			return 0;						// a number < b number
		}
	}
	
	return -1;								// a number = b number
}

void printResult(void)						// print out results
{
	int i, j;								// loop index
	int begin, end;							// start / end digit of a number
	int done;								// flag when found
	
	printf("N = %d\n", N);					// data size
	
	for (i = 1; i <= N; i++) {				// for every number
		printf("  %d ", i);					// label
		if (arr[i][0] == 0) {
			printf("-");					// negative sign
		}
		
		// initialize
		begin = 1;
		done = 0;
		
		// find the index of beginning
		for (j = 1; j <= 10 && done == 0; j++) {
			if (arr[i][j] != 0) {
				begin = j;
				done = 1;
			}
		}
		if (done == 0) {					// only decimal part
			begin = 10;
		}
		
		// initialize
		end = 1;
		done = 0;
		
		// find the index of end
		for (j = 20; j >= 11 && done == 0; j--) {
			if (arr[i][j] != 0) {
				end = j;
				done = 1;
			}
		}
		if (done == 0) {					// only integer part
			end = 10;
		}
		
		for (j = begin; j <= end; j++) {	// print out number
			if (j == 11) {
				printf(".%d", arr[i][j]);
			}
			else {
				printf("%d", arr[i][j]);
			}
		}
		
		printf("\n");
	}
}

void freeMemory(void)						// free dynamic memories
{
	int i;									// loop index
	
	// free dynamic memories
	for (i = 1; i <= N; i++) {
		free(arr[i]);
		free(data[i]);
	}
	
	free(arr);
	free(data);
}