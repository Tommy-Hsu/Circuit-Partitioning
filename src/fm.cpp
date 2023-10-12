#include "fm.h"

void FiducciaMattheyses_Heuristic::Parse_input(std::ifstream& input){

    Net* pNet;
    n_number = 0;
    c_number = 0;
    input >> balance_factor;

    while(getline(input, line, ';'))
    {
        ss.str(line);
        while (ss >> word) {
            if(word[0] == 'n'){
                n_id = stoi(word.substr(1));
                pNet = new Net( n_id );
                net_list[n_id] = pNet;
                n_number++;
            }
            else if(word[0] == 'c'){
                c_id = stoi(word.substr(1));
                Cell* pCell = getOrCreateCell(c_id);
                pNet->InsertCellList(pCell);
                pCell->InsertNetList(pNet);
            }
        }
        ss.clear();
    }
}

void FiducciaMattheyses_Heuristic::Initialization(){

    lower_bound = (1 - balance_factor)/2 * c_number;
    upper_bound = (1 + balance_factor)/2 * c_number;
    g1_size = 0;
    g2_size = 0;

    for(int i = 1; i <= c_number; i++){
        if( i < (c_number/2)){
            cell_list[i]->SetIsInG1(true);
            g1_size++;
        }
        else{
            cell_list[i]->SetIsInG1(false);
            g2_size++;
        }
    }
}

void FiducciaMattheyses_Heuristic::GetNetList(){

    for (const auto& pair : net_list) {
        std::cout << "Net: " << pair.first << ", Cells: " << (pair.second)->GetCellListSize() << std::endl;
    }
}

void FiducciaMattheyses_Heuristic::GetCellList(){

    for (const auto& pair : cell_list) {
        std::cout << "Cell: " << pair.first << ", Nets: " << (pair.second)->GetNetListSize() << std::endl;
    }
}

Cell* FiducciaMattheyses_Heuristic::getOrCreateCell(int c_id) {

    auto it = cell_list.find(c_id);
    
    // If c_id is found in the map, return the corresponding Cell pointer.
    if (it != cell_list.end()) {
        return it->second;
    }
    
    // If c_id is not found, create a new Cell, store it in the map, and return it.
    Cell* newCell = new Cell(c_id);
    cell_list[c_id] = newCell;
    c_number++;
    return newCell;
}

void FiducciaMattheyses_Heuristic::Pass(){
}

int FiducciaMattheyses_Heuristic::getCutSize(){
    
    int cut_size = 0;
    for(const auto& pair : net_list){
        for(const auto& pair2 : (pair.second)->GetCellList()){
            if((pair2.second)->GetIsInG1() != (*((pair.second)->GetCellList()).begin()).second->GetIsInG1()){
                cut_size++;
                break;
            }
        }
    }
    return cut_size;
}

void FiducciaMattheyses_Heuristic::Print_CutSize_Result(){

    std::cout << "Cutsize = " << getCutSize() << std::endl;
    std::cout << "G1 " << g1_size << std::endl;
    for(int i = 1; i <= c_number; i++){
        if(cell_list[i]->GetIsInG1()){
            std::cout << "c" << i << " ";
        }
    }
    std::cout << ";" << std::endl;
    std::cout << "G2 " << g2_size << std::endl;
    for(int i = 1; i <= c_number; i++){
        if(!cell_list[i]->GetIsInG1()){
            std::cout << "c" << i << " ";
        }
    }
    std::cout << ";" << std::endl;
}