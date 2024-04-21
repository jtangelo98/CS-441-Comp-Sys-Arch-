#include <iostream>
#include <fstream>
#include <math.h>
#include <vector>

using namespace std;

typedef struct {
    bool dirty = false;
    unsigned int LRU = 0;
    uint64_t tag;

} block;

int main(int argc, char* argv[]){
    if (argc < 5) {
        cerr << "Error: input should be the following ... <address_file> <sets_pow> <bps_pow> <wpb_pow>" << endl;
        return 1;
    }

    //open address file
    ifstream f(argv[1]);

    

    //cache parameters (set count, blocks per set, number of adresses per block.)
    int sets_pow = stoi(argv[2]);
    int sets = 1 << sets_pow;
    int bps_pow = stoi(argv[3]);
    int bps = 1 << bps_pow;
    int wpb_pow = stoi(argv[4]);
    int wpb = 1 << wpb_pow;
    int set_num;
    int tag;
    int total_accesses= 0;
    int num_hits = 0;
    int largest_lru = 0;
    string line;
    uint64_t addr;
    vector <block> cache(sets * bps);
// if(f.is_open()){
//     getline(f, line);
//         cout << line;
//         return 1;
//     }
    while(f.peek()!= EOF){
        f >> hex >> addr;
        addr >>= wpb_pow;
        set_num = addr & (~ (~ 0U << sets_pow));
        tag = addr >> sets_pow;

        for(int i = 0; i < bps; i++)
        {
            
            if(!cache[(set_num * bps) + i].dirty){
                cache[(set_num * bps) + i].LRU ++;
                if(tag == cache[(set_num * bps) + i].tag){
                    num_hits++;
                    cache[(set_num * bps) + i].LRU = 0;
                    break;
                }
            }
            
            if(i == (bps - 1)){
                for(int j = 0; j < bps - 1; j++){
                    if(cache[(set_num * bps) + (j + 1)].LRU > cache[(set_num * bps) + j].LRU){
                        largest_lru = (set_num * bps) + (j + 1);
                    }
                    else{
                         largest_lru = (set_num * bps) + j;
                    }
                }
                cache[largest_lru].tag = tag;
            }
            total_accesses++;
        }

    }

    cout << "hits: " << num_hits << "/" << total_accesses;;

    return 0;
}