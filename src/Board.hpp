#include "Cell.hpp"
#include <memory>
#include <vector>

struct Board {
  std::unique_ptr<int> mOpenedCells;
  std::unique_ptr<std::vector<Cell>> mBoard;
  std::unique_ptr<bool[]> mBackingBoard;

  // Based on this construct fucken board. Pass openedCellsPtr to all.
  // Need clickedCol and clickedRow since from there the board is built.
  Board(int cols, int rows, int mines, int clickedCol, int clickedRow);

  int mFlaggedCells;
  int mCols;
  int mRows;
  int mMines;
  // Open: return true if opened, false if bomb, if neighbors
  bool Open(int idx);
  bool Open(int col, int row);
  void Flag(int idx);
  void Flag(int col, int row);

private:
  void PushCellsToBoard(int idx, int* neighborIndices, int neightbors);
};
