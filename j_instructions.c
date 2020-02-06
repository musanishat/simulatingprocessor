#include <stdio.h>
#include "cpu.h"

#define MASK26 0x3FFFFF
#define address(x) (x & MASK26)


void j(unsigned int instruction){
	int address = address(instruction);
	npc = ((pc+4) & 0xf0000000)| (address << 2);
}
void jal(unsigned int instruction){
	int address = address(instruction);
	registers[31] = pc + 4;
	npc = ((pc+4) & 0xf0000000)| (address << 2);
}
