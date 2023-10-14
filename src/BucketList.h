#ifndef BUCKETLIST_H
#define BUCKETLIST_H

#include <map>
#include <list>
#include <iostream>
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

    void display() const {
        std::cout << "Bucket List: " << std::endl;

        for (auto it = gainLists.rbegin(); it != gainLists.rend(); ++it) {
            std::cout << "Gain: " << it->first << " -> ";
            for (Cell* cell : it->second) {
                std::cout << "Cell " << cell->GetCellID() << " -> ";
            }
            std::cout << std::endl;
        }
    }
};

#endif