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
#include "BucketList.h"

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
        int                             max_pin;
        int                             cut_size;
        BucketList                      bucket_list_g1, bucket_list_g2;

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
        Cell* getOrCreateCell(int c_id);
        int   getPinNum();
        int   F(Net* pNet, Cell* pCell);
        int   T(Net* pNet, Cell* pCell);
        
        // workflow
        void Parse_input(std::ifstream& input);
        void Initialization();
        void Pass();

        // display
        void Print_CutSize_Result();
        void GetNetList();
        void GetCellList();
};

#endif