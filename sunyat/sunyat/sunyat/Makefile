# Name        : Makefile
# Author      : William "Amos" Confer
#
# License     : Copyright 2008, 2009 William "Amos" Confer
#
#               This file is part of Sunyat.

all:	sunyat sunyat-asm example

#----------------------------------------
# sunyat
#----------------------------------------
sunyat:	sunyat.c sunyat.h
	gcc -o sunyat sunyat.c

#----------------------------------------
# sunyat-asm
#----------------------------------------
sunyat-asm:	lemon lex.yy.c sunyat-asm.c token.h sunyat.h
	gcc -o sunyat-asm lex.yy.c sunyat-asm.c

lemon:	lemon.c
	gcc -o lemon lemon.c

lex.yy.c:	sunyat-asm.lex
	flex -i sunyat-asm.lex

sunyat-asm.h:
sunyat-asm.c:	sunyat-asm.y sunyat-asm.h lemon
	./lemon sunyat-asm.y

#----------------------------------------
# example
#----------------------------------------
example: sunyat amos.rom
	./sunyat amos.rom

amos.rom: sunyat-asm amos.asm
	./sunyat-asm amos.asm

#----------------------------------------
# clean
#----------------------------------------
clean:
	rm -f sunyat sunyat-asm lemon lex.yy.c sunyat-asm.c sunyat-asm.h sunyat-asm.out amos.rom
