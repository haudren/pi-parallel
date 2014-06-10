#include <stdio.h>
#include <iostream>
#include "pi.hxx"

int main(int argc, const char* argv[])
{
	const int steps = 50;
	double sum = 0;
	for(int i = 0; i < steps; i++)
	{
		sum = sum + term(i);
	}
	std::cout.precision(15);
	std::cout << "Pi: " << std::fixed << sum << std::endl;
	return(1);
}

double term(int index)
{
	double num = factorial(index);
	double denom = oddfactorial(index);
	return(num/denom);
}

double factorial(int index)
{
	double factorial = 1;
	for(int k = 1; k <= index; k++)
	{
		factorial = k*factorial;
	}
	return(factorial);
}

double oddfactorial(int index)
{
	double result = 1;
	for(int k = 0; k <= index; k++)
	{
		result = (2*k+1)*result;
	}
	return(result);
}
