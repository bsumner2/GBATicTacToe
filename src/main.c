/* main.c
 * Written by Burton O Sumner
 * With Partial In-class, Lecture-Guidance from Dr. Hoskins in CSCE212
 * For Tic Tac Toe Extra Credit Assignment For CSCE212 taught by Dr. Hoskins
 */
#include <string.h>
#include "gba.h" //General convention = Using Angle brackets is for Stdlib headers, Using double quotes is for self-written headers.

/*            240
-------------------------
|                       |
|                       |
|                       |                    pixelIndex = (x + 240*y) = (240*row + column)
|                       |  160
|                       |
|                       |
|                       |
-------------------------
|       V-Blank         |   68
|                       |
-------------------------

*/

//CONSTANT
int startingSpeedMagnitudeZ = 1;
int playerSpeedConst = 2;





int main(void)
{
  //---------------------------------------------[INIT]--------------------------------------------------------------
#if 1

  while(1)
  {
    uint8 menu_selection = main_menu();
    if(menu_selection) return 0;
    else
    {
      game_loop();
    }
  }

#else
  game_loop();
#endif

  return 0;
}
