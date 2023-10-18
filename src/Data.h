#ifndef DATA_H
#define DATA_H

#include <unordered_map>

class Net;

class Cell;

class Cell
{
    private:
        int                           id_;
        bool                          is_locked_;
        int                           gain_;
        std::unordered_map<int, Net*> net_list_;
        bool                          is_in_g1_;
        Cell*                         next_cell_;
        Cell*                         prev_cell_;

    public:
        Cell(int id){ 
            id_ = id; 
            is_locked_ = false;
            gain_ = 0;
            next_cell_ = nullptr;
            prev_cell_ = nullptr;
        };
        ~Cell(){};

        int GetCellID()                            { return id_; };
        void InsertNetList(Net* pNet );
        int GetNetListSize()                       { return net_list_.size(); };
        Net* GetNet( int id )                      { return net_list_[id]; };
        void SetIsInG1(bool is_in_g1)              { is_in_g1_ = is_in_g1; };
        bool GetIsInG1()                           { return is_in_g1_; };
        void SetIsLocked(bool is_locked)           { is_locked_ = is_locked; };
        bool GetIsLocked()                         { return is_locked_; };
        void SetGain(int gain)                     { gain_ = gain; };
        int  GetGain()                             { return gain_; };
        std::unordered_map<int, Net*>& GetNetList() { return net_list_; };
        void SetNextCell(Cell* next_cell)          { next_cell_ = next_cell; };
        Cell* GetNextCell()                        { return next_cell_; };
        void SetPrevCell(Cell* prev_cell)          { prev_cell_ = prev_cell; };
        Cell* GetPrevCell()                        { return prev_cell_; };
};

class Net
{
    private:
        int                            id_;
        std::unordered_map<int, Cell*> cell_list_;
        int                            g1_num_, g2_num_;

    public:
        Net(int id){ 
            id_ = id; 
            g1_num_ = 0;
            g2_num_ = 0;
        };
        ~Net(){};

        int  GetNetID()                              { return id_; };   
        void InsertCellList(Cell* pCell );
        int GetCellListSize()                        { return cell_list_.size(); };
        Cell* GetCell( int id )                      { return cell_list_[id]; };
        std::unordered_map<int, Cell*>& GetCellList() { return cell_list_; };
        void SetG1Num(int g1_num)                    { g1_num_ = g1_num; };
        int GetG1Num()                               { return g1_num_; };
        void SetG2Num(int g2_num)                    { g2_num_ = g2_num; };
        int GetG2Num()                               { return g2_num_; };
};

struct pass_result
{
    int iteration;
    int cell_id;
    int gain;
    int cut_size;
    int partial_sum_gains;
};


inline void Cell::InsertNetList(Net* pNet) {
    net_list_[pNet->GetNetID()] = pNet;
}

inline void Net::InsertCellList(Cell* pCell) {
    cell_list_[pCell->GetCellID()] = pCell;
}

#endif