#ifndef FM_H
#define FM_H

#include <set>
#include <string>
#include <vector>
#include <unordered_set>
#include <sstream>
#include <fstream>
#include <iostream>
#include "Data.h"

class FiducciaMattheyses_Heuristic{
    private:
        double                          balance_factor;
        double                          lower_bound, upper_bound;
        std::string                     line, word;
        std::stringstream               ss;
        std::unordered_map<int, Cell*>  cell_list;
        std::unordered_map<int, Net*>   net_list;
        int                             n_id, c_id;
        int                             n_number, c_number;
        int                             g1_size, g2_size;

    public:
        // constructor and destructor
        FiducciaMattheyses_Heuristic() {};
        ~FiducciaMattheyses_Heuristic() {
            for (const auto& pair : net_list) {
                delete pair.second;
            }
            for (const auto& pair : cell_list) {
                delete pair.second;
            }
        };

        // functions
        int getCutSize();
        Cell* getOrCreateCell(int c_id);
        
        // main functions
        void Parse_input(std::ifstream& input);
        void Print_CutSize_Result();
        void Initialization();
        void Pass();
        void GetNetList();
        void GetCellList();
};

#endif