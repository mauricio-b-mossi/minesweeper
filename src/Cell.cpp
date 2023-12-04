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
  if (!mIsMarked) {
    if (mIsMine) {
      mIsOpen = true;
      return false;
    } else if (mIsOpen) {
      return true;
    } else if (mNeighborBombs == 0 && !mIsOpen) {
      mIsOpen = true;
      for (auto i : mNeighborCellsIndices) {
        if (!(*mBoard).at(i).mIsOpen && !(*mBoard).at(i).mIsMarked) {
          (*mBoard).at(i).Open();
        }
      }
    }

    mIsOpen = true;
    *mOpenedCells = *mOpenedCells + 1;
    return true;
  }
  return true;
};

void Cell::Flag() {
  if (!mIsOpen) {
    if (mIsMarked) {
      *mFlaggedCells = *mFlaggedCells - 1;
    } else {
      *mFlaggedCells = *mFlaggedCells + 1;
    }
    mIsMarked = !mIsMarked;
  }
}
