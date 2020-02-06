
#include <stdio.h>
#include <string.h>

#include "cpu.h"

//print_int:  $a0 = integer to be printed       
void print_int()
{
  printf("%d\n", registers[4]);
}

//print_float:  $f12 = float to be printed  *NOT SUPPORTED*
void print_float()
{
  printf("Syscall Error. print_float not supported\n");
  cpu_exit(1);
}

//print_double: $f12 = double to be printed *NOT SUPPORTED*
void print_double()
{
  printf("Syscall Error. print_double not supported\n");
  cpu_exit(1);
}

//print_string $a0 = address of string in memory       
void print_string()
{
#ifdef DEBUG
  printf("In print_string, A0 = %d\n", registers[4]);
#endif
  printf("%s", &(memory[registers[4]]));
#ifdef DEBUG
  printf("Done print_string\n");
#endif

}


//read_int: integer returned in $v0
void read_int()
{
#ifdef DEBUG
  printf("In read_int\n");
#endif

  int val;
  scanf("%d", &val);
  registers[2] = (unsigned int) val;
}


//read_float: float returned in $v0 *NOT SUPPORTED
void read_float()
{
  printf("Syscall Error. read_float not supported\n");
  cpu_exit(1);
}

//read_double:  double returned in $v0 *NOT SUPPORTED*
void read_double()
{
  printf("Syscall Error. read_double not supported\n");
  cpu_exit(1);
}

//read_string: $a0 = memory address of string input buffer
//             $a1 = length of string buffer (n)       
void read_string()
{
  unsigned char buf[1000];  //should be plenty of room!
  scanf("%s", buf);
  strncpy((char *) &(memory[registers[4]]), (char *) buf, registers[5]); //number of copied bytes <= $a1

  //Note: Intentionally didn't add a 0, it's a bug in the source program
  //if the value in $a1 is not big enough to include the terminating 0.
}

    

//sbrk   $a0 = amount    address in $v0  *NOT SUPPORTED
void sbrk()
{
  printf("Syscall Error. sbrk not supported\n");
  cpu_exit(1);
}




//This is the function providing the syscall functionality

void syscall()
{
#ifdef DEBUG
  printf("In syscall, V0 = %d\n", registers[2]);
#endif
  switch (registers[2]) {
  case 1: 
    print_int();
    break;
  case 2:
    print_float();
    break;
  case 3:
    print_double();
    break;
  case 4:
    print_string();
    break;
  case 5:
    read_int();
    break;
  case 6:
    read_float();
    break;
  case 7:
    read_double();
    break;
  case 8:
    read_string();
    break;
  case 9:
    sbrk();
    break;
  case 10:    //EXIT
    cpu_exit(0);
    break;
  default:
    printf("Syscall Error. Incorrect value in $v0: %d\n", registers[2]);
    cpu_exit(1);
    break;
  }
#ifdef DEBUG
  printf("Returning from syscall\n");
#endif
}
