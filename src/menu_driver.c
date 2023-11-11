// Written by Burton O Sumner
#include<string.h>
#include "gba.h"
#include "menu_bg.h"
#include "menu_buttons.h"

/** 0 = play button selected. 1 = quit button selected*/
static uint8 sel = 0;

static void draw_menu_item(uint32 x, uint32 y, uint32 width, uint32 height, bmp bitMap)
{
  uint32 h = 0;
  for(uint32 j = 0; j<height;j++)
    for(uint32 i = 0; i<width;i+=5)
    {
      SCREENBUFFER[SCREEN_WIDTH*(j+y) + (i+x)] =   bitMap[h];
      SCREENBUFFER[SCREEN_WIDTH*(j+y) + (i+x+1)] = bitMap[h+1];
      SCREENBUFFER[SCREEN_WIDTH*(j+y) + (i+x+2)] = bitMap[h+2];
      SCREENBUFFER[SCREEN_WIDTH*(j+y) + (i+x+3)] = bitMap[h+3];
      SCREENBUFFER[SCREEN_WIDTH*(j+y) + (i+x+4)] = bitMap[h+4];
      h+=5;
    }
}

static inline void draw_play_btn(bmp button)
{
  draw_menu_item((SCREEN_WIDTH/2)-(MENU_BUTTON_WIDTH/2), (SCREEN_HEIGHT/4)-(MENU_BUTTON_HEIGHT/2), MENU_BUTTON_WIDTH, MENU_BUTTON_HEIGHT, button);
}

static inline void draw_quit_btn(bmp button)
{
  draw_menu_item((SCREEN_WIDTH/2)-(MENU_BUTTON_WIDTH/2), ((SCREEN_HEIGHT*3)/4)-(MENU_BUTTON_HEIGHT/2), MENU_BUTTON_WIDTH, MENU_BUTTON_HEIGHT, button);
}

static void draw_background_and_buttons(void)
{
  uint32 *dest = (uint32*)SCREENBUFFER, *src = (uint32*)menu_bg_bitmap;
  memcpy(dest, src, MENU_BG_LEN);
  Rect cursor;
  init_rect(&cursor, 20, SCREEN_HEIGHT, 12, 12);
  draw_rect(cursor, 0x1495);
  while(cursor.y>0)
  {
    vsync();

    cursor.y -= 1;
    cursor.x += 1;
    draw_rect(cursor, 0x1498);
  }
  /** 0 = play selected, 1 = quit selected. */
  draw_play_btn(play_button_selected);
  draw_quit_btn(quit_button);

  


}

uint8 main_menu(void)
{
  set_video_mode3_sound_channel1();
  uint8 up_down = 0;
  uint8 down_down = 0;

  draw_background_and_buttons();

  while(1)
  {
    vsync();

    if(key_pressed(_A) || key_pressed(_START))
    {
      play_sqr_wave_sound(A_NOTE, 3, 1);
      while(key_pressed(_A) || key_pressed(_START));
      return sel;
    }

    if(key_pressed(_UP) && !up_down)
    {
      up_down = 1;
      sel = !sel;
    }
    else if(!key_pressed(_UP))
    {
      up_down = 0;
    }
    
    if(key_pressed(_DOWN) && !down_down)
    {
      down_down = 1;
      sel = !sel;
    }
    else if(!key_pressed(_DOWN))
    {
      down_down = 0;
    }

    
    if(sel)
    {
      draw_play_btn(play_button);
      draw_quit_btn(quit_button_selected);
    }
    else
    {
      draw_play_btn(play_button_selected);
      draw_quit_btn(quit_button);
    }

    
  }
}
