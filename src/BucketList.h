#ifndef BUCKETLIST_H
#define BUCKETLIST_H

#include <map>
#include <set>
#include <list>
#include <iostream>
#include <algorithm>
#include <unordered_map>
#include "Data.h"
#include "Debug.h"

class BucketList {
private:
    int maxGain = 0;
    std::set<int> gainSet; // Stores the set of all gains.
    std::unordered_map<int, std::list<Cell*>*> gainLists; // Mapping from gain to list of cells with that gain.
    std::unordered_map<Cell*, std::list<Cell*>::iterator> cellPosition; // Stores the iterator pointing to the cell's position in its gain list.

public:
    void insert(Cell* cell, int gain) {
        if (gainLists.find(gain) == gainLists.end()) {
            gainLists[gain] = new std::list<Cell*>;
            gainSet.insert(gain);
        }
        if(!gainLists[gain]->empty()){
            gainLists[gain]->back()->SetNextCell(cell);
            cell->SetPrevCell(gainLists[gain]->back());
        }
        gainLists[gain]->push_back(cell);
        cellPosition[cell] = std::prev(gainLists[gain]->end());
        if( gain > maxGain){
            maxGain = gain;
        }
    }

    void remove(Cell* cell, int gain) {
        auto it = cellPosition.find(cell);
        if (it != cellPosition.end()) {
            gainLists[gain]->erase(it->second);
            cellPosition.erase(it);
        }
        if (gainLists[gain]->empty()) {
            gainSet.erase(gain);
            maxGain = (!gainSet.empty())?(*gainSet.rbegin()):0;
            delete gainLists[gain];
            gainLists.erase(gain);
        }
        if(cell->GetPrevCell() != nullptr){
            cell->GetPrevCell()->SetNextCell(cell->GetNextCell());
        }
        if(cell->GetNextCell() != nullptr){
            cell->GetNextCell()->SetPrevCell(cell->GetPrevCell());
        }
        cell->SetNextCell(nullptr);
        cell->SetPrevCell(nullptr);
    }

    void updateGain(Cell* cell, int oldGain, int newGain) {
        remove(cell, oldGain);
        insert(cell, newGain);
    }

    Cell* getMaxGainCell() {
        // Assuming the map and lists are non-empty. Add appropriate checks if necessary.
        return gainLists[maxGain]->back();
    }

    Cell* getNextCell(Cell* currentCell) {
        
        if(currentCell->GetPrevCell() == nullptr){
            int currentGain = currentCell->GetGain();
            // to next gain
            while(gainSet.find(currentGain) != gainSet.begin()){
                currentGain--;
            }
            return gainLists[currentGain]->back();
        }
        return currentCell->GetPrevCell();
    }

    void display_detail() const {
        const int columnWidth = 30; // 调整列宽度以适应您的需求
        const int gainWidth = 8;   // 调整增益列宽度以适应您的需求

        DEBUG_COUT(std::left << std::setw(columnWidth) << "-------------" << std::left << std::setw(columnWidth) << "-------------" << std::endl);
        DEBUG_COUT(std::left << std::setw(columnWidth) << "G1 Bucket List" << std::left << std::setw(columnWidth) << "G2 Bucket List" << std::endl);
        DEBUG_COUT(std::left << std::setw(columnWidth) << "-------------" << std::left << std::setw(columnWidth) << "-------------" << std::endl);

        for(auto& pair : gainLists) {
            std::stringstream g1_stream, g2_stream;

            g1_stream << "Gain " << std::setw(gainWidth) << pair.first << ": ";
            g2_stream << "Gain " << std::setw(gainWidth) << pair.first << ": ";

            for(auto& cell : *(pair.second)) {
                if(cell->GetIsInG1()){
                    g1_stream << "Cell " << cell->GetCellID() << "-> ";
                } else {
                    g2_stream << "Cell " << cell->GetCellID() << "-> ";
                }
            }

            DEBUG_COUT(std::left << std::setw(columnWidth) << g1_stream.str() << std::left << std::setw(columnWidth) << g2_stream.str() << std::endl);
        }
        DEBUG_COUT(std::endl);
    }

    void display() const {
                const int columnWidth = 30; // 调整列宽度以适应您的需求
        const int gainWidth = 8;   // 调整增益列宽度以适应您的需求

        DEBUG_COUT(std::left << std::setw(columnWidth) << "-------------" << std::left << std::setw(columnWidth) << "-------------" << std::endl);
        DEBUG_COUT(std::left << std::setw(columnWidth) << "G1 Bucket List" << std::left << std::setw(columnWidth) << "G2 Bucket List" << std::endl);
        DEBUG_COUT(std::left << std::setw(columnWidth) << "-------------" << std::left << std::setw(columnWidth) << "-------------" << std::endl);

        for(auto& pair : gainLists) {
            std::stringstream g1_stream, g2_stream;

            g1_stream << "Gain " << std::setw(gainWidth) << pair.first << ": " << pair.second->size() << " cells";
            g2_stream << "Gain " << std::setw(gainWidth) << pair.first << ": " << pair.second->size() << " cells";

            DEBUG_COUT(std::left << std::setw(columnWidth) << g1_stream.str() << std::left << std::setw(columnWidth) << g2_stream.str() << std::endl);
        }
        DEBUG_COUT(std::endl);
    }
};


#endif