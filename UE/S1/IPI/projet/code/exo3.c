#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    int n;
	char matrix[1000][1002] = { "" };
	int i;
	int timer;
	
    scanf("%d",&n);
	fgetc(stdin);
    
    for(i = 0; i < n; i++) {
        fgets(matrix[i], 1002, stdin);
    }
	
    scanf("%d", &timer);
    fgetc(stdin);

    return EXIT_SUCCESS;
}