#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>
#include<time.h>
#include<sys/time.h>
#include "nCr.h"

unsigned int DEBUG = 0;

int main(int argc, char *argv[]) {
  double time_spent;
  struct timeval tv1, tv2;
  int i, n, r, coefficient;
  char *arg;
  int power;
  gettimeofday(&tv1, NULL);
  if(DEBUG){
    printf("argc: %d\n", argc);
  }

  if(strcmp(argv[1], "-h") == 0){
    printf("Usage: formula <positive integer>\n");
    exit(0);
  }

  if(argc >= 3){
    fprintf(stderr, "%s\n", "ERROR; Too many arguments!");
    exit(0);
  }

  if(DEBUG){
    for(i = 0; i < argc; i++){
      printf("argv[%d]: %s \n", i, argv[i]);
    }
  }

  arg = argv[1];

  for(i = 0; i < strlen(arg); i++){
    if(!isdigit(arg[i])){
      fprintf(stderr, "ERROR; Not an integer!\n");
      exit(0);
    }
  }
  if(DEBUG){
    printf("arg: %s\n", arg);
  }

  //parse the argument to an integer
  if(isdigit((char)*arg)){
    power = atoi(arg);
  }
  else{
    fprintf(stderr, "ERROR; Invalid Argument - Argument Not a Number!\n");
    exit(0);
  }

  if(DEBUG){
    printf("power: %d \n", power);
  }

  if(power >= 13){
    fprintf(stderr, "%s\n", "ERROR; Factorial exceeds range of int!");
    exit(0);
  }
  printf("(1+x)^%d = ", power);

  n = power;

  for(r = 0; r <= n; r++){
    coefficient = nCr(n, r);
    if(coefficient == -1){
      fprintf(stderr, "%s\n", "ERROR; Multiplication overflow!");
    }
    if(r == 0){
      printf("1 + ");
    }
    else if(r == n){
      printf("%d*x^%d ", coefficient, r);
    }
    else{
      printf("%d*x^%d + ", coefficient, r);
    }
  }

  gettimeofday(&tv2, NULL);
  time_spent= (double)(tv2.tv_usec - tv1.tv_usec)/1000000 + (double)(tv2.tv_sec - tv1.tv_sec);

  printf("\nTime Required: %f microseconds\n", time_spent*1000000);
  return 0;
}
