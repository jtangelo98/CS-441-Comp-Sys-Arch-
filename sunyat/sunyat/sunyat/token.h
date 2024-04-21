/*
 * Name        : token.h
 * Author      : William "Amos" Confer
 *
 * License     : Copyright 2008, 2009 William "Amos" Confer
 *
 *               This file is part of Sunyat.
 */

#ifndef _TOKEN_H_
#define _TOKEN_H_

typedef struct Token Token;

struct Token {
	char *token_str;
	char *long_str;
	int line_num;
	int char_pos;
	char data;	/* for immediates and GPR#*/
};

#endif /* _TOKEN_H_ */
