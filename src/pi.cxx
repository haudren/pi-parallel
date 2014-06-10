#include <mpi.h>
#include <stdio.h>
#include <iostream>
#include <algorithm>
#include <vector>
#include "pi.hxx"

int main(int argc, char* argv[])
{
	int masterPID = 0;
	int tag = 314;
	int myid, nrprocess;
	int steps = 100;
	MPI_Status status;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);
	MPI_Comm_size(MPI_COMM_WORLD, &nrprocess);
	std::cout.precision(15);

	//Is process pid done ?
	std::vector<bool> done;
	done.resize(nrprocess);
	std::fill(done.begin(), done.end(), false);
	done[0] = true;
	//Which terms have been computed
	std::vector<bool> computed;
	computed.resize(steps);
	std::fill(computed.begin(), computed.end(), false);
	//Which terms are now being computed
	int computing[nrprocess];

	int curindex = 0;

	if(myid == masterPID)
	{
		double partial, result;
		result = 0;
		for(int pid = 1; pid < nrprocess; pid++)
		{
			MPI_Send(&curindex, 1, MPI_INT, pid, tag,
					MPI_COMM_WORLD);
			computing[pid] = curindex;
			curindex++;
		}

		while(!all(computed))
		{
			MPI_Recv(&partial, 1, MPI_DOUBLE, MPI_ANY_SOURCE, tag,
					MPI_COMM_WORLD, &status);
			result = result + partial;
			computed[computing[status.MPI_SOURCE]] = true;
			if(curindex < steps)
			{
				MPI_Send(&curindex, 1, MPI_INT, status.MPI_SOURCE, tag,
					MPI_COMM_WORLD);
				computing[status.MPI_SOURCE] = curindex;
				curindex++;
			}
		}

		for(int pid = 1; pid < nrprocess; pid++)
		{
			curindex = -1;
			MPI_Send(&curindex, 1, MPI_INT, pid, tag,
					MPI_COMM_WORLD);
		}

		std::cout << "End result : " << result << std::fixed << std::endl;
	}
	else
	{
		while(1)
		{
			MPI_Recv(&curindex, 1, MPI_INT, masterPID, tag,
					MPI_COMM_WORLD, &status);
			if(curindex == -1)
			{
				break;
			}
			double t = term(curindex);
			std::cout << "Term: "<< curindex << " is " << std::scientific << t << " by pid "<< myid << std::endl;
			MPI_Send(&t, 1, MPI_DOUBLE, masterPID, tag, MPI_COMM_WORLD);
		}
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

bool all(std::vector<bool>& v)
{
	for(std::vector<bool>::iterator it=v.begin(); it!=v.end(); ++it)
	{
		if(!*it)
		{
			return(false);
		}
	}
	return(true);
}
