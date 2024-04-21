#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <deque>
#include "encodings.h"

using namespace std;


typedef struct{
    char letter;
    bool is_blank;
} cell;

void print_tape(deque <cell> tape, int head_pos);
void get_next_tape(deque <cell> &tape, int tape_num, int total_tapes, vector <string> tape_strings, int &head_pos);

#define RAM_INSTRUCTIONS (1 << 11)
#define RAM_BYTES (RAM_INSTRUCTIONS * sizeof(tm_encoding))

int main(int argc, char *argv[]) {

    if(argc < 3) {
        cerr << " needs a binary and tape file for input" << endl;
    }

    tm_encoding IR;
    ifstream f(argv[1], ios::binary);
    ifstream t(argv[2]);
	tm_encoding ram[RAM_INSTRUCTIONS];
	memset(ram, 0, RAM_BYTES);
	f.read((char *)ram, RAM_BYTES);
	f.close();

    /* TODO: Implement tape data structure, then adapt print_tape */

    bool alpha[256];
    int PC = 0;
    bool equals_flag = false;
    int head_pos = 0;
    int head_moves = 0;
    int instruction_cnt = 0;
    int total_instruction_cnt = 0;
    int total_head_moves = 0;
    deque <cell> tape;
    int tape_num = 0;
    int total_tapes = 0;

    //grab lines of tape file to load into our tapes (deque)
    vector <string> tape_strings;
    string line;
    while(getline(t, line)){
        tape_strings.push_back(line);
        total_tapes++;
    }

    //grab first tape
    get_next_tape(tape, tape_num, total_tapes, tape_strings, head_pos);
    
    //process loop
    while(ram[PC].word.u != 0) {
        if(tape_num > total_tapes){
            break;
        }
        /*                  FETCH                       */

        //grab next instruction
        IR = ram[PC];

        /*             DECODE/EXECUTE                   */
        switch(IR.generic.opcode) {
            case TM_OPCODE_ALPHA:
                alpha[IR.alpha.letter] = true;
                break;
            case TM_OPCODE_CMP:
                //if we are not doing an OR
                if(!(bool)IR.cmp.oring) {
                    //set equals flag to false
                    equals_flag = false;
                }
                if((bool)IR.cmp.blank){
                    if(tape[head_pos].is_blank){
                        equals_flag = true;
                    }
                }
                else if(alpha[IR.cmp.letter]){
                    if (tape[head_pos].letter == (char)IR.cmp.letter) {
                        //set equals flag to true
                        equals_flag = true;
                    }
                    else {
                        //set equals flag to false
                        equals_flag = false;
                    }
                     
                }
                /*else{
                    //print tape failure (compare attempt not in alphabet)
                    cout << "Failed after " << head_moves << " moves and " << instruction_cnt << "instructions executed" << endl;
                    cerr << "CMP attempt failure: '" << IR.draw.letter << "' not in alphabet." << endl;
                    print_tape(tape, head_pos);

                    //reset local head moves and instruction counts to 0 for new tape
                    head_moves = 0;
                    instruction_cnt = 0;

                    //reset PC to beginning
                    PC = 0;

                    //increment to next tape
                    tape_num++;

                    //grab next tape
                    tape.clear();
                    for(size_t i = 0; i < tape_strings[tape_num].size(); i++){
                        tape.push_back({tape_strings[tape_num][i], false});
                    }
                }*/
                // cout << "cmp " << (char)IR.cmp.letter<< " == " << tape[head_pos].letter 
                // <<"? - " << (bool)(tape[head_pos].letter == (char)IR.cmp.letter) << "  Head Pos: " << head_pos;
                break;
            case TM_OPCODE_JMP:
                if(IR.jmp.eq && IR.jmp.ne){
                    PC = IR.jmp.addr-1;
                }
                else if(IR.jmp.eq){
                    if(equals_flag){
                        PC = IR.jmp.addr-1;
                    }
                }
                else if(IR.jmp.ne){
                    if(!equals_flag){
                        PC = IR.jmp.addr-1;
                    }
                }
                else{
                    //print nonsense since this should never happen
                    cout << "GONDOR CALLS FOR AID!!!";
                }
                break;
            case TM_OPCODE_DRAW:
                if(IR.draw.blank){
                    tape[head_pos].letter = 0;
                    tape[head_pos].is_blank = true;
                }
                else if(alpha[IR.draw.letter]){
                    tape[head_pos].letter = IR.draw.letter;
                    tape[head_pos].is_blank = false;
                }
                /*else{
                    //print tape failure (draw attempt not in alphabet)
                    cout << "Failed after " << head_moves << " moves and " << instruction_cnt << " instructions executed" << endl;
                    cerr << "Draw attempt failure: '" << IR.draw.letter << "' not in alphabet." << endl;
                    print_tape(tape, head_pos);

                    //reset local head moves and instruction counts to 0 for new tape
                    head_moves = 0;
                    instruction_cnt = 0;

                    //reset PC to beginning
                    PC = 0;

                    //increment to next tape
                    tape_num++;

                    //grab next tape
                    tape.clear();
                    for(size_t i = 0; i < tape_strings[tape_num].size(); i++){
                        tape.push_back({tape_strings[tape_num][i], false});
                    }
                }*/
                break;
            case TM_OPCODE_MOVE:
                head_pos += IR.move.amount;
                //check for stepping off end of tape in either direction and fill with blanks (is_blank set to true)
                if(head_pos < 0){
                    for(int i = 0; i > head_pos; i--){
                        tape.push_front({'\0', true});
                    }
                }
                if(head_pos > tape.size()){
                    for(int i = tape.size(); i < head_pos; i++){
                        tape.push_back({'\0', true});
                   }
                }
                //update head moves
                head_moves += std :: abs(IR.move.amount);
                total_head_moves += std :: abs(IR.move.amount);
                break;
            case TM_OPCODE_STOP:
                if(IR.stop.halt){
                    cout << "Halted ";
                }
                else{
                    cout << "Failed ";
                }
                cout << "after " << head_moves << " moves and " << instruction_cnt << " instructions executed" << endl;
                print_tape(tape, head_pos);

                //reset local head moves and instruction counts to 0 for new tape
                head_moves = 0;
                instruction_cnt = 0;

                //reset PC to beginning
                PC = 0;

                //increment to next tape
                tape_num++;

                //grab next tape
                get_next_tape(tape, tape_num, total_tapes, tape_strings, head_pos);
                break;
            default:
            break;
        }

        //increment total instruction count and local instruction count
        total_instruction_cnt++;
        instruction_cnt++;

        //increment PC
        PC++;
    }

    cout << "Totals across all tapes..." << endl;
    cout << "      moves : " << total_head_moves << endl;
    cout << "instructions: " << total_instruction_cnt << endl;

    return EXIT_SUCCESS;
}

void print_tape(deque <cell> tape, int head_pos){
    for(int i = 0; i < tape.size(); i++){
        if(tape[i].is_blank){
            cout << " ";
        }
        else{
            cout << tape[i].letter;
        }
    }
    cout << endl;
    for(int i = 0; i < head_pos; i++){
        cout << " ";
    }
    cout << "^" << endl;
}

void get_next_tape(deque <cell> &tape, int tape_num, int total_tapes, vector <string> tape_strings, int &head_pos){
    if(tape_num < total_tapes){
        tape.clear();
        head_pos = 0;
        for(int i = 0; i < tape_strings[tape_num].size(); i++){
            tape.push_back({tape_strings[tape_num][i], false});
        }
    }

}