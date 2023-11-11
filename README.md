# Tic Tac Toe for the GBA

## To do:

- (bsumner): Cleaning up my old C code to adhere to my degree of standards nowadays
- (arshiaessl): Try to get the GBA development SDK up and running. We can meet up, too,
  if you want some help setting it up! Also install mgba to play the built gba rom.

## Notes:

I moved TikTacToe.cc to ./src and adjusted your makefile to accomodate this change!
Since there are two makefiles now, I just made a shell script, ./run_make.sh \<target\>,
, that takes in a param for which makefile to run.
``` shell
# Script command opt to build and run GBA Executable:
./run_make.sh gba_driver

# Script command opt to build and run the Minimax algorithm-based GBA TUI Executable:
./run_make.sh cpu_driver
```


