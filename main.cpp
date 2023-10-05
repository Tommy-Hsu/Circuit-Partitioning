#include<iostream>
#include<string>
#include<fstream>
#include<vector>
#include<sstream>
#include <algorithm>
using namespace std;

int main(int argc, char* argv[]){

    ifstream            input(argv[1]);
    ofstream            output(argv[2]);
    double              balance_factor;
    string              line, word;
    stringstream        ss;
    vector<vector<int>> nc_table;
    vector<vector<int>> cn_table;
    vector<int>         nc_row, empty_row;
    int                 n_number, c_number;

    if(!input.is_open()){
        cerr << "Error opening file" << endl;
    }
    else{

        input >> balance_factor;

        while(getline(input, line, ';'))
        {
            ss.str(line);
            while (ss >> word) {
                if(word[0] == 'n'){
                    n_number = stoi(word.substr(1));
                }
                else if(word[0] == 'c'){
                    c_number = stoi(word.substr(1));
                    nc_row.push_back(c_number);

                    while(cn_table.size() < c_number){
                        cn_table.push_back(empty_row);
                    }

                    cn_table[c_number-1].push_back(n_number);
                }
            }
            nc_table.push_back(nc_row);
            nc_row.clear();
            ss.clear();
        }

        for(size_t i = 0; i < nc_table.size(); i++){
            for(size_t j = 0; j < nc_table[i].size(); j++){
                cout << nc_table[i][j] << " ";
            }
            cout << endl;
        }
        cout << endl;
        for(size_t i = 0; i < cn_table.size(); i++){
            for(size_t j = 0; j < cn_table[i].size(); j++){
                cout << cn_table[i][j] << " ";
            }
            cout << endl;
        }
        
        input.close();
        output.close();
    }

    return 0;   
}