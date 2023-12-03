#include "Board.hpp"
#include <algorithm>
#include <chrono>
#include <iostream>
#include <iterator>
#include <random>
#include <vector>

Board::Board(int cols, int rows, int mines, int clickedCol, int clickedRow)
    : mCols(cols), mRows(rows), mMines(mines),
      mBackingBoard(new bool[rows * cols]) {
  enum Position {
    TOP = (1 << 0),
    BOTTOM = (1 << 1),
    LEFT = (1 << 2),
    RIGHT = (1 << 3)
  };
  // -1 to get relative ot array.
  int clickedIdx = (((clickedRow - 1) * cols) + clickedCol) - 1;

  // bool backingBoard[rows * cols];
  //  Initializing board to false.
  for (int i = 0; i < (rows * cols); i++) {
    mBackingBoard[i] = false;
  }

  std::vector<int> shuffler;
  shuffler.reserve(rows * cols);

  // Populating shuffler.
  for (int i = 0; i < shuffler.size(); i++) {
    shuffler.push_back(i);
  }

  // obtain a time-based seed:
  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  std::default_random_engine e(seed);

  std::shuffle(shuffler.begin(), shuffler.end(), e);

  int counter = 0;

  while (counter < mines) {
    if (shuffler.back() != clickedIdx) {
      mBackingBoard[shuffler.back()] = true;
      shuffler.pop_back();
      counter++;
    } else
      shuffler.pop_back();
  }

  // Filled backingBoard, construct mBoard.
  mBoard->reserve(rows * cols);

  for (int idx = 0; idx < (rows * cols); idx++) {
    int positionAccumulator = 0;

    if (idx < cols)
      positionAccumulator |= Position::TOP;
    if (idx > (cols * rows - 1) - cols)
      positionAccumulator |= Position::BOTTOM;
    if (idx % cols == cols - 1)
      positionAccumulator |= Position::RIGHT;
    if (idx % cols == 0)
      positionAccumulator |= Position::LEFT;

    // Flags set.
    if (positionAccumulator == (Position::TOP | Position::RIGHT)) {
      int neighborIndices[3] = {idx - 1, idx + cols, idx + cols - 1};

      PushCellsToBoard(idx, neighborIndices,
                       sizeof(neighborIndices) / sizeof(neighborIndices[0]));
    } else if (positionAccumulator == (Position::TOP | Position::LEFT)) {
      int neighborIndices[3] = {idx + 1, idx + cols, idx + cols + 1};

      PushCellsToBoard(idx, neighborIndices,
                       sizeof(neighborIndices) / sizeof(neighborIndices[0]));
    } else if (positionAccumulator == (Position::BOTTOM | Position::LEFT)) {
      int neighborIndices[3] = {idx + 1, idx - cols, idx - cols + 1};

      PushCellsToBoard(idx, neighborIndices,
                       sizeof(neighborIndices) / sizeof(neighborIndices[0]));
    } else if (positionAccumulator == (Position::BOTTOM | Position::RIGHT)) {
      int neighborIndices[3] = {idx - 1, idx - cols, idx - cols - 1};

      PushCellsToBoard(idx, neighborIndices,
                       sizeof(neighborIndices) / sizeof(neighborIndices[0]));

    } else if (positionAccumulator == (Position::BOTTOM)) {

      int neighborIndices[5] = {idx - 1, idx + 1, idx - cols, idx - cols + 1,
                                idx - cols - 1};

      PushCellsToBoard(idx, neighborIndices,
                       sizeof(neighborIndices) / sizeof(neighborIndices[0]));

    } else if (positionAccumulator == (Position::TOP)) {
      int neighborIndices[5] = {idx - 1, idx + 1, idx + cols, idx + cols + 1,
                                idx + cols - 1};

      PushCellsToBoard(idx, neighborIndices,
                       sizeof(neighborIndices) / sizeof(neighborIndices[0]));

    } else if (positionAccumulator == (Position::RIGHT)) {
      int neighborIndices[5] = {idx - 1, idx + cols, idx + cols - 1, idx - cols,
                                idx - cols - 1};

      PushCellsToBoard(idx, neighborIndices,
                       sizeof(neighborIndices) / sizeof(neighborIndices[0]));

    } else if (positionAccumulator == (Position::LEFT)) {
      int neighborIndices[5] = {idx + 1, idx + cols, idx + cols + 1, idx - cols,
                                idx - cols + 1};

      PushCellsToBoard(idx, neighborIndices,
                       sizeof(neighborIndices) / sizeof(neighborIndices[0]));

    } else {
      // Need to build neighborbombs and cells.
      int neighborIndices[8] = {idx + 1,        idx - 1,        idx + cols,
                                idx + cols + 1, idx + cols - 1, idx - cols,
                                idx - cols + 1, idx - cols - 1};

      PushCellsToBoard(idx, neighborIndices,
                       sizeof(neighborIndices) / sizeof(neighborIndices[0]));
    }
  }

  // Opening clicked mine.
  mBoard->at(clickedIdx).mIsOpen = true;
}

void Board::PushCellsToBoard(int idx, int *neighborIndices,
                             int neighborsCount) {
  int neighborBombs = 0;
  std::vector<int> neighborIndicesVector(neighborIndices,
                                         neighborIndices + neighborsCount);

  for (auto idx : neighborIndicesVector) {
    if (mBackingBoard[idx] == true)
      neighborBombs++;
  }

  mBoard->push_back(Cell(idx, mBackingBoard[idx], false, neighborBombs,
                         neighborIndicesVector, mOpenedCells.get(),
                         mBoard.get()));
}
