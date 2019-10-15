/* EE3980 HW10 Coin Set Design II
 * 105061212, Chia-Chun Wang
 * 2019/05/11
 */

#include <stdio.h>
#include <stdlib.h>

void current(int D, int NCoin, int Coins[]);	// function for current dollars
void redesign1(int D, int NCoin, int Coins[]);	// redesign the biggest
void redesign2(int D, int NCoin, int Coins[]);	// redesign the second biggest
void redesign3(int D, int NCoin, int Coins[]);	// redesign the biggest two
int NCoinDP(int D, int NCoin, int Coins[]);		// DP algorithm
int findMin(int D, int NCoin, int Coins[]);		// recursive function

int* table;										// number of coins for DP

int main(void)
{
	int D = 99;									// total dollar amount
	int NCoin = 4;								// number of different coins
	int* Coins = (int*)malloc(sizeof(int) * 4);	// allocate dynamic memories

	current(D, NCoin, Coins);					// function for current dollars
	redesign1(D, NCoin, Coins);					// redesign the biggest
	redesign2(D, NCoin, Coins);					// redesign the second biggest
	redesign3(D, NCoin, Coins);					// redesign the biggest two
	
	free(Coins);								// free dynamic memories

    return 0;
}

void current(int D, int NCoin, int Coins[])		// function for current dollars
{
	float total_coin;							// total_coin needed
	float avg;

	// assign coin values
	Coins[0] = 1;
	Coins[1] = 5;
	Coins[2] = 10;
	Coins[3] = 50;

	total_coin = NCoinDP(D, NCoin, Coins);		// calculate coins needed

	avg = total_coin / D;						// calculate average

	// print out result
	printf("For coin set {1, 5, 10, 50} the average is %.5f\n", avg);
}

void redesign1(int D, int NCoin, int Coins[])	// redesign the biggest
{
	int i;										// loop index
	int coin_value;								// target coin value
	float min_total_coin;						// minimum coins needed
	float total_coin;							// coins needed
	float avg;									// average coins needed

	// assign coin values
	Coins[0] = 1;
	Coins[1] = 5;
	Coins[2] = 10;
	
	// get value when coin3 is the successor of coin2
	Coins[3] = Coins[2] + 1;
	coin_value = Coins[3];
	min_total_coin = NCoinDP(D, NCoin, Coins);
	
	// for any possibility of coin3
	for (i = Coins[2] + 2; i <= D; i++) {
		// calculate coins needed
		Coins[3] = i;
		total_coin = NCoinDP(D, NCoin, Coins);
		if (total_coin < min_total_coin) {		// find minimum
			coin_value = i;
			min_total_coin = total_coin;
		}
	}

	avg = min_total_coin / D;					// calculate average value

	// print out result
	printf("Coin set {1, 5, 10, %d} has the minimum average of %.5f\n", 
			coin_value, avg);
}

void redesign2(int D, int NCoin, int Coins[])	// redesign the second biggest
{
	int i;										// loop index
	int coin_value;								// target coin value
	float min_total_coin;						// minimum coins needed
	float total_coin;							// coins needed
	float avg;									// average coins needed

	// assign coin values
	Coins[0] = 1;
	Coins[1] = 5;
	Coins[3] = 50;
	
	// get value when coin2 is the successor of coin1
	Coins[2] = Coins[1] + 1;
	coin_value = Coins[2];
	min_total_coin = NCoinDP(D, NCoin, Coins);
	
	// for any possibility of coin2
	for (i = Coins[1] + 2; i < Coins[3]; i++) {
		// calculate coins needed
		Coins[2] = i;
		total_coin = NCoinDP(D, NCoin, Coins);
		if (total_coin < min_total_coin) {		// find minimum
			coin_value = i;
			min_total_coin = total_coin;
		}
	}

	avg = min_total_coin / D;					// calculate average

	// print out result
	printf("Coin set {1, 5, %d, 50} has the minimum average of %.5f\n", 
			coin_value, avg);
}

void redesign3(int D, int NCoin, int Coins[])	// redesign the two biggest
{
	int i, j;									// loop index
	int coin_value1, coin_value2;				// target coin values
	float min_total_coin;						// minimum coins needed
	float total_coin;							// coins needed
	float avg;									// average coins needed

	// assign coin values
	Coins[0] = 1;
	Coins[1] = 5;
	
	// get value when coin2 is the successor of coin1
	// and coin3 is the successor of coin2
	Coins[2] = Coins[1] + 1;
	Coins[3] = Coins[2] + 1;
	coin_value1 = Coins[2];
	coin_value2 = Coins[3];
	min_total_coin = NCoinDP(D, NCoin, Coins);
	
	// for any possibility of coin2
	for (i = Coins[1] + 2; i <= D - 1; i++) {
		Coins[2] = i;
		// for any possibility of coin3
		for (j = i + 1; j <= D; j++) {
			// calculate coins needed
			Coins[3] = j;
			total_coin = NCoinDP(D, NCoin, Coins);
			if (total_coin < min_total_coin) {	// find minimum
				coin_value1 = i;
				coin_value2 = j;
				min_total_coin = total_coin;
			}
		}
	}

	avg = min_total_coin / D;					// calculate average

	// print out result
	printf("Coin set {1, 5, %d, %d} has the minimum average of %.5f\n", 
			coin_value1, coin_value2, avg);
}

int NCoinDP(int D, int NCoin, int Coins[])		// DP algorithm
{
	int i;										// loop index
	int res;									// result of each value
	int cnt;									// counter from 1 to D
	
	// initialize
	cnt = 0;
	table = (int*)malloc(sizeof(int) * (D + 1));
	for (i = 1; i <= D; i++) table[i] = 0;
	
	for (i = 1; i <= D; i++) {					// for value from 1 to D
		res = findMin(i, NCoin, Coins);			// minimum coins needed
		table[i] = res;							// record value
		cnt += res;
	}
	
	free(table);								// free dynamic memories
	
	return cnt;
}

int findMin(int D, int NCoin, int Coins[])		// recursive function
{
	int i;										// loop index
	int res, res_tmp;							// calculating result
	
	if (table[D] != 0) return table[D]; 		// if value has been recorded
	if (NCoin == 1) return D;					// terminal condition
	
	res = findMin(D, NCoin - 1, Coins);			// number of target coins = 0
	
	// all possibility of number of target coins
	for (i = 1; i * Coins[NCoin - 1] <= D; i++) {
		// recursive relation
		res_tmp = i + findMin(D - i * Coins[NCoin - 1], NCoin - 1, Coins);
		if (res_tmp < res) res = res_tmp;		// find minimum
	}
	
	return res;
}
