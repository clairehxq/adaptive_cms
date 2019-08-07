#include <iostream>


float err(int a, int b){
  return (float) (a-b) / (float) b;
}

int main(){
  printf("error of 3, 5 is %f", err(3, 5));
  return 1;
}

  
