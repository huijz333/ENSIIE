# include <time.h>

void mysleep(int ns) {
	struct timespec req;
	req.tv_sec = ns / 1000000000; //extract raw seconds
	req.tv_nsec = ns % 1000000000; //extract nano seconds
	struct timespec rem;
	nanosleep(&req, &rem);
}
