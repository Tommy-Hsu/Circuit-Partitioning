#include "fm.h"

FiducciaMattheyses_Heuristic::FiducciaMattheyses_Heuristic(){
    
}

FiducciaMattheyses_Heuristic::~FiducciaMattheyses_Heuristic(){

}

void FiducciaMattheyses_Heuristic::Parse_input(std::ifstream& input){

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

    lower_bound = (1 - balance_factor)/2 * cn_table.size();
    upper_bound = (1 + balance_factor)/2 * cn_table.size();
}

void FiducciaMattheyses_Heuristic::Print_NC_Table(){

    for(size_t i = 0; i < nc_table.size(); i++){
        for(size_t j = 0; j < nc_table[i].size(); j++){
            std::cout << nc_table[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

void FiducciaMattheyses_Heuristic::Print_CN_Table(){

    for(size_t i = 0; i < cn_table.size(); i++){
        for(size_t j = 0; j < cn_table[i].size(); j++){
            std::cout << cn_table[i][j] << " ";
        }
        std::cout << std::endl;
    }
}