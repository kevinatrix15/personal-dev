#include <omp.h>
#include <iostream>

int main()
{
	std::cout << "num_procs: " << omp_get_num_procs() << std::endl;
	return 0;
}