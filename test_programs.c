
/***************************************************

IMPORTANT: DO NOT MODIFY THIS FILE UNLESS YOU *REALLY*  
           KNOW WHAT YOU ARE DOING.

***************************************************/

#include <stdio.h>

#include "cpu.h"
#include "register_names.h"

#include "utilities.h"

#include "programming.h"


//This MIPS assembly program adds the number from 1 to N
//using a loop.
void sum_loop()
{
  int code_addr = 0;
  unsigned int *code_segment = (unsigned int *) &(memory[CODE_STARTING_ADDRESS]);
  int label1;
  code_segment[code_addr++] = ADDI_INSTRUCTION(T0, ZERO, 20); // $T0 = 0 + 20
  code_segment[code_addr++] = ADDI_INSTRUCTION(T1, ZERO, 0); // $T1 = 0

  label1 = code_addr;  //saving the word offset for this label (only works for backwards jumps)
  code_segment[code_addr++] = BEQ_INSTRUCTION(T0, ZERO, 3); // if ($T0==0) jump 3 instructions ahead of next one
  //Testing ADDU here, ADD would have been fine 
  code_segment[code_addr++] = ADDU_INSTRUCTION(T1,T1,T0); // $T1 = $T1 + $T0
  code_segment[code_addr++] = ADDI_INSTRUCTION(T0,T0,-1); //$T0 = $T0-1
  code_segment[code_addr++] = J_INSTRUCTION((CODE_STARTING_ADDRESS_IN_WORDS)+label1); //jump to instruction at memory[CODE_STARTING_ADDRESS+(2*4)]

  code_segment[code_addr++] = ADD_INSTRUCTION(A0,ZERO,T1); // A0 = $T1, the result to print
  code_segment[code_addr++] = ADDI_INSTRUCTION(V0, ZERO, 1); // $v0 = $0 + 1, for print_int syscall.
  code_segment[code_addr++] = SYSCALL_INSTRUCTION; 
  code_segment[code_addr++] = ADDI_INSTRUCTION(V0, ZERO, 10); // $v0 = $0 + 10, for exit syscall.
  code_segment[code_addr++] = SYSCALL_INSTRUCTION; 
}


//This MIPS assembly program computes factorial iteratively.

void fac_iter()
{
  int code_addr = 0;
  unsigned int *code_segment = (unsigned int *) &(memory[CODE_STARTING_ADDRESS]);
  int label1;
  code_segment[code_addr++] = ADDI_INSTRUCTION(T0, ZERO, 7); // N: $T0 = 0 + 7
  code_segment[code_addr++] = ADDI_INSTRUCTION(T1, ZERO, 1); // Running product: $T1 = 1

  label1 = code_addr;  //saving the word offset for this label (only works for backwards jumps)
  code_segment[code_addr++] = BEQ_INSTRUCTION(T0, ZERO, 4); // if ($T0(N)==0) jump 4 instructions ahead of next one
  code_segment[code_addr++] = MULT_INSTRUCTION(T1,T0); // LO = $T1 * $T0
  code_segment[code_addr++] = MFLO_INSTRUCTION(T1);  // $T1 = LO  Running product *= $T0 (N)
  code_segment[code_addr++] = ADDI_INSTRUCTION(T0,T0,-1); //$T0 = $T0-1  (N--)
  code_segment[code_addr++] = J_INSTRUCTION((CODE_STARTING_ADDRESS_IN_WORDS)+label1); //jump to instruction at memory[CODE_STARTING_ADDRESS+(2*4)]

  code_segment[code_addr++] = ADD_INSTRUCTION(A0,ZERO,T1); // A0 = $T1, the result to print
  code_segment[code_addr++] = ADDI_INSTRUCTION(V0, ZERO, 1); // $v0 = $0 + 1, for print_int syscall.
  code_segment[code_addr++] = SYSCALL_INSTRUCTION; 
  code_segment[code_addr++] = ADDI_INSTRUCTION(V0, ZERO, 10); // $v0 = $0 + 10, for exit syscall.
  code_segment[code_addr++] = SYSCALL_INSTRUCTION; 
}


/* This MIPS assembly program computes factorial iteratively.

The MIPS code is as follows:
...

#Code for fac

Fac: 

fac: 
bne $a0, $zero, gen # if $a0<>0, goto generic case
ori $v0, $zero, 1 # else set result $v0 = 1
jr $ra # return
addiu $sp, $sp, -8 # make room for 2 registers on stack
sw $ra, 4($sp) # save return address register $ra
sw $a0, 0($sp) # save argument register $a0=n

addiu $a0, $a0, -1 # $a0 = n-1
jal fac # $v0 = fac(n-1)

lw $a0, 0($sp) # restore $a0=n
lw $ra, 4($sp) # restore $ra
addiu $sp, $sp, 8 # multipop stack

mul $v0, $v0, $a0 # $v0 = fac(n-1) x n
jr $ra # return
      
*/

void fac_rec()
{
  int code_addr = 0;
  unsigned int *code_segment = (unsigned int *) &(memory[CODE_STARTING_ADDRESS]);
  //put a jump to the main code here
  code_segment[code_addr++] = J_INSTRUCTION(CODE_STARTING_ADDRESS_IN_WORDS+15);  //  jump to MAIN, starting at the 15th instruction

  //FAC:
  int fac_label = code_addr;  //this is the address (in words) of the start of fac.
  code_segment[code_addr++] = BNE_INSTRUCTION(A0, ZERO, 2); // if (A0 != 0) jump to RECURSIVE (2 inst. later)
  code_segment[code_addr++] = ADDI_INSTRUCTION(V0, ZERO, 1);  //  $V0 (the return value) = 1
  code_segment[code_addr++] = J_INSTRUCTION(CODE_STARTING_ADDRESS_IN_WORDS+14);  //  jump to DONE (at the 14th instruction)
 
  //RECURSE: 
  code_segment[code_addr++] = ADDIU_INSTRUCTION(SP,SP,-8); //make room for two registers on the stack
  code_segment[code_addr++] = SW_INSTRUCTION(RA,SP,4);  //store $ra at 4($SP)
  code_segment[code_addr++] = SW_INSTRUCTION(A0,SP,0);  //store $A0 at ($SP)
  code_segment[code_addr++] = ADDIU_INSTRUCTION(A0,A0,-1); // A0 = A0-1 , to pass N-1 as the argument 
  code_segment[code_addr++] = JAL_INSTRUCTION(CODE_STARTING_ADDRESS_IN_WORDS+fac_label);  //call FAC
  code_segment[code_addr++] = LW_INSTRUCTION(A0,SP,0); //restore A0 from ($SP)
  code_segment[code_addr++] = LW_INSTRUCTION(RA,SP,4);  //restore $ra from 4($SP)
  code_segment[code_addr++] = ADDIU_INSTRUCTION(SP,SP,8); //adjust SP
  code_segment[code_addr++] = MULT_INSTRUCTION(V0,A0);  //multiply V0 (the return value from the recursive call) by A0 (N)
  code_segment[code_addr++] = MFLO_INSTRUCTION(V0);  //put LO (the result of MULT) into V0
  //DONE:

  code_segment[code_addr++] = JR_INSTRUCTION(RA);  //  jump to DONE  

  //MAIN:
  code_segment[code_addr++] = ADDI_INSTRUCTION(A0,ZERO,6); // $A0 = 6, the initial value for N

  //Testing JALR to make a call, rather than JAL as above.
  code_segment[code_addr++] = LUI_INSTRUCTION(T4, (CODE_STARTING_ADDRESS + (fac_label<<2)) >> 16); //put upper 16 bits of target address into T4
  code_segment[code_addr++] = ORI_INSTRUCTION(T4,T4, CODE_STARTING_ADDRESS + (fac_label<<2)); //insert lower 16 bits into T4
  code_segment[code_addr++] = JALR_INSTRUCTION(T4); //call to the address in T4
  code_segment[code_addr++] = ADD_INSTRUCTION(A0,ZERO,V0); // $A0 = V0, the result to print
  code_segment[code_addr++] = ADDI_INSTRUCTION(V0, ZERO, 1); // $v0 = $0 + 1, for print_int syscall.
  code_segment[code_addr++] = SYSCALL_INSTRUCTION; 
  code_segment[code_addr++] = ADDI_INSTRUCTION(V0, ZERO, 10); // $v0 = $0 + 10, for exit syscall.
  code_segment[code_addr++] = SYSCALL_INSTRUCTION; 
}


//This is a general test of as many instructions (not tested by
//the other programs) as possible.

void instruction_test()
{
  unsigned int code_addr = 0;
  unsigned int *code_segment = (unsigned int *) &(memory[CODE_STARTING_ADDRESS]);;
  unsigned int data_addr = DATA_STARTING_ADDRESS;
  data_addr += 50; //test using location other than the start of the data section
  //Testing LH and ORI
  unsigned int x_addr = data_addr;
  unsigned int *x_ptr = (unsigned int *) &(memory[data_addr]);
  data_addr += 4;  //made room for x

  *x_ptr = 0x110111; // x = 1114385

  code_segment[code_addr++] = LUI_INSTRUCTION(T0,DATA_STARTING_ADDRESS >> 16);  //loads top 16 bits
  code_segment[code_addr++] = ORI_INSTRUCTION(T0,T0,DATA_STARTING_ADDRESS);  //lower 16 bits

  // Loads x = 1114385 into A0
  code_segment[code_addr++] = LW_INSTRUCTION(A0, T0, x_addr-DATA_STARTING_ADDRESS);

  //This code prints the int that is in A0
  //SHOULD PRINT 1114385 
  code_segment[code_addr++] = ADDI_INSTRUCTION(V0, ZERO, 1); // $v0 = 1, for print_int,
  code_segment[code_addr++] = SYSCALL_INSTRUCTION;   

  code_segment[code_addr++] = LUI_INSTRUCTION(T0,DATA_STARTING_ADDRESS >> 16);  //loads top 16 bits
  code_segment[code_addr++] = ORI_INSTRUCTION(T0,T0,DATA_STARTING_ADDRESS);  //lower 16 bits

  //Loads bottom half of x, namely 0x111 = 273 into A0
  code_segment[code_addr++] = LH_INSTRUCTION(A0, T0, x_addr-DATA_STARTING_ADDRESS);

  //This code prints the int that is in A0
  //SHOULD PRINT 273
  code_segment[code_addr++] = ADDI_INSTRUCTION(V0, ZERO, 1); // $v0 = 1, for print_int,
  code_segment[code_addr++] = SYSCALL_INSTRUCTION;   

  //Testing LB
  code_segment[code_addr++] = LUI_INSTRUCTION(T0,DATA_STARTING_ADDRESS >> 16);  //loads top 16 bits
  code_segment[code_addr++] = ORI_INSTRUCTION(T0,T0,DATA_STARTING_ADDRESS);  //lower 16 bits

  //Loads bottom byte of x, namely 0x11 = 17 into A0
  code_segment[code_addr++] = LB_INSTRUCTION(A0, T0, x_addr-DATA_STARTING_ADDRESS);

  //This code prints the int that is in A0
  //SHOULD PRINT 17
  code_segment[code_addr++] = ADDI_INSTRUCTION(V0, ZERO, 1); // $v0 = 1, for print_int,
  code_segment[code_addr++] = SYSCALL_INSTRUCTION;   

  //Testing NOR, ANDI
  code_segment[code_addr++] = NOR_INSTRUCTION(T1,ZERO,ZERO);  //sets T1 to all 1's

  code_segment[code_addr++] = ANDI_INSTRUCTION(A0,T1,345);  //since T1 is all 1's, T0 = 345

  //This code prints the int that is in A0
  //SHOULD PRINT 345
  code_segment[code_addr++] = ADDI_INSTRUCTION(V0, ZERO, 1); // $v0 = 1, for print_int,
  code_segment[code_addr++] = SYSCALL_INSTRUCTION;   

  //Testing XORI (Note: A xor B xor A => B)
  code_segment[code_addr++] = ADDI_INSTRUCTION(A0,ZERO,125);  //A0 = 125
  code_segment[code_addr++] = XORI_INSTRUCTION(A0,A0,99);  //A0 = A0 XOR 99
  code_segment[code_addr++] = XORI_INSTRUCTION(A0,A0,125);  //A0 = A0 XOR 125, should be 99

  //This code prints the int that is in A0
  //SHOULD PRINT 99
  code_segment[code_addr++] = ADDI_INSTRUCTION(V0, ZERO, 1); // $v0 = 1, for print_int,
  code_segment[code_addr++] = SYSCALL_INSTRUCTION;   

  //Testing SLL, by shifting A0 left by 2. If A0 contains 345, the result should be 1380
  code_segment[code_addr++] = ADDI_INSTRUCTION(A0,ZERO,345);  //A0 = 345
  code_segment[code_addr++] = SLL_INSTRUCTION(A0, A0, 2);

  //This code prints the int that is in A0
  //SHOULD PRINT 1380
  code_segment[code_addr++] = ADDI_INSTRUCTION(V0, ZERO, 1); // $v0 = 1, for print_int,
  code_segment[code_addr++] = SYSCALL_INSTRUCTION;   

  //Testing SRL, by shifting A0 right by 3. Since A0 contains 1380, the result should be 172
  code_segment[code_addr++] = SRL_INSTRUCTION(A0, A0, 3);

  //This code prints the int that is in A0
  //SHOULD PRINT 172
  code_segment[code_addr++] = ADDI_INSTRUCTION(V0, ZERO, 1); // $v0 = 1, for print_int,
  code_segment[code_addr++] = SYSCALL_INSTRUCTION;   

  //Testing SUB, and SRA. Put -25 into A0 and shift right by 2. The result should be -7
  
  code_segment[code_addr++] = ADDI_INSTRUCTION(T2,ZERO, 25);
  code_segment[code_addr++] = SUB_INSTRUCTION(A0, ZERO, T2); //A0 = -25

  code_segment[code_addr++] = SRA_INSTRUCTION(A0, A0, 2); //Now A0 = -7 (rounds down)

  //This code prints the int that is in A0
  //SHOULD PRINT -7
  code_segment[code_addr++] = ADDI_INSTRUCTION(V0, ZERO, 1); // $v0 = 1, for print_int,
  code_segment[code_addr++] = SYSCALL_INSTRUCTION;   

  code_segment[code_addr++] = ADDI_INSTRUCTION(A0, ZERO, 7); // A0 = 7
  code_segment[code_addr++] = ADDI_INSTRUCTION(T0, ZERO, 3); // T = 3, the shift amount
  //Testing SLLV, by shifting A0 left by T0 (3). Since A0 contains 7, the result should be 56
  code_segment[code_addr++] = SLLV_INSTRUCTION(A0, A0, T0);

  //This code prints the int that is in A0
  //SHOULD PRINT 56
  code_segment[code_addr++] = ADDI_INSTRUCTION(V0, ZERO, 1); // $v0 = 1, for print_int,
  code_segment[code_addr++] = SYSCALL_INSTRUCTION;   

  //Testing SRL, by shifting A0 right by 3. Since A0 contains 56, the result should be 14
  code_segment[code_addr++] = ADDI_INSTRUCTION(T0, ZERO, 2); // T = 2, the shift amount
  code_segment[code_addr++] = SRLV_INSTRUCTION(A0, A0, T0);

  //This code prints the int that is in A0
  //SHOULD PRINT 14
  code_segment[code_addr++] = ADDI_INSTRUCTION(V0, ZERO, 1); // $v0 = 1, for print_int,
  code_segment[code_addr++] = SYSCALL_INSTRUCTION;   

  //Testing SUB, and SRAV. Put -25 into A0 and shift right by 2. The result should be -7
  
  code_segment[code_addr++] = ADDI_INSTRUCTION(T2,ZERO, 25);
  code_segment[code_addr++] = SUB_INSTRUCTION(A0, ZERO, T2); //A0 = -25

  code_segment[code_addr++] = SRAV_INSTRUCTION(A0, A0, T0); //Now A0 = -7 (rounds down)

  //This code prints the int that is in A0
  //SHOULD PRINT -7
  code_segment[code_addr++] = ADDI_INSTRUCTION(V0, ZERO, 1); // $v0 = 1, for print_int,
  code_segment[code_addr++] = SYSCALL_INSTRUCTION;   


  //Testing SH, writing 0x0020 to low half-word of x
  //x should then be 0x110020 = 1114144 decimal
  code_segment[code_addr++] = LUI_INSTRUCTION(T0,DATA_STARTING_ADDRESS >> 16);  //loads top 16 bits
  code_segment[code_addr++] = ORI_INSTRUCTION(T0,T0,DATA_STARTING_ADDRESS);  //lower 16 bits

  code_segment[code_addr++] = ADDI_INSTRUCTION(T1,ZERO, 0x20);
  code_segment[code_addr++] = SH_INSTRUCTION(T1, T0, x_addr-DATA_STARTING_ADDRESS);

  //Now loading all of x and printing it.
  code_segment[code_addr++] = LW_INSTRUCTION(A0, T0, x_addr-DATA_STARTING_ADDRESS);

  //This code prints the int that is in A0
  //SHOULD PRINT 1114144
  code_segment[code_addr++] = ADDI_INSTRUCTION(V0, ZERO, 1); // $v0 = 1, for print_int,
  code_segment[code_addr++] = SYSCALL_INSTRUCTION;   


  //Testing SB, writing 0x00 to low byte of x (although the register will contain 0xff20
  //x should then be 0x110000 = 1114112 decimal
  code_segment[code_addr++] = LUI_INSTRUCTION(T0,DATA_STARTING_ADDRESS >> 16);  //loads top 16 bits
  code_segment[code_addr++] = ORI_INSTRUCTION(T0,T0,DATA_STARTING_ADDRESS);  //lower 16 bits

  code_segment[code_addr++] = ADDI_INSTRUCTION(T1,ZERO, 0xff00);
  code_segment[code_addr++] = SB_INSTRUCTION(T1, T0, x_addr-DATA_STARTING_ADDRESS);

  //Now loading all of x and printing it.
  code_segment[code_addr++] = LW_INSTRUCTION(A0, T0, x_addr-DATA_STARTING_ADDRESS);

  //This code prints the int that is in A0
  //SHOULD PRINT 1114112
  code_segment[code_addr++] = ADDI_INSTRUCTION(V0, ZERO, 1); // $v0 = 1, for print_int,
  code_segment[code_addr++] = SYSCALL_INSTRUCTION;   


  //Testing LH and LHU.  Set x = 0x0000ffff and perform LH and then LHU. LH should
  //load a negative number (-1), LHU should load a positive number (65535)

  code_segment[code_addr++] = LUI_INSTRUCTION(T0,DATA_STARTING_ADDRESS >> 16);  //loads top 16 bits
  code_segment[code_addr++] = ORI_INSTRUCTION(T0,T0,DATA_STARTING_ADDRESS);  //lower 16 bits

  code_segment[code_addr++] = ADDI_INSTRUCTION(T3,ZERO,0xffff); //just enough bits (16) in the constant field

  code_segment[code_addr++] = SW_INSTRUCTION(T3, T0, x_addr-DATA_STARTING_ADDRESS);

  code_segment[code_addr++] = LH_INSTRUCTION(A0, T0, x_addr-DATA_STARTING_ADDRESS);

  //This code prints the int that is in A0
  //SHOULD PRINT -1
  code_segment[code_addr++] = ADDI_INSTRUCTION(V0, ZERO, 1); // $v0 = 1, for print_int,
  code_segment[code_addr++] = SYSCALL_INSTRUCTION;   

  code_segment[code_addr++] = LUI_INSTRUCTION(T0,DATA_STARTING_ADDRESS >> 16);  //loads top 16 bits
  code_segment[code_addr++] = ORI_INSTRUCTION(T0,T0,DATA_STARTING_ADDRESS);  //lower 16 bits

  code_segment[code_addr++] = LHU_INSTRUCTION(A0, T0, x_addr-DATA_STARTING_ADDRESS);

  //This code prints the int that is in A0
  //SHOULD PRINT 65535
  code_segment[code_addr++] = ADDI_INSTRUCTION(V0, ZERO, 1); // $v0 = 1, for print_int,
  code_segment[code_addr++] = SYSCALL_INSTRUCTION;   

  //Testing LB and LBU.  Set x = 0x000011ff and perform LH and then LHU. LH should
  //load a negative number (-1), LBU should load a positive number (255)

  code_segment[code_addr++] = LUI_INSTRUCTION(T0,DATA_STARTING_ADDRESS >> 16);  //loads top 16 bits
  code_segment[code_addr++] = ORI_INSTRUCTION(T0,T0,DATA_STARTING_ADDRESS);  //lower 16 bits

  code_segment[code_addr++] = ADDI_INSTRUCTION(T3,ZERO,0x11ff); //just enough bits (16) in the constant field

  code_segment[code_addr++] = SW_INSTRUCTION(T3, T0, x_addr-DATA_STARTING_ADDRESS);

  code_segment[code_addr++] = LB_INSTRUCTION(A0, T0, x_addr-DATA_STARTING_ADDRESS);

  //This code prints the int that is in A0
  //SHOULD PRINT -1
  code_segment[code_addr++] = ADDI_INSTRUCTION(V0, ZERO, 1); // $v0 = 1, for print_int,
  code_segment[code_addr++] = SYSCALL_INSTRUCTION;   

  code_segment[code_addr++] = LUI_INSTRUCTION(T0,DATA_STARTING_ADDRESS >> 16);  //loads top 16 bits
  code_segment[code_addr++] = ORI_INSTRUCTION(T0,T0,DATA_STARTING_ADDRESS);  //lower 16 bits

  code_segment[code_addr++] = LBU_INSTRUCTION(A0, T0, x_addr-DATA_STARTING_ADDRESS);

  //This code prints the int that is in A0
  //SHOULD PRINT 255
  code_segment[code_addr++] = ADDI_INSTRUCTION(V0, ZERO, 1); // $v0 = 1, for print_int,
  code_segment[code_addr++] = SYSCALL_INSTRUCTION;   

  //Testing LUI.  Load 2 into the upper word of A0, printing 2^17 = 131072

  code_segment[code_addr++] = LUI_INSTRUCTION(A0, 2);

  //This code prints the int that is in A0
  //SHOULD PRINT 131072
  code_segment[code_addr++] = ADDI_INSTRUCTION(V0, ZERO, 1); // $v0 = 1, for print_int,
  code_segment[code_addr++] = SYSCALL_INSTRUCTION;   

  //Testing SLT.  Setting A0 = (T5 (-1) < T6(200)) = 1 

  code_segment[code_addr++] = ADDI_INSTRUCTION(T5, ZERO, 0xffff); //sets T5 = -1 (due to sign extension of 0xffff)
  code_segment[code_addr++] = ORI_INSTRUCTION(T6, ZERO, 200);
  code_segment[code_addr++] = SLT_INSTRUCTION(A0, T5, T6);

  //This code prints the int that is in A0
  //SHOULD PRINT 1
  code_segment[code_addr++] = ADDI_INSTRUCTION(V0, ZERO, 1); // $v0 = 1, for print_int,
  code_segment[code_addr++] = SYSCALL_INSTRUCTION;   

  //Now reverse order of operands to SLT, should print 0

  code_segment[code_addr++] = SLT_INSTRUCTION(A0, T6, T5);

  //This code prints the int that is in A0
  //SHOULD PRINT 0
  code_segment[code_addr++] = ADDI_INSTRUCTION(V0, ZERO, 1); // $v0 = 1, for print_int,
  code_segment[code_addr++] = SYSCALL_INSTRUCTION;   

  //Testing SLTI.  Setting A0 = (T5 (-1) < 200) = 1 

  code_segment[code_addr++] = ADDI_INSTRUCTION(T5, ZERO, 0xffff); //sets T5 = -1 (due to sign extension of 0xffff)
  code_segment[code_addr++] = SLTI_INSTRUCTION(A0, T5, 200);

  //This code prints the int that is in A0
  //SHOULD PRINT 1
  code_segment[code_addr++] = ADDI_INSTRUCTION(V0, ZERO, 1); // $v0 = 1, for print_int,
  code_segment[code_addr++] = SYSCALL_INSTRUCTION;   

  //Testing SLTI.  Setting A0 = (T5 (-1) < -2) = 0

  code_segment[code_addr++] = ADDI_INSTRUCTION(T5, ZERO, 0xffff); //sets T5 = -1 (due to sign extension of 0xffff)
  code_segment[code_addr++] = SLTI_INSTRUCTION(A0, T5, 0xfffe); //0xfffe = -2

  //This code prints the int that is in A0
  //SHOULD PRINT 0
  code_segment[code_addr++] = ADDI_INSTRUCTION(V0, ZERO, 1); // $v0 = 1, for print_int,
  code_segment[code_addr++] = SYSCALL_INSTRUCTION;   

  //Testing SLTIU.  Setting A0 = (T5 (-1) < 200) = 0  (because -1 is interpreted as a large positive number)

  code_segment[code_addr++] = ADDI_INSTRUCTION(T5, ZERO, 0xffff); //sets T5 = -1 (due to sign extension of 0xffff)
  code_segment[code_addr++] = SLTIU_INSTRUCTION(A0, T5, 200);

  //This code prints the int that is in A0
  //SHOULD PRINT 0
  code_segment[code_addr++] = ADDI_INSTRUCTION(V0, ZERO, 1); // $v0 = 1, for print_int,
  code_segment[code_addr++] = SYSCALL_INSTRUCTION;   

  //Testing SLTIU.  Setting A0 = (T5 (200) < -1) = 1  (because -1 is interpreted as a large positive number)

  code_segment[code_addr++] = ADDI_INSTRUCTION(T5, ZERO, 200); //sets T5 = 200
  code_segment[code_addr++] = SLTIU_INSTRUCTION(A0, T5, 0xffff); // A0 = (200 < -1) =  1

  //This code prints the int that is in A0
  //SHOULD PRINT 1
  code_segment[code_addr++] = ADDI_INSTRUCTION(V0, ZERO, 1); // $v0 = 1, for print_int,
  code_segment[code_addr++] = SYSCALL_INSTRUCTION;   

  //Testing SUBU -- does the same thing as SUB for our purposes (normally doesn't set overflow flag)
  //T5 holds 200, T6 holds 200, so should get 0 as a result.
  code_segment[code_addr++] = SUBU_INSTRUCTION(A0, T5, T6);

  //This code prints the int that is in A0
  //SHOULD PRINT 0
  code_segment[code_addr++] = ADDI_INSTRUCTION(V0, ZERO, 1); // $v0 = 1, for print_int,
  code_segment[code_addr++] = SYSCALL_INSTRUCTION;   

  //Test MULT (with large result) and MFHI.

  code_segment[code_addr++] = LUI_INSTRUCTION(T1,0x4000);  //sets the top 16 bits to 0x4000 (i.e. bit 30 is 1)
  code_segment[code_addr++] = ORI_INSTRUCTION(T1,T1,10);  //T1 = T1 | 10, SO T1 = 0X40000000 + 10 = 1073741834 (decimal)

  code_segment[code_addr++] = ADD_INSTRUCTION(A0, T1, ZERO);  

  //This code prints the int that is in A0
  //SHOULD PRINT 1073741834
  code_segment[code_addr++] = ADDI_INSTRUCTION(V0, ZERO, 1); // $v0 = 1, for print_int,
  code_segment[code_addr++] = SYSCALL_INSTRUCTION;   

  code_segment[code_addr++] = ADDI_INSTRUCTION(T2,ZERO,4);  //T2 = 4
  code_segment[code_addr++] = MULT_INSTRUCTION(T1,T2); //HI:LO = T1 * T2,  should set HI to 1 and LO to 40
  code_segment[code_addr++] = MFHI_INSTRUCTION(A0); 
  
  //This code prints the int that is in A0
  //SHOULD PRINT 1
  code_segment[code_addr++] = ADDI_INSTRUCTION(V0, ZERO, 1); // $v0 = 1, for print_int,
  code_segment[code_addr++] = SYSCALL_INSTRUCTION;   

  code_segment[code_addr++] = MFLO_INSTRUCTION(A0);
  
  //This code prints the int that is in A0
  //SHOULD PRINT 40
  code_segment[code_addr++] = ADDI_INSTRUCTION(V0, ZERO, 1); // $v0 = 1, for print_int,
  code_segment[code_addr++] = SYSCALL_INSTRUCTION;   

  //Test MULTU (with negative number) and MFHI.

  code_segment[code_addr++] = LUI_INSTRUCTION(T1,-1);  //sets the top 16 bits to 0xffff 
  code_segment[code_addr++] = ORI_INSTRUCTION(T1, T1, -1);  //T1 = T1 -1, SO T1 = -1

  code_segment[code_addr++] = ADD_INSTRUCTION(A0, T1, ZERO);  

  //This code prints the int that is in A0
  //SHOULD PRINT -1
  code_segment[code_addr++] = ADDI_INSTRUCTION(V0, ZERO, 1); // $v0 = 1, for print_int,
  code_segment[code_addr++] = SYSCALL_INSTRUCTION;   

  code_segment[code_addr++] = ADDI_INSTRUCTION(T2,ZERO,2);  //T2 = 2
  code_segment[code_addr++] = MULTU_INSTRUCTION(T1,T2); //HI:LO = T1 * T2,  should set HI to 1, LO to -2
  code_segment[code_addr++] = MFHI_INSTRUCTION(A0); 
  
  //This code prints the int that is in A0
  //SHOULD PRINT 1
  code_segment[code_addr++] = ADDI_INSTRUCTION(V0, ZERO, 1); // $v0 = 1, for print_int,
  code_segment[code_addr++] = SYSCALL_INSTRUCTION;   

  code_segment[code_addr++] = MFLO_INSTRUCTION(A0);
  
  //This code prints the int that is in A0
  //SHOULD PRINT -2
  code_segment[code_addr++] = ADDI_INSTRUCTION(V0, ZERO, 1); // $v0 = 1, for print_int,
  code_segment[code_addr++] = SYSCALL_INSTRUCTION;   


  //Testing DIV
  code_segment[code_addr++] = ADDI_INSTRUCTION(T2,ZERO,237);  //T2 = 237
  code_segment[code_addr++] = ADDI_INSTRUCTION(T1,ZERO,-10);  //T2 = -10
  code_segment[code_addr++] = DIV_INSTRUCTION(T2,T1);  //LO = T2/T1 = -23, HI = T2%T1 = 7
  
  code_segment[code_addr++] = MFLO_INSTRUCTION(A0); //printing LO, should be -23

  //This code prints the int that is in A0
  //SHOULD PRINT -23
  code_segment[code_addr++] = ADDI_INSTRUCTION(V0, ZERO, 1); // $v0 = 1, for print_int,
  code_segment[code_addr++] = SYSCALL_INSTRUCTION;   

  code_segment[code_addr++] = MFHI_INSTRUCTION(A0); //printing HI, should be 7

  //This code prints the int that is in A0
  //SHOULD PRINT 7
  code_segment[code_addr++] = ADDI_INSTRUCTION(V0, ZERO, 1); // $v0 = 1, for print_int,
  code_segment[code_addr++] = SYSCALL_INSTRUCTION;   


  //Testing DIVU

  code_segment[code_addr++] = ADDI_INSTRUCTION(T2,ZERO,237);  //T2 = 237
  code_segment[code_addr++] = ADDI_INSTRUCTION(T1,ZERO,-10);  //T2 = -10 (which is interpreted as large postive number)
  code_segment[code_addr++] = DIVU_INSTRUCTION(T2,T1);  //LO = T2/T1 = 0, HI = T2%T1 = 237
  
  code_segment[code_addr++] = MFLO_INSTRUCTION(A0); //printing LO, should be 0

  //This code prints the int that is in A0
  //SHOULD PRINT 0
  code_segment[code_addr++] = ADDI_INSTRUCTION(V0, ZERO, 1); // $v0 = 1, for print_int,
  code_segment[code_addr++] = SYSCALL_INSTRUCTION;   

  code_segment[code_addr++] = MFHI_INSTRUCTION(A0); //printing HI, should be 237

  //This code prints the int that is in A0
  //SHOULD PRINT 237
  code_segment[code_addr++] = ADDI_INSTRUCTION(V0, ZERO, 1); // $v0 = 1, for print_int,
  code_segment[code_addr++] = SYSCALL_INSTRUCTION;   

  //Testing AND, OR, and XOR

  code_segment[code_addr++] = ADDI_INSTRUCTION(T1,ZERO, 0xff); //T1 = 0xff
  code_segment[code_addr++] = ADDI_INSTRUCTION(T2,ZERO, 0x34); //T2 = 0x34
  code_segment[code_addr++] = AND_INSTRUCTION(T1,T1, T2); //T1 = T1 & T2 = 0x34
  code_segment[code_addr++] = ADDI_INSTRUCTION(T2,ZERO, 0x70); //NOW T2 = 0x70
  code_segment[code_addr++] = OR_INSTRUCTION(T1,T1, T2); //T1 = T1 OR T2 = 0x74

  code_segment[code_addr++] = ADD_INSTRUCTION(A0,T1,ZERO); //A0 = T1 = 0x74 = 116 for printing

  //This code prints the int that is in A0
  //SHOULD PRINT 116
  code_segment[code_addr++] = ADDI_INSTRUCTION(V0, ZERO, 1); // $v0 = 1, for print_int,
  code_segment[code_addr++] = SYSCALL_INSTRUCTION;   

  code_segment[code_addr++] = ADDI_INSTRUCTION(T2,ZERO, 0xf); //NOW T2 = 0xf 

  code_segment[code_addr++] = XOR_INSTRUCTION(A0,T1,T2); //A0 = T1 ^ T2 = 0x74 ^ 0xf = 0x7n = 123

  //This code prints the int that is in A0
  //SHOULD PRINT 123
  code_segment[code_addr++] = ADDI_INSTRUCTION(V0, ZERO, 1); // $v0 = 1, for print_int,
  code_segment[code_addr++] = SYSCALL_INSTRUCTION;   

  //---- EXIT ------
  code_segment[code_addr++] = ADDI_INSTRUCTION(V0, ZERO, 10); // $v0 = $0 + 10, for exit syscall.
  code_segment[code_addr++] = SYSCALL_INSTRUCTION; 

}


void syscall_test()
{
  unsigned int code_addr = 0;
  unsigned int *code_segment = (unsigned int *) &(memory[CODE_STARTING_ADDRESS]);
  unsigned int data_addr = DATA_STARTING_ADDRESS; //careful, this may be more than 16 bits.
  unsigned int i;
    
  //print_string test
  unsigned char prompt[] = "Enter Number >";
  unsigned int prompt_label = data_addr;
  //copy string into memory at the beginning of the data section
  for(i=0; prompt[i] != 0; i++) {
    memory[data_addr++] = prompt[i];
  }
  memory[data_addr++] = 0;
  //Another prompt
  unsigned char prompt_str[] = "Enter String >";
  unsigned int prompt_str_label = data_addr;
  //copy string into memory at the beginning of the data section
  for(i=0; prompt_str[i] != 0; i++) {
    memory[data_addr++] = prompt_str[i];
  }
  memory[data_addr++] = 0;
  //Since a label may be at an address that requires more a 16 bit immediate, will need to use
  //LUI.

  code_segment[code_addr++] = LUI_INSTRUCTION(A0,prompt_label>>16); //load A0 with the top half of the address of string
  code_segment[code_addr++] = ORI_INSTRUCTION(A0,A0,prompt_label); // fill in the bottom half

  code_segment[code_addr++] = ADDI_INSTRUCTION(V0, ZERO, 4); // $v0 = $0 + 4, for print_string syscall.
  code_segment[code_addr++] = SYSCALL_INSTRUCTION; 

  //read_int test
  
  code_segment[code_addr++] = ADDI_INSTRUCTION(V0, ZERO, 5); // $v0 = $0 + 5, for read_int, result is put in V0
  code_segment[code_addr++] = SYSCALL_INSTRUCTION;   
  code_segment[code_addr++] = ADD_INSTRUCTION(A0,ZERO,V0); //$a0 = $v0, getting ready call print_int
  code_segment[code_addr++] = ADDI_INSTRUCTION(V0, ZERO, 1); // $v0 = 1, for print_int,
  code_segment[code_addr++] = SYSCALL_INSTRUCTION;   

  //printing string prompt

  code_segment[code_addr++] = LUI_INSTRUCTION(A0,prompt_str_label>>16); //load A0 with the top half of the address of string
  code_segment[code_addr++] = ORI_INSTRUCTION(A0,A0,prompt_str_label); // fill in the bottom half
  code_segment[code_addr++] = ADDI_INSTRUCTION(V0, ZERO, 4); // $v0 = $0 + 4, for print_string syscall.
  code_segment[code_addr++] = SYSCALL_INSTRUCTION; 

  //read_string test
  //read_string: $a0 = memory address of string input buffer
  //             $a1 = length of string buffer (n)       

#define STRING_BUF_SIZE 80
  unsigned int input_string_addr = data_addr; //careful, may need more than 16 bits
  data_addr += STRING_BUF_SIZE;
  code_segment[code_addr++] = ADDI_INSTRUCTION(V0, ZERO, 8); // $v0 = $8, for read_int, 

  code_segment[code_addr++] = LUI_INSTRUCTION(A0,input_string_addr>>16); //load A0 with the top half of the address of buffer
  code_segment[code_addr++] = ORI_INSTRUCTION(A0,A0, input_string_addr); // fill in the bottom half

  code_segment[code_addr++] = ADDI_INSTRUCTION(A1, ZERO, STRING_BUF_SIZE); // $a1 = size of string buffer
  code_segment[code_addr++] = SYSCALL_INSTRUCTION;  

  //now print the string just read
  code_segment[code_addr++] = LUI_INSTRUCTION(A0,input_string_addr>>16); //load A0 with the top half of the address of buffer
  code_segment[code_addr++] = ORI_INSTRUCTION(A0,A0, input_string_addr); // fill in the bottom half

  code_segment[code_addr++] = ADDI_INSTRUCTION(V0, ZERO, 4); // $v0 = $0 + 4, for print_string syscall.
  code_segment[code_addr++] = SYSCALL_INSTRUCTION; 

  //exit

  code_segment[code_addr++] = ADDI_INSTRUCTION(V0, ZERO, 10); // $v0 = $0 + 10, for exit syscall.
  code_segment[code_addr++] = SYSCALL_INSTRUCTION; 
}



int main()
{
  cpu_initialize();
  sum_loop();
  printf("Executing a program testing a simple loop\n");
  cpu_execute();

  cpu_initialize();
  fac_iter();
  printf("\nExecuting an iterative factorial program\n");
  cpu_execute();

  cpu_initialize();
  fac_rec();
  printf("\nExecuting a recursive factorial program\n");
  cpu_execute();

  cpu_initialize();
  syscall_test();
  printf("\nExecuting a program testing the syscall (I/O) function\n");
  cpu_execute();

  cpu_initialize();
  printf("\nExecuting a program testing all instructions not tested above\n");
  instruction_test();
  cpu_execute();

}
