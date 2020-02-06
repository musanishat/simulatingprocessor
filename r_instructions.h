
//These are the R-instructions implemented by the simulted MIPS processor

void add(unsigned int instruction);

void addu(unsigned int instruction); 

void sub(unsigned int instruction); 

void subu(unsigned int instruction); 

void mult(unsigned int instruction); 

void multu(unsigned int instruction); 

void div(unsigned int instruction); 

void divu(unsigned int instruction); 

void and(unsigned int instruction); 

void nor(unsigned int instruction); 

void or(unsigned int instruction); 

void xor(unsigned int instruction); 

void sll(unsigned int instruction); 

void sllv(unsigned int instruction); 

void srl(unsigned int instruction); 

void sra(unsigned int instruction); 

void srlv(unsigned int instruction); 

void srav(unsigned int instruction); 

void slt(unsigned int instruction); 

void sltu(unsigned int instruction); 

void jr(unsigned int instruction); 

void jalr(unsigned int instruction); 

void mfhi(unsigned int instruction); 

void mflo(unsigned int instruction); 

void syscall(unsigned int instruction);  //This is actually defined in syscall.c
