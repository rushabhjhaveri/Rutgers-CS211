#ifndef _Y86EMUL_H_
#define _Y86EMUL_H_
typedef struct CPU{
  int registers[8]; //8 registers + 1 end of array delimiter ('\0')
  int program_counter; //Instruction Pointer.
  int cpu_size;
  /*
  The following variables are meant to be boolean types. However, since declaring
  them as bool is going to get them promoted to int (0/1) anyway, we're going to
  go ahead and just declare them as ints, and they'll either hold the value 0 or 1.
  */
//  int cont; //No idea what this guy's for. Lol. The search continues...
  int overflow_flag; //overflow flag
  int zero_flag; //zero flag
  int negative_flag; //sign flag
} cpu;

//Two 4-bit ints to represent the two chars -> byte
typedef union{
  struct{
    unsigned int part_two : 4;
    unsigned int part_one : 4;
  } parts;

  char byte_char;
} byte;

//32-bit ints to chars
typedef union{
  struct{
    char byte1, byte2, byte3, byte4;
  } char_part;

  int int_part;
} int32_to_char;

//Pointer to allocated memory
byte *memory;

char* parse_file(char*);
//int process_y86_program(char*);
long convert_hexa_to_decimal(char*);
char* strdup(char*);
char* substr(char*, int, int);
long pwer(int, int);
int* convert_hexadecimal_to_binary(char*);
long convert_binary_to_decimal(int*);
void addl(cpu *cpu);
void subl(cpu *cpu);
void xorl(cpu *cpu);
void mull(cpu *cpu);
void cmpl(cpu *cpu);
#endif
