#include "Cell.hpp"
#include <iostream>
#include <vector>

Cell::Cell(int idx, bool isMine, bool isOpen, int neighborBombs,
           std::vector<int> neighborCellsIndices, int *openedCells,
           int *flaggedCells, std::vector<Cell> *board)
    : mIdx(idx), mIsMine(isMine), mIsOpen(isOpen), mIsMarked(false),
      mNeighborBombs(neighborBombs),
      mNeighborCellsIndices(neighborCellsIndices), mOpenedCells(openedCells),
      mFlaggedCells(flaggedCells), mBoard(board) {}

bool Cell::Open() {
  if (mIsMine) {
    return false;
  } else if (mIsOpen) {
    return true;
  } else if (mNeighborBombs == 0 && !mIsOpen) {
    mIsOpen = true;
    for (auto i : mNeighborCellsIndices) {
      if (!(*mBoard).at(i).mIsOpen) {
        (*mBoard).at(i).Open();
      }
    }
  }

  mIsOpen = true;
  *mOpenedCells = *mOpenedCells + 1;
  return true;
};

void Cell::Flag() {
  if (mIsMarked) {
    *mFlaggedCells = *mFlaggedCells - 1;
  } else {
    *mFlaggedCells = *mFlaggedCells + 1;
  }
}
