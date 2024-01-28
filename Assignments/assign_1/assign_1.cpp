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
    vector <vector<int>> trans;
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
    inFile >> start;
    cur = start;
    vector <bool> final(state_cnt);

    //initialize vector of final states to false
    for(int i = 0; i < state_cnt; i++)
    {
        final[i] = false;
        cout << final[i] << endl;
    }

    //set accept(final) states to true according to input file
    while(inFile.peek() != '\n')
    {
        inFile >> i_temp;
        cout << i_temp;
        final.at(i_temp) = true;
    }
    cout << endl << endl;
    for(int i = 0; i < state_cnt; i++)
    {
        final[i] = false;
        cout << final[i] << endl;
    }
    //initialize alphabet string
    while(inFile.peek() != '\n')
    {
        alphabet += inFile.get();
    }

    //initialize transition functions vector
    for(int i = 0; i < state_cnt + 1; i++)
    {
        for (int j = 0; j < alphabet.length(); j++)
        {
            if(i == 0)
            {
                trans[i].push_back({0}); 
            }
            trans[i].push_back({inFile.get()});
        }
        
    }


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
        
    }

    //now that data is gathered, test provided strings against DFA
    for(int i = 0; i < inputStrings.size(); i++)
    {
        for(int j = 0; j < inputStrings[i].length(); j++)
        {
            while (inputStrings[i] != "<empty>" && cur != 0)
            {
                cur = trans[cur][alphabet.find(inputStrings[i].at(j))];
            }
        }
        if (final[cur])
        {
            cout << "good  ";
        }
        else
        {
            cout << "bad   ";
        }
        cout << inputStrings[i];
    }

    return 0;
}