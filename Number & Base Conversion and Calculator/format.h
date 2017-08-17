#ifndef FORMAT_H_ //include guard
#define FORMAT_H_

void arg_check(int, char* argv[]);
int convert_binary_to_int(char* , int);
char* flip_bits(char*);
void intToASCII(char*, int);
static void get_float_digit(float, char*, int*, float*);
static void append(char*, char);
void floatToASCII( float x, char*);
void intToDecASCII(int, char*);
#endif //FORMAT_H_
