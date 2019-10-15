/* EE3980 HW08 Huffman Code
 * 105061212, Chia-Chun Wang
 * 2019/04/27
 * Revised at 2019/07/28
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node						// binary merge tree node
{
	struct node* lchild;				// pointer to left child
	struct node* rchild;				// pointer to right child
	char c;								// character data
	int freq;							// used frequency
} Node;

int N;									// word size
int* cnt_table;							// table to count frequency
Node* list;								// array for min heap
Node** priority_queue;					// priority queue for min heap
int size;								// number of different character used
int* stack;								// stack to record encoding
int stack_top;							// top of stack
int bits;								// total bits used when encoding

void readInput(void);					// read all input
void constructArray(void);				// construct list array
void Heapify(int root, int n);			// rearrange the nodes of heap
void buildMinHeap(int n);				// use Heapify to build min heap
void BMT(void);							// construct binary merge tree
void printResult(void);					// print out the result
void DFS(Node v);						// depth first search traversal
void freeMemories(void);				// free dynamic memories

int main(void)
{
	readInput();						// read all input
	constructArray();					// construct list array
	//BMT();								// construct binary merge tree
	//printResult();						// print out the result
	//freeMemories();						// free dynamic memories

	int i;
	for (i = 1; i <= size; i++) {
		printf("%c %d\n", priority_queue[i]->c, priority_queue[i]->freq);
	}

    return 0;
}

void readInput(void)					// read all input
{
	int i, j;							// loop index
	int t;								// temporary
	char s[15];							// input string

	scanf("%d", &N);					// read word size

	// allocate dynamic memories
	cnt_table = (int*)malloc(sizeof(int) * 123);

	// initialize
	cnt_table[0] = 0;
	for (i = 97; i <= 122; i++) {
		cnt_table[i] = 0;
	}

	// counting the frequency for each character
	for (i = 1; i <= N; i++) {
		scanf("%s", s);
		for (j = 0; j < strlen(s); j++) {
			t = s[j];
			cnt_table[t]++;
		}
		cnt_table[0]++;
	}
}

void constructArray(void)				// construct list array
{
	int i;								// loop index
	char t;								// temporary

	// allocate dynamic memories
	list = (Node*)malloc(sizeof(Node) * 28);

	size = 1;							// initialize

	// construct array
	for (i = 1; i <= 26; i++) {
		t = i + 96;
		list[size].c = t;
		list[size].freq = cnt_table[i + 96];
		list[size].lchild = NULL;
		list[size].rchild = NULL;
		if (list[size].freq != 0) size++;
	}

	list[size].c = 'A';					// store 'A' for '\n'
	list[size].freq = cnt_table[0];
	list[size].lchild = NULL;
	list[size].rchild = NULL;

	// priority queue for heapify
	priority_queue = (Node**)malloc(sizeof(Node*) * 28);

	for (i = 1; i <= size; i++) {
		priority_queue[i] = &list[i];
	}
}
 
void Heapify(int root, int n)			// rearrange the nodes of heap
{
    Node* t = priority_queue[root];		// assign root value to t
    int j;								// loop index
	int done = 0;						// check if heapify ends

	// j is the leftchild of root and keeps finding its leftchild
    for (j = root * 2; j <= n && done == 0;) {
        if (j < n && priority_queue[j]->freq > priority_queue[j+1]->freq) {
			// j is the rchild if rchild < lchild
            j++;
        }
        if (t.freq < priority_queue[j]->freq) {
            done = 1;					// done if root < children
        }
        else {
			// place child node to parent
            priority_queue[j/2] = priority_queue[j];
			j = j * 2;
        }
    }

    priority_queue[j/2] = t;			// put root to the proper place
}

void buildMinHeap(int n)				// use Heapify to build min heap
{
	int i;								// loop index

	for (i = n/2; i >= 1; i--) {
        Heapify(i, n);					// heapify
    }
}
 
void BMT(void)							// construct binary merge tree
{
	int i;								// loop index
	Node t;								// temporary node

	for (i = size; i >= 2; i--) {		// find least until two nodes remain
		buildMinHeap(i);
		
	}
}

void printResult(void)					// print out the result
{
	int i;								// loop index
	int characters;						// number of characters
	double ratio;						// bits / characters

	// sum up number of characters
	characters = cnt_table[0];
	for (i = 97; i <= 122; i++) {
		characters += cnt_table[i];
	}

	printf("Number of words: %d\n", N);
	printf("Number of characters: %d\n", characters);

	// initialize stack
	stack = (int*)malloc(sizeof(int) * 60);
	stack_top = 0;

	bits = 0;
	DFS(tree[tree_root]);				// use DFS to traversal the tree

	// calculate ratio
	ratio = (100.0 * bits) / (characters * 8.0);

	printf("Number of encoded bits: %d\n", bits);
	printf("Ratio: %lf %% \n", ratio);
}

void DFS(Node v)						// depth first search traversal
{
	int i;								// loop index

	if (v.leaf == 1) {					// if leaf node
		// print out the encoding
		if (v.c == 'A') printf("\\n: ");
		else printf("%c:  ", v.c);
		for (i = 0; i < stack_top; i++) {
			printf("%d", stack[i]);
		}
		printf("\n");
		bits += stack_top * v.freq;		// sum up bits used
	}
	else {								// if not leaf node
		stack[stack_top] = 0;
		stack_top++;
		DFS(*v.lchild);					// go to left child
		stack[stack_top-1] = 1;
		DFS(*v.rchild);					// go to right child
		stack_top--;
	}
}

void freeMemories(void)				    // free dynamic memories
{
	free(cnt_table);
	free(list);
	free(tree);
	free(stack);
}
