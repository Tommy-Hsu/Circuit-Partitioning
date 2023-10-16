#include "fm.h"

// functions

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

int FiducciaMattheyses_Heuristic::getPinNum(){

    int mp = 0;
    for(const auto& pair : cell_list){
        if((pair.second)->GetNetListSize() > mp){
            mp = (pair.second)->GetNetListSize();
        }
    }
    return mp;
}

int FiducciaMattheyses_Heuristic::F(Net* pNet, Cell* pCell){

    int fn = 0;
    /* 
        return number of cells on net n in the From Block
        i.e. find the number of cells on net n in the same group as cell c
    */
    if(pCell->GetIsInG1()){
        fn = pNet->GetG1Num();
    }
    else{
        fn = pNet->GetG2Num();
    }
    // for(const auto& pair : (pNet->GetCellList())){
    //     if((pair.second)->GetIsInG1() == pCell->GetIsInG1()){
    //         fn++;
    //     }
    // }
    return fn;
}

int FiducciaMattheyses_Heuristic::T(Net* pNet, Cell* pCell){

    int tn = 0;
    /* 
        return number of cells on net n in the To Block
        i.e. find the number of cells on net n in the opposite group as cell c
    */
    if(pCell->GetIsInG1()){
        tn = pNet->GetG2Num();
    }
    else{
        tn = pNet->GetG1Num();
    }
    // for(const auto& pair : (pNet->GetCellList())){
    //     if((pair.second)->GetIsInG1() != pCell->GetIsInG1()){
    //         tn++;
    //     }
    // }
    return tn;
}

bool FiducciaMattheyses_Heuristic::MeetAreaCons(Cell* pCell){

    int temp_g1_size = g1_size;
    
    // attempt to move the cell
    if(pCell->GetIsInG1()){
        temp_g1_size--;
    }
    else{
        temp_g1_size++;
    }

    if( temp_g1_size >= lower_bound && temp_g1_size <= upper_bound ){
        return true;
    }
    else{
        return false;
    }
}

void FiducciaMattheyses_Heuristic::UpdateCutSize(Cell* pCell){
    
    // move net's g1_num_ and g2_num_
    for(const auto& pair : pCell->GetNetList()){

        int before_g1_num = (pair.second)->GetG1Num();
        int before_g2_num = (pair.second)->GetG2Num();

        if( pCell->GetIsInG1() ){
            (pair.second)->SetG1Num( (pair.second)->GetG1Num() - 1 );
            (pair.second)->SetG2Num( (pair.second)->GetG2Num() + 1 );
        }
        else{
            (pair.second)->SetG1Num( (pair.second)->GetG1Num() + 1 );
            (pair.second)->SetG2Num( (pair.second)->GetG2Num() - 1 );
        }    

        int after_g1_num = (pair.second)->GetG1Num();
        int after_g2_num = (pair.second)->GetG2Num();

        // update cut size
        if( before_g1_num > 0 && before_g2_num > 0 && after_g1_num == 0 && after_g2_num > 0){
            cut_size--;
        }
        else if( before_g1_num > 0 && before_g2_num > 0 && after_g1_num > 0 && after_g2_num == 0){
            cut_size--;
        }
        else if( before_g1_num == 0 && before_g2_num > 0 && after_g1_num > 0 && after_g2_num > 0){
            cut_size++;
        }
        else if( before_g1_num > 0 && before_g2_num == 0 && after_g1_num > 0 && after_g2_num > 0){
            cut_size++;
        }
    }
}

void FiducciaMattheyses_Heuristic::UpdateGain(Cell* pCell){
    
    int tn, fn;
    for(const auto& pair : pCell->GetNetList()){

        tn = T(pair.second, pCell);
        fn = F(pair.second, pCell);
        
        if(tn == 0){ // increment gains of all free cells on net n
            for(const auto& pair2 : (pair.second)->GetCellList()){
                if( !(pair2.second)->GetIsLocked() ){
                    (pair2.second)->SetGain( (pair2.second)->GetGain() + 1 );
                    bucket_list.updateGain(pair2.second, (pair2.second)->GetGain() - 1, (pair2.second)->GetGain());
                }
            }
        }
        else if(tn == 1){ // decrement gain of the only T cell ( opposite group with pCell ) on n, if it is free
            for(const auto& pair2 : (pair.second)->GetCellList()){
                if( !(pair2.second)->GetIsLocked() && (pair2.second)->GetIsInG1() != pCell->GetIsInG1() ){
                    (pair2.second)->SetGain( (pair2.second)->GetGain() - 1 );
                    bucket_list.updateGain(pair2.second, (pair2.second)->GetGain() + 1, (pair2.second)->GetGain());
                }
            }
        }

        fn--;
        tn++;

        if(fn == 0){ // decrement gains of all free cells on n
            for(const auto& pair2 : (pair.second)->GetCellList()){
                if( !(pair2.second)->GetIsLocked() ){
                    (pair2.second)->SetGain( (pair2.second)->GetGain() - 1 );
                    bucket_list.updateGain(pair2.second, (pair2.second)->GetGain() + 1, (pair2.second)->GetGain());
                }
            }
        }
        else if(fn == 1){ // increment gain of the only F cell (same group with pCell ) on n, if it is free
            for(const auto& pair2 : (pair.second)->GetCellList()){
                if( !(pair2.second)->GetIsLocked() && (pair2.second)->GetIsInG1() == pCell->GetIsInG1() ){
                    (pair2.second)->SetGain( (pair2.second)->GetGain() + 1 );
                    bucket_list.updateGain(pair2.second, (pair2.second)->GetGain() - 1, (pair2.second)->GetGain());
                }
            }
        }
    }
}

// workflow
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

    lower_bound = float((float)(c_number * 0.5) - (int)(balance_factor * c_number * 0.5));
    std::cout << "lower_bound = " << lower_bound << std::endl;
    upper_bound = float((float)(c_number * 0.5) + (int)(balance_factor * c_number * 0.5));
    std::cout << "upper_bound = " << upper_bound << std::endl;
    g1_size = 0;
    g2_size = 0;
    cut_size = 0;
    max_pin = getPinNum();

    // put cells into G1 and G2
    for(int i = 1; i <= c_number; i++){
        if( i <= (c_number/2)){
            cell_list[i]->SetIsInG1(true);
            g1_size++;
        }
        else{
            cell_list[i]->SetIsInG1(false);
            g2_size++;
        }
    }

    /* initialize cut size
        -> initialize net's g1_num_ and g2_num_
        -> count cut size
    */
    for(const auto& pair : net_list){
        for(const auto& pair2 : (pair.second)->GetCellList()){
            if( (pair2.second)->GetIsInG1() ){
                (pair.second)->SetG1Num( (pair.second)->GetG1Num() + 1 );
            }
            else{
                (pair.second)->SetG2Num( (pair.second)->GetG2Num() + 1 );
            }
        }
    }

    for(const auto& pair : net_list){
        if( (pair.second)->GetG1Num() > 0 && (pair.second)->GetG2Num() > 0){
            cut_size++;
        }
    }

    // initialize gain
    for(int i = 1; i <= c_number; i++){
        for(const auto& pair : cell_list[i]->GetNetList()){
            if(F(pair.second, cell_list[i]) == 1){
                cell_list[i]->SetGain( cell_list[i]->GetGain() + 1);
            }
            if(T(pair.second, cell_list[i]) == 0){
                cell_list[i]->SetGain( cell_list[i]->GetGain() - 1);
            }
        }
    }

    // put cells into bucket list by group
    for(int i = 1; i <= c_number; i++){
        bucket_list.insert(cell_list[i], cell_list[i]->GetGain());
    }

    std::cout << " ============== Initialization ============== " << std::endl;
    bucket_list.display();
    std::cout << "G1 size = " << g1_size << ", G2 size = " << g2_size << std::endl;
    std::cout << "Cutsize = " << cut_size << std::endl;
    std::endl(std::cout);
}

int FiducciaMattheyses_Heuristic::Pass(int pass_cnt){

    int maximum_partial_sum_Gk = 0;
    int partial_sum_Gk = 0;
    Cell* pCell;

    std::cout << " ============== Pass " << pass_cnt << " ============== " << std::endl;

    for(int i = 1; i <= c_number; i++){

        /*
            Choose a vertex V such that gain is largest 
            and moving V will not violate the area constraint
            and V is not locked
        */
        pCell = bucket_list.getMaxGainCell();
        std::cout << "someome is chosen" << std::endl;
        std::cout << "Cell " << pCell->GetCellID() << " is chosened" << std::endl;
        while( !(MeetAreaCons(pCell)) ){
            pCell = bucket_list.getNextMaxGainCell(pCell);
            std::cout << "someome is chosen" << std::endl;
            std::cout << "Cell " << pCell->GetCellID() << " is chosened" << std::endl;
        }

        pCell->SetIsLocked(true);
        partial_sum_Gk += pCell->GetGain();
        if(partial_sum_Gk > maximum_partial_sum_Gk){
            maximum_partial_sum_Gk = partial_sum_Gk;
        }

        UpdateGain(pCell);
        UpdateCutSize(pCell);
        g1_size = g1_size - pCell->GetIsInG1() + !(pCell->GetIsInG1());
        g2_size = g2_size - !(pCell->GetIsInG1()) + pCell->GetIsInG1();
        pCell->SetIsInG1(!(pCell->GetIsInG1()));

        bucket_list.remove(pCell, pCell->GetGain());

        // display
        std::cout << " ============== iteration " << i << " ============== " << std::endl;
        bucket_list.display();
        std::cout << "G1 size = " << g1_size << ", G2 size = " << g2_size << std::endl;
        std::cout << "Cell " << pCell->GetCellID() << " is moved, " << "Gain = " << pCell->GetGain() << std::endl;
        std::cout << "partial_sum_Gk = " << partial_sum_Gk << std::endl;
        std::cout << "UpdateCutSize to " << cut_size <<  std::endl;

        std::endl(std::cout);
    }

    return maximum_partial_sum_Gk;
}

// display
void FiducciaMattheyses_Heuristic::Print_Result(){

    std::cout << "Cutsize = " << cut_size << std::endl;
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

    // temporary output
    std::cout << "max_pin = " << max_pin << std::endl;

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
