#ifndef FM_H
#define FM_H

#include <set>
#include <string>
#include <vector>
#include <unordered_set>
#include <sstream>
#include <fstream>
#include <iostream>
#include <iomanip>
#include "Data.h"
#include "BucketList.h"
#include "Debug.h"

struct One_move_result
{
    int cell_id_;          // cell character
    int gain_;        // g(i) value
    int partial_sum_gain_;   // Σ g(i) value
    int cutsize_;        // cutsize value
};


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

        // variable
        int                             g1_size, g2_size;
        int                             cut_size;
        BucketList                      bucket_list;
        std::vector<One_move_result*>   pass_result;

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
            for (const auto& result : pass_result) {
                delete result;
            }            
        };

        // functions
        Cell* getOrCreateCell(int c_id);
        int   F(Net* pNet, Cell* pCell);
        int   T(Net* pNet, Cell* pCell);
        bool  MeetAreaCons(Cell* pCell);
        void  UpdateCutSize(Cell* pCell);
        void  UpdateGain(Cell* pCell);
        void  MoveBack(int max_partial_sum_gain);
        
        // workflow
        void  Parse_input(std::ifstream& input);
        void  Initialization();
        int   Pass(int pass_num);

        // display
        void  Print_Result(std::ofstream& output);
        void  Get_Cell_and_Net_List();
        void  GetPassResult();
        void  GetCellptr();
};

#endif