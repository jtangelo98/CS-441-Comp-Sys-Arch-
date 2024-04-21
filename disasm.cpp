#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>

#include "encodings.h"

using namespace std;

#define RAM_INSTRUCTIONS (1 << 11)
#define RAM_BYTES (RAM_INSTRUCTIONS * sizeof(tm_encoding))

int main(int argc, char *argv[]) {
    if(argc != 2) {
        cerr << " needs a binary file for input" << endl;
    }

    ifstream f(argv[1], ios::binary);
    
	tm_encoding ram[RAM_INSTRUCTIONS];
	memset(ram, 0, RAM_BYTES);
	f.read((char *)ram, RAM_BYTES);
	f.close();


    int addr = 0;
    while(ram[addr].word.u != 0) {
        tm_encoding IR = ram[addr];

        switch(IR.generic.opcode) {
        case TM_OPCODE_ALPHA:
            cout << "alpha " << "'" << (char)(IR.alpha.letter) << "'" << endl;
            break;
        case TM_OPCODE_CMP:
            if(IR.cmp.oring) {
                cout << "or    "; 
            }
            else {
                cout << "cmp   ";
            }
            if(IR.cmp.blank) {
                cout << "BLANK" << endl;
            }
            else {
                cout << "'" << (char)(IR.cmp.letter) << "'" << endl;
            }
            break;
        case TM_OPCODE_JMP:
            if(IR.jmp.eq && IR.jmp.ne) {
                cout << "jmp   ";
            }
            else if(IR.jmp.eq) {
                cout << "je    ";
            }
            else if(IR.jmp.ne) {
                cout << "jne   ";
            }
            else {
                cout << "nop  ; This should never happen" << endl;
                break;
            }
            cout << IR.jmp.addr << endl;
            break;
        case TM_OPCODE_DRAW:
            if(IR.draw.blank) {
                cout << "erase" << endl;
            }
            else {
                cout << "draw  " << "'" << (char)(IR.draw.letter) << "'" << endl;
            }
            break;
        case TM_OPCODE_MOVE:
            if(IR.move.amount < 0) {
                cout << "left";
                if(IR.move.amount != -1) {
                    cout << "  " << -IR.move.amount;
                }
                cout << endl;
            }
            else {
                cout << "right";
                if(IR.move.amount != 1) {
                    cout << " " << IR.move.amount;
                }
                cout << endl;
            }
            break;
        case TM_OPCODE_STOP:
            if(IR.stop.halt) {
                cout << "halt" << endl;
            }
            else {
                cout << "fail" << endl;
            }
            break;

        default:
            cout << "unknown opcode " << IR.generic.opcode;
            cout << " at address " << addr << endl;
        }


        addr++;
    }













    return EXIT_SUCCESS;
}



