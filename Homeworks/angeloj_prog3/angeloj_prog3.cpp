#include <iostream>
#include <fstream>
#include <cstring>

using namespace std;

int main(int argc, char* argv[]){
    int PE = 0b01;
    bool pred = PE & (1<<1);
    ifstream f (argv[1]);
    string line;
    uint64_t times_correct = 0;
    uint64_t num_branches = 0;
    for(int i = 0; i < 3; i++){
        if (i == 0){
            while(getline(f, line)){
                
                for(int x = 0; x < line.length(); x++){
                if(line[x] == '.'){
                    times_correct++;
                }
                num_branches++;
                }
            }   
            cout << "Fixed-F: " << times_correct << "/" << num_branches << endl;
        }
        f.clear();
        f.seekg(0);
        times_correct = 0;
        num_branches = 0;
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
        f.clear();
        f.seekg(0);
        times_correct = 0;
        num_branches = 0;
        if(i == 2){
            while(getline(f, line)){
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
            cout << "Bimodal: " << times_correct << "/" << num_branches;
        }

        f.clear();
        f.seekg(0);
        times_correct = 0;
        num_branches = 0;
        if(i == 3){

        }
        }
    return EXIT_SUCCESS;
  
}  