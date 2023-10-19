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

void FiducciaMattheyses_Heuristic::MoveBack(int max_partial_sum_gain){

    // unlock all cells
    for(int i = 1; i <= c_number; i++){
        cell_list[i]->SetIsLocked(false);
        cell_list[i]->SetGain(0);
    }

    // find the best iterator and move back some cells
    for(int i = c_number; i >= 1; i--){
        if(pass_result[i-1]->partial_sum_gain_ == max_partial_sum_gain){
            break;
        }
        else{
            Cell* pCell = cell_list[pass_result[i-1]->cell_id_];
            UpdateCutSize(pCell);
            g1_size = g1_size - pCell->GetIsInG1() + !(pCell->GetIsInG1());
            g2_size = g2_size - !(pCell->GetIsInG1()) + pCell->GetIsInG1();
            pCell->SetIsInG1(!(pCell->GetIsInG1()));
        }
    }

    // reconstruct bucket list and update gain
    for(int i = 1; i <= c_number; i++){
        for(const auto& pair : cell_list[i]->GetNetList()){
            if(F(pair.second, cell_list[i]) == 1){
                cell_list[i]->SetGain( cell_list[i]->GetGain() + 1);
            }
            if(T(pair.second, cell_list[i]) == 0){
                cell_list[i]->SetGain( cell_list[i]->GetGain() - 1);
            }
        }
        bucket_list.insert(cell_list[i], cell_list[i]->GetGain());
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

    DEBUG_COUT(std::endl);
    lower_bound = float((float)(c_number * 0.5) - (int)(balance_factor * c_number * 0.5));
    DEBUG_COUT("lower_bound = " << lower_bound);
    upper_bound = float((float)(c_number * 0.5) + (int)(balance_factor * c_number * 0.5));
    DEBUG_COUT(", upper_bound = " << upper_bound << std::endl);
    g1_size = 0;
    g2_size = 0;
    cut_size = 0;

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
                 
    DEBUG_COUT(" ************** Initialization ************** " << std::endl);
    DEBUG_COUT("G1 size = " << g1_size << ", G2 size = " << g2_size << std::endl);
    DEBUG_COUT("Cutsize = " << cut_size << std::endl);
    // bucket_list.display();
    // GetCellptr();
}

int FiducciaMattheyses_Heuristic::Pass(int pass_cnt){

    int maximum_partial_sum_Gk = 0;
    int partial_sum_Gk = 0;
    Cell* pCell;
                   
    DEBUG_COUT(" -------------- Pass " << pass_cnt << " -------------- " << std::endl;)

    for(int i = 1; i <= c_number; i++){

        One_move_result* result = new One_move_result();
        /*
            Choose a vertex V such that gain is largest 
            and moving V will not violate the area constraint
            and V is not locked
        */
        pCell = bucket_list.getMaxGainCell();
        // DEBUG_COUT (" ++++++++++++++ iteration " << i << " ++++++++++++++ " << std::endl);
        // DEBUG_COUT ("someome is chosen" << std::endl);
        // DEBUG_COUT ("Cell " <<。 pCell->GetCellID() << " is chosened" << std::endl);
        // DEBUG_COUT ("Gain = " << pCell->GetGain() << std::endl);
        while( !(MeetAreaCons(pCell)) ){
            pCell = bucket_list.getNextCell(pCell);
            // DEBUG_COUT("someome is chosen" << std::endl;)
            // DEBUG_COUT("Cell " << pCell->GetCellID() << " is chosened" << std::endl;)
            // DEBUG_COUT("Gain = " << pCell->GetGain() << std::endl;)
        }

        pCell->SetIsLocked(true);
        partial_sum_Gk += pCell->GetGain();
        // DEBUG_COUT("partial_sum_Gk = " << partial_sum_Gk << std::endl);
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
        // bucket_list.display_detail();
        // GetCellptr();
        result->cell_id_ = pCell->GetCellID();
        result->cutsize_ = cut_size;
        result->gain_ = pCell->GetGain();
        result->partial_sum_gain_ = partial_sum_Gk;
        pass_result.push_back(result);
    }
    // GetPassResult();

    // move back
    MoveBack(maximum_partial_sum_Gk);

    for(One_move_result* result : pass_result){
        delete result;
    }
    pass_result.clear();
    return maximum_partial_sum_Gk;
}

// display

void FiducciaMattheyses_Heuristic::Print_Result(std::ofstream& output) {
    output << "Cutsize = " << cut_size << std::endl;
    output << "G1 " << g1_size << std::endl;
    for (int i = 1; i <= c_number; i++) {
        if (cell_list[i]->GetIsInG1()) {
            output << "c" << i << " ";
        }
    }
    output << ";" << std::endl;
    output << "G2 " << g2_size << std::endl;
    for (int i = 1; i <= c_number; i++) {
        if (!cell_list[i]->GetIsInG1()) {
            output << "c" << i << " ";
        }
    }
    output << ";" << std::endl;

    // 如果您还想在终端上看到输出，请使用 DEBUG_COUT
    DEBUG_COUT("Cutsize = " << cut_size << std::endl);
    DEBUG_COUT("G1 " << g1_size << std::endl);
    for (int i = 1; i <= c_number; i++) {
        if (cell_list[i]->GetIsInG1()) {
            DEBUG_COUT("c" << i << " ");
        }
    }
    DEBUG_COUT(";" << std::endl);
    DEBUG_COUT("G2 " << g2_size << std::endl);
    for (int i = 1; i <= c_number; i++) {
        if (!cell_list[i]->GetIsInG1()) {
            DEBUG_COUT("c" << i << " ");
        }
    }
    DEBUG_COUT(";" << std::endl);
}

void FiducciaMattheyses_Heuristic::Get_Cell_and_Net_List(){

    for (const auto& pair : net_list) {
        DEBUG_COUT("Net " << pair.first << " :");
        for (const auto& pair2 : (pair.second)->GetCellList()) {
            DEBUG_COUT(" Cell " << (pair2.second)->GetCellID() << " -> ");
        }
        DEBUG_COUT(std::endl);
    }
    DEBUG_COUT(std::endl);
    for (const auto& pair : cell_list) {
        DEBUG_COUT("Cell " << pair.first << " :");
        for (const auto& pair2 : (pair.second)->GetNetList()) {
            DEBUG_COUT(" Net " << (pair2.second)->GetNetID() << " -> ");
        }
        DEBUG_COUT(std::endl);
    }
}

void FiducciaMattheyses_Heuristic::GetPassResult(){
    
    DEBUG_COUT("|" << std::setw(5) << " i" 
            << "|" << std::setw(10) << " cell" 
            << "|" << std::setw(10) << " g(i)" 
            << "|" << std::setw(11) << " Σg(i)" 
            << "|" << std::setw(11) << " cutsize |" << std::endl);
    DEBUG_COUT("|" << std::setw(5) << "----"
            << "|" << std::setw(10) << "----------" 
            << "|" << std::setw(10) << "----------" 
            << "|" << std::setw(10) << "----------" 
            << "|" << std::setw(10) << "----------|" << std::endl);

    for(int i = 1; i <= c_number; i++){
        DEBUG_COUT("|" << std::setw(5) << i 
                << "|" << std::setw(10) << pass_result[i-1]->cell_id_ 
                << "|" << std::setw(10) << pass_result[i-1]->gain_ 
                << "|" << std::setw(10) << pass_result[i-1]->partial_sum_gain_ 
                << "|" << std::setw(10) << pass_result[i-1]->cutsize_ << "|" << std::endl);
    }
    DEBUG_COUT(std::endl);
}

void FiducciaMattheyses_Heuristic::GetCellptr(){

    for(int i = 1; i <= c_number; i++){
        Cell* p = cell_list[i]->GetPrevCell();
        int o = (p!=nullptr)?p->GetCellID():0;
        Cell* n = cell_list[i]->GetNextCell();
        int m = (n!=nullptr)?n->GetCellID():0;
        DEBUG_COUT(o<< " <- *"<< cell_list[i]->GetCellID() << " -> " << m << std::endl);
    }
    DEBUG_COUT(std::endl);
}