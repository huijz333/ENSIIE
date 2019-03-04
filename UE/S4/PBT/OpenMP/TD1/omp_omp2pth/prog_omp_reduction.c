#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
	int N = atoi(argv[1]);
	int sum = 0;

#pragma omp parallel shared(sum,N)
{
	#pragma omp master
	{
		printf("Je suis le maitre !\n");
	}

	int i;
	# pragma omp for reduction (+:sum)
	for (i = 0 ; i < N ; i++) {
		sum += i;
	}
}

	printf("sum = %d\n", sum);

	return 0;
}

