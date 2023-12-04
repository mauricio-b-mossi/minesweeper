#include "Cell.hpp"
#include <iostream>
#include <vector>

Cell::Cell(int idx, bool isMine, bool isOpen, int neighborBombs,
           std::vector<int> neighborCellsIndices, int *openedCells,
           std::vector<Cell> *board)
    : mIdx(idx), mIsMine(isMine), mIsOpen(isOpen), mIsMarked(false),
      mNeighborBombs(neighborBombs), mNeighborCellsIndices(neighborCellsIndices),
      mOpenedCells(openedCells), mBoard(board) {}
