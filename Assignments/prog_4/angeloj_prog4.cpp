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
        cerr << "Error: input should be the following ... <executable> <address_file> <sets_pow> <bps_pow> <wpb_pow>" << endl;
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
    uint64_t tag;
    int total_accesses= 0;
    int num_hits = 0;
    int largest_lru = 0;
    string line;
    uint64_t addr;
    vector <block> cache(sets * bps);
    cout << cache.size(); return 0;

    while(f >> hex >> addr){
        addr >>= wpb_pow; 
        set_num = addr & (~ (~ 0U << sets_pow));
        tag = addr >> sets_pow;
        
        //multiply set number by blocks per set to get index into the cache
        set_num = (set_num * bps);
        int index_of_LRU = set_num;
        largest_lru = cache[set_num].LRU;
        for(int i = 0; i < bps; i++){
            cache[set_num + i].LRU++;
            if(!cache[set_num + i].dirty){
                cache[set_num + i].tag = tag;
                cache[set_num + i].dirty = true;
                cache[(set_num + i)].LRU = 0;
                break;
            }
            else{
                if(tag == cache[set_num + i].tag){
                    num_hits++;
                    cache[(set_num + i)].LRU = 0;
                    break;
                }
            }
            if(largest_lru < cache[set_num + i].LRU){
                largest_lru = cache[set_num + i].LRU;
                index_of_LRU = set_num + i;
            }
            if(i == (bps - 1)){
                cache[index_of_LRU].tag = tag;
                cache[index_of_LRU].LRU = 0;
            }
        }

        total_accesses++;
    }
    cout << "hits: " << num_hits << "/" << total_accesses;
    return 0;
}