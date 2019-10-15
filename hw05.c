/* EE3980 HW05 Trading Stock II
 * 105061212, Chia-Chun Wang
 * 2019/04/05
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
	int buy_date;								// date to buy
	int sell_date;								// date to sell
	double earning;								// price difference
} Result;

void readInput(void);							// read data input
double GetTime(void);							// get current CPU time
Result MaxSubArrayBF(void);						// brute-force approach
Result MaxSubArray(void);						// Kadane's method
// print out results
void printResult(double t_BF, double t_DandC, Result r_BF, Result r_DandC);

int N;											// number of data input
int Nrepeat = 500;								// number of repetitions
STKprice* data;									// Array to store input data

int main(void)
{
	int i;										// loop index
	double t0, t1, t2;							// CPU time
	double t_BF, t_Kadane;						// average CPU time
	Result r_BF, r_Kadane;						// max contigous sum results

	readInput();								// read data input

	t0 = GetTime();								// get current CPU time
	for (i = 1; i <= Nrepeat; i++) {			// repeat times
		r_BF = MaxSubArrayBF();					// find result by brute-force
	}

	t1 = GetTime();								// get current CPU time

	for (i = 1; i <= Nrepeat; i++) {			// repeat Nrepeat times
		r_Kadane = MaxSubArray();				// find result by Kadane
    }

	t2 = GetTime();								// get current CPU time

	// calculate average CPU time
	t_BF = (t1 - t0) / Nrepeat;
	t_Kadane = (t2 - t1) / Nrepeat;

	printResult(t_BF, t_Kadane, r_BF, r_Kadane);// print out results

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
	int j, k;									// loop index
	double sum;									// temporary sum
	Result r;									// result

	r.earning = 0;								// initialize r

	for (j = 1; j <= N; j++) {					// try begin from 1 to N
		for (k = j; k <= N; k++) {				// try end from begin to N
			sum = data[k].price - data[j].price;// sum is the price difference
			if (sum > r.earning) {				// record max value and range
				r.earning = sum;
				r.buy_date = j;
				r.sell_date = k;
			}
		}
	}

	return r;
}

Result MaxSubArray(void)						// Kadane's method
{
	int i;										// loop index
	int start = 2, end = 1;						// two ends of max subarray
	int start_tmp = 2;							// temporary start point
	double max = 0;								// max value of subarray so far
	double now = 0;								// max value of subarray now
	Result r;									// result returned

	for (i = 2; i <= N; i++) {					// go through the array
		now = now + data[i].change;				// subarray value until i
		if (now < 0) {							// if value < 0, reset
			now = 0;
			start_tmp = i + 1;
		}
		if (now > max) {						// record if value is largest
			max = now;
			start = start_tmp;
			end = i;
		}
	}
	
	// return the result
	r.buy_date = start - 1;
	r.sell_date = end;
	r.earning = max;
	
	return r;
}

// print out the results
void printResult(double t_BF, double t_Kadane, Result r_BF, Result r_Kadane)
{
	// the buy/sell date data
	STKprice BF_buy, BF_sell, Kadane_buy, Kadane_sell;

	// find data by the results we got
	BF_buy = data[r_BF.buy_date];
	BF_sell = data[r_BF.sell_date];
	Kadane_buy = data[r_Kadane.buy_date];
	Kadane_sell = data[r_Kadane.sell_date];

	// print out all the results
	printf("N = %d\n", N);
	printf("Brute-force approach: time %e s\n", t_BF);
	printf("Buy: %d/%d/%d at %lf\n", BF_buy.year, BF_buy.month,
			BF_buy.day, BF_buy.price);
	printf("Sell: %d/%d/%d at %lf\n", BF_sell.year, BF_sell.month,
			BF_sell.day, BF_sell.price);
	printf("Earning: %lf per share.\n", r_BF.earning);
	printf("Divide and Conquer: time %e s\n", t_Kadane);
	printf("Buy: %d/%d/%d at %lf\n", Kadane_buy.year, Kadane_buy.month,
			Kadane_buy.day, Kadane_buy.price);
	printf("Sell: %d/%d/%d at %lf\n", Kadane_sell.year, Kadane_sell.month,
			Kadane_sell.day, Kadane_sell.price);
	printf("Earning: %lf per share.\n", r_Kadane.earning);
}
