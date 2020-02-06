#include <stdio.h>
#include "cpu.h"


#define MASK5 0x1F
#define MASK16 0xFFFF
#define RS(x) ((x >> 21) & MASK5)
#define RT(x) ((x >> 16) & MASK5)
#define imm(x) (x & MASK16)

unsigned int signExtend(short x16){
	unsigned int mask15= 1 << 16;
	unsigned int mask1s = ((1 << 16) - 1) << (16);
	int x32= (int)x16;
	if(x32 & mask15){
		x32 = x32 | mask1s;
	}
	return (unsigned int)x32;
}
unsigned int signExtendChar(char x8){
	unsigned int mask3= 1 << 8;
	unsigned int mask1s = ((1 << 8) - 1) << (24);
	int x32= (int)x8;
	if(x32 & mask3){
		x32 = x32 | mask1s;
	}
	return (unsigned int)x32;
}

void addi(unsigned int instruction){
	unsigned int RS = RS(instruction);
	unsigned int RT = RT(instruction);
	int imm = signExtend(imm(instruction));
	registers[RT] = (unsigned int)((int)registers[RS] + imm); 
}

void addiu(unsigned int instruction){
	unsigned int RS = RS(instruction);
	unsigned int RT = RT(instruction);
	unsigned int imm = signExtend(imm(instruction));
	registers[RT] = registers[RS] + imm; 
}

void andi(unsigned int instruction){
	unsigned int RS = RS(instruction);
	unsigned int RT = RT(instruction);
	unsigned int imm = signExtend(imm(instruction));
	registers[RT] = (registers[RS] & imm); 
}

void ori(unsigned int instruction){
	unsigned int RS = RS(instruction);
	unsigned int RT = RT(instruction);
	unsigned int imm = signExtend(imm(instruction));
	registers[RT] = (registers[RS] | imm); 
}

void xori(unsigned int instruction){
	unsigned int RS = RS(instruction);
	unsigned int RT = RT(instruction);
	unsigned int imm = signExtend(imm(instruction));
	registers[RT] = (registers[RS] ^ imm); 
}

void slti(unsigned int instruction){
	unsigned int RS = RS(instruction);
	unsigned int RT = RT(instruction);
	unsigned int imm = signExtend(imm(instruction));
	if((int)registers[RS] < (int)imm){
		registers[RT] = 1;
	}
	else{
		registers[RT] = 0;
	}
}

void sltiu(unsigned int instruction){
	unsigned int RS = RS(instruction);
	unsigned int RT = RT(instruction);
	unsigned int imm = signExtend(imm(instruction));
	if(registers[RS] < imm){
		registers[RT] = 1;
	}
	else{
		registers[RT] = 0;
	}
}

void beq(unsigned int instruction){
	unsigned int RS = RS(instruction);
	unsigned int RT = RT(instruction);
	unsigned int imm = signExtend(imm(instruction));
	if(registers[RS] == registers[RT]){
		npc = pc + 4 + (signExtend(imm) << 2);
	}
}

void bne(unsigned int instruction){
	unsigned int RS = RS(instruction);
	unsigned int RT = RT(instruction);
	unsigned int imm = signExtend(imm(instruction));
	if(registers[RS] != registers[RT]){
		npc = pc + 4 + (signExtend(imm) << 2);
	}
}

void lb(unsigned int instruction){
	unsigned int RS = RS(instruction);
	unsigned int RT = RT(instruction);
	unsigned int imm = signExtend(imm(instruction));
	unsigned char *mem = (unsigned char*) &(memory[((int) registers[RS] + (int)imm)]);
	registers[RT] = signExtendChar(*mem);
}

void lbu(unsigned int instruction){
	unsigned int RS = RS(instruction);
	unsigned int RT = RT(instruction);
	unsigned int imm = signExtend(imm(instruction));
	unsigned char *mem = (unsigned char*) &(memory[(registers[RS] + imm)]);
	registers[RT] = *mem;
}

void lh(unsigned int instruction){
	unsigned int RS = RS(instruction);
	unsigned int RT = RT(instruction);
	unsigned int imm = signExtend(imm(instruction));
	unsigned short *mem = (unsigned short *) &(memory[((int) registers[RS] + (int)imm)]);
	registers[RT] = signExtend(*mem);
}

void lhu(unsigned int instruction){
	unsigned int RS = RS(instruction);
	unsigned int RT = RT(instruction);
	unsigned int imm = signExtend(imm(instruction));
	unsigned short *mem = (unsigned short *) &(memory[(registers[RS] + imm)]);
	registers[RT] = *mem;

}

void lw(unsigned int instruction){
	unsigned int RS = RS(instruction);
	unsigned int RT = RT(instruction);
	unsigned int imm = signExtend(imm(instruction));
	unsigned int *mem = (unsigned int *) &(memory[((int) registers[RS] + (int)imm)]);
	registers[RT] = *mem;
}

void lui(unsigned int instruction){
	unsigned int RT = RT(instruction);
	unsigned int imm = signExtend(imm(instruction));
	registers[RT] = imm;
	registers[RT] = registers[RT] << 16;

}

void sb(unsigned int instruction){
	unsigned int RS = RS(instruction);
	unsigned int RT = RT(instruction);
	unsigned int imm = signExtend(imm(instruction));
	unsigned char *mem = (unsigned char *) &memory[registers[RS] + imm];
	*mem = ((unsigned char) registers[RT]);
}

void sh(unsigned int instruction){
	unsigned int RS = RS(instruction);
	unsigned int RT = RT(instruction);
	unsigned int imm = signExtend(imm(instruction));
	unsigned short *mem = (unsigned short *) &memory[registers[RS] + imm];
	*mem = (unsigned short) registers[RT];
}

void sw(unsigned int instruction){
	unsigned int RS = RS(instruction);
	unsigned int RT = RT(instruction);
	unsigned int imm = signExtend(imm(instruction));
	unsigned int *mem = (unsigned int *) &memory[registers[RS] + imm];
	*mem = registers[RT];
}



