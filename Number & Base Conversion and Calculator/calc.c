#include<stdio.h>
#include<stdbool.h>
#include<math.h>
#include<string.h>
#include<stdlib.h>
#include "calc.h"

unsigned int DEBUG = 0;  //debug = 1; no debug = 0
unsigned int ARR_SIZE = 64;

int main(int argc, char *argv[]) {
  //Variable declarations

  //variables for the first operand
  char *first_operand;
  bool first_negative;
  char first_base;
  int first_sigdigs;
  //bool first_greater;
  int* first_binary;             // is a pointer to a integer arroy of binary

  //variables for second operand
  char *second_operand;
  bool second_negative;
  char second_base;
  int second_sigdigs;
//  bool second_greater;
  int* second_binary;             // is a pointer to a integer arroy of binary


  //variables for output
//  char *output;
  char output_base;
  char output_operation;
//  bool output_negative;
  int* output_bin;

  //temp variables
  int tmp;
  //char *temp;

  //counter variables
  int i;

  //Variable declarations end here

  //error check
  arg_check(argc, argv);

  //error check ends here
  if(DEBUG){
    printf("%s\n", "Processing arguments.");
  }

  //processing arguments - can we modularize this?
  i = 0;
  while (i < argc) {
    //char *arg = argv[i];
    switch (i) {
      case 0: break;
      //First argument: this is the operation that is supposed to be performed on
      //the two operands.
      //Store in output_operation variable.
      case 1: if(DEBUG){printf("%s\n", "Case 1");}
              process_operation(argv[i], &output_operation);
               if (DEBUG){
                 printf("case 1:output_operation=%c",output_operation);
               }
              break;

      //Second argument: this is the first number.
      case 2:  if(DEBUG){printf("%s\n", "Case 2:");}
              first_operand = process_first_operand(argv[i], &first_negative, &first_base, &first_sigdigs);
              if (DEBUG){
                printf("case 2: first_operand: %s", first_operand);
                printf("first_negative: %d\n", first_negative);
                printf("%s%c\n", "first_base: ", first_base);
                printf("%s%d\n", "first_sigdigs: ", first_sigdigs);
              }
              break;



      //Third argument: this is the second number.
      case 3:
              second_operand = process_second_operand(argv[i], &second_negative, &second_base, &second_sigdigs);
              if (DEBUG){
                printf("case 3: second_operand: %s", second_operand);
                printf("second_negative: %d\n", second_negative);
                printf("%s%c\n", "second_base: ", second_base);
                printf("%s%d\n", "second_sigdigs: ", second_sigdigs);
              }
              break;

      //Fourth argument: this is the output base.
      case 4: process_output_base(&output_base, argv[i]);
              break;
      default: fprintf(stderr, "%s\n", "ERROR; 2-Too many input arguments!");
               return 1;
               break;

    } //end switch
    i++;
  } //end while

  //debug - to test whether arguments have been processed correctly
  if(DEBUG){
    printf("%s%c\n", "Operation to be performed: ", output_operation);
    //can also be written as:
    //printf("Operation to be performed:%c", output_operation);
    printf("%s%s\n", "First number: ", first_operand);
    printf("%s%d\n", "Is Negative: ", first_negative);
    printf("%s%c\n", "Base: ", first_base);
    printf("%s%d\n", "Number of Significant Digits: ", first_sigdigs);
    printf("\n");
    printf("%s%s\n", "Second number: ", second_operand);
    printf("%s%d\n", "Is Negative: ", second_negative);
    printf("%s%c\n", "Base: ", second_base);
    printf("%s%d\n", "Number of Significant Digits: ", second_sigdigs);
    printf("%s%c\n", "Output base: ", output_base);
    printf("\n");
  } //end if; end debug

  if(DEBUG){
    printf("%s\n", "We reached here.");
  }
  //determining the number systems and converting to binary

  tmp = 0;

  switch (first_base) {
    //if binary, do nothing
    case 'b': first_binary = convert_bascii_to_binary(first_operand);
              break;

    //if decimal, convert to binary
    case 'd': //parse the string number to an int number
              tmp = parseInt(first_operand);
              if(DEBUG){
                printf("%s%d\n", "tmp(int): ", tmp);
              }
              first_binary = convert_decimal_to_binary(tmp);
              break;

    //if octal, convert to binary
    case 'o': first_binary = convert_octal_to_binary(first_operand);
              break;

    //if hexadecimal, convert to binary
    case 'x': first_binary = convert_hexadecimal_to_binary(first_operand);
              break;
    default: fprintf(stderr, "%s\n", "ERROR; Base not recognized!");
             break;
  } //end switch
  if (DEBUG){
    printf("%s\n", "In main, first base switch: ");
    for(i = 0; i < ARR_SIZE; i++){
      printf("%d%s", first_binary[i], " ");
    }
    printf("\n");
  }
  if(DEBUG){
    printf("%s%d\n", "tmp 2: ", tmp);
  }

  switch (second_base) {
    //if binary, do nothing
    case 'b': second_binary = convert_bascii_to_binary(second_operand);
              break;

    //if decimal, convert to binary
    case 'd': tmp = parseInt(second_operand);
              if(DEBUG){
                printf("%s%d\n", "tmp 2: ", tmp);
              }
              second_binary = convert_decimal_to_binary(tmp);

              break;

    //if octal, convert to binary
    case 'o': second_binary = convert_octal_to_binary(second_operand);

              break;

    //if hexadecimal, convert to binary
    case 'x': second_binary = convert_hexadecimal_to_binary(second_operand);
              break;
    default: fprintf(stderr, "%s\n", "ERROR; Base not recognized!");
             break;
  } //end switch
  if (DEBUG){
    printf("\n%s\n", "In main, second base switch: ");
    for(i = 0; i < ARR_SIZE; i++){
      printf("%d%s", second_binary[i], " ");
    }
    printf("\n");
  }

  /* At this point, we should have two operands converted into binary.
    Time to do some math.
  */
  if(output_operation == '+'){ //add
    //Check if first and/or second operands are negative.
    //If so, convert to two's complement and add one.
    //Pass resultant array as operand.
    if(first_negative){
      if(DEBUG){
        printf("Entered negative case for first argument.");
      }
      first_binary = convert_to_twos_complement(first_binary);
    }
    if(second_negative){
      second_binary = convert_to_twos_complement(second_binary);
    }
    output_bin = add_binary(first_binary, second_binary);
    if (DEBUG){
      printf("%s\n", "In main, binary add: ");
      for(i = 0; i < ARR_SIZE; i++){
        printf("%d%s", output_bin[i], " ");
      }
      printf("\n");
    }
  }
  else if(output_operation == '-'){
    //Check if first and/or second operands are negative.
    //If so, convert to two's complement and add one.
    //Pass resultant array as operand.
    if(first_negative){
      if(DEBUG){
        printf("Entered negative case for first argument.");
      }
      first_binary = convert_to_twos_complement(first_binary);
    }
    if(second_negative){
      //second_binary = convert_to_twos_complement(second_binary);
      output_bin = add_binary(first_binary, second_binary);
    }
    else{
      output_bin = subtract_binary(first_binary, second_binary);
    }
    if (DEBUG){
      printf("%s\n", "In main, binary subtract: ");
      for(i = 0; i < ARR_SIZE; i++){
        printf("%d%s", output_bin[i], " ");
      }
      printf("\n");
    }
  }

  //At this point, we have the binary array containing the added operands.
  //Now, testing the convert_binary_to_decimal function.
  //This is for if the desired output base is 'd'.
  if(output_base == 'd'){
    int output_d = convert_binary_to_decimal(output_bin);
    printf("%c%d\n", 'd', output_d);
  }

  if(output_base == 'b'){ //Just print the array
     if(DEBUG){
       printf("%s\n", "Output in binary: ");
     }
     printf("%c", 'b');
     if (output_bin[0] == 1) {
        for(i = 0; i < ARR_SIZE; i++){
           printf("%d", output_bin[i]);
        }
     } else {
       i = 0;
       while(output_bin[i++]== 0){};

       for(int bj = (i-1); bj < ARR_SIZE; bj++){
          printf("%d", output_bin[bj]);
       }
     }
     printf("\n");
  }

  if(output_base == 'o'){
    char *output_o = convert_binary_to_octal(output_bin);
    printf("%c", 'o');
    for(i = 0; i < ARR_SIZE; i++){
      printf("%c", output_o[i]);
    }
    printf("\n");
  }

  if(output_base == 'x'){
    char* output_x = convert_binary_to_hexadecimal(output_bin);
    //printf("%s%c%s\n", "Output in hexadecimal: ", 'x', output_x);
    printf("%c", 'x');
    for(i = 0; i < ARR_SIZE; i++){
      printf("%c", output_x[i]);
    }
    printf("\n");
  }
    return 0;
} //end main

int* convert_bascii_to_binary(char *str){
  if(DEBUG){
    printf("%s%s\n", "str: ", str);
  }
  int i, k;
  int len;
  char ch = '0';
  int *bin = malloc(ARR_SIZE*sizeof(int));
  k = ARR_SIZE-1;
  for(i = 0; i < ARR_SIZE; i++){
    bin[i] = 0;
  }
  len = strlen(str);
  for(i = (len-1); i >= 0; i--){
    ch = str[i];
    if(ch == '0'){
      bin[k--] = 0;
    }
    else if(ch == '1'){
      bin[k--] = 1;
    }
  }
  if(DEBUG){
    printf("%s\n", "The array is: ");
    for(i = 0; i < ARR_SIZE; i++){
      printf("%d", bin[i]);
    }
    printf("\n");
  }
  return bin;
}

int* convert_to_twos_complement(int *n){
  int *one = malloc(ARR_SIZE*sizeof(int));
  int i;
  for(i = 0; i < ARR_SIZE; i++){
    one[i] = 0;
  }
  one[ARR_SIZE-1] = 1;
  n = flip_bits(n);
  n = add_binary(n, one);
  if(DEBUG){
    printf("%s\n", "In convert_to_twos_complement: n: ");
   for(i = 0; i < ARR_SIZE; i++){
     printf("%d", n[i]);
    }
    printf("\n");
  }
  return n;
}

int* flip_bits(int *n){
  int i;
  for(i = 0; i < ARR_SIZE; i++){
    if(n[i] == 0){
      n[i] = 1;
    }
    else if(n[i] == 1){
      n[i] = 0;
    }
  }
  return n;
}
int* add_binary(int *first_binary, int *second_binary){
  int i;
  int *output = malloc(ARR_SIZE * sizeof(int));
  bool should_carry = false;
  //This should technically never happen!
  if((sizeof(first_binary)/sizeof(int)) != (sizeof(second_binary)/sizeof(int))){
    fprintf(stderr, "%s\n", "ERROR; Mismatching array lengths!");
  }

  for(i = 0; i < ARR_SIZE; i++){
    output[i] = 0;
  }

  i = ARR_SIZE-1;
  for(; i >= 0; i--){
    if(should_carry){
      if(first_binary[i] == 0 && second_binary[i] == 0){
        output[i] = 1;
        should_carry = false;
      }
      else if((first_binary[i] == 1 && second_binary[i] == 0) ||
              (first_binary[i] == 0 && second_binary[i] == 1)){
        output[i] = 0;
        should_carry = true;
      }
      else if(first_binary[i] == 1 && second_binary[i] == 1){
        output[i] = 1;
        should_carry = true;
      }
    } else{
      if(first_binary[i] == 0 && second_binary[i] == 0){
        output[i] = 0;
        should_carry = false;
      }
      else if((first_binary[i] == 1 && second_binary[i] == 0) ||
              (first_binary[i] == 0 && second_binary[i] == 1)){
        output[i] = 1;
        should_carry = false;
      }
      else if(first_binary[i] == 1 && second_binary[i] == 1){
        output[i] = 0;
        should_carry = true;
      }
    }
  }
  return output;
}

int* subtract_binary(int *first_binary, int *second_binary){
  int i;
  int *output = malloc(ARR_SIZE * sizeof(int));
  bool should_carry = false;
  //This should technically never happen!
  if((sizeof(first_binary)/sizeof(int)) != (sizeof(second_binary)/sizeof(int))){
    fprintf(stderr, "%s\n", "ERROR; Mismatching array lengths!");
  }

  for(i = 0; i < ARR_SIZE; i++){
    output[i] = 0;
  }

  i = ARR_SIZE-1;
  for(i = ARR_SIZE-1; i >= 0; i--){
    /*
    if(DEBUG){
      printf("%s%d\n", "first_binary[i]: ", first_binary[i]);
      printf("%s%d\n", "second_binary[i]: ", second_binary[i]);
    }
    */
    if(should_carry){
      if(first_binary[i] == 0 && second_binary[i] == 0){
        //printf("%s\n", "Executing case: 1A");
        output[i] = 1; should_carry = true;
      }
      else if(first_binary[i] == 1 && second_binary[i] == 0){
        //printf("%s\n", "Executing case: 1B");
        output[i] = 0; should_carry = false;
      }
      else if(first_binary[i] == 0 && second_binary[i] == 1){
        //printf("%s\n", "Executing case: 1C");
        output[i] = 0; should_carry = true;
      }
      else if(first_binary[i] == 1 && second_binary[i] == 1){
        //printf("%s\n", "Executing case: 1D");
        output[i] = 1; should_carry = true;
      }
    }
    else{ //carry is not true
      if(first_binary[i] == 0 && second_binary[i] == 0){
        //printf("%s\n", "Executing case: 2A");
        output[i] = 0; should_carry = false;
      }
      else if(first_binary[i] == 1 && second_binary[i] == 0){
        //printf("%s\n", "Executing case: 2B");
        output[i] = 1; should_carry = false;
      }
      else if(first_binary[i] == 0 && second_binary[i] == 1){
        //printf("%s\n", "Executing case: 2C");
        output[i] = 1; should_carry = true;
      }
      else if(first_binary[i] == 1 && second_binary[i] == 1){
        //printf("%s\n", "Executing case: 2D");
        output[i] = 0; should_carry = false;
      }
    }
    //printf("%s%d\n", "output[i]: ", output[i]);
  }
  return output;
}
int convert_binary_to_decimal(int *n){
  int dec_n = 0, i = 0;
  if(DEBUG){
    printf("%s%d\n", "init dec_n: ", dec_n);
  }
  for(i = (ARR_SIZE-1); i >= 0; i--){
    if(n[i] == 1){
      dec_n += pw(2, (ARR_SIZE-i-1));
    }
  }
  if(DEBUG){
    printf("%s%d\n", "dec_n: ", dec_n);
  }
  return dec_n;
}
/*
int convert_binary_to_octal(int *n){
  int octal_n = 0, dec_n = 0, i = 0;
  dec_n = convert_binary_to_decimal(n);
  i = 1;
  while(dec_n != 0){
    octal_n = octal_n + (dec_n % 8)*i;
    dec_n = dec_n / 8;
    i = i * 10;
  }
  printf("%s%d\n", "octal_n: ", octal_n);
  return octal_n;
}
*/
char* convert_binary_to_octal(int *n){
  int octa_n = 0, i, j, k, ol;
  char ch = '0';
  char *octa_output = malloc(ARR_SIZE*(sizeof(char)));

  for(i = 0; i < ARR_SIZE; i++){
    octa_output[i] = '0';
  }

  i = ARR_SIZE-1;
  j = 1;
  k = ARR_SIZE-1;
  ol = 1;
  for(; i >= 0; i--){
     octa_n += n[i]*j;
     if (DEBUG) {
        printf(" ol = %d n[%d] = %d  octa_n = %d\n",ol, i,n[i],octa_n);
     }
     j = j*2;
    if((ol % 3) == 0){
      if (DEBUG) {
          printf("%s%d\n", "Entering if at index: ", i);
      }
      if(octa_n < 8){
        ch = octa_n + '0';
      } else{
        fprintf(stderr, "%s\n", "ERROR; invalid octal character!");
      } //end else
      if (DEBUG){
         printf("%s%c", "ch: ", ch);
      }
      octa_n = 0;
      j = 1;
      ol = 1;
      octa_output[k--] = ch;
      ch = '0';
    } else {
        ol++;
    }//end if
  } //end for
  if (ol != 3) {
    octa_output[0] = octa_n + '0';
  }

  if (DEBUG) {
     //print octa_output
     for(i = 0; i < (ARR_SIZE); i++){
         printf("%c", octa_output[i]);
    }
      printf("\n\n");
   }

  return octa_output;
}

char* convert_binary_to_hexadecimal(int *n){
  int hexa_n = 0, i, j, k;
  char ch = '0';
  char *hexa_output = malloc(ARR_SIZE*(sizeof(char)));

  for(i = 0; i < ARR_SIZE; i++){
    hexa_output[i] = '0';
  }

  i = ARR_SIZE-1;
  j = 1;
  k = ARR_SIZE-1;
  for(; i >= 0; i--){
    hexa_n += n[i]*j;
    // printf("n[%d] = %d  hexa_n = %d\n",i,n[i],hexa_n);
    j = j*2;
    if((i % 4) == 0){
      // printf("%s%d\n", "Entering if at index: ", i);
      // printf("%s%d\n", "hexa_n: ", hexa_n);
      if(hexa_n <= 9){
        ch = hexa_n + '0';
      } else{
        switch (hexa_n) {
          case 10: ch = 'A'; break;
          case 11: ch = 'B'; break;
          case 12: ch = 'C'; break;
          case 13: ch = 'D'; break;
          case 14: ch = 'E'; break;
          case 15: ch = 'F'; break;
          default: fprintf(stderr, "%s\n", "ERROR; Invalid character!");
        } //end switch
      } //end else
      if (DEBUG){
         printf("%s%c\n", "ch: ", ch);
      }
      hexa_n = 0;
      j = 1;
      hexa_output[k--] = ch;
    } //end if
  } //end for
  if(DEBUG){
    for(i = 0; i < (ARR_SIZE); i++){
      printf("%c", hexa_output[i]);
    }
    printf("\n\n");
  }
  //print hexa_output

  return hexa_output;
}

int pw(int a, int b){
  int i;
  int prod = 1;
  for(i = 0; i < b; i++){
    prod *= a;
  }
  return prod;
}
void process_operation(char *arg, char *output_op){
  if(DEBUG){
    printf("%s\n", "In process_operation");
  }
  if(strlen(arg) >= 2){
    fprintf(stderr, "%s\n", "ERROR; Invalid operator!");
    exit(0);
  }
  *output_op = arg[0];
}

char* process_first_operand(char *arg, bool* first_negative, char* first_base, int* first_sigdigs){
  char *first_operand;
  int j, k;
  if (arg[0] == '-') { //check if number is negative
    *first_negative = true;
  } else{
    *first_negative = false;
  }
  //extract the base system of the first operand
  if(*first_negative){ //if the number is negative, base char is
    //the first character
    *first_base = arg[1];
  } else{ //otherwise, base char is the first character
    *first_base = arg[0];
  } //end if-else

  //extract number of significant digits
  if(*first_negative){
    *first_sigdigs = (int)strlen(arg)-2;
  } else{
    *first_sigdigs = (int)strlen(arg)-1;
  } //end if-else
  k = *first_negative ? 2 : 1;
  first_operand = (char *) malloc((*first_sigdigs)+1);
  first_operand[0] = '\0';
  first_operand[*first_sigdigs] = '\0';
  j = 0;
  char current_digit;
  if (DEBUG){
    printf("in process_first_operand: first_operand: %s\n", first_operand);
    printf("first_negative: %d\n", *first_negative);
    printf("%s%c\n", "first_base: ", *first_base);
    printf("%s%d\n", "first_sigdigs: ", *first_sigdigs);
    printf("%s%d\n", "k: ", k);
  }
  while(k < strlen(arg)){
    current_digit = arg[k];
    if (DEBUG){printf("current_digit=%c \n",current_digit);}
    first_operand[j] = current_digit;
    j++;
    k++;
  } //end while
  first_operand[j] = '\0';
  if (DEBUG){
     printf("%s%s\n", "first_operand: ", first_operand);
  }
  return first_operand;
}

char* process_second_operand(char *arg, bool *second_negative, char *second_base, int *second_sigdigs){
  char *second_operand;
  int j, k;
  if (arg[0] == '-') { //check if number is negative
    *second_negative = true;
  }
  else{
    *second_negative = false;
  }

  //extract the base system of the second operand
  if(*second_negative){ //if the number is negative, base char is
    //the second character
    *second_base = arg[1];
  }
  else{ //otherwise, base char is the first character
    *second_base = arg[0];
  } //end if-else

  //extract number of significant digits
  if(*second_negative){
    *second_sigdigs = (int)strlen(arg)-2;
  }
  else{
    *second_sigdigs = (int)strlen(arg)-1;
  } //end if-else

  k = *second_negative ? 2 : 1;
  second_operand = (char*) malloc(*second_sigdigs+1);
  second_operand[*second_sigdigs] = '\0';
  j = 0;
  if (DEBUG){
    printf("in process_second_operand: second_operand: %s\n", second_operand);
    printf("second_negative: %d\n", *second_negative);
    printf("%s%c\n", "second_base: ", *second_base);
    printf("%s%d\n", "second_sigdigs: ", *second_sigdigs);
    printf("%s%d\n", "k: ", k);
  }
  while(k < strlen(arg)){
    char current_digit = arg[k];
    if (DEBUG){printf("current_digit=%c \n",current_digit);}
    second_operand[j] = current_digit;
    j++;
    k++;
  } //end while

  second_operand[j] = '\0';
  if (DEBUG){
     printf("%s%s\n", "second_operand: ", second_operand);
  }
  return second_operand;
}

void process_output_base(char *output_base, char *arg){
  *output_base = arg[0];
}

void arg_check(int argc, char *argv[]){
  int i;
  if(DEBUG){
    printf("%s%d\n", "Number of input arguments: ", argc);
    for(i = 0; i < argc; i++){
      printf("%d:%s\n", i, argv[i]);
    }
  }


  if(argc < 5){
    fprintf(stderr, "%s", "ERROR; Too few arguments!");
    exit(0);
  }
  else if(argc >= 6){
    fprintf(stderr, "%s", "ERROR; Too many arguments!");
    exit(0);
  }
}

int* convert_decimal_to_binary(int number){
  int temp = number;
  int rem = 0;
  int* bin = malloc(ARR_SIZE * sizeof(int));

  int i;

  // init the binary arrary
  for(i = 0; i < ARR_SIZE; i++){
    bin[i] = 0;
  }

  //debug
  if(DEBUG){
    printf("%s%d\n", "The number (int) is: ", temp);
  } //end if; end debug

  //Now, we start converting to binary.
  i = 1;

  while(temp != 0){
    rem = temp % 2;
    temp = temp / 2;
    bin[ARR_SIZE - i] = rem;
    i++;
  }

  if(DEBUG){
    printf("%s\n", "Binary form is: ");
    for(i = 0; i < ARR_SIZE; i++){
      printf("%d%s", bin[i], " ");
    }
  }
  //printf("\n");
  return bin;
}
int* convert_octal_to_binary(char *number){
  int i, j;
  char ch;
  int a, b, c;
  int len = strlen(number);
  int* bin = malloc(ARR_SIZE * sizeof(int));
  for(i = 0; i < ARR_SIZE; i++){
    bin[i] = 0;
  }
  if(DEBUG){
    printf("%s%d\n", "len: ", len);
  } //end if
  j = ARR_SIZE-1;
  for(i = len-1; i >= 0; i--){
    ch = number[i];
    printf("%s%c\n", "ch: ", ch);
    switch(ch){
      case '0': a = 0; b = 0; c = 0;
                break;
      case '1': //binary form is 001
                a = 0; b = 0; c = 1;
                break;
      case '2': //binary form is 010
                a = 0; b = 1; c = 0;
                break;
      case '3': //binary form is 011
                a = 0; b = 1; c = 1;
                break;
      case '4': //binary form is 100
                a = 1; b = 0; c = 0;
                break;
      case '5': //binary form is 101
                a = 1; b = 0; c = 1;
                break;
      case '6': //binary form is 110
                a = 1; b = 1; c = 0;
                break;
      case '7': //binary form is 111
                a = 1; b = 1; c = 1;
                break;
      default:  fprintf(stderr, "%s\n", "ERROR; Invalid character!");
    }
    bin[j--] = c;
    bin[j--] = b;
    bin[j--] = a;
  }
  return bin;
}

int* convert_hexadecimal_to_binary(char *number){
  char *bin_set[] = {"0000", "0001", "0010", "0011", "0100", "0101", "0110",
  		             "0111", "1000", "1001", "1010", "1011", "1100", "1101",
  		             "1110", "1111"
                  };
  char *part;
  int sub;
  char ch;
  int i,j, k;
  int len = strlen(number);
  int len2;
  int* bin = malloc(ARR_SIZE * sizeof(int));

  for(i = 0; i < ARR_SIZE; i++){
    bin[i] = 0;
  }

  if(DEBUG){
	  printf("%s%d\n", "In convert_hexa: len: ", len);
  }
  k = (ARR_SIZE -1);
  for(i = len-1; i >= 0; i--){
    ch = number[i];
    if((ch - '0') < 9){
      sub = ch - '0';
    }
    else{
      switch(ch){
        case 'A':
        case 'a': sub = 10; break;
        case 'B':
        case 'b': sub = 11; break;
        case 'C':
        case 'c': sub = 12; break;
        case 'D':
        case 'd': sub = 13; break;
        case 'E':
        case 'e': sub = 14; break;
        case 'F':
        case 'f': sub = 15; break;
        default: fprintf(stderr, "%s\n", "ERROR; Invalid character!");
      } //end switch
    } //end if-else
    if(DEBUG){
      printf("%s\n", bin_set[sub]);
    }
    part = bin_set[sub];
    len2 = strlen(part);
    if(DEBUG){
      printf("part = %s len = %d\n", part, len2);
    }

    for(j = (len2-1); j >= 0; j--){
      if(k >= 0){
        bin[k--] = part[j]-'0';
      }
    } //end inner for

  } //end outer for

  return bin;
}

int parseInt(char *number){
  int len = 0;
  int i;
  int num = 0;
  len = strlen(number);
  /*
  if(DEBUG){
    printf("%s%d\n", "The length is: ", len);
  } //end if; end debug
  */
  for(i = 0; i < len; i++){
    /*
    if(DEBUG){
      printf("%s%d%s%c\n", "i: ", i, "number[i]: ", number[i]);
      printf("number[%d] - '0': %d\n", i, number[i]-'0');
    }
    */
    //num += pow(10, (len-i-1))*(number[i]-'0');
    num = num*10 + (number[i]-'0');
    /*
    if(DEBUG){
      printf("%s%d\n", "num: ", num);
    }
    */
  } //end for

  if(DEBUG){
    printf("%s%d\n", "Number: ", num);
  } //end debug
  return num;
}
