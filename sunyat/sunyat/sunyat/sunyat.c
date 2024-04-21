/*
 * Name        : sunyat.c
 * Author      : William "Amos" Confer
 * Version     : 0x0
 * Description : A simple RISC-like virtual machine designed to demonstrate
 *               one basic way to simulate a theoretical ISA.  Because the
 *               SUNYAT is not based on an ISA ever intended for the IC world,
 *               a few unrealistic liberties were taken to keep the
 *               implementation minimal.  For example, a "clock tick" refers to
 *               a complete fetch-decode-execute cycle so that all instructions
 *               (even memory requests) take a single "clock tick" - although
 *               nothing resembling an instruction pipeline is virtualized.
 *               Also, character input (request at address 0x254) requires an
 *               indeterminate amount of time as it hangs the application until
 *               the user types a character and presses the Enter key... still
 *               counts as only one clock tick :-)
 *
 * Usage       : 1st parameter is a 324 byte ROM image to execute. The first
 *               70 bytes is the application's identification string printed
 *               upon successfull load.  The remaining 254 bytes are the
 *               application "ROM image" which overwrite the entire 254 bytes of
 *               RAM.  Address 0xFE (254) is a key input device and 0xFF (255)
 *               is a character output device (terminal).
 *
 * License     : Copyright 2008, 2009 William "Amos" Confer
 *
 *               This file is part of Sunyat.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sunyat.h"

//////////////////////////////////////////////////

const char MSG_STARTUP [] =
	"\nThe SUNYAT Virtual Machine version 0x0 - (C) 2008, William \"Amos\" Confer\n\nLoading application: %s\n";

const char ERR_NO_APP [] =
	"\tNo application provided...  SUNYAT <filename>\n";
const char ERR_BAD_USAGE [] =
	"\tIncorrect usage... SUNYAT <filename>\n";
const char ERR_BYTE_SIZE [] =
	"\tApplication is not the correct byte size.\n";
const char ERR_FILE_NOT_OPEN [] =
	"\tApplication file could not be opened.\n";

const char ERR_INVALID_PC [] =
	"\tProgram counter is invalid\n";
const char ERR_IMPOSSIBLE_INSTRUCTION [] =
	"\tAttempted to execute an impossible instruction\n";
const char ERR_DIV_ZERO [] =
	"\tDivide by zero error\n";
const char ERR_JMP_RANGE [] =
	"\tJMP instruction targets an out of range address\n";
const char ERR_LOAD [] =
	"\tLOAD instruction uses an out of range address\n";
const char ERR_STOR [] =
	"\tSTOR instruction uses an out of range address\n";
const char ERR_POP [] =
	"\tPOP underflowed the stack\n";
const char ERR_PUSH [] =
		"\tPSH overflowed the stack\n";
const char ERR_CALL_OVERFLOW [] =
		"\tCALL overflowed the stack\n";
const char ERR_CALL_RANGE [] =
	"\tCALL instruction targets an out of range address\n";
//////////////////////////////////////////////////

char app_id [APP_ID_SIZE + 1];	/* +1 is to add a guaranteed null terminator */

unsigned char sunyat_ram [APP_ROM_SIZE];

/*
 * register file for the sunyat-1.
 * 0-7:	general purpose
 * 8:	PC
 * 9:	IR high
 * 10:	IR low
 * 11:	SP
 */
#define REG_PC   8
#define REG_IRH  9
#define REG_IRL 10
#define REG_SP  11
unsigned char sunyat_regs [12] = {
		'0', '7', '2', '8', '2', '0', '0', '7', /* GPRs default to my wedding date :-) */
		0, 0, 0, 								/* PC, IR-H, IR-L */
		APP_ROM_SIZE	 						/* stack grows down from top of RAM */
};

int sunyat_flag_zero = 0;
int sunyat_flag_sign = 0;

long int sunyat_clock_ticks = 0;

//////////////////////////////////////////////////

unsigned char get_opcode ();
unsigned char get_dreg ();
unsigned char get_sreg ();
unsigned char get_mem ();
signed char get_imm ();

void set_flags (signed char result);

void sunyat_execute ();

//////////////////////////////////////////////////

int main (int argc, char *argv []) {
	printf (MSG_STARTUP, argv [1]);

	// check for application parameter
	if (argc != 2) 	{
		printf (ERR_BAD_USAGE);
		return 0;
	}
	else {
		// test application size
		unsigned char file_buffer [APP_FULL_SIZE];
		FILE *infile = NULL;
		if ((infile = fopen (argv [1], "rb")) != NULL) {
			// is it at least APP_FULL_SIZE big ?
			if (APP_FULL_SIZE != fread (file_buffer, sizeof (unsigned char), APP_FULL_SIZE, infile)) {
				// not big enough
				printf (ERR_BYTE_SIZE);
				return 0;
			}
			else {
				// make sure we're at the EOF
				if (fgetc (infile) != EOF) {
					printf (ERR_BYTE_SIZE);
					return 0;
				}
			}
		}
		else {
			// file could not be opened
			printf (ERR_FILE_NOT_OPEN);
			return 0;
		}

		/*
		 * The application image looks good. Let's load it up and run.
		 */

		//close the file... we're done with it now
		fclose (infile);

		// print the application identification message
		memcpy (app_id, file_buffer, APP_ID_SIZE);
		app_id [APP_ID_SIZE] = '\0';	// make sure the ID message is terminated
		printf ("%s\n\n", app_id);

		// load RAM from the ROM image
		memcpy (sunyat_ram, file_buffer + APP_ID_SIZE, APP_ROM_SIZE);

		// fetch->decode->exceute until returned beyond RAM
		sunyat_execute ();

		printf ("\n\nSUNYAT exited after %ld clock cycles\n\n", sunyat_clock_ticks);
	}

	return EXIT_SUCCESS;
}

void sunyat_execute () {
	for (;;) {
		unsigned char opcode;
		unsigned char sreg;
		unsigned char dreg;
		unsigned char mem;
		signed char imm;
		unsigned char cmp_result;

		sunyat_clock_ticks++;

		/*
		 * FETCH
		 */

		// make sure the PC is valid... -2 because each instruction is 2 bytes
		if (sunyat_regs [REG_PC] > (APP_ROM_SIZE - 2)) {
			printf (ERR_INVALID_PC);
			return;
		}

		sunyat_regs [REG_IRH] = sunyat_ram [sunyat_regs [REG_PC]++];
		sunyat_regs [REG_IRL] = sunyat_ram [sunyat_regs [REG_PC]++];

		/*
		 * DECODE
		 */
		opcode = get_opcode ();
		sreg = get_sreg ();
		dreg = get_dreg ();
		mem = get_mem ();
		imm = get_imm ();

		/*
		 * EXECUTE
		 */
		switch (opcode) {
		case OPCODE_MOV_RR:
			sunyat_regs [dreg] = sunyat_regs [sreg];
			break;
		case OPCODE_MOV_RI:
			sunyat_regs [dreg] = imm;
			break;
		case OPCODE_ADD_RR:
			// this should work on signed values as well
			sunyat_regs [dreg] = sunyat_regs [dreg] + sunyat_regs [sreg];
			set_flags (sunyat_regs [dreg]);
			break;
		case OPCODE_ADD_RI:
			// this should work on signed values as well
			sunyat_regs [dreg] = sunyat_regs [dreg] + imm;
			set_flags (sunyat_regs [dreg]);
			break;
		case OPCODE_SUB_RR:
			// this should work on signed values as well
			sunyat_regs [dreg] = sunyat_regs [dreg] - sunyat_regs [sreg];
			set_flags (sunyat_regs [dreg]);
			break;
		case OPCODE_SUB_RI:
			// this should work on signed values as well
			sunyat_regs [dreg] = sunyat_regs [dreg] - imm;
			set_flags (sunyat_regs [dreg]);
			break;
		case OPCODE_MUL_RR:
			// this should work on signed values as well
			sunyat_regs [dreg] = sunyat_regs [dreg] * sunyat_regs [sreg];
			set_flags (sunyat_regs [dreg]);
			break;
		case OPCODE_MUL_RI:
			// this should work on signed values as well
			sunyat_regs [dreg] = sunyat_regs [dreg] * imm;
			set_flags (sunyat_regs [dreg]);
			break;
		case OPCODE_DIV_RR:
			// this should work on signed values as well
			if (sunyat_regs [sreg] == 0) {
				printf (ERR_DIV_ZERO);
				return;
			}
			sunyat_regs [dreg] = sunyat_regs [dreg] / sunyat_regs [sreg];
			set_flags (sunyat_regs [dreg]);
			break;
		case OPCODE_DIV_RI:
			// this should work on signed values as well
			if (imm == 0) {
				printf (ERR_DIV_ZERO);
				return;
			}
			sunyat_regs [dreg] = sunyat_regs [dreg] / imm;
			set_flags (sunyat_regs [dreg]);
			break;
		case OPCODE_CMP_RR:
			// this should work on signed values as well
			cmp_result = sunyat_regs [dreg] - sunyat_regs [sreg];
			set_flags (cmp_result);
			break;
		case OPCODE_CMP_RI:
			// this should work on signed values as well
			cmp_result = sunyat_regs [dreg] - imm;
			set_flags (cmp_result);
			break;
		case OPCODE_JMP_M:
			if (mem >= APP_ROM_SIZE) {
				printf (ERR_JMP_RANGE );
				return;
			}
			sunyat_regs [REG_PC] = mem;
			break;
		case OPCODE_JEQ_M:
			if (sunyat_flag_zero)
				sunyat_regs [REG_PC] = mem;
			break;
		case OPCODE_JNE_M:
			if (!sunyat_flag_zero)
				sunyat_regs [REG_PC] = mem;
			break;
		case OPCODE_JGR_M:
			if (!sunyat_flag_zero && !sunyat_flag_sign)
				sunyat_regs [REG_PC] = mem;
			break;
		case OPCODE_JLS_M:
			if (sunyat_flag_sign)
				sunyat_regs [REG_PC] = mem;
			break;
			if (sunyat_regs [REG_SP] <= 0) {
				printf (ERR_CALL_OVERFLOW);
				return;
			}
			if (mem >= APP_ROM_SIZE) {
				printf (ERR_CALL_RANGE);
				return;
			}
			sunyat_regs [REG_SP]--;
			sunyat_ram [sunyat_regs [REG_SP]] = sunyat_regs [REG_PC];
			sunyat_regs [REG_PC] = mem;
			break;
		case OPCODE_CALL_M:
			if (sunyat_regs [REG_SP] <= 0) {
				printf (ERR_CALL_OVERFLOW);
				return;
			}
			if (mem >= APP_ROM_SIZE) {
				printf (ERR_CALL_RANGE);
				return;
			}
			sunyat_regs [REG_SP]--;
			sunyat_ram [sunyat_regs [REG_SP]] = sunyat_regs [REG_PC];
			sunyat_regs [REG_PC] = mem;
			break;
		case OPCODE_RET:
			if (sunyat_regs [REG_SP] >= APP_ROM_SIZE)
				return;
			sunyat_regs [REG_PC] = sunyat_ram [sunyat_regs [REG_SP]];
			sunyat_regs [REG_SP]++;
			break;
		case OPCODE_AND_RR:
			sunyat_regs [dreg] = sunyat_regs [dreg] & sunyat_regs [sreg];
			set_flags (sunyat_regs [dreg]);
			break;
		case OPCODE_AND_RI:
			sunyat_regs [dreg] = sunyat_regs [dreg] & imm;
			set_flags (sunyat_regs [dreg]);
			break;
		case OPCODE_OR_RR:
			sunyat_regs [dreg] = sunyat_regs [dreg] | sunyat_regs [sreg];
			set_flags (sunyat_regs [dreg]);
			break;
		case OPCODE_OR_RI:
			sunyat_regs [dreg] = sunyat_regs [dreg] | imm;
			set_flags (sunyat_regs [dreg]);
			break;
		case OPCODE_XOR_RR:
			sunyat_regs [dreg] = sunyat_regs [dreg] ^ sunyat_regs [sreg];
			set_flags (sunyat_regs [dreg]);
			break;
		case OPCODE_XOR_RI:
			sunyat_regs [dreg] = sunyat_regs [dreg] ^ imm;
			set_flags (sunyat_regs [dreg]);
			break;
		case OPCODE_NEG_R:
			sunyat_regs [dreg] = -(signed char)(sunyat_regs [dreg]);
			set_flags (sunyat_regs [dreg]);
			break;
		case OPCODE_LOAD_RM:
			if (mem < APP_ROM_SIZE)
				sunyat_regs [dreg] = sunyat_ram [mem];
			else if (mem == APP_KEYBOARD)
				do {
					scanf ("%c", & sunyat_regs [dreg]);
				}
				while (sunyat_regs [dreg] == '\n');
			else {
				printf (ERR_LOAD);
				return;
			}
			break;
		case OPCODE_LOADP_RR:
			if (sunyat_regs [sreg] < APP_ROM_SIZE)
				sunyat_regs [dreg] = sunyat_ram [sunyat_regs [sreg]];
			else if (sunyat_ram [sunyat_regs [sreg]] == APP_KEYBOARD)
				do {
					scanf ("%c", & sunyat_regs [dreg]);
				}
				while (sunyat_regs [dreg] == '\n');
			else {
				printf (ERR_LOAD);
				return;
			}
			break;
		case OPCODE_STOR_MR:
			if (mem < APP_ROM_SIZE)
				sunyat_ram [mem] = sunyat_regs [dreg]; //yes, dreg is correct for this one
			else if (mem == APP_SCREEN) {
				printf ("%c", sunyat_regs [dreg]);
				fflush (stdout);
			}
			else {
				printf (ERR_STOR);
				return;
			}
			break;
		case OPCODE_STORP_RR:
			if (sunyat_regs [dreg] < APP_ROM_SIZE)
				sunyat_ram [sunyat_regs [dreg]] = sunyat_regs [sreg];
			else if (sunyat_regs [dreg] == APP_SCREEN) {
				printf ("%c", sunyat_regs [sreg]);
				fflush (stdout);
			}
			else {
				printf (ERR_STOR);
				return;
			}
			break;
		case OPCODE_PUSH_R:
			if (sunyat_regs [REG_SP] <= 0)
			{
				printf (ERR_PUSH);
				return;
			}
			sunyat_regs [REG_SP]--;
			sunyat_ram [sunyat_regs [REG_SP]] = sunyat_regs [dreg]; //yes, dreg is correct for this one
			break;
		case OPCODE_POP_R:
			if (sunyat_regs [REG_SP] >= APP_ROM_SIZE) {
				printf (ERR_POP);
				return;
			}
			sunyat_regs [dreg] = sunyat_ram [sunyat_regs [REG_SP]];
			sunyat_regs [REG_SP]++;
			break;
		default:
			// This should be impossible since every opcode is accounted for
			printf (ERR_IMPOSSIBLE_INSTRUCTION);
			printf ("opcode = %d\n", opcode);
			return;
			break;
		}

	}
}

unsigned char get_opcode () {
	return sunyat_regs [REG_IRH] >> 3; // top 5 bits are opcode
}

unsigned char get_dreg () {
	return sunyat_regs [REG_IRH] & 0x07; // bottom 3 bits are dreg
}

unsigned char get_sreg () {
	return sunyat_regs [REG_IRL] & 0x07; // bottom 3 bits are sreg
}

unsigned char get_mem () {
	return sunyat_regs [REG_IRL];
}

signed char get_imm () {
	return (signed char)(sunyat_regs [REG_IRL]);
}

void set_flags (signed char result) {
	if (result == 0) {
		sunyat_flag_zero = 1;
		sunyat_flag_sign = 0;
	}
	else if (result > 0) {
		sunyat_flag_zero = 0;
		sunyat_flag_sign = 0;
	}
	else {
		sunyat_flag_zero = 0;
		sunyat_flag_sign = 1;
	}
}