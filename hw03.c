/* EE3980 HW03 Heap Sort
 * 105061212, Chia-Chun Wang
 * 2019/03/23
 * Revised at 2019/07/21
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

void readGraph(void);				        // read and store all input
double GetTime(void);				        // get current CPU time
void Connect1(void);				        // network connectivity
void Connect2(void);				        // network connectivity with
						                    // weighted set union
void Connect3(void);				        // network connectivity with
						                    // collapsing set find
int SetFind(int in);				        // find the set of the element
void SetUnion(int i, int j);			    // unite two sets
void WeightedUnion(int i, int j);		    // unite two sets by weight
int CollapsingFind(int in);			        // find the set of the element
						                    // and collapse the tree
void freeMemory(void);				        // free dynamic memories

int V;						                // number of vertices
int E;						                // number of edges
int* S;						                // array of sets
int** edge;					                // 2D array of edges
int* R;						                // the set table
int NS;						                // number of sets
int Nrepeat = 100;				            // number of repetitions

int main(void)
{
    int i;					                // loop index
    double t0, t1, t2, t3;			        // current CPU time
    double t1_avg, t2_avg, t3_avg;  		// average time per execution
    int Ns1, Ns2, Ns3;				        // number of sets

    readGraph();				            // read and store all input

    t0 = GetTime();				            // get current CPU time

    for (i = 1; i <= Nrepeat; i++) {		// repeat Nrepeat times
        Connect1();				            // do connect1
    }

    t1 = GetTime();				            // get current CPU time
    Ns1 = NS;					            // record the number of sets

    for (i = 1; i <= Nrepeat; i++) {		// repeat Nrepeat times
	    Connect2();				            // do connect2
    }

    t2 = GetTime();				            // get current CPU time
    Ns2 = NS;					            // record the number of sets

    for (i = 1; i <= Nrepeat; i++) {		// repeat Nrepeat times
        Connect3();				            // do connect3
    }

    t3 = GetTime();				            // get current CPU time
    Ns3 = NS;					            // record the number of sets

    // calculate the average CPU time per execution
    t1_avg = (t1-t0) / Nrepeat;
    t2_avg = (t2-t1) / Nrepeat;
    t3_avg = (t3-t2) / Nrepeat;

    // print out the results
    printf("Connect1:\n  Time: %e s\n  Number of Set: %d\n", t1_avg, Ns1);
    printf("Connect2:\n  Time: %e s\n  Number of Set: %d\n", t2_avg, Ns2);
    printf("Connect3:\n  Time: %e s\n  Number of Set: %d\n", t3_avg, Ns3);

    freeMemory();				            // free dynamic memories

    return 0;
}

void readGraph(void)				        // read and store all input
{
    int i;					                // loop index

    scanf("%d", &V);				        // number of vertices
    scanf("%d", &E);				        // number of edges

    // allocate dynamic memories
    S = (int*)malloc(sizeof(int) * (V + 1));
    R = (int*)malloc(sizeof(int) * (V + 1));

    edge = (int**)malloc(sizeof(int*) * (E + 1));
    for (i = 1; i <= E; i++) {
	    edge[i] = (int*)malloc(sizeof(int) * 2);
    }

    // store all edges with a 2D array
    for (i = 1; i <= E; i++) {
	    scanf("%d %d", &edge[i][0], &edge[i][1]);
    }
}

double GetTime(void)				        // get local time in seconds
{
    struct timeval tv;
    gettimeofday(&tv, NULL);

    // return time with microsecond
    return tv.tv_sec + tv.tv_usec * 0.000001;   
}

// network connectivity
void Connect1(void)
{
    int i;					                // loop index
    int Si, Sj;					            // set root

    for (i = 1; i <= V; i++) {			    // initailze the set
	    S[i] = -1;				            // only one element in each set
    }

    NS = V;					                // number of sets = vertices

    for (i = 1; i <= E; i++) {			    // for every edge
	    // find the set root of vertices of each edge
        Si = SetFind(edge[i][0]);
	    Sj = SetFind(edge[i][1]);

        if (Si != Sj) {				        // if not in the same set
            NS--;				            // number of sets - 1
            SetUnion(Si, Sj);			    // unite two sets
        }
    }

    for (i = 1; i <= V; i++) {			    // for each vertex
        R[i] = SetFind(i);			        // record the set
    }
}

// network connectivity with weighted set union
void Connect2(void)
{
    int i;					                // loop index
    int Si, Sj;					            // set root

    for (i = 1; i <= V; i++) {			    // initailze the set
        S[i] = -1;				            // only one element in each set
    }

    NS = V;					                // number of sets = vertices

    for (i = 1; i <= E; i++) {			    // for every edge
        // find the set root of vertices of each edge
        Si = SetFind(edge[i][0]);
        Sj = SetFind(edge[i][1]);

        if (Si != Sj) {				        // if not in the same set
            NS--;				            // number of sets - 1
            WeightedUnion(Si, Sj);		    // unite two sets
        }
    }

    for (i = 1; i <= V; i++) {			    // for each vertex
        R[i] = SetFind(i);			        // record the set
    }
}

// network connectivity with weighted set union and collapsing set find
void Connect3(void)
{
    int i;					                // loop index
    int Si, Sj;					            // set root

    for (i = 1; i <= V; i++) {			    // initailze the set
        S[i] = -1;				            // only one element in each set
    }

    NS = V;					                // number of sets = vertices

    for (i = 1; i <= E; i++) {			    // for every edge
        // find the set root of vertices of each edge
        Si = CollapsingFind(edge[i][0]);
        Sj = CollapsingFind(edge[i][1]);

        if (Si != Sj) {				        // if not in the same set
            NS--;				            // number of sets - 1
            WeightedUnion(Si, Sj);		    // unite two sets
        }
    }

    for (i = 1; i <= V; i++) {			    // for each vertex
        R[i] = SetFind(i);			        // record the set
    }
}

int SetFind(int in)				            // find the set of the element
{
    int i = in;					            // input element

    while (S[i] >= 0) {				        // if the element is not root
        i = S[i];				            // keep finding
    }

    return i;
}

void SetUnion(int i, int j)			        // unite two sets
{
    S[i] = j;					            // assign one set to another
}

void WeightedUnion(int i, int j)		    // unite two sets by weight
{
    int temp;					            // temporary integer

    temp = S[i] + S[j];				        // sum the number of elements

    // if i has fewer elements, assign set i to j
    if (S[i] > S[j]) {				        
        S[i] = j;
        S[j] = temp;
    }
    // else, assign set j to i
    else{
        S[j] = i;
        S[i] = temp;
    }
}

// find the set of an element and collapse the tree
int CollapsingFind(int in)
{
    int i = in;					            // input element
    int r = in;					            // root to be found
    int temp;					            // temporary integer

    while (S[r] > 0) {				        // if not the root
	    r = S[r];				            // keep finding
    }

    while (i != r) {				        // if not the root
        // make the element point to root
        temp = S[i];
        S[i] = r;
        i = temp;
    }

    return r;
}

void freeMemory(void)				        // release dynamic memories
{
    int i;					                // loop index

    // free all dynamic memories allocated
    free(S);
    free(R);

    for (i = 1; i <= E; i++) {
        free(edge[i]);
    }
    free(edge);
}
