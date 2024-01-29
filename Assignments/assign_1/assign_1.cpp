#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

int main(int argc, char* argv[])
{
    string alphabet;
    int state_cnt = 0;
    int start;
    int cur;
    int i_temp = 0;
    string temp = "";
    vector <string> inputStrings;

    //check to see if file was passed to program using command line
    if(argc < 2)
    {
        cerr << "Must provide filename when calling program";
    }
 
    //file has been passed, retrieve and open file
    const char* fileName = argv[1];
    ifstream inFile(fileName);

    //check if file could be opened
    if(!inFile.is_open())
    {
        cerr << "Could not open file";
    }

    //read content of dfa file into variables
    inFile >> state_cnt;
    state_cnt += 1;
    inFile >> start;
    cur = start;
    vector <bool> final(state_cnt);

    //initialize vector of final states to false
    for(int i = 0; i < state_cnt; i++)
    {
        final[i] = false;
    }

    //grab new line char and move to accept state line, also set '0' state to false as a fail state
    inFile.get();
    final[0] = false;

    //set accept(final) states to true according to input file
    while(inFile.peek() != '\n')
    {
        inFile >> i_temp;
        final.at(i_temp) = true;
    }

    inFile.get();
    //initialize alphabet string
    while(inFile.peek() != '\n')
    {
        alphabet += inFile.get();
    }

    inFile.get();

    vector <vector<int>> trans((state_cnt + 1), vector<int> (alphabet.length()));
    //initialize transition functions vector
    for(int i = 0; i < state_cnt; i++)
    {
        for (int j = 0; j < alphabet.length(); j++)
        {
            if(i == 0)
            {
                trans[i][j] = 0; 
            }
            else
            {
                inFile >> i_temp;
                trans[i][j] = i_temp;
            }
        }
        
    }

    inFile.get();

    //get strings to test against DFA
    while(inFile.peek() != EOF)
    {
        //see if empty line
        if(inFile.peek() == '\n')
        {
            inputStrings.push_back({"<empty>"});
            inFile.get();
        }
        else
        {
            inFile >> temp;
            inputStrings.push_back(temp);
        }
        inFile.get();
        
    }

    //now that data is gathered, test provided strings against DFA
    for(int i = 0; i < inputStrings.size(); i++)
    {
        cur = start;
        for(int j = 0; j < inputStrings[i].length(); j++)
        {
            if (inputStrings[i] != "<empty>" && cur != 0)
            {
                cur = trans[cur][alphabet.find(inputStrings[i].at(j))];
            }
        }
        if (final[cur + 1])
        {
            cout << "good  ";
        }
        else
        {
            cout << "bad   ";
        }
        cout << inputStrings[i] << endl;
    }

    return 0;
}