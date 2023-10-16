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
        // immutable
        float                           balance_factor;
        float                           lower_bound, upper_bound;
        std::string                     line, word;
        std::stringstream               ss;
        std::unordered_map<int, Cell*>  cell_list;
        std::unordered_map<int, Net*>   net_list;
        int                             n_id, c_id;
        int                             n_number, c_number;
        int                             max_pin;

        // variable
        int                             g1_size, g2_size;
        int                             cut_size;
        BucketList                      bucket_list;

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
        bool  MeetAreaCons(Cell* pCell);
        void  UpdateCutSize(Cell* pCell);
        void  UpdateGain(Cell* pCell);
        
        // workflow
        void  Parse_input(std::ifstream& input);
        void  Initialization();
        int   Pass(int pass_num);

        // display
        void  Print_Result();
        void  GetNetList();
        void  GetCellList();
};

#endif