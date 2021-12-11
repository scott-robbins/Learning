#include <stdbool.h>
#include <stdio.h>

bool isIntPrime(int n){
	bool is_prime = false;
	// check if n is too big?
	// check small n
	if (n < 2)
		return false;
	else if (n==2)
		return true;
	// make sure it is not evenly divisible by any number
	for (int i=2; i < n; i++)
		if((n % i) == 0) return is_prime;
	is_prime = true;
	return is_prime;
}

int maxPrime(int N){
	// return largest prime <= N
	int largest_prime = 2;
	int result = largest_prime;
	if (isIntPrime(N))
		return N;
	while (largest_prime <= N){
		if (isIntPrime(largest_prime))
			result = largest_prime;
		largest_prime = largest_prime + 1;
	}
	return result;
}


void showPrimes(int N){
	// show all primes < N
	int pcounter = 0;
	while( pcounter < N){
		if(isIntPrime(pcounter)){
			printf("%d\n", pcounter);
		}
		pcounter++;
	}
}


void main(int argc, char *argv[]){

	if ((3 > argc) && (argc > 1)){
		int num = atoi(argv[1]);
		int largePrime = maxPrime(num);
		if (largePrime > num){
			printf("%d is not prime, but %d is\n", num, largePrime);
		}else{
			printf("%d is prime \n", largePrime);
		}
	}else if (argc == 3){
		if(strcmp(argv[1], "under")==0){
			showPrimes(atoi(argv[2]));
		}
	}

}