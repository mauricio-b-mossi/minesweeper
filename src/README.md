Name: Mauricio Mossi
Section: 12119
UFL email: m.mossi@ufl.edu
System: Windows
Compiler: (MinGW GCC-6.3.0-1) 6.3.0
SFML Version: 2.4.2
IDE: Vim.

Other notes: If you want to run the standard program just execute it without flags.
The standard program is what is described on the Document provided.
- `-d`: Stands for debug. Enables clicking ANY key in the keyboard to open all non-mine tiles
and win the game.
- `-c`: Stands for custom. Added it just for fun, includes more icons. ***Do not run as 
it uses other assests.***
- `-cd`: You guessed it! Runs the program both in `Debug` and `Custom` mode.

I did not use the suggested window approach. I used a `StateMachine` to change the 
`State` (Screen) being displayed on the current window. In otherwords, this program
just contains ***1*** window. The `StateMachine` is based on the stack data structure
as I `push` and `pop` as a response to events on the screen. 

I documented all the development of the project on [GitHub](https://github.com/mauricio-b-mossi/minesweeper).
