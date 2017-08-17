#ifndef CALC_H_ //include guard
#define CALC_H_
void arg_check(int argc, char *argv[]);
void process_operation(char*, char*);
char* process_first_operand(char *, bool*, char*, int*);
char* process_second_operand(char*, bool*, char*, int*);
void process_output_base(char*, char *);
int* convert_decimal_to_binary(int);
int* convert_octal_to_binary(char*);
int* convert_hexadecimal_to_binary(char*);
int parseInt(char*);
int* convert_to_twos_complement(int*);
int* flip_bits(int*);
int* add_binary(int*, int*);
int* subtract_binary(int*, int*);
int convert_binary_to_decimal(int*);
char* convert_binary_to_octal(int*);
char* convert_binary_to_hexadecimal(int*);
int* convert_bascii_to_binary(char*);
int pw(int, int);
#endif //CALC_H_
