#include<iomanip>
#include<iostream>
#include<fstream>
#include"src/fm.h"
using namespace std;

int main(int argc, char* argv[]){

    ifstream    input(argv[1]);
    ofstream    output(argv[2]);
    int         pass_cnt = 1;
    int         G = 567;

    if(!input.is_open()){
        cerr << "Error opening file" << endl;
        exit(1);
    }

    FiducciaMattheyses_Heuristic fm;
    clock_t start, end;
    start = clock();

    fm.Parse_input(input);
    std::cout << "Parsing input done." << std::endl;
    // fm.Get_Cell_and_Net_List();
    fm.Initialization();
    std::cout << "Initialization done." << std::endl;
    while(G){
        G = fm.Pass(pass_cnt);
        std::cout << "Pass " << pass_cnt++ << " done." << std::endl;
    }
    fm.Print_Result(output);
    std::cout << "Print result done." << std::endl;

    end = clock();
    cout << "CPU time: " << fixed << setprecision(5) 
    << (double)(end - start) / CLOCKS_PER_SEC << " sec" << endl;

    return 0;   
}