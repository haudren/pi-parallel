#include <mpi.h>
#include <stdio.h>
#include <iostream>
#include "pi.hxx"

int main(int argc, char* argv[])
{
	int masterPID = 0;
	int tag = 314;
	int myid, nrprocess;
	MPI_Status status;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);
	MPI_Comm_size(MPI_COMM_WORLD, &nrprocess);
	std::cout.precision(15);

	if(myid == 0)
	{
		double partial, result;
		result = 0;
		for(int pid = 1; pid < nrprocess; pid++)
		{
			MPI_Recv(&partial, 1, MPI_DOUBLE, pid, tag,
					MPI_COMM_WORLD, &status);
			result = result + partial;
		}
		std::cout << "End result : " << result << std::fixed << std::endl;
	}
	else
	{
		int steps = 50;
		double sum = 0;
		for(int i = 0; i < steps; i++)
		{
			sum = sum + term(i);
		}
		std::cout << "Pi: " << std::fixed << sum << std::endl;
		MPI_Send(&sum, 1, MPI_DOUBLE, masterPID, tag, MPI_COMM_WORLD);
	}
	MPI_Finalize();
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
