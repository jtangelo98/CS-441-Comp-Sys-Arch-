#include <iostream>
#include <fstream>
#include <math.h>
#include <vector>
#include <chrono>
using namespace std;
using namespace std :: chrono;

typedef struct {
    bool dirty = false;
    unsigned int LRU = 0;
    uint64_t tag;
} block;

int main(int argc, char* argv[]){
    if (argc < 5) {
        cerr << "Error: input should be the following ... <executable> <address_file> <sets_pow> <bps_pow> <wpb_pow>" << endl;
        return 1;
    }
    ifstream f(argv[1]);

    //variable list
    int sets_pow = stoi(argv[2]);
    int sets = 1 << sets_pow;
    int bps_pow = stoi(argv[3]);
    int bps = 1 << bps_pow;
    int wpb_pow = stoi(argv[4]);
    int wpb = 1 << wpb_pow;
    int total_accesses= 0;
    int num_hits = 0;
    int set_num;
    uint64_t tag;
    uint64_t addr;
    vector<vector<block>> cache(sets, vector<block>(bps));

    //begin clock timer (not required for assignment, just personal curiosity)
    auto start = high_resolution_clock::now();

    //process loop
    while(f >> hex >> addr){
        addr >>= wpb_pow; 
        set_num = addr & (~ (~ 0U << sets_pow));
        tag = addr >> sets_pow;

        bool hit = false;
        for(int i = 0; i < bps; i++){
            if(cache[set_num][i].tag == tag){
                hit = true;
                num_hits++;
                cache[set_num][i].LRU = 0;
            }
            else{
                    cache[set_num][i].LRU++;
            }
        }
        if (!hit) {
            int lru_index = 0;
            for (int i = 1; i < bps; ++i) {
                if (cache[set_num][i].LRU > cache[set_num][lru_index].LRU || cache[set_num][i].dirty == false) {
                    lru_index = i;
                }
            }
            cache[set_num][lru_index].tag = tag;
            cache[set_num][lru_index].LRU = 0;
            cache[set_num][lru_index].dirty = true;
        }
        total_accesses++;
    }
    //stop timer and print hit results. Uncomment last print statement to display execution time
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);
    cout << "hits: " << num_hits << "/" << total_accesses;
    //cout << endl << "Time Elapsed: " << ((double)duration.count()/1000) << " seconds" << endl;
    return 0;
}