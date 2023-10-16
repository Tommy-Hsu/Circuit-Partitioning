#ifndef BUCKETLIST_H
#define BUCKETLIST_H

#include <map>
#include <list>
#include <iostream>
#include <algorithm>
#include <unordered_map>
#include "Data.h"

class BucketList {
private:
    std::map<int, std::list<Cell*>> gainLists; // Mapping from gain to list of cells with that gain.
    std::unordered_map<Cell*, std::list<Cell*>::iterator> cellPosition; // Stores the iterator pointing to the cell's position in its gain list.

public:
    void insert(Cell* cell, int gain) {
        gainLists[gain].push_back(cell);
        cellPosition[cell] = std::prev(gainLists[gain].end());
    }

    void remove(Cell* cell, int gain) {
        gainLists[gain].erase(cellPosition[cell]);
        if (gainLists[gain].empty()) {
            gainLists.erase(gain);
        }
        cellPosition.erase(cell);
    }

    void updateGain(Cell* cell, int oldGain, int newGain) {
        remove(cell, oldGain);
        insert(cell, newGain);
    }

    Cell* getMaxGainCell() {
        // Assuming the map and lists are non-empty. Add appropriate checks if necessary.
        return gainLists.rbegin()->second.back();
    }

    Cell* getNextMaxGainCell(Cell* currentCell) {
        if (!currentCell) return nullptr;

        int currentGain = currentCell->GetGain();
        auto currentGainListIt = gainLists.find(currentGain);
        if (currentGainListIt == gainLists.end()) {
            // This shouldn't happen; it means currentCell has a gain not present in the map.
            return nullptr;
        }

        // Get iterator pointing to currentCell in the list.
        auto currentCellIt = cellPosition[currentCell];

        // Check if there's a next cell in the current gain list.
        auto nextCellIt = std::next(currentCellIt);
        if (nextCellIt != currentGainListIt->second.end()) {
            return *nextCellIt;
        }

        // Move to the next gain in the map.
        if (currentGainListIt != gainLists.begin()) {
            --currentGainListIt;
            return currentGainListIt->second.back();
        }

        // currentCell has the highest gain and is the last in its list.
        return nullptr;
    }

    void display() const {

        std::cout << " ============== G1 Bucket List ============== " << std::endl;
        for(auto& pair : gainLists) {
            std::cout << "Gain " << pair.first << ":";
            for(auto& cell : pair.second) {
                if(cell->GetIsInG1()){
                    std::cout << " Cell " << cell->GetCellID() << "->";
                }
            }
            std::cout << std::endl;
        }

        std::endl(std::cout);
        std::cout << " ============== G2 Bucket List ============== " << std::endl;
        for(auto& pair : gainLists) {
            std::cout << "Gain " << pair.first << ":";
            for(auto& cell : pair.second) {
                if(!cell->GetIsInG1()){
                    std::cout << " Cell " << cell->GetCellID() << " ->";
                }
            }
            std::cout << std::endl;
        }
        std::endl(std::cout);
    }
};

#endif