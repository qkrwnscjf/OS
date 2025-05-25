#include <stdio.h>
#include <sys/user.h>
#include <sys/types.h>

void print_regs(struct user_regs_struct regs) {

	printf("r15	%16llx\n", regs.r15);
	printf("r14	%16llx\n", regs.r14);
	printf("r13	%16llx\n", regs.r13);
	printf("r12	%16llx\n", regs.r12);
	printf("rbp	%16llx\n", regs.rbp);
	printf("rbx	%16llx\n", regs.rbx);
	printf("r11	%16llx\n", regs.r11);
	printf("r10	%16llx\n", regs.r10);
	printf("r9	%16llx\n", regs.r9);
	printf("r8	%16llx\n", regs.r8);
	printf("rax	%16llx\n", regs.rax);
	printf("rcx	%16llx\n", regs.rcx);
	printf("rdx	%16llx\n", regs.rdx);
	printf("rsi	%16llx\n", regs.rsi);
	printf("rdi	%16llx\n", regs.rdi);
	printf("orig_rax	%16llx\n", regs.orig_rax);
	printf("rip		%16llx\n", regs.rip);
	printf("cs		%16llx\n", regs.cs);
	printf("eflags		%16llx\n", regs.eflags);
	printf("rsp		%16llx\n", regs.rsp);
	printf("ss		%16llx\n", regs.ss);
	printf("fs_base		%16llx\n", regs.fs_base);
	printf("gs_base		%16llx\n", regs.gs_base);
	printf("ds		%16llx\n", regs.ds);
	printf("es		%16llx\n", regs.es);
	printf("fs		%16llx\n", regs.fs);
	printf("gs		%16llx\n", regs.gs);
}

void peek_data(pid_t tracee_pid) {
	long long int input;
	long long int output;
	printf("peekdata hexaddr : ");
	scanf("%llx", &input);

	output = ptrace(PTRACE_PEEKDATA, tracee_pid, input, 0);  
	if( output == -1 ) 
		perror("PEEKDATA");
	printf("[PEEKDATA]: %llx\n", output);
}

void poke_data(pid_t tracee_pid) {
	long long int output;
	long long int byte_offset;
	long long int word;

	printf("poke hexaddr: ");
	scanf("%llx", &byte_offset);

	printf("poke word: ");
	scanf("%llx", &word);

	output = ptrace(PTRACE_POKEDATA, tracee_pid, byte_offset, word);
	if( output == -1 ) 
		perror("POKEDATA");
}

int pokepeek_iteration(pid_t tracee_pid, struct user_regs_struct regs) {
	printf("next (s)tep, (p)eek data, (P)oke data, (r)egisters, (q)uit \n");
	char input_char = 0;

	while( input_char != 's' && input_char != 'q' ) {
		input_char = getchar();
		if( input_char != '\n' ) {
			switch( input_char ) {
				case 'p' : peek_data(tracee_pid); break;
				case 'P' : poke_data(tracee_pid); break;
				case 'r' : print_regs(regs); break;
			}
		}
	}
	
	if( input_char == 'q' )
		return 0;

	return 1;
}


