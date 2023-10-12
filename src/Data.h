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
    public:
        Cell(int id){ id_ = id; };
        ~Cell(){};

        int GetCellID() { return id_; };
        void InsertNetList(Net* pNet );
        int GetNetListSize() { return net_list_.size(); };
        Net* GetNet( int id ){ return net_list_[id]; };
        void SetIsInG1(bool is_in_g1) { is_in_g1_ = is_in_g1; };
        bool GetIsInG1() { return is_in_g1_; };
};

class Net
{
    private:
        int                            id_;
        std::unordered_map<int, Cell*> cell_list_;

    public:
        Net(int id){ id_ = id; };
        ~Net(){};

        int  GetNetID() { return id_; };   
        void InsertCellList(Cell* pCell );
        int GetCellListSize() { return cell_list_.size(); };
        Cell* GetCell( int id ){ return cell_list_[id]; };
        std::unordered_map<int, Cell*> GetCellList() { return cell_list_; };
};

inline void Cell::InsertNetList(Net* pNet) {
    net_list_[pNet->GetNetID()] = pNet;
}

inline void Net::InsertCellList(Cell* pCell) {
    cell_list_[pCell->GetCellID()] = pCell;
}

#endif