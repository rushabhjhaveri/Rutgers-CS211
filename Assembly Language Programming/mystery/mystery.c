#include<stdio.h>
#include<stdlib.h>
#include "mystery.h"

unsigned int DEBUG = 1;

int arr[200];

int add(int num1, int num2){
  return (num1 + num2);
}

int compute_fibonacci(int number){
  int temp;
  temp = 0;

  if(arr[number] == -1){

    if(number == 0){
      arr[number] = 0;
    }
    else if(number == 1){
      arr[number] = 1;
    }
    else{
      arr[number] = add(compute_fibonacci(number-2), compute_fibonacci(number-1));
    }

  }
  temp = arr[number];
  return temp;

}

int main(int argc, char *argv[]) {
  char *arg;
  int nth_fibonacci;
  int i, result;



  arg = argv[1];
  //parse the argument to an integer
  if(isdigit((char)*arg)){
    power = atoi(arg);
  }
  else{
    fprintf(stderr, "ERROR; Invalid Argument - Argument Not a Number!\n");
    exit(0);
  }

  for(i = 0; i < 200; i++){
    arr[i] = -1;
  }

  result = compute_fibonacci(nth_fibonacci);

  printf("Value: %d\n", result);
  return 0;
}
