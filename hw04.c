/* EE3980 HW04 Trading Stock
 * 105061212, Chia-Chun Wang
 * 2019/03/30
 * Revised at 2019/07/27
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

typedef struct sSTKprice						// stock data structure
{
	int year, month, day;
	double price, change;
} STKprice;

typedef struct sResult							// max contiguous sum structure
{
	int buy_date;
	int sell_date;
	double earning;
} Result;

void readInput(void);							// read data input
double GetTime(void);							// get current CPU time
Result MaxSubArrayBF(void);						// brute-force approach
Result MaxSubArray(int begin, int end);			// divide-and-conquer
// find maxsubArray with cross boundary
Result MaxSubArrayXB(int begin, int mid, int end);
// print out results
void printResult(double t_BF, double t_DandC, Result r_BF, Result r_DandC);

int N;											// number of data input
int Nrepeat = 1000;								// number of repetitions
STKprice* data;									// Array to store input data

int main(void)
{
	int i;										// loop index
	double t0, t1, t2;							// CPU time
	double t_BF, t_DandC;						// average CPU time
	Result r_BF, r_DandC;						// max contigous sum results

	readInput();								// read data input

	t0 = GetTime();								// get current CPU time

	r_BF = MaxSubArrayBF();						// find result by brute-force

	t1 = GetTime();								// get current CPU time

	for (i = 1; i <= Nrepeat; i++) {			// repeat times
		// find result by divide-and-conquer
        r_DandC = MaxSubArray(1, N);
    }

	t2 = GetTime();								// get current CPU time

	// calculate average CPU time
	t_BF = t1 - t0;
	t_DandC = (t2 - t1) / Nrepeat;

	printResult(t_BF, t_DandC, r_BF, r_DandC);	// print out results

	free(data);									// free dynamic memories

    return 0;
}

void readInput(void)							// read data input
{
	int i;										// loop index

	scanf("%d", &N);							// number of data

	// allocate dynamic memories for data input
	data = (STKprice*)malloc(sizeof(STKprice) * (N+1));

	// read the first data
	scanf("%d", &data[1].year);
	scanf("%d", &data[1].month);
	scanf("%d", &data[1].day);
	scanf("%lf", &data[1].price);
	data[1].change = 0;							// change of the first data = 0

	for (i = 2; i <= N; i++) {					// read the rest data
		scanf("%d", &data[i].year);
		scanf("%d", &data[i].month);
		scanf("%d", &data[i].day);
		scanf("%lf", &data[i].price);
		// calcute the price changes
		data[i].change = data[i].price - data[i-1].price;
	}
}

double GetTime(void)							// get local time in seconds
{
    struct timeval tv;							// time interval structure

    gettimeofday(&tv, NULL);					// write local time into tv

    return tv.tv_sec + tv.tv_usec * 0.000001;	// return time with microsecond
}

Result MaxSubArrayBF(void)						// brute-force approach
{
	int i, j, k;								// loop index
	double sum;									// temporary sum
	Result r;									// result

	r.earning = 0;								// initialize r

	for (j = 1; j <= N; j++) {					// try begin from 1 to N
		for (k = j; k <= N; k++) {				// try end from begin to N
			sum = 0;
			for (i = j; i <= k; i++) {			// summation from begin to end
				sum += data[i].change;
			}
			if (sum > r.earning) {				// record max value and range
				r.earning = sum;
				r.buy_date = j;
				r.sell_date = k;
			}
		}
	}

	return r;
}

Result MaxSubArray(int begin, int end)			// divide-and-conquer
{
	int mid;									// middle point
	Result r, lsum, rsum, xsum;					// result

	if (begin == end) {							// terminal condition
		r.buy_date = begin;
		r.sell_date = end;
		r.earning = data[begin].change;
		return r;
	}

	mid = (begin + end) / 2;
	lsum = MaxSubArray(begin, mid);				// left region
	rsum = MaxSubArray(mid+1, end);				// right region
	xsum = MaxSubArrayXB(begin, mid, end);		// cross boundary region

	if (lsum.earning >= rsum.earning && lsum.earning >= xsum.earning) {
		return lsum;							// lsum is the largest
	}
	if (rsum.earning >= lsum.earning && rsum.earning >= xsum.earning) {
		return rsum;							// rsum is the largest
	}
	return xsum;								// xsum is the largest
}

// find max subarray with cross boundary
Result MaxSubArrayXB(int begin, int mid, int end)
{
	int i;										// loop index
	double sum, lsum, rsum;						// summation
	Result r;									// result

	lsum = 0; sum = 0; r.buy_date = mid;		// initialize
	for (i = mid; i >= begin; i--) {			// go through the left region
		sum += data[i].change;					// continue to add
		if (sum >= lsum) {						// record if larger
			lsum = sum;
			r.buy_date = i;
		}
	}

	rsum = 0; sum = 0; r.sell_date = mid + 1;	// initialize
	for (i = mid + 1; i <= end; i++) {			// go the the right region
		sum += data[i].change;					// continue to add
		if (sum >= rsum) {						// record if larger
			rsum = sum;
			r.sell_date = i;
		}
	}

	r.earning = lsum + rsum;					// overall sum
	return r;
}

// print out the results
void printResult(double t_BF, double t_DandC, Result r_BF, Result r_DandC)
{
	// the buy/sell date data
	STKprice BF_buy, BF_sell, DandC_buy, DandC_sell;

	// find data by the results we got
	BF_buy = data[r_BF.buy_date];
	BF_sell = data[r_BF.sell_date];
	DandC_buy = data[r_DandC.buy_date];
	DandC_sell = data[r_DandC.sell_date];

	// print out all the results
	printf("N = %d\n", N);
	printf("Brute-force approach: time %e s\n", t_BF);
	printf("Buy: %d/%d/%d at %lf\n", BF_buy.year, BF_buy.month, 
			BF_buy.day, BF_buy.price);
	printf("Sell: %d/%d/%d at %lf\n", BF_sell.year, BF_sell.month, 
			BF_sell.day, BF_sell.price);
	printf("Earning: %lf per share.\n", r_BF.earning);
	printf("Divide and Conquer: time %e s\n", t_DandC);
	printf("Buy: %d/%d/%d at %lf\n", DandC_buy.year, DandC_buy.month, 
			DandC_buy.day, DandC_buy.price);
	printf("Sell: %d/%d/%d at %lf\n", DandC_sell.year, DandC_sell.month, 
			DandC_sell.day, DandC_sell.price);
	printf("Earning: %lf per share.\n", r_DandC.earning);
}
