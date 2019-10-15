/* EE3980 HW02 Heap Sort
 * 105061212, Chia-Chun Wang
 * 2019/03/13
 * Revised at 2019/07/20
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

int N;						                    // input size
char** data;					                // input data
char** A;					                    // array to be sorted
int R = 500;					                // number of repetitions

int sorting_algorithm = 0;				        // type of sorting algorithm
/* modify the value to choose the sorting algorithm
 * 0: Selection Sort
 * 1: Insertion Sort
 * 2: Bubble Sort
 * 3: Shaker Sort
 * 4 or others: Heap Sort
 */

void readInput(void);				            // read all inputs
void printArray(char** A);			            // print the content of array A
void copyArray(char** data, char** A);		    // copy data to array A
double GetTime(void);				            // get local time in seconds
void swap(int word1, int word2);                // swap the place of two words
void SelectionSort(char** list, int n);		    // in-place selection sort
void InsertionSort(char** list, int n);		    // in-place insertion sort
void BubbleSort(char** list, int n);		    // in-place bubble sort
void ShakerSort(char** list, int n);		    // in-place shaker sort
void HeapSort(char** list, int n);		        // in-place heap sort
void Heapify(char** list, int root, int n);	    // rearrange to form max heap
void freeMemory(void);				            // free all dynamic memories

int main(void)
{
    int i;					                    // loop index
    double t;					                // for CPU time tracking

    readInput();				                // read input data

    t = GetTime();				                // initialize time counter

    for (i = 1; i <= R; i++) {
        copyArray(data, A);			            // initialize array for sorting

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
            case 3:
                ShakerSort(A, N);
                break;
            default:
                HeapSort(A, N);
        }

        if (i == 1) printArray(A);		        // print sorted results
    }

    t = (GetTime() - t) / R;			        // calculate CPU time

    printf("  CPU time = %e seconds\n",t);  	// print out CPU time

    freeMemory();				                // free dynamic memories

    return 0;
}

void readInput(void)				            // read all inputs
{
    int i;					                    // loop index
    char t[50];					                // temporary string for input

    scanf("%d", &N);				            // read input size

    // allocate dynamic memories for the two arrays
    data = (char**)malloc(sizeof(char*) * (N + 1));
    A = (char**)malloc(sizeof(char*) * (N + 1));

    for(i = 1; i <= N; i++){
        scanf("%s", t);				            // read input string to s
        data[i] = (char*)malloc(sizeof(char) * (strlen(t) + 1));
        strcpy(data[i], t);			            // put the string s into data
    }
}

void printArray(char** A)			            // print the content of array A
{
    int i;					                    // loop index

    for (i = 1; i <= N; i++) {
        printf("%d %s\n",i ,A[i]);		        // print the index and words
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
        case 3:
            printf("Shaker sort:\n");
            break;
        default:
            printf("Heap sort:\n");
    }

    printf("  N = %d\n", N);			        // print the input size
}

void copyArray(char** data, char** A)		    // copy data to array A
{
    int i;					                    // loop index

    for (i = 1; i <= N; i++) {
        A[i] = data[i];			                // copy string to array A[i]
    }
}

double GetTime(void)				            // get local time in seconds
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + tv.tv_usec * 0.000001;	// return time with microsecond
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

        for(j = l; j <= r-1; j++){		    // word exchange from l to r
            if(strcmp(list[j], list[j+1]) > 0){
		        // swap list[j] and list[j+1]
                swap(j, j+1);
            }
        }
        r--;
    }
}

void HeapSort(char** list, int n)		    // in-place heap sort
{
    int i;					                // loop index

    // heapify all the subtrees and be a max heap
    for (i = n/2; i >= 1; i--) {
        Heapify(list, i, n);
    }

    for (i = n; i >= 2; i--) {			    // repeat n-1 times
        swap(1, i);                         //swap the first and the last                        
        Heapify(list, 1, i-1);              // make list[1:i-1] be a max heap
    }
}

void Heapify(char** list, int root, int n)	// rearrange to form max heap
{
    int j;					                // loop index
    int done = 0;                           // boolean value of finished
    char* tp = list[root];			        // assign root value to tp
    
    j = root * 2;                           // j is the left child

    while (j <= n && done == 0) {           // keeps finding its left child
        if (j < n && strcmp(list[j], list[j+1]) < 0) {
            j++;				            // find right child
        }
        if (strcmp(tp, list[j]) > 0) {
            done = 1;				        // done if root > children
        }
        else{
            list[j/2] = list[j];		    // place child node to parent
            j *= 2;                         // continue to left child
        }
    }

    list[j/2] = tp;				            // put root to the proper place
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
