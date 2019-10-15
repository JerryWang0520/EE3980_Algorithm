/* EE3980 HW01 Quadratic Sorts
 * 105061212, 王家駿
 * 2019/03/04
 * Revised at 2019/07/20
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

int N;						                // input size
char** data;					            // input data
char** A;					                // array to be sorted
int R = 500;					            // number of repetitions

int sorting_algorithm = 0;		            // type of sorting algorithm
/* modify the value to choose the sorting algorithm
 * 0: Selection Sort
 * 1: Insertion Sort
 * 2: Bubble Sort
 * 3 or others: Shaker Sort
 */

void readInput(void);		                // read all inputs
void printArray(char** A);			        // print the content of array A
void copyArray(char** data, char** A);		// copy data to array A
double GetTime(void);				        // get local time in seconds
void swap(int word1, int word2);            // swap the place of two words
void SelectionSort(char** list,int n);		// in-place selection sort
void InsertionSort(char** list,int n);		// in-place insertion sort
void BubbleSort(char** list,int n);		    // in-place bubble sort
void ShakerSort(char** list,int n);		    // in-place shaker sort
void freeMemory(void);				        // free all dynamic memories

int main(void)
{
    int i;					                // loop index
    double t;					            // for CPU time tracking

    readInput();				            // read input data

    t = GetTime();				            // initialize time counter

    for (i = 1; i <= R; i++) {
        copyArray(data, A);			        // initialize array for sorting
	
	    // execute sorting based on the algorithm chosen
        switch (sorting_algorithm)
        {
            case 0:
                SelectionSort(A, N);
                break;
            case 1:
                InsertionSort(A, N);
                break;
            case 2:
                BubbleSort(A, N);
                break;
            default:
                ShakerSort(A, N);
        }

        if (i == 1) printArray(A);		    // print sorted results
    }

    // calculate CPU time per iteration
    t = (GetTime() - t) / R;

    printf("  CPU time = %e seconds\n",t);	// print out CPU time

    freeMemory();				            // free dynamic memories

    return 0;
}

void readInput(void)				        // read all inputs
{
    int i;					                // loop index
    char t[50];                             // temporary string input

    scanf("%d", &N);				        // read input size

    // allocate dynamic memories for input 2D array
    data = (char**)malloc(sizeof(char*) * (N + 1));
    A = (char**)malloc(sizeof(char*) * (N + 1));

    for (i = 1; i <= N; i++) {              // read input words
        scanf("%s", t);	
        data[i] = (char*)malloc(sizeof(char) * (strlen(t) + 1));
        strcpy(data[i], t);
    }
}

void printArray(char** A)			        // print the content of array A
{
    int i;					                // loop index

    for (i = 1; i <= N; i++) {
        printf("%d %s\n", i, A[i]);		    // print the index and words 
    }

    // print the type of sorting algorithm
    switch (sorting_algorithm)
    {
    case 0:
        printf("Selection sort:\n");
        break;
    case 1:
        printf("Insertion sort:\n");
        break;
    case 2:
        printf("Bubble sort:\n");
        break;
    default:
        printf("Shaker sort:\n");
    }

    printf("  N = %d\n", N);			    // print the input size
}

void copyArray(char** data, char** A)		// copy data to array A
{
    int i;					                // loop index

    for (i = 1; i <= N; i++) {
        A[i] = data[i];			            // copy string to array A[i]
    }
}

double GetTime(void)				        // get local time in seconds
{
    struct timeval tv;
    gettimeofday(&tv, NULL);

    // return time with microsecond
    return tv.tv_sec + tv.tv_usec * 0.000001;
}

void swap(int word1, int word2)             // swap the place of two words
{
    char* tp;                               // temporary string for swap

    // swapping
    tp = A[word1];
    A[word1] = A[word2];
    A[word2] = tp;
}

void SelectionSort(char** list,int n)		// in-place selection sort
{
    int i, j, k;				            // loop index

    for (i = 1; i <= n; i++) {			    // for all items in array
        j = i;					
        for (k = i + 1; k <= n; k++ ){		// find smallest in A[i+1:n]
            if (strcmp(list[k], list[j]) < 0) {	
                j = k;				        // if found, remember it in j
            }
        }
        swap(i, j);                         // swap list[i] and list[j]
    }
}

void InsertionSort(char** list,int n)		// in-place insertion sort
{
    int i, j;					            // loop index
    char* tp;                               // string for swapping

    for (j = 2; j <= n; j++) {			    // for all items in array
        tp = list[j];			            // save content of list[j] to tp
        i = j - 1;
	    // from list[j-1], find i for list[i] > tp
        while (i >= 1 && strcmp(tp, list[i]) < 0) {
            list[i+1] = list[i];
            i--;
        }
        list[i+1] = tp;			            // place tp to the proper place
    }
}

void BubbleSort(char** list,int n)		    // in-place bubble sort
{
    int i, j;					            // loop index

    for (i = 1; i <= n-1; i++) {			// i runs through the array
        for (j = n; j >= i+1; j--){		    // j runs from n to i+1
	        // if list[j] < list[j-1], swap them
            if (strcmp(list[j], list[j-1]) < 0) {
                swap(j, j-1);
            }
        }
    }
}

void ShakerSort(char** list,int n)		    // in-place shaker sort
{
    int j;					                // loop index
    int l = 1;					            // loop index
    int r = n;				                // loop index

    while (l <= r) {
        for (j = r; j >= l + 1; j--) {		// word exchange from r to l
            if (strcmp(list[j], list[j-1]) < 0) { 
		    // swap list[j] and list[j-1]
                swap(j, j-1);
            }
        }
        l++;

        for(j = l; j <= r-1; j++){		// word exchange from l to r
            if(strcmp(list[j], list[j+1]) > 0){
		    // swap list[j] and list[j+1]
                swap(j, j+1);
            }
        }
        r--;
    }
}

void freeMemory(void)				        // free all dynamic memories
{  
    int i;					                // loop index

    for (i = 1; i <= N; i++) {			    // free memories of array data
        free(data[i]);
    }
    free(data);
    free(A);
}
