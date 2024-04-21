/*
 * Name        : sunyat.h
 * Author      : William "Amos" Confer
 *
 * License     : Copyright 2008, 2009 William "Amos" Confer
 *
 *               This file is part of Sunyat.
 */
#ifndef _SUNYAT_H_
#define _SUNYAT_H_

#define APP_ID_SIZE	70
#define APP_ROM_SIZE	254
#define APP_FULL_SIZE	(APP_ID_SIZE + APP_ROM_SIZE)
#define APP_KEYBOARD	APP_ROM_SIZE
#define APP_SCREEN	(APP_ROM_SIZE + 1)

#define OPCODE_MOV_RR	0
#define OPCODE_MOV_RI	1
#define OPCODE_ADD_RR	2
#define OPCODE_ADD_RI	3
#define OPCODE_SUB_RR	4
#define OPCODE_SUB_RI	5
#define OPCODE_MUL_RR	6
#define OPCODE_MUL_RI	7
#define OPCODE_DIV_RR	8
#define OPCODE_DIV_RI	9
#define OPCODE_CMP_RR	10
#define OPCODE_CMP_RI	11
#define OPCODE_JMP_M	12
#define OPCODE_JEQ_M	13
#define OPCODE_JNE_M	14
#define OPCODE_JGR_M	15
#define OPCODE_JLS_M	16
#define OPCODE_CALL_M	17
#define OPCODE_RET	18
#define OPCODE_AND_RR	19
#define OPCODE_AND_RI	20
#define OPCODE_OR_RR	21
#define OPCODE_OR_RI	22
#define OPCODE_XOR_RR	23
#define OPCODE_XOR_RI	24
#define OPCODE_NEG_R	25
#define OPCODE_LOAD_RM	26
#define OPCODE_LOADP_RR	27
#define OPCODE_STOR_MR	28
#define OPCODE_STORP_RR	29
#define OPCODE_PUSH_R	30
#define OPCODE_POP_R	31

#endif /* _SUNYAT_H_ */
