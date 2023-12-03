Going to have a cell class each with a `isMine` flag and a vector of `neighbours` containing 
the indices of the neighbours. Use recursion, when a tile is open and is 

### Making Board and Filling with mines.
The first spot must not be a mine. Therefore, the mines are placed after first move. Construct 
board with `arr[cols * rows]` since it is of fixed size. To determine position of mines fill 
vector with `0, 1 ... (cols * rows - 1)` elements, excluding the selected title, the use a 
combination of `std::shuffle`, `vector.back()`, and `vector.pop_back()` to get the position
of `n` mines. With these numbers fill `arr` with `true` is is mine.

`arr` is just a representation that will be used later to build up the UI.

### Filling up indexes of neighbours.
Only items living in borders will have less 8 neighbours, corners have 3 neighbour, and margins have 5.
To determine if item is on top or bottom `idx < cols || idx > (cols * rows - 1) - cols` and the item is
on the left or right if `idx % cols == 0 || idx % cols == (cols - 1)`. If item satisfies both conditions 
it is a corner, if it satisfies just one it is an edge, if it satisfies none it has 8 neighbours.

For 8 neighbours finding the indexes of neighbours is easy.
Just `idx + 1, idx + - 1, idx + (cols), idx + (cols + 1), idx + (cols - 1), idx + (-cols),
idx + (-cols + 1), idx + (-cols - 1)`

For top skip the `(-cols)`
For bottom skip the `(cols)`
For right skip the `(+1)`
For left skip the `(-1)`

There should be an algorithm I could build with these considerations. Could use bit flags and for loops.
In the for loop first check for `top` `bottom`, then in the inner loop check for `left` `right`

### Need a away to find neighbour bombs.
After construction query the array for each of the neighbours index and increment the number of neighbour
bombs. If the item itself is a bomb, set `neighbourBombs` to `-1`. Else fill in form `0-8`.

If a `0` neighbour bomb or empty is clicked, open all neighbour and if one of those neighbour also is 
`0` open it up.

> Important distinction I will have a game board `arr` and a `vector<Cell>` as game state. `bool[cols * rows] 
> arr` just indicates whether spot is bomb or non bomb. `vector<Cell>` it the actual game state, will be 
> used to render, open, etc.
