# include <stdlib.h>
# include <stdlib.h>

int check(char cond) {
	return(cond == 0);
}

void loop(void) {
	int a = 0;
	
	while (check(a)) {
		usleep(1000);
	}
}

int main(void) {
	loop();
	exit(0);
}
