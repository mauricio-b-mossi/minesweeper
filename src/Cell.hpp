
#include <vector>

struct Cell {
  Cell();
  // Recursively searches for number of neighborsBombs.
  void FindNeighborBombs();
  // Return true is succedes, False if Bomb.
  bool Open();
  bool mIsMine;
  // Used with open to open neighbors if empty and also to find
  // number of neighbors.
  std::vector<int> neighbors;
  // -1 if is bomb else 1-8.
  int neighborBombs;
  // Necessary to fill neighbors.
  int idx;

  // Flags for drawing.
  bool mIsOpen;
  bool mIsMarked;
};
