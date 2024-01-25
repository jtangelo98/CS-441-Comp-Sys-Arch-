#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

int main(int argc, char* argv[])
{
    string alphabet;
    int state_cnt;
    int start;
    bool final[] = {false, false, false, false, false};
    int cur;
    char *s;

    //check to see if file was passed to program using command line
    if(argc < 2)
    {
        cerr << "Must provide filename when calling program";
    }
 
    //file has been passed, retrieve and open file
    const char* path = argv[1];
    ifstream inFile(path);

    //check if file could be opened
    if(!inFile.is_open())
    {
        cerr << "Could not open file";
    }

    //read content of dfa file into variables
    while(inFile.peek() != EOF)
    {
        state_cnt << inFile.get() + 1;
        start << inFile.get();
        while(inFile.peek() != '\n')
        {
            final[inFile.get()] = true;
        }
        while(inFile.peek() != '\n')
        {
           alphabet += inFile.get();
        }
        vector <vector<int>> trans;

        for(int i = 0; i < state_cnt + 1; i++)
        {
            for (int j = 0; j < alphabet.length(); j++)
            {
                trans[i][j] << inFile.get();
            }
            
        }
    }



    while (*s != '\0' && cur != 0)
    {
        cur = trans[cur][alphabet.find(*s)];
        s++;
    }

    if (final[cur])
    {
        cout << "\n\ngood\n\n";
    }
    else
    {
        cout << "\n\nbad\n\n";
    }

    return 0;
}