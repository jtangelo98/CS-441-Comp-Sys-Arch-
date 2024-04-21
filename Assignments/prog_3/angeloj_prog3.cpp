#include <iostream>
#include <fstream>
#include <string>

using namespace std;

//function prototypes
void reset(uint64_t &times_correct, uint64_t &num_branches, istream &f);
void two_layer_reset(int *two_layer_PE, int &history);

int main(int argc, char* argv[]){
    ifstream f (argv[1]);
    string line;
    uint64_t times_correct = 0;
    uint64_t num_branches = 0;
    int PE = 0b01;
    int two_layer_PE [4] = {0b01,0b11,0b01,0b11};
    int history = 0b00;

    for(int i = 0; i < 4; i++){
        if (i == 0){
            while(getline(f, line)){
                
                for(int x = 0; x < line.length(); x++){
                if(line[x] == '.'){
                    times_correct++;
                }
                num_branches++;
                }
            }   
            cout << endl << endl << "Fixed-F: " << times_correct << "/" << num_branches << endl;
        }
        reset(times_correct, num_branches, f);
        if (i == 1){
            char pred_char = '\0';
            while(getline(f, line)){
                for(int x = 0; x < line.length(); x++){
                    if(x == 0){
                        pred_char = line[x];
                        if(line[0] == '.'){
                            times_correct++;
                        }
                    }
                    else if(line[x] == pred_char){
                        times_correct++;
                    }
                    num_branches++;
                    }
            }   
            cout << "StaticF: " << times_correct << "/" << num_branches << endl;
        }
        reset(times_correct, num_branches, f);
        if(i == 2){
            while(getline(f, line)){
                PE = 0b01;
                for(int x = 0; x < line.length(); x++){
                    if((PE & (1<<1))){
                        if(line[x] == '@'){
                            PE &= ~(1<<0);
                            times_correct++;
                        }
                        else{
                            PE++;
                        }
                    }
                    else{
                        if(line[x] == '.'){
                            PE &= ~(1<<0);
                            times_correct++;
                        }
                        else{
                            PE++;
                        }
                    }
                    num_branches++;
                    }
            }   
            cout << "Bimodal: " << times_correct << "/" << num_branches << endl;
        }
        reset(times_correct, num_branches, f);       
        if(i == 3){
            while(getline(f, line)){
                two_layer_reset(two_layer_PE, history);
                for(int x = 0; x < line.length(); x++){
                    if(two_layer_PE[history] & (1<<1)){
                        if(line[x] == '@'){
                            two_layer_PE[history] &= ~(1<<0);
                            times_correct++;
                        }
                        else{
                            two_layer_PE[history]++;
                        }
                    }
                    else{
                        if(line[x] == '.'){
                            two_layer_PE[history] &= ~(1<<0);
                            times_correct++;
                        }
                        else{
                            two_layer_PE[history]++;
                        }
                    }
                    //update history and num_branches
                    history <<= 1;
                    if(line[x] == '@'){
                        history |= 1<<0;
                    }
                    history &= (1<<1)|(1<<0);
                    num_branches++;
                }   
            }
            cout << "2-Layer: " << times_correct << "/" << num_branches << endl << endl;
        }
    }
    return EXIT_SUCCESS;
}
void reset(uint64_t &times_correct, uint64_t &num_branches, istream &f){
    f.clear();
    f.seekg(0);
    times_correct = 0;
    num_branches = 0;
}

void two_layer_reset(int *two_layer_PE, int &history){
    //reset values for two-layer
    two_layer_PE[0] = 0b01;
    two_layer_PE[1] = 0b11;
    two_layer_PE[2] = 0b01;
    two_layer_PE[3] = 0b11;
    history = 0b00;
}