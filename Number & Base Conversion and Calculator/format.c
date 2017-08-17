#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<stdbool.h>
#include<string.h>
#include "format.h"

unsigned int DEBUG = 0;

typedef union Number {
	int	i;
	float	f;
} Num;

int main(int argc, char *argv[]) {

  //Variable declarations.
  char *bit_sequence;
  char *type;
  int output_int;
  //float output_float;
  char output_str[32];
  memset(output_str, '\0', sizeof(output_str));
  //Counters
  int i, j;

  //error check
  arg_check(argc, argv);

  //process arguments
  i = 0;
  while(i < argc){
    switch(i){
      case 0: break;

      //First argument: this is the input bit sequence.
      //Store in bit_sequence variable.
      case 1: if(DEBUG){printf("%s\n", "Case 1");}
              bit_sequence = argv[i];
              //process_bit_sequence(argv[i], bit_sequence);
              if(DEBUG){
                printf("%s%s\n", "In case 1: bit_sequence: ", bit_sequence);
              }
              //check if bit sequence is of right length
              if(strlen(bit_sequence) != 32){
                fprintf(stderr, "%s\n", "ERROR; Incorrect bit length!");
              }
              //check if bit sequence has non-conforming characters
              for(j = 0; j < strlen(bit_sequence); j++){
                if(bit_sequence[j] != '1' && bit_sequence[j] != '0'){
                  fprintf(stderr, "%s\n", "ERROR; Invalid characters in sequence");
                }
              }
              break;
      //Second argument: this is the type to interpret the input bit sequence.
      //Store in type variable.
      case 2: if(DEBUG){printf("%s\n", "Case 2");}
              type = argv[i];
              //process_type(argv[i], type);
              if(DEBUG){
                printf("%s%s\n", "In case 2: type: ", type);
              }
              break;
      default: fprintf(stderr, "%s\n", "ERROR; too many arguments!");
    } //end switch
    i++;
  } //end while

  //Upon reaching here, arguments have been processed and
  //Stored in respective variables.


  if(strcmp(type, "int") == 0){
    if(DEBUG){
      printf("%s\n", "Entering int case");
    }
    output_int = convert_binary_to_int(bit_sequence, 1);
    if(DEBUG){
      printf("%s\n", "The output int is: ");
      printf("%i\n", output_int);
    }
    if(output_int < 0){
      append(output_str, '-');
      output_int *= -1; //Need to pass a positive number as argument.
    }
    intToASCII(output_str, output_int);
    printf("%s\n", output_str);
  }

  if(strcmp(type, "float") == 0){
    if(DEBUG){
      printf("%s\n", "Entering float case");
    }
    Num n;
    n.i = 0;
    j = 0;
    /*
    for(; j < strlen(bit_sequence); j++){
      n.i = n.i << 1;
      n.i = n.i + (bit_sequence[i] + '0');
    }
    */
    char * ptr = argv[1];
    while(*ptr != '\0'){
      n.i = n.i << 1;
      n.i += *ptr - '0';
      ptr++;
    }
    floatToASCII(n.f, output_str);
    if(n.f == -0.0 && bit_sequence[0] == '1'){
      printf("-%s\n", output_str);
    }
    else{
      printf("%s\n", output_str);
    }
  }

  return 0;
} //end main

void intToDecASCII(int i, char* str){
  char *tmp;
  char sign = '+';
  int n = 0, len = 0;
	int is_neg;
	len = 12;
  //32 bits for an int.
  //But it's in exponential form.
  //So only require an array of size 12.

  char arr[len];
  memset(arr, '0', (len-2));
	arr[(len-1)] = '\0';
  if (DEBUG) {
		printf("i = %d str = %s\n",i,str );
	}
  //If sign is negative, then make the sign '-' and
  //make i positive.

	is_neg = (i < 0) ? 1:0;

	switch(is_neg){

		case 0: //False; number is positive.
						sign = '+';
						i *= 1;
						break;

		case 1: //True; number is negative.
						sign = '-';
						i *= -1;
						break;
	}

  n = (len-2);
	if(DEBUG){
		printf("%s%d\n", "n: ", n);
	}

	while(n >= 0){
		arr[n] = (i % 10) + '0';
		i = i / 10;
		if(i == 0){
			break;
		}
		n--;
	}


  //Add hyphen if the number is negative.
  if(sign == '-'){
    n--;
		arr[n] = '-';
  }

  //Copy resultant array into allocated memory.


  tmp = str;
  while(n < 12){
    *tmp = arr[n];
    tmp++;
    n++;
  }

	if (DEBUG){
		printf("%s%s\n", "arr: ", arr);
		printf(" str = %s\n",str);
	}

}
void intToASCII(char* outputStr, int intNum) {
    char  remChar  ;

    if (DEBUG) {
      printf("intNum = %d  \n",intNum);
    }

    /*  this populates the outputStr in reverse --- wrong
    while (intNum !=0){
        remChar = ((intNum % 10) + '0');
        if (DEBUG) {
           printf("intToASCII : outputStr= %s  intNum = %lld remChar = %c \n",outputStr,intNum,remChar);
        }
        append(outputStr, remChar);
        intNum = intNum / 10;
    }
    */

    if (intNum !=0){
        remChar = ((intNum % 10) + '0');
        if (DEBUG) {
           printf("intToASCII : outputStr= %s  intNum = %d remChar = %c \n",outputStr,intNum,remChar);
        }
        intToASCII(outputStr,(intNum / 10));                 // recursive call
        append(outputStr, remChar);

    } else {
        return;
    }
}


int convert_binary_to_int(char *bit_sequence, int flip){
  int i, max_power, flipped, len, result;
  max_power = strlen(bit_sequence);
  len = strlen(bit_sequence);
  result = 0;
  if(DEBUG){
    printf("%s%d\n", "max_power: ", max_power);
  }
  if(bit_sequence[0] == '1' && flip == 1){
    if(DEBUG){
      printf("%s\n", "Entered main if.");
    }
    if(DEBUG){
      printf("%s%s\n", "bit_sequence before flipping: ", bit_sequence);
    }
    bit_sequence = flip_bits(bit_sequence);
    if(DEBUG){
      printf("%s%s\n", "bit_sequence after flipping: ", bit_sequence);
    }
    flipped = 1;
  }

  for(i = (len-1); i > 0; i--){
    if(bit_sequence[i] == '1'){
      result += pow(2, (max_power-i-1));
    }
  }

  if(flipped == 1){
    result += 1;
    result *= -1;
  }
  return result;
}

char* flip_bits(char *n){
  int i;
  for(i = 1; i < strlen(n); i++){
    if(n[i] == '0'){
      n[i] = '1';
    }
    else if(n[i] == '1'){
      n[i] = '0';
    }
  }
  return n;
}

void arg_check(int argc, char *argv[]){
  int i;
  if(DEBUG){
    printf("%s%d\n", "Number of input arguments: ", argc);
    for(i = 0; i < argc; i++){
      printf("%d:%s\n", i, argv[i]);
    }
  }
  if(argc < 3){
    fprintf(stderr, "%s", "ERROR; Too few arguments!");
    exit(0);
  }
  else if(argc >= 4){
    fprintf(stderr, "%s", "ERROR; Too many arguments!");
    exit(0);
  }

}




/*
 *Begin Professor Russel's code.

*/
// Convert IEEE 32-bit floating point to printable ASCII string

// input:  x is the 32-bit input.
// output:  digit is the output ASCII character
// output:  pow10 is the original pre-normalization power of 10.
// output: remainder is x after subtracting the higi-digit value

static void
get_float_digit( float x, char * digit, int * pow10, float * remainder )
{
	int			p10;

	if ( x == 0.0 )
	{
		*digit = '0';			// digit is always '0'
		*pow10 = 0;
		*remainder = 0.0;
	}
	else
	{
		*pow10 = 0;			// start with 10^0
		while ( x >= 10.0 )		// reduce
		{
			x /= 10.0;		// large values get smaller
			*pow10 += 1;
		}
		while ( x < 1.0 )		// increase
		{
			x *= 10.0;		// small values get larger
			*pow10 -= 1;
		}
		*digit = '0';
		do {				// 10.0 > x >= 1.0
			x -= 1.0;
			*digit += 1;		// digit is always non-'0'zero
		} while ( x >= 1.0 );
		p10 = 0;
		while ( p10 > *pow10 )		// leading digit is now removed from x
		{
			x /= 10;
			p10 -= 1;
		}
		while ( p10 < *pow10 )
		{
			x *= 10;
			p10 += 1;
		}
		*remainder = x;
	}
}

static void
append( char * s, char c )
{
	char		buf[2];

	buf[0] = c;
	buf[1] = '\0';
	strcat( s, buf );
}

void
floatToASCII( float x, char * output )
{
	char		c;
	int		pow10, p10, plast;
	int		i;
	float		remainder;
	char		exponent[10];
	union Number	a;
	unsigned int	biasedExp;
	unsigned int	mantissa;
	int		sign;

  memset(exponent,'\0',10);
	a.f = x;
	biasedExp = a.i >> 23 & 0x000000ff;
	mantissa = a.i & 0x007fffff;
	sign = a.i >> 31;
	if(DEBUG){
		printf("%s%d  mantissa:  %d  sign: %d\n", "biasedExp: ", biasedExp, mantissa, sign);
	}
	// +/- inf, +/- Nan stuff here
  if(biasedExp == 0xff){
    if(mantissa == 0){
      if(sign != 0){
        strcpy( output, "ninf" );
        return;
      } else {
        strcpy( output, "pinf" );
        return;
      }
    } else{
      if(sign != 0){
        strcpy( output, "NaN" ); // +NaN
        return;
      } else {
        strcpy( output, "NaN" ); //-NaN
        return;
      }
    }
  }
	output[0] ='\0';
	if ( x < 0.0 )
	{
		append( output, '-' );
		x = -x;					// make x positive
	}
	get_float_digit( x, &c, &pow10, &remainder );
	append( output, c );
	append( output, '.' );
	x = remainder;
	plast = p10 = pow10;			// pow10 set by get_float_digit()
	for ( i = 1 ; i < 7 ; i++ )		// 7 significant digits in 32-bit float
	{
		get_float_digit( x, &c, &p10, &remainder );
		if ( (plast - p10) > 1 )
		{
			append( output, '0' );	// fill in zero to next nonzero digit
			plast -= 1;
		}
		else
		{
			append( output, c );
			x = remainder;
			plast = p10;
		}
	}
	if (DEBUG) { printf("pow10 = %d exponent = %s\n",pow10,exponent );}
	if ( pow10 < 0 )		// negative exponent
	{
		exponent[0] = 'e';
		intToDecASCII( pow10, exponent+1 );
	}
	else if ( pow10 < 10 )		// positive single-digit exponent
	{
		exponent[0] = 'e';
		exponent[1] = '+';
		exponent[2] = '0';
		intToDecASCII( pow10, exponent+3 );
		/*
		if(pow10 != 0)
			intToDecASCII( pow10, exponent+1 );
		else {
			exponent[1] = '0';
			exponent[2] = '\0';
		}
		*/
	}
	else				// positive multi-digit exponent
	{
		exponent[0] = 'e';
		exponent[1] = '+';
		intToDecASCII( pow10, exponent+2 );
	}
	strcat( output, exponent );
}
