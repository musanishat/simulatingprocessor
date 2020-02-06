
#include <stdio.h>

void print_bits(unsigned int n)
{
  int i;
  for(i=31;i>=0;i--) {
    if (n & (1 << i))  // if there's a 1 in the current position
      printf("1");
    else 
      printf("0");
    if ((i%8) == 0) printf(" ");
  }
  printf("\n");
}

void print_bits_no_newline(unsigned int n)
{
  int i;
  for(i=31;i>=0;i--) {
    if (n & (1 << i))  // if there's a 1 in the current position
      printf("1");
    else 
      printf("0");
    if ((i%8) == 0) printf(" ");
  }
}

void print_bits64(unsigned long n)
{
  unsigned int *p = (unsigned int *) &n;
  print_bits_no_newline(*(p+1));
  printf(" ");
  print_bits(*p);
  
}
