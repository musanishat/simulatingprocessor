#include <stdio.h>
#include "cpu.h"


#define MASK5 0x1F
#define RS(x) ((x >> 21) & MASK5);
#define RT(x) ((x >> 16) & MASK5);
#define RD(x) ((x >> 11) & MASK5);
#define shamt(x) ((x >> 6) & MASK5);

void add(unsigned int instruction){
	int RS = RS(instruction);
	int RT = RT(instruction);
	int RD = RD(instruction);
	registers[RD]= (unsigned int)((int)registers[RS]+(int)registers[RT]);
}

void addu(unsigned int instruction){
	unsigned int RS = RS(instruction);
	unsigned int RT = RT(instruction);
	unsigned int RD = RD(instruction);
	registers[RD]= registers[RS]+registers[RT];	
}

void sub(unsigned int instruction){
	int RS = RS(instruction);
	int RT = RT(instruction);
	int RD = RD(instruction);
	registers[RD]= (unsigned int)((int)registers[RS]-(int)registers[RT]);
}

void subu(unsigned int instruction){
	unsigned int RS = RS(instruction);
	unsigned int RT = RT(instruction);
	unsigned int RD = RD(instruction);
	registers[RD]= registers[RS]-registers[RT];	

}

void mult(unsigned int instruction){
	long RS = RS(instruction);
	long RT = RT(instruction);
	long res = (long)registers[RS]*(long)registers[RT];
	LO = (unsigned int)res;
	HI = (unsigned int)(res >> 32);
}

void multu(unsigned int instruction){
	unsigned long RS = RS(instruction);
	unsigned long RT = RT(instruction);
	unsigned long res = (unsigned long)registers[RS] * (unsigned long)registers[RT]; 
	LO = (int)res;
	HI = (int)(res >> 32);
} 

void div(unsigned int instruction){
	int RS = RS(instruction);
	int RT = RT(instruction);
	LO = (unsigned int)((int)registers[RS] / (int)registers[RT]);
	HI = (unsigned int)((int)registers[RS] % (int)registers[RT]);
}

void divu(unsigned int instruction){
	unsigned int RS = RS(instruction);
	unsigned int RT = RT(instruction);
	LO = registers[RS] / registers[RT];
	HI = registers[RS] % registers[RT];
}

void mfhi(unsigned int instruction){
	unsigned int RD = RD(instruction);
	registers[RD] = HI;
}

void mflo(unsigned int instruction){
	unsigned int RD = RD(instruction);
	registers[RD] = LO;
}

void and(unsigned int instruction){
	unsigned int RS = RS(instruction);
	unsigned int RT = RT(instruction);
	unsigned int RD = RD(instruction);
	registers[RD] = registers[RS] & registers[RT];
}

void or(unsigned int instruction){
	unsigned int RS = RS(instruction);
	unsigned int RT = RT(instruction);
	unsigned int RD = RD(instruction);
	registers[RD] = registers[RS] | registers[RT];
}

void xor(unsigned int instruction){
	unsigned int RS = RS(instruction);
	unsigned int RT = RT(instruction);
	unsigned int RD = RD(instruction);
	registers[RD] = registers[RS] ^ registers[RT];
}

void nor(unsigned int instruction){
	unsigned int RS = RS(instruction);
	unsigned int RT = RT(instruction);
	unsigned int RD = RD(instruction);
	registers[RD] = ~(registers[RS] | registers[RT]);
}

void slt(unsigned int instruction){
	int RS = (int)RS(instruction);
	int RT = (int)RT(instruction);
	int RD = (int)RD(instruction);
	if ((int)registers[RS] < (int)registers[RT]){
		registers[RD] = 1;
	}
	else{
		registers[RD] = 0;
	}
}

void sltu(unsigned int instruction){
	unsigned int RS = RS(instruction);
	unsigned int RT = RT(instruction);
	unsigned int RD = RD(instruction);
	if (registers[RS] < registers[RT]){
		registers[RD] = 1;
	}
	else{
		registers[RD] = 0;
	}
}

void sll(unsigned int instruction){
	unsigned int RT = RT(instruction);
	unsigned int RD = RD(instruction);
	unsigned int shamt = shamt(instruction);
	registers[RD] = registers[RT] << shamt;
}

void sllv(unsigned int instruction){
	unsigned int RS = RS(instruction);
	unsigned int RT = RT(instruction);
	unsigned int RD = RD(instruction);
	registers[RD] = registers[RS] << registers[RT];
}

void srl(unsigned int instruction){
	unsigned int RT = RT(instruction);
	unsigned int RD = RD(instruction);
	unsigned int shamt = shamt(instruction);
	registers[RD] = registers[RT] >> shamt;
}

void srlv(unsigned int instruction){
	unsigned int RS = RS(instruction);
	unsigned int RT = RT(instruction);
	unsigned int RD = RD(instruction);
	registers[RD] = registers[RS] >> registers[RT];
}

void sra(unsigned int instruction){
	unsigned int RT = RT(instruction);
	unsigned int RD = RD(instruction);
	unsigned int shamt = shamt(instruction);
	unsigned int mask32 = 1 << 31;
	unsigned int mask1s = ((1 << shamt) - 1) << (32 - shamt);
	if(registers[RT] & mask32){
		registers[RD] = registers[RT] >> shamt;
		registers[RD] = registers[RD] | mask1s;
	}
	else{
		registers[RD] = registers[RT] >> shamt;
	}
	
}

void srav(unsigned int instruction){
	unsigned int RS = RS(instruction);
	unsigned int RT = RT(instruction);
	unsigned int RD = RD(instruction);
	unsigned int mask31 = 1 << 31;
	unsigned int mask1s = ((1 << registers[RT]) - 1) << (32 - registers[RT]);
	if(registers[RS] & mask31){
		registers[RD] = registers[RS] >> registers[RT];
		registers[RD] = registers[RD] | mask1s;
	}
	else{
		registers[RD] = registers[RS] >> registers[RT];
	}
}

void jr(unsigned int instruction){
	unsigned int RS = RS(instruction);
	npc = registers[RS];
}

void jalr(unsigned int instruction){
	unsigned int RS = RS(instruction);
	registers[31] = pc + 4;
	npc = registers[RS]; 
}
