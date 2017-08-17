#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>

#include "nCr.h"

//unsigned int DEBUG = 1; //DEBUG = 1; NO DEBUG 0

extern int Factorial(int n){
  if(n == 0 || n == 1){
    return 1;
  }
  else{
    return n * Factorial(n-1);
  }
}

extern int nCr(int n, int r){
  int coefficient;
  coefficient = Factorial(n) / (Factorial(r) * Factorial((n-r)));
  return coefficient;
}
