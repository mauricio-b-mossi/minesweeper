#include "Board.hpp"
#include <algorithm>
#include <chrono>
#include <iostream>
#include <iterator>
#include <random>
#include <vector>

// ClickedCol and ClickedRow are zero indexed based on the handling.
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
  // int clickedIdx = (((clickedRow - 1) * mCols) + clickedCol) - 1;
  int clickedIdx = (clickedRow * mCols) + clickedCol;

  // bool backingBoard[mRows * mCols];
  //  Initializing board to false.
  for (int i = 0; i < (mRows * mCols); i++) {
    mBackingBoard[i] = false;
  }

  std::vector<int> shuffler;
  shuffler.reserve(mRows * mCols);

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
  mBoard->reserve(mRows * mCols);

  for (int idx = 0; idx < (mRows * mCols); idx++) {
    int positionAccumulator = 0;

    if (idx < mCols)
      positionAccumulator |= Position::TOP;
    if (idx > (mCols * mRows - 1) - mCols)
      positionAccumulator |= Position::BOTTOM;
    if (idx % mCols == mCols - 1)
      positionAccumulator |= Position::RIGHT;
    if (idx % mCols == 0)
      positionAccumulator |= Position::LEFT;

    // Flags set.
    if (positionAccumulator == (Position::TOP | Position::RIGHT)) {
      int neighborIndices[3] = {idx - 1, idx + mCols, idx + mCols - 1};

      PushCellsToBoard(idx, neighborIndices,
                       sizeof(neighborIndices) / sizeof(neighborIndices[0]));
    } else if (positionAccumulator == (Position::TOP | Position::LEFT)) {
      int neighborIndices[3] = {idx + 1, idx + mCols, idx + mCols + 1};

      PushCellsToBoard(idx, neighborIndices,
                       sizeof(neighborIndices) / sizeof(neighborIndices[0]));
    } else if (positionAccumulator == (Position::BOTTOM | Position::LEFT)) {
      int neighborIndices[3] = {idx + 1, idx - mCols, idx - mCols + 1};

      PushCellsToBoard(idx, neighborIndices,
                       sizeof(neighborIndices) / sizeof(neighborIndices[0]));
    } else if (positionAccumulator == (Position::BOTTOM | Position::RIGHT)) {
      int neighborIndices[3] = {idx - 1, idx - mCols, idx - mCols - 1};

      PushCellsToBoard(idx, neighborIndices,
                       sizeof(neighborIndices) / sizeof(neighborIndices[0]));

    } else if (positionAccumulator == (Position::BOTTOM)) {

      int neighborIndices[5] = {idx - 1, idx + 1, idx - mCols, idx - mCols + 1,
                                idx - mCols - 1};

      PushCellsToBoard(idx, neighborIndices,
                       sizeof(neighborIndices) / sizeof(neighborIndices[0]));

    } else if (positionAccumulator == (Position::TOP)) {
      int neighborIndices[5] = {idx - 1, idx + 1, idx + mCols, idx + mCols + 1,
                                idx + mCols - 1};

      PushCellsToBoard(idx, neighborIndices,
                       sizeof(neighborIndices) / sizeof(neighborIndices[0]));

    } else if (positionAccumulator == (Position::RIGHT)) {
      int neighborIndices[5] = {idx - 1, idx + mCols, idx + mCols - 1,
                                idx - mCols, idx - mCols - 1};

      PushCellsToBoard(idx, neighborIndices,
                       sizeof(neighborIndices) / sizeof(neighborIndices[0]));

    } else if (positionAccumulator == (Position::LEFT)) {
      int neighborIndices[5] = {idx + 1, idx + mCols, idx + mCols + 1,
                                idx - mCols, idx - mCols + 1};

      PushCellsToBoard(idx, neighborIndices,
                       sizeof(neighborIndices) / sizeof(neighborIndices[0]));

    } else {
      // Need to build neighborbombs and cells.
      int neighborIndices[8] = {idx + 1,         idx - 1,         idx + mCols,
                                idx + mCols + 1, idx + mCols - 1, idx - mCols,
                                idx - mCols + 1, idx - mCols - 1};

      PushCellsToBoard(idx, neighborIndices,
                       sizeof(neighborIndices) / sizeof(neighborIndices[0]));
    }
  }

  // Opening clicked mine.
  mBoard->at(clickedIdx).mIsOpen = true;
}

// Just fill all with default closed. Cannot fill mines. Just set default for
// init. Set condition to add squares if mBoard.size() == 0;
Board::Board(int cols, int rows, int mines)
    : mCols(cols), mRows(rows), mMines(mines),
      mBackingBoard(new bool[rows * cols]) {}

// Works for reset.
void Board::Init(int clickedCol, int clickedRow) {
  enum Position {
    TOP = (1 << 0),
    BOTTOM = (1 << 1),
    LEFT = (1 << 2),
    RIGHT = (1 << 3)
  };

  int clickedIdx = (clickedRow * mCols) + clickedCol;

  // bool backingBoard[mRows * mCols];
  //  Initializing board to false.
  for (int i = 0; i < (mRows * mCols); i++) {
    mBackingBoard[i] = false;
  }

  std::vector<int> shuffler;
  shuffler.reserve(mRows * mCols);

  // Populating shuffler.
  for (int i = 0; i < (mRows * mCols); i++) {
    shuffler.push_back(i);
  }

  // obtain a time-based seed:
  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  std::default_random_engine e(seed);

  std::shuffle(shuffler.begin(), shuffler.end(), e);

  int counter = 0;

  while (counter < mMines) {

    if (shuffler.back() != clickedIdx) {
      mBackingBoard[shuffler.back()] = true;
      shuffler.pop_back();
      counter++;
    } else {
      shuffler.pop_back();
    }
  }

  // Deleting vector.
  if (!mBoard->empty()) {
    mBoard->erase(mBoard->begin(), mBoard->end());
  }
  // Filled backingBoard, construct mBoard.
  mBoard->reserve(mRows * mCols);

  for (int idx = 0; idx < (mRows * mCols); idx++) {
    int positionAccumulator = 0;

    if (idx < mCols)
      positionAccumulator |= Position::TOP;
    if (idx > (mCols * mRows - 1) - mCols)
      positionAccumulator |= Position::BOTTOM;
    if (idx % mCols == mCols - 1)
      positionAccumulator |= Position::RIGHT;
    if (idx % mCols == 0)
      positionAccumulator |= Position::LEFT;

    // Flags set.
    if (positionAccumulator == (Position::TOP | Position::RIGHT)) {
      int neighborIndices[3] = {idx - 1, idx + mCols, idx + mCols - 1};

      PushCellsToBoard(idx, neighborIndices,
                       sizeof(neighborIndices) / sizeof(neighborIndices[0]));
    } else if (positionAccumulator == (Position::TOP | Position::LEFT)) {
      int neighborIndices[3] = {idx + 1, idx + mCols, idx + mCols + 1};

      PushCellsToBoard(idx, neighborIndices,
                       sizeof(neighborIndices) / sizeof(neighborIndices[0]));
    } else if (positionAccumulator == (Position::BOTTOM | Position::LEFT)) {
      int neighborIndices[3] = {idx + 1, idx - mCols, idx - mCols + 1};

      PushCellsToBoard(idx, neighborIndices,
                       sizeof(neighborIndices) / sizeof(neighborIndices[0]));
    } else if (positionAccumulator == (Position::BOTTOM | Position::RIGHT)) {
      int neighborIndices[3] = {idx - 1, idx - mCols, idx - mCols - 1};

      PushCellsToBoard(idx, neighborIndices,
                       sizeof(neighborIndices) / sizeof(neighborIndices[0]));

    } else if (positionAccumulator == (Position::BOTTOM)) {

      int neighborIndices[5] = {idx - 1, idx + 1, idx - mCols, idx - mCols + 1,
                                idx - mCols - 1};

      PushCellsToBoard(idx, neighborIndices,
                       sizeof(neighborIndices) / sizeof(neighborIndices[0]));

    } else if (positionAccumulator == (Position::TOP)) {
      int neighborIndices[5] = {idx - 1, idx + 1, idx + mCols, idx + mCols + 1,
                                idx + mCols - 1};

      PushCellsToBoard(idx, neighborIndices,
                       sizeof(neighborIndices) / sizeof(neighborIndices[0]));

    } else if (positionAccumulator == (Position::RIGHT)) {
      int neighborIndices[5] = {idx - 1, idx + mCols, idx + mCols - 1,
                                idx - mCols, idx - mCols - 1};

      PushCellsToBoard(idx, neighborIndices,
                       sizeof(neighborIndices) / sizeof(neighborIndices[0]));

    } else if (positionAccumulator == (Position::LEFT)) {
      int neighborIndices[5] = {idx + 1, idx + mCols, idx + mCols + 1,
                                idx - mCols, idx - mCols + 1};

      PushCellsToBoard(idx, neighborIndices,
                       sizeof(neighborIndices) / sizeof(neighborIndices[0]));

    } else {
      // Need to build neighborbombs and cells.
      int neighborIndices[8] = {idx + 1,         idx - 1,         idx + mCols,
                                idx + mCols + 1, idx + mCols - 1, idx - mCols,
                                idx - mCols + 1, idx - mCols - 1};

      PushCellsToBoard(idx, neighborIndices,
                       sizeof(neighborIndices) / sizeof(neighborIndices[0]));
    }
  }

  // Opening clicked mine.
  mBoard->at(clickedIdx).Open();

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
                         mFlaggedCells.get(), mBoard.get()));
}

void Board::Erase() {
  *mOpenedCells = 0;
  *mFlaggedCells = 0;
  std::cout << "Enter erase" << std::endl;
  mBoard->erase(mBoard->begin(), mBoard->end());
  std::cout << "Board erased. " << mBoard->size() << std::endl;
}

void Board::Open(int clickedCol, int clickedRow) {
  int clickedIdx = (clickedRow * mCols) + clickedCol;
  if (!mBoard->at(clickedIdx).mIsOpen) {
    if (!mBoard->at(clickedIdx).Open()) {
      mHasLost = true;
    }
  }
}

void Board::Open(int idx) {
  if (!mBoard->at(idx).mIsOpen) {
    if (!mBoard->at(idx).Open()) {
      mHasLost = true;
    }
  }
}

void Board::Flag(int clickedCol, int clickedRow) {
  int clickedIdx = (clickedRow * mCols) + clickedCol;
  mBoard->at(clickedIdx).Flag();
}

void Board::Flag(int idx) { mBoard->at(idx).Flag(); }
