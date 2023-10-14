#include<iostream>
#include<fstream>
#include"src/fm.h"
using namespace std;

int main(int argc, char* argv[]){

    ifstream    input(argv[1]);
    ofstream    output(argv[2]);

    if(!input.is_open()){
        cerr << "Error opening file" << endl;
        exit(1);
    }

    FiducciaMattheyses_Heuristic fm;
    fm.Parse_input(input);
    // fm.GetNetList();
    // fm.GetCellList();
    fm.Initialization();
    // fm.Print_CutSize_Result();

    return 0;   
}