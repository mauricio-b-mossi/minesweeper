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

  std::cout << "backingBoard built" << std::endl;

  std::vector<int> shuffler;
  shuffler.reserve(mRows * mCols);

  std::cout << "shuffler built" << std::endl;

  // Populating shuffler.
  for (int i = 0; i < (mRows * mCols); i++) {
    shuffler.push_back(i);
    std::cout << shuffler.back() << std::endl;
  }

  std::cout << "shuffler size: " << shuffler.size() << std::endl;
  std::cout << "shuffler filled" << std::endl;

  // obtain a time-based seed:
  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  std::default_random_engine e(seed);

  std::cout << "time seeded" << std::endl;

  std::shuffle(shuffler.begin(), shuffler.end(), e);

  std::cout << "shuffler shuffled" << std::endl;

  int counter = 0;

  while (counter < mMines) {

    std::cout << "counter: " << counter << std::endl;
    std::cout << "shuffler size: " << shuffler.size() << std::endl;
    std::cout << "shuffler first: " << shuffler.front() << std::endl;
    std::cout << "shuffler last: " << shuffler.back() << std::endl;

    if (shuffler.back() != clickedIdx) {
      std::cout << "filled mBacking with: " << shuffler.back() << std::endl;
      mBackingBoard[shuffler.back()] = true;
      shuffler.pop_back();
      counter++;
    } else {
      std::cout << "discarded: " << shuffler.back() << std::endl;
      shuffler.pop_back();
    }
  }

  std::cout << "mines built" << std::endl;

  // Filled backingBoard, construct mBoard.
  mBoard->reserve(mRows * mCols);

  std::cout << "mBoard reserve" << std::endl;

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
