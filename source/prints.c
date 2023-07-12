/*
 * File       : PRINTS.C
 * Version    : 3.0
 * License    : GNU GPLv3
 * Author     : Joshua Fain
 * Copyright (c) 2020 - 2023
 * 
 * Description: Implements PRINTS.H. These functions can be used to print 
 *              unsigned integers (decimal, binary, hex) and strings. 
 * 
 * NOTE 
 * Each of the print functions calls the static transmit function 
 * defined here. This function will pass each character arg (one at a time) to
 * an appropriate I/O transmit function (e.g. USART). This I/O transmit 
 * function must be defined by the user elsewhere and included here. It will 
 * only need to be called here from within the static transmit function.
 */

#include <stdint.h>
#include "prints.h"

/*
 ******************************************************************************
 *                             "PRIVATE" FUNCTIONS 
 ******************************************************************************
 */

// SEE NOTE AT TOP
static void transmit(char val)
{
  // placeholder for io_Transmit(val)
}

/*
 ******************************************************************************
 *                                  FUNCTIONS 
 ******************************************************************************
 */


/*
 * ----------------------------------------------------------------------------
 *                             PRINT UNSIGNED DECIMAL (BASE-10) FORM OF INTEGER 
 * 
 * Description : Prints unsigned decimal integer form of argument.
 * 
 * Argument    : num   - Unsigned decimal integer to be printed to screen.
 * ----------------------------------------------------------------------------
 */
void print_Dec(uint32_t num)
{
  const uint8_t radix = 10;                 // decimal radix
  char digit[DEC_CHAR_LEN_MAX];             
  int  digitCnt;                            // total number of digits required

  //
  // 1) Load last digit (remainder) of num into array when divided by radix.
  // 2) Update the value of num by dividing itself by radix.
  // 3) Repeat until number is 0. 
  // Note: The array is loaded in reverse order.
  //
  for (digitCnt = 0; num > 0; ++digitCnt)
  {
    digit[digitCnt] = num % radix + '0';    // convert to ascii
    num /= radix; 
  }

  // print digits
  if (digitCnt == 0)
    transmit('0');
  else
    for (--digitCnt; digitCnt >= 0; digitCnt--)
      transmit(digit[digitCnt]);
}

/*
 * ----------------------------------------------------------------------------
 *                                                 PRINT BINARY FORM OF INTEGER 
 *                                        
 * Description : Prints the binary integer form of the argument.
 * 
 * Argument    : num   - Unsigned decimal integer to be printed to screen.
 * 
 * Notes       : 1) The function will only print the number of bits required.
 *               2) A space will be printed between every BIN_CHARS_GRP_SIZE 
 *                  group of digits to make it more easily readable.
 * ----------------------------------------------------------------------------
 */
void print_Bin(uint32_t num)
{
  const uint8_t radix = 2;                  // binary radix
  char digit[BIN_CHAR_LEN_MAX];             
  int  digitCnt;                            // total number of digits required

  //
  // 1) Load remainder of num into digit array when divided by radix. 
  // 2) Update the value of the num by dividing itself by radix.
  // 3) Repeat until number is 0. 
  // Note: The array is loaded in reverse order.
  //
  for (digitCnt = 0; num > 0; digitCnt++)
  {
    digit[digitCnt] = num % radix + '0';    // convert to ascii
    num /= radix; 
  }

  // print digits.
  if (digitCnt == 0)
    transmit('0');
  else
    for (--digitCnt; digitCnt >= 0; digitCnt--)
    {
      transmit(digit[digitCnt]);
      if (digitCnt % BIN_CHARS_GRP_SIZE == 0)         // print a space ?
        transmit(' ');
    }
}

/*
 * ----------------------------------------------------------------------------
 *                                         PRINT HEXADECIMAL FORM OF AN INTEGER
 *                                       
 * Description : Prints the hexadecimal form of the argument.
 * 
 * Argument    : num   - Unsigned decimal integer to be printed to screen.
 * ----------------------------------------------------------------------------
 */
void print_Hex(uint32_t num)
{
  const uint8_t radix = 16;                 // hex radix
  char digit[HEX_CHAR_LEN_MAX];             
  int  digitCnt;                            // total number of digits required
  
  //
  // 1) Load last digit (remainder) of num into array when divided by radix.
  // 2) Update the value of num by dividing itself by the radix.
  // 3) Convert the array value to an ascii number or letter (A-F) character.
  // 4) Repeat until num is 0. 
  // Note: The array is loaded in reverse order.
  //
  for (digitCnt = 0; num > 0; digitCnt++)
  {
    digit[digitCnt] = num % radix;
    num /= radix;

    if (digit[digitCnt] < 10)
      digit[digitCnt] += '0';               // convert to ascii numbers
    else
      digit[digitCnt] += 'A' - 10;          // convert ascii A to F. Offset 10
  }

  // print digits.
  if (digitCnt == 0)
    transmit('0');
  else
    for (--digitCnt; digitCnt >= 0; digitCnt--)
      transmit(digit[digitCnt]);
}    

/*
 * ----------------------------------------------------------------------------
 *                                                               PRINT C-STRING
 *                                       
 * Description : Prints the C-string passed as the argument.
 * 
 * Argument    : str   - Pointer to a null-terminated char array (i.e. string)
 *                       that will be printed to the screen.
 * 
 * Warning     : There is currently no limit on the length of the string, but
 *               if the array is not null-terminiated then it will loop
 *               continuously until it happens to hit a null in memory.
 * ----------------------------------------------------------------------------
 */
void print_Str(char *str)
{
  for (; *str; str++)
    transmit(*str);
}

