#pragma once
#include <memory>
#include <vector>

typedef std::unique_ptr<std::vector<int>> NeighborCellsRef;

struct Cell {

  // Board and opened cells are just for recursion purposes.
  Cell(int idx, bool isMine, bool isOpen, int neighborBombs,
       std::vector<int> NeighborCells, int *openedCells, int *flaggedCells,
       std::vector<Cell> *board);
  // Usefull for drawing.
  //
  bool Open();
  void Flag();

  int *mOpenedCells;
  int *mFlaggedCells;
  std::vector<Cell> *mBoard;
  std::vector<int> mNeighborCellsIndices;
  int mNeighborBombs;
  int mIdx;
  bool mIsMine;
  bool mIsOpen;
  bool mIsMarked;
};
