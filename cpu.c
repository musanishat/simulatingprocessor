#include <stdio.h>
#include "cpu.h"
#include "r_instructions.h"
#include "i_instructions.h"
#include "j_instructions.h"

#define OPCODEMASK 0x3F
#define opcode(x) ((x >> 26) & OPCODEMASK)
#define FUNCTMASK 0x3F
#define funct(x) (x & FUNCTMASK)

#define ADDI_OPCODE (0x08)
#define ADDIU_OPCODE (0x09)
#define LW_OPCODE (0x23)
#define LH_OPCODE (0x21)
#define LHU_OPCODE (0x25)
#define LB_OPCODE (0x20)
#define LBU_OPCODE (0x24)
#define SW_OPCODE (0x2B)
#define SH_OPCODE (0x29)
#define SB_OPCODE (0x28)
#define LUI_OPCODE (0x0F)
#define ORI_OPCODE (0x0D)
#define ANDI_OPCODE (0x0C)
#define XORI_OPCODE (0x0E)
#define SLTI_OPCODE (0x0A)
#define SLTIU_OPCODE (0x0B)
#define BEQ_OPCODE (0x04)
#define BNE_OPCODE (0x05)

#define J_OPCODE (0x02)
#define JAL_OPCODE (0x03)

#define ADD_FUNCT (0x20)
#define ADDU_FUNCT (0x21)
#define SUB_FUNCT (0x22)
#define SUBU_FUNCT (0x23)
#define MULT_FUNCT (0x18)
#define MULTU_FUNCT (0x19)
#define DIV_FUNCT (0x1A)
#define DIVU_FUNCT (0x1B)
#define MFHI_FUNCT (0x10)
#define MFLO_FUNCT (0x12)
#define AND_FUNCT (0x24)
#define OR_FUNCT (0x25)
#define XOR_FUNCT (0x26)
#define NOR_FUNCT (0x27)
#define SLT_FUNCT (0x2A)
#define SLTU_FUNCT (0x2B)
#define SLL_FUNCT (0x00)
#define SLLV_FUNCT (0x04)
#define SRL_FUNCT (0x02)
#define SRLV_FUNCT (0x06)
#define SRA_FUNCT (0x03)
#define SRAV_FUNCT (0x07)
#define JR_FUNCT (0x08)
#define JALR_FUNCT (0x09)
#define SYSCALL_FUNCT (0x0C)

unsigned int registers[NUM_REGISTERS];
unsigned char memory[MEMORY_SIZE_IN_BYTES];
unsigned int pc;
unsigned int npc;
unsigned int HI;
unsigned int LO;
unsigned int continueCheck = 1;

void handle_r_instructions(unsigned int instruction){
	unsigned int funct = funct(instruction);
	switch (funct) {
 		case ADD_FUNCT: add(instruction); //printf("add");
 			break;
		case ADDU_FUNCT: addu(instruction);
			break;
		case SUB_FUNCT: sub(instruction);
			break;
		case SUBU_FUNCT: subu(instruction);
			break;
		case MULT_FUNCT: mult(instruction); //printf("mult");
			break;
		case MULTU_FUNCT: multu(instruction);
			break;
		case DIV_FUNCT: div(instruction);
			break;
		case DIVU_FUNCT: divu(instruction);
			break;
		case MFHI_FUNCT: mfhi(instruction);
			break;
		case MFLO_FUNCT: mflo(instruction); //printf("mult");
			break;
		case AND_FUNCT: and(instruction);
			break;
		case OR_FUNCT: or(instruction);
			break;
		case XOR_FUNCT: xor(instruction);
			break;
		case NOR_FUNCT: nor(instruction);
			break;
		case SLT_FUNCT: slt(instruction);
			break;
		case SLTU_FUNCT: sltu(instruction);
			break;
		case SLL_FUNCT: sll(instruction);
			break;
		case SLLV_FUNCT: sllv(instruction);
			break;
		case SRL_FUNCT: srl(instruction);
			break;
		case SRLV_FUNCT: srlv(instruction);
			break;
		case SRA_FUNCT: sra(instruction);
			break;
		case SRAV_FUNCT: srav(instruction);
			break;
		case JR_FUNCT: jr(instruction); //printf("jr");
			break;
		case JALR_FUNCT: jalr(instruction); //printf("jalr");
			break;
		case SYSCALL_FUNCT: syscall(instruction); //printf("syscall");
			break;
		default:
			break;
	}
}

void cpu_initialize(){
	pc = CODE_STARTING_ADDRESS;
	npc = CODE_STARTING_ADDRESS;
	registers[29] = STACK_STARTING_ADDRESS;
	registers[0] = 0;
}

void cpu_execute(){
	continueCheck=1;
	while(continueCheck){
		pc = npc;
		npc += 4;
		unsigned int *inst = (unsigned int *) &memory[pc];
		unsigned int instruction = *inst;
		unsigned int opcode = opcode(instruction);

		switch (opcode) {
 		case 0: handle_r_instructions(instruction);
 			break;
 		case ADDI_OPCODE: addi(instruction); //printf("addi");
 			break;
 		case ADDIU_OPCODE: addiu(instruction); //printf("addi");
 			break;
		case LW_OPCODE: lw(instruction); //printf("lw");
			break;
		case LH_OPCODE: lh(instruction);
			break;
		case LHU_OPCODE: lhu(instruction);
			break;
		case LB_OPCODE: lb(instruction);
			break;
		case LBU_OPCODE: lbu(instruction);
			break;
		case SW_OPCODE: sw(instruction); //printf("sw");
			break;
		case SH_OPCODE: sh(instruction);
			break;
		case SB_OPCODE: sb(instruction);
			break;
		case LUI_OPCODE: lui(instruction);
			break;
		case ORI_OPCODE: ori(instruction);
			break;
		case ANDI_OPCODE: andi(instruction);
			break;
		case XORI_OPCODE: xori(instruction);
			break;
		case SLTI_OPCODE: slti(instruction);
			break;
		case SLTIU_OPCODE: sltiu(instruction);
			break;
		case BEQ_OPCODE: beq(instruction); //printf("beq");
			break;
		case BNE_OPCODE: bne(instruction); //printf("bne");
			break;
		case J_OPCODE: j(instruction); //printf("j");
			break;
		case JAL_OPCODE: jal(instruction); //printf("jal");
			break;
		default:
			printf("wrong opcode");
			break;
		}
	}
}

void cpu_exit(int errorcode){
	if(errorcode==0){
		printf("\nProgram terminated normally\n");
	}else{
		printf("\nProgram terminated with an error value %d\n", errorcode);
	}
	continueCheck = 0;
}


