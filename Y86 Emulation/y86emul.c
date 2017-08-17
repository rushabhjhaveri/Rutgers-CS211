#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#include "y86emul.h"

int DEBUG = 0;
unsigned int ARR_SIZE = 64;

long segmentCheck(long address, cpu *cpu){
	if(address < 0 || address >= cpu->cpu_size){
		fprintf(stderr, "ERROR!\n");
		exit(0);
	}
	return address;
} //end check_point


int getChar(char c){
  if(c >= '0' && c <= '9')
    return c - '0';
  if(c >= 'A' && c <= 'F')
    return 10 + c - 'A';
  if(c >= 'a' && c <= 'f')
    return 10 + c - 'a';
  fprintf(stderr, "ERROR; Invalid hex character!\n");
  exit(0);
}


char * parse_file(char* filename){
  long len = 0;
  char *buffer = 0;
  FILE *fp = fopen(filename, "rb");

  if(fp){
    fseek(fp, 0, SEEK_END);
    len = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    buffer = malloc(len);

    if(buffer){
      fread(buffer, 1, len, fp);
    }
    fclose(fp);
  }
  else{
    fprintf(stderr, "%s\n", "ERROR: File does not exist.");
    exit(0);
  }

	buffer[len] = '\0';
  if(buffer){
    if(DEBUG){
      printf("buffer: %s \n", buffer);
    }
    return buffer;
  }
  else{
    fprintf(stderr, "%s\n", "ERROR: Something wrong with the given file!");
    return "";
  }
} // end parse_file


long pwer(int x, int y){
  int i;
  long result = 1;
  for(i = 1; i <= y; i++){
    result = result * x;
  }
  return result;
} //end pwer

int* convert_hexadecimal_to_binary(char* number){
	int a, b;
  int h, i, j, k;
  b = ARR_SIZE - 1;
  int* H_binary = malloc(ARR_SIZE * sizeof(int));
    //initialize H_binary to 0
  for(a = 0; a < ARR_SIZE; a++)
  {
    H_binary[a] = 0;
  }

  for(a = (strlen(number))-1 ; a >= 0; a--)
  {
    //printf("hexadecimal digits: %c\n", number[a]);
    switch (number[a])
    {
      case '0' : h = 0; i = 0; j = 0; k = 0; //0000
                 break;

      case '1' : h = 0; i = 0; j = 0; k = 1; //0001
                 break;

      case '2' : h = 0; i = 0; j = 1; k = 0; //0010
                 break;

      case '3' : h = 0; i = 0; j = 1; k = 1; //0011
                 break;

      case '4' : h = 0; i = 1; j = 0; k = 0; //0100
                 break;

      case '5' : h = 0; i = 1; j = 0; k = 1; //0101
                 break;

      case '6' : h = 0; i = 1; j = 1; k = 0; //0110
                break;

      case '7' : h = 0; i = 1; j = 1; k = 1; //0111
                 break;

      case '8' : h = 1; i = 0; j = 0; k = 0; //1000
                 break;

      case '9' : h = 1; i = 0; j = 0; k = 1; //1001
                 break;

      case 'A' :
      case 'a' : h = 1; i = 0; j = 1; k = 0; //1010
                 break;

      case 'B' :
      case 'b' : h = 1; i = 0; j = 1; k = 1; //1011
                 break;

      case 'C' :
      case 'c' : h = 1; i = 1; j = 0; k = 0; //1100
                 break;

      case 'D' :
      case 'd' : h = 1; i = 1; j = 0; k = 1; //1101
                 break;

      case 'E' :
      case 'e' : h = 1; i = 1; j = 1; k = 0; //1110
                 break;

      case 'F' :
      case 'f' : h = 1; i = 1; j = 1; k = 1; //1111
                 break;

     default : printf("\n%s\n","Invalid argument");
                break;

    }
      H_binary[b--] = k;
      H_binary[b--] = j;
      H_binary[b--] = i;
      H_binary[b--] = h;
  }
  return H_binary;
} //end convert_hexadecimal_to_binary


long convert_binary_to_decimal(int* number)
{
  long decimal = 0, i;

  for(i = (ARR_SIZE - 1); i >= 0; i--)
  {
    if(number[i] == 1)
    {
      decimal += pwer(2,(ARR_SIZE-i-1));
    }
  }

  return decimal;
} // end convert_binary_to_decimal

long convert_hexa_to_decimal(char* num)
{
  int * bin = 0;
	//int j = 0;
	bin = convert_hexadecimal_to_binary(num);

	long dec = 0;
 	dec = convert_binary_to_decimal(bin);
	return dec;

}//end convert_hexa_to_decimal

//starting switch functions

void increment(int num, cpu* cpu){
	cpu->program_counter += num;
	if(cpu->program_counter >= cpu->cpu_size){
		fprintf(stderr, "%s\n", "ERROR!");
		exit(0);
	}
}//end inc

int is_register(int r){
	if(r > 7){
		fprintf(stderr, "%s\n", "ERROR: Invalid register");
		exit(0);
	}
	return r;
} //end reg

void rrmovl(cpu *cpu){

  if(memory[cpu->program_counter].parts.part_two != 0){
    fprintf(stderr, "ERROR: INS - Second half of first byte is not 0 for rrmovl.\n");
    exit(0);
  }

  increment(1, cpu);
  is_register(memory[cpu->program_counter].parts.part_one);
  is_register(memory[cpu->program_counter].parts.part_two);

  cpu->registers[memory[cpu->program_counter].parts.part_two] = cpu->registers[memory[cpu->program_counter].parts.part_one];
  increment(1, cpu);
}//end rrmovl

void irmovl(cpu *cpu){
	unsigned int r = 0;
	int32_to_char val;

  if(memory[cpu->program_counter].parts.part_two != 0){
    fprintf(stderr, "ERROR: INS - Second half of first byte is not 0 for irmovl.\n");
    exit(0);
  }

  increment(1, cpu);
  if(memory[cpu->program_counter].parts.part_one != 15){
    fprintf(stderr, "ERROR: INS - irmovl Syntax - Register filler is not F.\n");
    exit(0);
  }

  r = is_register(memory[cpu->program_counter].parts.part_two);

  increment(1, cpu);
  val.char_part.byte1 = memory[cpu->program_counter].byte_char;
  increment(1, cpu);
  val.char_part.byte2 = memory[cpu->program_counter].byte_char;
  increment(1, cpu);
  val.char_part.byte3 = memory[cpu->program_counter].byte_char;
  increment(1, cpu);
  val.char_part.byte4 = memory[cpu->program_counter].byte_char;

  cpu->registers[r] = val.int_part;
  increment(1, cpu);
}//end rmmovl

void rmmovl(cpu *cpu){
  if(memory[cpu->program_counter].parts.part_two != 0){
    fprintf(stderr, "ERROR: INS - Second half of first byte is not 0 for rmmovl.\n");
    exit(0);
  }

  increment(1, cpu);

  unsigned int rA = is_register(memory[cpu->program_counter].parts.part_one);
  unsigned int rB = is_register(memory[cpu->program_counter].parts.part_two);
  int32_to_char val;

  increment(1, cpu);
  val.char_part.byte1 = memory[cpu->program_counter].byte_char;
  increment(1, cpu);
  val.char_part.byte2 = memory[cpu->program_counter].byte_char;
  increment(1, cpu);
  val.char_part.byte3 = memory[cpu->program_counter].byte_char;
  increment(1, cpu);
  val.char_part.byte4 = memory[cpu->program_counter].byte_char;

  val.int_part = val.int_part + cpu->registers[rB];

  if(val.int_part < 0 || val.int_part + 3 >= cpu->cpu_size){
    fprintf(stderr, "ERROR: ADR - rmmovl Segmentation Fault.\n");
    exit(0);
  }

  int32_to_char out;
  out.int_part = cpu->registers[rA];

  memory[val.int_part].byte_char = out.char_part.byte1;
  memory[val.int_part + 1].byte_char = out.char_part.byte2;
  memory[val.int_part + 2].byte_char = out.char_part.byte3;
  memory[val.int_part + 3].byte_char = out.char_part.byte4;

  increment(1, cpu);
}

//mrmovl - 50 - 80 - moves memory to reg
void mrmovl(cpu *cpu){
  if(memory[cpu->program_counter].parts.part_two != 0){
    fprintf(stderr, "ERROR: INS - Second half of first byte is not 0 for mrmovl.\n");
    exit(0);
  }

  increment(1, cpu);

  unsigned int rA = is_register(memory[cpu->program_counter].parts.part_one);
  unsigned int rB = is_register(memory[cpu->program_counter].parts.part_two);
  int32_to_char val;

  increment(1, cpu);
  val.char_part.byte1 = memory[cpu->program_counter].byte_char;
  increment(1, cpu);
  val.char_part.byte2 = memory[cpu->program_counter].byte_char;
  increment(1, cpu);
  val.char_part.byte3 = memory[cpu->program_counter].byte_char;
  increment(1, cpu);
  val.char_part.byte4 = memory[cpu->program_counter].byte_char;

  val.int_part = val.int_part + cpu->registers[rB];

  if(val.int_part < 0 || val.int_part + 3 >= cpu->cpu_size){
    fprintf(stderr, "ERROR: ADR - mrmovl Segmentation Fault.\n");
    exit(0);
  }

  int32_to_char out;

  out.char_part.byte1 = memory[val.int_part].byte_char;
  out.char_part.byte2 = memory[val.int_part + 1].byte_char;
  out.char_part.byte3 = memory[val.int_part + 2].byte_char;
  out.char_part.byte4 = memory[val.int_part + 3].byte_char;

  cpu->registers[rA] = out.int_part;

  increment(1, cpu);
}

/*opl Helpers*/
void addl(cpu *cpu){ //0
  int rA = cpu->registers[memory[cpu->program_counter].parts.part_one];
  int rB = cpu->registers[memory[cpu->program_counter].parts.part_two];

  if(rA > 0 && rB > 0 && rA + rB < 0)
    cpu->overflow_flag = 1;
  else if(rA < 0 && rB < 0 && rA + rB > 0)
    cpu->overflow_flag = 1;
  else
    cpu->overflow_flag = 0;

  cpu->registers[memory[cpu->program_counter].parts.part_two] = rB + rA;

  if(cpu->registers[memory[cpu->program_counter].parts.part_two] == 0)
    cpu->zero_flag = 1;
  else
    cpu->zero_flag = 0;

  if(cpu->registers[memory[cpu->program_counter].parts.part_two] < 0)
    cpu->negative_flag = 1;
  else
    cpu->negative_flag = 0;
}

void subl(cpu *cpu){ //1
  int rA = cpu->registers[memory[cpu->program_counter].parts.part_one];
  int rB = cpu->registers[memory[cpu->program_counter].parts.part_two];

  if(rA < 0 && rB > 0 && rA - rB > 0)
    cpu->overflow_flag = 1;
  else if(rA > 0 && rB < 0 && rA - rB < 0)
    cpu->overflow_flag = 1;
  else
    cpu->overflow_flag = 0;

  cpu->registers[memory[cpu->program_counter].parts.part_two] = rB - rA;

  if(cpu->registers[memory[cpu->program_counter].parts.part_two] == 0)
    cpu->zero_flag = 1;
  else
    cpu->zero_flag = 0;

  if(cpu->registers[memory[cpu->program_counter].parts.part_two] < 0)
    cpu->negative_flag = 1;
  else
    cpu->negative_flag = 0;
}

void andl(cpu *cpu){ //2
  int newVal = cpu->registers[memory[cpu->program_counter].parts.part_one] & cpu->registers[memory[cpu->program_counter].parts.part_two];

  cpu->overflow_flag = 0;

  if(newVal == 0)
    cpu->zero_flag = 1;
  else
    cpu->zero_flag = 0;

  if(newVal < 0)
    cpu->negative_flag = 1;
  else
    cpu->negative_flag = 0;
}

void xorl(cpu *cpu){ //3
  int newVal = cpu->registers[memory[cpu->program_counter].parts.part_one] ^ cpu->registers[memory[cpu->program_counter].parts.part_two];

  cpu->overflow_flag = 0;

  if(newVal == 0)
    cpu->zero_flag = 1;
  else
    cpu->zero_flag = 0;

  if(newVal < 0)
    cpu->negative_flag = 1;
  else
    cpu->negative_flag = 0;
}

void mull(cpu *cpu){ //4
  int rA = cpu->registers[memory[cpu->program_counter].parts.part_one];
  int rB = cpu->registers[memory[cpu->program_counter].parts.part_two];

  if(rA > 0 && rB > 0 && rA * rB < 0)
    cpu->overflow_flag = 1;
  else if(rA < 0 && rB < 0 && rA * rB < 0)
    cpu->overflow_flag = 1;
  else if(rA > 0 && rB < 0 && rA * rB > 0)
    cpu->overflow_flag = 1;
  else if(rA < 0 && rB > 0 && rA * rB > 0)
    cpu->overflow_flag = 1;
  else
    cpu->overflow_flag = 0;

  cpu->registers[memory[cpu->program_counter].parts.part_two] = rB * rA;

  if(cpu->registers[memory[cpu->program_counter].parts.part_two] == 0)
    cpu->zero_flag = 1;
  else
    cpu->zero_flag = 0;

  if(cpu->registers[memory[cpu->program_counter].parts.part_two] < 0)
    cpu->negative_flag = 1;
  else
    cpu->negative_flag = 0;
}

void cmpl(cpu *cpu){ //5
  int rA = cpu->registers[memory[cpu->program_counter].parts.part_one];
  int rB = cpu->registers[memory[cpu->program_counter].parts.part_two];

  cpu->overflow_flag = 0;

  int res =  rB - rA;

  if(res == 0)
    cpu->zero_flag = 1;
  else
    cpu->zero_flag = 0;

  if(res < 0)
    cpu->negative_flag = 1;
  else
    cpu->negative_flag = 0;
}

//opl - 60_65 - 96_101 - performs op and stores in rA
void opl(cpu *cpu){
  int op = memory[cpu->program_counter].parts.part_two;
  increment(1, cpu);

  switch(op){
    case 0:
      addl(cpu);
      break;

    case 1:
      subl(cpu);
      break;

    case 2:
      andl(cpu);
      break;

    case 3:
      xorl(cpu);
      break;

    case 4:
      mull(cpu);
      break;

    case 5:
      cmpl(cpu);
      break;

    default:
      fprintf(stderr, "ERROR: INS - Improper operation type for opl.\n");
      exit(0);
  }

  increment(1, cpu);
}

/*jcon helper*/

void jmp(cpu *cpu) {
  int32_to_char dest;

  dest.char_part.byte1 = memory[cpu->program_counter].byte_char;
  increment(1, cpu);
  dest.char_part.byte2 = memory[cpu->program_counter].byte_char;
  increment(1, cpu);
  dest.char_part.byte3 = memory[cpu->program_counter].byte_char;
  increment(1, cpu);
  dest.char_part.byte4 = memory[cpu->program_counter].byte_char;

  if(dest.int_part >= cpu->cpu_size || dest.int_part < 0){
    fprintf(stderr, "ERROR: ADR - jmp Segmentation Fault.\n");
    exit(0);
  }

  cpu->program_counter = dest.int_part;
}

//jcon - 70_76 - 112_118 - moves cpu->program_counter to new location
void jcon(cpu *cpu){
  int op = memory[cpu->program_counter].parts.part_two;
  increment(1, cpu);

  switch(op){
    case 0: //jmp
      jmp(cpu);
      break;

    case 1: //jle
      if((cpu->negative_flag ^ cpu->overflow_flag) == 1 || cpu->zero_flag == 1)
        jmp(cpu);
      else
        increment(4, cpu);
      break;

    case 2: //jl
      if((cpu->negative_flag ^ cpu->overflow_flag) == 1)
        jmp(cpu);
      else
        increment(4, cpu);
      break;

    case 3: //je
      if(cpu->zero_flag == 1)
        jmp(cpu);
      else
        increment(4, cpu);
      break;

    case 4: //jne
      if(cpu->zero_flag == 0)
        jmp(cpu);
      else
        increment(4, cpu);
      break;

    case 5: //jge
      if((cpu->negative_flag ^ cpu->overflow_flag) == 0)
        jmp(cpu);
      else
        increment(4, cpu);
      break;

    case 6: //jg
      if((cpu->negative_flag ^ cpu->overflow_flag) == 0 && cpu->zero_flag == 0)
        jmp(cpu);
      else
        increment(4, cpu);
      break;

    default:
      fprintf(stderr, "ERROR: INS - Improper operation type for jump.\n");
      exit(0);
  }

}

void call(cpu *cpu){ //80 - 128 - calls a function
  if(memory[cpu->program_counter].parts.part_two != 0){
    fprintf(stderr, "ERROR: INS - Second half of first byte is not 0 for call.\n");
    exit(0);
  }

  increment(1, cpu);

  //Store dest
  int32_to_char dest;

  dest.char_part.byte1 = memory[cpu->program_counter].byte_char;
  increment(1, cpu);
  dest.char_part.byte2 = memory[cpu->program_counter].byte_char;
  increment(1, cpu);
  dest.char_part.byte3 = memory[cpu->program_counter].byte_char;
  increment(1, cpu);
  dest.char_part.byte4 = memory[cpu->program_counter].byte_char;
  increment(1, cpu);

  //Push cpu->program_counter
  int32_to_char retAdd;
  retAdd.int_part = cpu->program_counter;

  if(cpu->registers[4] - 4 <= 0) {
    fprintf(stderr, "ERROR: ADR - Segmentation Fault for esp.\n");
    exit(0);
  }

  memory[cpu->registers[4] - 4].byte_char = retAdd.char_part.byte1;
  memory[cpu->registers[4] - 3].byte_char = retAdd.char_part.byte2;
  memory[cpu->registers[4] - 2].byte_char = retAdd.char_part.byte3;
  memory[cpu->registers[4] - 1].byte_char = retAdd.char_part.byte4;

  cpu->registers[4] -= 4;

  //printf("dest: %d\t", dest.int_part);

  if(dest.int_part < 0 || dest.int_part >= cpu->cpu_size){
    fprintf(stderr, "ERROR: ADR - Segmentation Fault for dest in call.\n");
    exit(0);
  }

  cpu->program_counter = dest.int_part;
}

void ret(cpu *cpu){ //90 - 144 - pops, copies ret address into cpu->program_counter,
  if(memory[cpu->program_counter].parts.part_two != 0){
    fprintf(stderr, "ERROR: INS - Second half of first byte is not 0 for ret.\n");
    exit(0);
  }

  if(cpu->registers[4] + 3 >= cpu->cpu_size) {
    fprintf(stderr, "ERROR: ADR - Segmentation Fault for esp.\n");
    exit(0);
  }

  int32_to_char retAdd;

  retAdd.char_part.byte4 = memory[cpu->registers[4] + 3].byte_char;
  retAdd.char_part.byte3 = memory[cpu->registers[4] + 2].byte_char;
  retAdd.char_part.byte2 = memory[cpu->registers[4] + 1].byte_char;
  retAdd.char_part.byte1 = memory[cpu->registers[4]].byte_char;

  if(retAdd.int_part < 0 || retAdd.int_part >= cpu->cpu_size){
    fprintf(stderr, "ERROR: ADR - Segmentation Fault for return addres in ret.\n");
    exit(0);
  }

  cpu->registers[4] += 4;

  cpu->program_counter = retAdd.int_part;
}

void pushl(cpu *cpu){ //A0 - 160 - pushes rA onto the stack
  if(memory[cpu->program_counter].parts.part_two != 0){
    fprintf(stderr, "ERROR: INS - Second half of first byte is not 0 for pushl.\n");
    exit(0);
  }

  increment(1, cpu);
  is_register(memory[cpu->program_counter].parts.part_one);

  if(memory[cpu->program_counter].parts.part_two != 15){
    fprintf(stderr, "ERROR: INS - pushl Syntax - Register filler is not F.\n");
    exit(0);
  }

  if(cpu->registers[4] - 4 <= 0) {
    fprintf(stderr, "ERROR: ADR - Segmentation Fault for esp.\n");
    exit(0);
  }

  int32_to_char retAdd;
  retAdd.int_part = cpu->registers[memory[cpu->program_counter].parts.part_one];

  //may need to be reversed
  memory[cpu->registers[4] - 4].byte_char = retAdd.char_part.byte1;
  memory[cpu->registers[4] - 3].byte_char = retAdd.char_part.byte2;
  memory[cpu->registers[4] - 2].byte_char = retAdd.char_part.byte3;
  memory[cpu->registers[4] - 1].byte_char = retAdd.char_part.byte4;

  cpu->registers[4] -= 4;

  increment(1, cpu);
}

void popl(cpu *cpu){ //B0 - 176 - pops from stack into rA
  if(memory[cpu->program_counter].parts.part_two != 0){
    fprintf(stderr, "ERROR: INS - Second half of first byte is not 0 for popl.\n");
    exit(0);
  }

  increment(1, cpu);
  is_register(memory[cpu->program_counter].parts.part_one);

  if(memory[cpu->program_counter].parts.part_two != 15){
    fprintf(stderr, "ERROR: INS - pushl Syntax - Register filler is not F.\n");
    exit(0);
  }

  if(cpu->registers[4] + 3 >= cpu->cpu_size) {
    fprintf(stderr, "ERROR: ADR - Segmentation Fault for esp.\n");
    exit(0);
  }

  int32_to_char forReg;

  forReg.char_part.byte1 = memory[cpu->registers[4]].byte_char;
  forReg.char_part.byte2 = memory[cpu->registers[4] + 1].byte_char;
  forReg.char_part.byte3 = memory[cpu->registers[4] + 2].byte_char;
  forReg.char_part.byte4 = memory[cpu->registers[4] + 3].byte_char;

  cpu->registers[4] += 4;

  cpu->registers[memory[cpu->program_counter].parts.part_one] = forReg.int_part;
  increment(1, cpu);
}

void readX(cpu *cpu){ //C0_C1 192_193 - reads a character or an integer and stores it
  int op = memory[cpu->program_counter].parts.part_two;

  if(op < 0 || op > 1){
    fprintf(stderr, "ERROR: INS - Improper function type for readX.\n");
    exit(0);
  }

  increment(1, cpu);

  if(memory[cpu->program_counter].parts.part_two != 15){
    fprintf(stderr, "ERROR: INS - readX Syntax - Register filler is not F.\n");
    exit(0);
  }

  is_register(memory[cpu->program_counter].parts.part_one);
  int dest = cpu->registers[memory[cpu->program_counter].parts.part_one];

  int32_to_char disp;

  increment(1, cpu);
  disp.char_part.byte1 = memory[cpu->program_counter].byte_char;
  increment(1, cpu);
  disp.char_part.byte2 = memory[cpu->program_counter].byte_char;
  increment(1, cpu);
  disp.char_part.byte3 = memory[cpu->program_counter].byte_char;
  increment(1, cpu);
  disp.char_part.byte4 = memory[cpu->program_counter].byte_char;
  increment(1, cpu);

  dest += disp.int_part;

  if(dest < 0 || dest >= cpu->cpu_size){
    fprintf(stderr, "ERROR: ADR - Segmentation Fault for dest in readX.\n");
    exit(0);
  }

  if(op == 0){ //char
    memory[dest].byte_char = getchar();

    if(memory[dest].byte_char == -1)
      cpu->zero_flag = 1;
    else
      cpu->zero_flag = 0;

  } else { //int
    if(dest + 4 < 0 || dest + 4 >= cpu->cpu_size){
      fprintf(stderr, "ERROR: ADR - Segmentation Fault for dest in readX.\n");
      exit(0);
    }

    if(EOF == scanf("%d", &disp.int_part))
      cpu->zero_flag = 1;
    else
      cpu->zero_flag = 0;

    memory[dest + 3].byte_char = disp.char_part.byte4;
    memory[dest + 2].byte_char = disp.char_part.byte3;
    memory[dest + 1].byte_char = disp.char_part.byte2;
    memory[dest].byte_char = disp.char_part.byte1;
  }
}

void writeX(cpu *cpu){ //D0_D1 - N/A - reads a character or an integer from memory and prints
  int op = memory[cpu->program_counter].parts.part_two;

  if(op < 0 || op > 1){
    fprintf(stderr, "ERROR: INS - Improper function type for writeX.\n");
    exit(0);
  }

  increment(1, cpu);

  if(memory[cpu->program_counter].parts.part_two != 15){
    fprintf(stderr, "ERROR: INS - writeX Syntax - Register filler is not F.\n");
    exit(0);
  }

  is_register(memory[cpu->program_counter].parts.part_one);
  int dest = cpu->registers[memory[cpu->program_counter].parts.part_one];

  int32_to_char disp;

  increment(1, cpu);
  disp.char_part.byte1 = memory[cpu->program_counter].byte_char;
  increment(1, cpu);
  disp.char_part.byte2 = memory[cpu->program_counter].byte_char;
  increment(1, cpu);
  disp.char_part.byte3 = memory[cpu->program_counter].byte_char;
  increment(1, cpu);
  disp.char_part.byte4 = memory[cpu->program_counter].byte_char;
  increment(1, cpu);

  dest += disp.int_part;

  if(dest < 0 || dest >= cpu->cpu_size){
    fprintf(stderr, "ERROR: ADR - Segmentation Fault for dest in writeX.\n");
    exit(0);
  }

  if(op == 0){ //Char
    printf("%c", memory[dest].byte_char);

  } else { //Int
    if(dest + 4 < 0 || dest + 4 >= cpu->cpu_size){
      fprintf(stderr, "ERROR: ADR - Segmentation Fault for dest in writeX.\n");
      exit(0);
    }

    disp.char_part.byte4 = memory[dest + 3].byte_char;
    disp.char_part.byte3 = memory[dest + 2].byte_char;
    disp.char_part.byte2 = memory[dest + 1].byte_char;
    disp.char_part.byte1 = memory[dest].byte_char;

    printf("%d", disp.int_part);
  }
}

void movsbl(cpu *cpu){ //E0 - N/A - takes a byte and extends it into a long
  if(memory[cpu->program_counter].parts.part_two != 0){
    fprintf(stderr, "ERROR: INS - Second half of first byte is not 0 in movsbl.");
    exit(0);
  }

  increment(1, cpu);

  is_register(memory[cpu->program_counter].parts.part_one);
  is_register(memory[cpu->program_counter].parts.part_two);

  int storageReg = memory[cpu->program_counter].parts.part_one;
  int dest = cpu->registers[memory[cpu->program_counter].parts.part_two];

  int32_to_char disp;

  increment(1, cpu);
  disp.char_part.byte1 = memory[cpu->program_counter].byte_char;
  increment(1, cpu);
  disp.char_part.byte2 = memory[cpu->program_counter].byte_char;
  increment(1, cpu);
  disp.char_part.byte3 = memory[cpu->program_counter].byte_char;
  increment(1, cpu);
  disp.char_part.byte4 = memory[cpu->program_counter].byte_char;
  increment(1, cpu);

  dest += disp.int_part;

  if(dest < 0 || dest >= cpu->cpu_size){
    fprintf(stderr, "ERROR: ADR - Segmentation Fault for dest in movsbl.\n");
    exit(0);
  }

  if(memory[dest].parts.part_one >= 8){ //Extend sign
    disp.int_part = -1;
    disp.char_part.byte1 = memory[dest].byte_char;

  } else { //Do nothing
    disp.int_part = 0;
    disp.char_part.byte1 = memory[dest].byte_char;
  }

  cpu->registers[storageReg] = disp.int_part;
}

int process_y86_program(char * file){
	//processing
	long size = 0;
	char *token = 0;
	char *hex_address;
	long address;
	char *argument;
	char *ptr;
	int32_to_char var;
	int val;
	cpu *cpu;

	char * dup_str = strdup(file);
	token = strtok(dup_str, "\t\n");

	while(token != NULL){
		if(strcmp(token, ".size") == 0){
			token = strtok(NULL, "\t\n");
			if(DEBUG){
				printf("size in hex: %s\n", token);
			}
			size = (int) strtol(token, NULL, 16);
			if(DEBUG){
				printf("size in decimal: %ld\n", size);
			}
			memory = malloc(size*sizeof(byte));
			if(DEBUG){
				printf("memory allocated\n");
			}
		}//if .size
		else{
			token = strtok(NULL, "\t\n");
		}
	} //while

	free(dup_str);
	if(DEBUG){
		printf("Duplicated string again\n");
	}

	cpu = malloc(sizeof(*cpu));
	if(DEBUG){
		printf("memory allocated to cpu\n");
	}

	cpu->program_counter = -1;
	if(DEBUG){
		printf("Program counter is -1\n");
	}

	cpu->cpu_size = size;
	if(DEBUG){
		printf("cpu size: %d\n", cpu->cpu_size);
	}

	int i;
	for(i = 0; i < 8; i++){
		cpu->registers[i] = 0;
	}

	dup_str = strdup(file);
	token = strtok(dup_str, "\t\n");
	if(DEBUG){
		printf("reduplicated and retokanized\n");
	}

	while(token){
		if (DEBUG){
			printf("token: %s\n", token);
		}

		if(strcmp(token, ".size") == 0){
			token = strtok(NULL, "\t\n");
			if(DEBUG){
				printf("token: %s\n", token);
			}
			if (DEBUG){
				printf("Skipped .size\n");
			}
		}//if .size
		else if(strcmp(token, ".string") == 0){
			if(DEBUG){
				printf("In .string \n");
			}
			token = strtok(NULL, "\t\n");
			hex_address = token;
			address = convert_hexa_to_decimal(hex_address);
			if(DEBUG){
				printf("hex: %s, decimal: %ld\n", hex_address, address);
			}
			argument = strtok(NULL, "\t\n");
			ptr = argument + 1;

			while(*ptr != '\0'){
				address = segmentCheck(address, cpu);
				memory[address].byte_char = *ptr;
				ptr++;
				address++;
			}
		}//if .string
		else if(strcmp(token, ".long") == 0){
			if(DEBUG){
				printf("In .long \n");
			}
			token = strtok(NULL, "\t\n");
			hex_address = token;
			address = convert_hexa_to_decimal(hex_address);
			if(DEBUG){
				printf("hex: %s, decimal: %ld\n", hex_address, address);
			}
			argument = strtok(NULL, "\t\n");
			var.int_part = (int) strtol(argument, NULL, 16);

			memory[segmentCheck(address, cpu)].byte_char = var.char_part.byte1;
			memory[segmentCheck(address, cpu) + 1].byte_char = var.char_part.byte2;
			memory[segmentCheck(address, cpu) + 2].byte_char = var.char_part.byte3;
			memory[segmentCheck(address, cpu) + 3].byte_char = var.char_part.byte4;
		}//if .long
		else if(strcmp(token, ".byte") == 0){
			if(DEBUG){
				printf("In .byte\n");
			}
			token = strtok(NULL, "\t\n");
			hex_address = token;
			address = convert_hexa_to_decimal(hex_address);
			if(DEBUG){
				printf("hex: %s, decimal: %ld\n", hex_address, address);
			}
			argument = strtok(NULL, "\t\n");

			val = (int) strtol(argument, NULL, 16);
			if(DEBUG){
				printf("value: %d\n", val);
			}
			memory[segmentCheck(address, cpu)].byte_char = (char) val;
		}//if .byte
		else if(strcmp(token, ".text") == 0){
			if(DEBUG){
				printf("In .text\n");
			}
			token = strtok(NULL, "\t\n");
			hex_address = token;
			address = convert_hexa_to_decimal(hex_address);
			if(DEBUG){
				printf("hex: %s, decimal: %ld\n", hex_address, address);
			}

			cpu->program_counter = (int)address;
			if(DEBUG){
				printf("program counter: %d\n", cpu->program_counter);
			}
			argument = strtok(NULL, "\t\n");
			ptr = argument;

			while(*ptr != '\0' && isspace(*ptr) == 0){
				address = segmentCheck(address, cpu);
				memory[address].parts.part_one = getChar(*ptr);
				ptr++;
				if(*ptr!= '\0' && isspace(*ptr) == 0){
					memory[address].parts.part_two = getChar(*ptr);
					ptr++;
				}
				address++;
			}
		}//if .text
		token = strtok(NULL, "\t\n");
	} //while

//end processing
//start executing

	int cmd = 0;

	while(cmd != -1){
		cmd = memory[cpu->program_counter].parts.part_one;

		switch (cmd){
			case 0:	increment(1, cpu);
							break;

			case 1:	cmd = -1;
							break;

			case 2:	rrmovl(cpu);
							break;

			case 3:	irmovl(cpu);
							break;

			case 4:	rmmovl(cpu);
							break;

			case 5:	mrmovl(cpu);
							break;

			case 6:	opl(cpu);
							break;

			case 7:	jcon(cpu);
							break;

			case 8:	call(cpu);
							break;

			case 9:	ret(cpu);
							break;

			case 10: pushl(cpu);
							 break;

			case 11: popl(cpu);
							 break;

			case 12: readX(cpu);
							 break;

			case 13: writeX(cpu);
							 break;

			case 14: movsbl(cpu);
							 break;

			default: fprintf(stderr, "%s\n", "ERROR: Invalid command.");
							 exit(0);
		}//switch
	}//end while execute

  return 0;
} // end process_y86_program

int main(int argc, char *argv[]){
//  int i = 0;
  char *arg = 0;
  char *file = 0;
	//cpu * cpu;

  if(argc != 2){
    fprintf(stderr, "%s\n", "ERROR: Invalid number of arguments");
    exit(0);
  }

  if (argv[1][0] == '-' && argv[1][1] == 'h'){
  		printf("Usage: y86emul <y86 input file>\n");
      exit(0);
  }

	//Ensures the file is a .y86 file
	char * input = strstr(argv[1],".y86");
	if(input == NULL || strlen(input) != 4 || strlen(argv[1]) < 5){
		fprintf(stderr, "ERROR: Improper file name.\n");
		exit(0);
	}

  arg = argv[1];
  file = parse_file(arg);
  process_y86_program(file);

  return 0;
} //end main
