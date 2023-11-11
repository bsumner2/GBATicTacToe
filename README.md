# Tic Tac Toe for the GBA

## To do:

- (bsumner): Cleaning up my old C code to adhere to my degree of standards nowadays (In Progress)
- (bsumner): Work on porting TikTakToe.cc to work with the GBA Code
- (arshiaessl): Try to get the GBA development SDK up and running. We can meet up, too,
  if you want some help setting it up! Also install mgba to play the built gba rom.

## Notes:

I moved TikTacToe.cc to ./src and added the TikTakToe.cc build target into
the GBA makefile, its target name is minimax_tic_tac_toe

``` shell
# Make target to build & run the Minimax TicTacToe cpu player TUI driver:
make minimax_tic_tac_toe

# Make target to build & run the GBA Tic Tac Toe ROM:
make gba
```


