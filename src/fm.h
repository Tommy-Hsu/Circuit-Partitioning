#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>

class FiducciaMattheyses_Heuristic{
    private:
        double                        balance_factor;
        double                        lower_bound, upper_bound;
        std::string                   line, word;
        std::stringstream             ss;
        std::vector<std::vector<int>> nc_table;
        std::vector<std::vector<int>> cn_table;
        std::vector<int>              nc_row, empty_row;
        int                           n_number, c_number;

    public:
        // constructor and destructor
        FiducciaMattheyses_Heuristic();
        ~FiducciaMattheyses_Heuristic();
        
        void Parse_input(std::ifstream& input);
        void Print_NC_Table();
        void Print_CN_Table();
};

