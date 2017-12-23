# include "customlibs/exo3.h"

int main(void){
  int n;
  scanf("%d",&n);fgetc(stdin);
  
  char matrix[1000][1002] = { "" };
  int i;
  for(i = 0; i < n; i++){
    fgets(matrix[i], 1002, stdin);
  }

  int timer;
  scanf("%d", &timer);fgetc(stdin);

  return 0;
}
