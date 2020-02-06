
CC=gcc

all:	cpu


cpu:	cpu.o r_instructions.o i_instructions.o j_instructions.o syscall.o utilities.o test_programs.o
		$(CC) $(CFLAGS) -o cpu cpu.o r_instructions.o i_instructions.o j_instructions.o syscall.o utilities.o test_programs.o


debug:  
		$(CC) -D DEBUG $(CFLAGS) -o cpu cpu.c r_instructions.c i_instructions.c j_instructions.c syscall.c utilities.c test_programs.c
		rm cpu.o r_instructions.o i_instructions.o j_instructions.o syscall.o utilities.o test_programs.o


clean:
		rm cpu.o r_instructions.o i_instructions.o j_instructions.o syscall.o utilities.o test_programs.o


