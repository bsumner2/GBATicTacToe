//Written by Burton O Sumner With help from Tonc Lib
#include <string.h>
#include "gba.h"
#include "game_tiles.h"
OBJ_ATTRIBUTE obj_buffer[128];
OBJ_AFFINE *obj_affine_buffer = (OBJ_AFFINE*) obj_buffer;
static const uint8 BOARD_TID = 32;
static const uint8 PLACEHOLDER_TID = 48;
static const uint8 X_TID = 0;
static const uint8 O_TID = 16;
static uint8 obj_count = 0;


static inline void set_sprite(OBJ_ATTRIBUTE *target_OBJ, uint8 sprite_ID)
{
  target_OBJ->attribute2 = ATTRIBUTE2_BUILD(sprite_ID, 0, 0b10&0b11);
}

static inline uint8 get_sprite(OBJ_ATTRIBUTE source_OBJ)
{
  return source_OBJ.attribute2&0x3FF;
}

static inline void play_move_sound(void)
{
  play_sqr_wave_sound(A_NOTE, 1, 31);
}

static inline void play_end_turn_sound(void)
{
  play_sqr_wave_sound(G_NOTE, 0, 15);
}

static inline void play_win_animation_noise(void)
{
  play_sqr_wave_sound(FS_NOTE, 0, 31);
}

static void game_init(void)
{

  OBJ_ATTRIBUTE *top_left, *top_center, *top_right;
  OBJ_ATTRIBUTE *middle_left, *middle_center, *middle_right;
  OBJ_ATTRIBUTE *bottom_left, *bottom_center, *bottom_right;
  obj_count = 0;

  set_regs(OBJ_1D_ARRAY_MODE, OBJ_MODE, 1, SOUND_DMG_SQUARE_1);
  init_sound1_regs(SOUND_SQUARE_ENVELOPE_BUILD(12, 0, 7), SOUND_SQUARE_DUTY1_2ND);
  memcpy(&tile_mem[4][0], ttt_tilesTiles, ttt_tilesTilesLen);
  memcpy(PALETTE_OBJ_BUFFER, ttt_tilesPal, ttt_tilesPalLen);

  oam_init(obj_buffer, 128);

  top_left     = &obj_buffer[0];
  obj_set_attribute(top_left, 0, 0x8000, (((0&0b1111)<<12) | ((0b10&0b11)<<10) |(PLACEHOLDER_TID&0x3FF)));
  obj_set_pos(top_left, 10, 0);
  obj_count++;

  top_center     = &obj_buffer[1];
  obj_set_attribute(top_center, 0, 0x8000, (((0&0b1111)<<12) | ((0b10&0b11)<<10) |(PLACEHOLDER_TID&0x3FF)));
  obj_set_pos(top_center, 94, 0);
  obj_count++;

  top_right    = &obj_buffer[2];
  obj_set_attribute(top_right, 0, 0x8000, (((0&0b1111)<<12) | ((0b10&0b11)<<10) |(PLACEHOLDER_TID&0x3FF)));
  obj_set_pos(top_right, 178, 0);
  obj_count++;


  middle_left    = &obj_buffer[3];
  obj_set_attribute(middle_left, 0, 0x8000, (((0&0b1111)<<12) | ((0b10&0b11)<<10) |(PLACEHOLDER_TID&0x3FF)));
  obj_set_pos(middle_left, 10, 64);
  obj_count++;

  middle_center  = &obj_buffer[4];
  obj_set_attribute(middle_center, 0, 0x8000, (((0&0b1111)<<12) | ((0b10&0b11)<<10) |(PLACEHOLDER_TID&0x3FF)));
  obj_set_pos(middle_center, 94, 64);
  obj_count++;

  middle_right   = &obj_buffer[5];
  obj_set_attribute(middle_right, 0, 0x8000, (((0&0b1111)<<12) | ((0b10&0b11)<<10) |(PLACEHOLDER_TID&0x3FF)));
  obj_set_pos(middle_right, 178, 64);
  obj_count++;


  bottom_left    = &obj_buffer[6];
  obj_set_attribute(bottom_left, 0, 0x8000, (((0&0b1111)<<12) | ((0b10&0b11)<<10) |(PLACEHOLDER_TID&0x3FF)));
  obj_set_pos(bottom_left, 10, 128);
  obj_count++;

  bottom_center  = &obj_buffer[7];
  obj_set_attribute(bottom_center, 0, 0x8000, (((0&0b1111)<<12) | ((0b10&0b11)<<10) |(PLACEHOLDER_TID&0x3FF)));
  obj_set_pos(bottom_center, 94, 128);
  obj_count++;

  bottom_right   = &obj_buffer[8];
  obj_set_attribute(bottom_right, 0, 0x8000, (((0&0b1111)<<12) | ((0b10&0b11)<<10) |(PLACEHOLDER_TID&0x3FF)));
  obj_set_pos(bottom_right, 178, 128);
  obj_count++;

  OBJ_ATTRIBUTE *BOARD_PARTS = &obj_buffer[9];
  int board_index;
  for(board_index = 0; board_index<7;board_index++)
  {
    obj_set_attribute(&BOARD_PARTS[board_index], 0, 0x8000, (((0&0b1111)<<12) | ((0b10&0b11)<<10) |(BOARD_TID&0x3FF)));
    obj_set_pos(&BOARD_PARTS[board_index], board_index*32, 32);
    obj_count++;
  }
  obj_set_attribute(&BOARD_PARTS[7], 0, 0x8000, (((0&0b1111)<<12) | ((0b10&0b11)<<10) |(BOARD_TID&0x3FF)));
  obj_set_pos(&BOARD_PARTS[7], SCREEN_WIDTH-32, 32);
  obj_count++;
  
  // SET BOARD TOP HORIZONTAL LINE
  for(board_index = 0; board_index<7; board_index++)
  {
    obj_set_attribute(&BOARD_PARTS[board_index+8], 0, 0x8000, (((0&0b1111)<<12) | ((0b10&0b11)<<10) |(BOARD_TID&0x3FF)));
    obj_set_pos(&BOARD_PARTS[board_index+8], board_index*32, 96);
    obj_count++;
  }
  obj_set_attribute(&BOARD_PARTS[15], 0, 0x8000, (((0&0b1111)<<12) | ((0b10&0b11)<<10) |(BOARD_TID&0x3FF)));
  obj_set_pos(&BOARD_PARTS[15], SCREEN_WIDTH-32, 96);
  obj_count++;
  for(int line = 0; line < 2; line++)
  for(board_index = 0; board_index<3; board_index++)
  {
    obj_set_attribute(&BOARD_PARTS[board_index+(16 + (3*line))], 0, 0x8000, (((0&0b1111)<<12) | ((0b10&0b11)<<10) |(BOARD_TID&0x3FF)));
    obj_set_pos(&BOARD_PARTS[board_index+(16 + (3*line))], (line)?136 : 52, board_index*64);
    obj_count++;
  }

  oam_copy(oam_mem, obj_buffer, obj_count);
  
}

static void player_1s_turn(void)
{
  uint8 cursor_prev = 0, cursor_curr = 0;
  while(get_sprite(obj_buffer[cursor_curr])!=PLACEHOLDER_TID)
  {
    cursor_curr++;
    cursor_prev++;
  }
  set_sprite(&obj_buffer[cursor_curr], X_TID);
  uint8 up_down = 0, down_down = 0, left_down = 0, right_down = 0;
  oam_copy(oam_mem, obj_buffer, 8);
  
  while(1)
  {
    vsync();
    if(key_pressed(_A))
    {
      play_end_turn_sound();
      oam_copy(oam_mem, obj_buffer, 9);
      break;
    }
    
    if(key_pressed(_UP) && !up_down)
    {
      up_down = 1;
      cursor_prev = cursor_curr;
      do 
      {
        cursor_curr = (cursor_curr>2)? cursor_curr-3 : 6+cursor_curr;
        play_move_sound();
      }
      while(get_sprite(obj_buffer[cursor_curr])!=PLACEHOLDER_TID && cursor_curr!=cursor_prev);
      if(cursor_curr==cursor_prev) while(get_sprite(obj_buffer[cursor_curr])!=PLACEHOLDER_TID) cursor_curr++;
      goto move_occurred;
      
    }
    else if(!key_pressed(_UP))
    {
      up_down = 0;
    }

    if(key_pressed(_DOWN) && !down_down)
    {
      down_down = 1;
      cursor_prev = cursor_curr;
      do 
      {
        cursor_curr = (cursor_curr<6) ? cursor_curr+3 : cursor_curr - 6;
        play_move_sound();
      }
      while(get_sprite(obj_buffer[cursor_curr])!=PLACEHOLDER_TID && cursor_curr!=cursor_prev);
      if(cursor_curr==cursor_prev) while(get_sprite(obj_buffer[cursor_curr])!=PLACEHOLDER_TID) cursor_curr--;
      goto move_occurred;
    } 
    else if(!key_pressed(_DOWN))
    {
      down_down = 0;
    }

    if(key_pressed(_LEFT) && !left_down)
    {
      left_down = 1;
      cursor_prev = cursor_curr;
      do 
      {
        cursor_curr = (cursor_curr%3) ? cursor_curr - 1: cursor_curr + 2;
        play_move_sound();
      }
      while(get_sprite(obj_buffer[cursor_curr])!=PLACEHOLDER_TID && cursor_curr!=cursor_prev);
      if(cursor_curr==cursor_prev) while(get_sprite(obj_buffer[cursor_curr])!=PLACEHOLDER_TID) cursor_curr--;
      goto move_occurred;
    } 
    else if(!key_pressed(_LEFT))
    {
      left_down = 0;
    }

    if(key_pressed(_RIGHT) && !right_down)
    {
      right_down = 1;
      cursor_prev = cursor_curr;
      do 
      {
        cursor_curr = ((cursor_curr+1)%3) ? cursor_curr + 1: cursor_curr - 2;
        play_move_sound();
      }
      while(get_sprite(obj_buffer[cursor_curr])!=PLACEHOLDER_TID && cursor_curr!=cursor_prev);
      if(cursor_curr==cursor_prev) while(get_sprite(obj_buffer[cursor_curr])!=PLACEHOLDER_TID) cursor_curr++;
      goto move_occurred;
    } 
    else if(!key_pressed(_RIGHT))
    {
      right_down = 0;
    }
    
//I hate to have to use a label like this, as I recognize it's considered *messy* but it works in a pinch I did this to prevent false permanent sprite overwrites if two arrow keys are pressed simultaneously. 
move_occurred:
    if(cursor_curr!=cursor_prev)
    {
      set_sprite(&obj_buffer[cursor_prev], PLACEHOLDER_TID);
      set_sprite(&obj_buffer[cursor_curr], X_TID);
      oam_copy(oam_mem, obj_buffer, 9);
    }
  }
  // Make sure A key is no longer being registered as pressed before returning to main game loop.
  while(key_pressed(_A));
}

static void player_2s_turn(void)
{
  uint8 cursor_prev = 0, cursor_curr = 0;
  while(get_sprite(obj_buffer[cursor_curr])!=PLACEHOLDER_TID)
  {
    cursor_curr++;
    cursor_prev++;
  }
  set_sprite(&obj_buffer[cursor_curr], O_TID);
  uint8 up_down = 0, down_down = 0, left_down = 0, right_down = 0;
  oam_copy(oam_mem, obj_buffer, 8);
  
  while(1)
  {
    vsync();
    if(key_pressed(_A))
    {
      oam_copy(oam_mem, obj_buffer, 9);
      play_end_turn_sound();
      break;
    }
    
    if(key_pressed(_UP) && !up_down)
    {
      up_down = 1;
      cursor_prev = cursor_curr;
      do
      {
        cursor_curr = (cursor_curr>2)? cursor_curr-3 : 6+cursor_curr;
        play_move_sound();
      }
      while(get_sprite(obj_buffer[cursor_curr])!=PLACEHOLDER_TID && cursor_curr!=cursor_prev);
      if(cursor_curr==cursor_prev) while(get_sprite(obj_buffer[cursor_curr])!=PLACEHOLDER_TID) cursor_curr++;
      goto move_occurred;
      
    }
    else if(!key_pressed(_UP))
    {
      up_down = 0;
    }

    if(key_pressed(_DOWN) && !down_down)
    {
      down_down = 1;
      cursor_prev = cursor_curr;
      do 
      {
        cursor_curr = (cursor_curr<6) ? cursor_curr+3 : cursor_curr - 6;
        play_move_sound();
      }
      while(get_sprite(obj_buffer[cursor_curr])!=PLACEHOLDER_TID && cursor_curr!=cursor_prev);
      if(cursor_curr==cursor_prev) while(get_sprite(obj_buffer[cursor_curr])!=PLACEHOLDER_TID) cursor_curr++;
      goto move_occurred;
    } 
    else if(!key_pressed(_DOWN))
    {
      down_down = 0;
    }

    if(key_pressed(_LEFT) && !left_down)
    {
      left_down = 1;
      cursor_prev = cursor_curr;
      do 
      {
        cursor_curr = (cursor_curr%3) ? cursor_curr - 1: cursor_curr + 2;
        play_move_sound();
      }
      while(get_sprite(obj_buffer[cursor_curr])!=PLACEHOLDER_TID && cursor_curr!=cursor_prev);
      if(cursor_curr==cursor_prev) while(get_sprite(obj_buffer[cursor_curr])!=PLACEHOLDER_TID) cursor_curr++;
      goto move_occurred;
    } 
    else if(!key_pressed(_LEFT))
    {
      left_down = 0;
    }

    if(key_pressed(_RIGHT) && !right_down)
    {
      right_down = 1;
      cursor_prev = cursor_curr;
      do 
      {
        cursor_curr = ((cursor_curr+1)%3) ? cursor_curr + 1: cursor_curr - 2;
        play_move_sound();
      }
      while(get_sprite(obj_buffer[cursor_curr])!=PLACEHOLDER_TID && cursor_curr!=cursor_prev);
      if(cursor_curr==cursor_prev) while(get_sprite(obj_buffer[cursor_curr])!=PLACEHOLDER_TID) cursor_curr++;
      goto move_occurred;
    } 
    else if(!key_pressed(_RIGHT))
    {
      right_down = 0;
    }
    
//I hate to have to use a label like this, as I recognize it's considered *messy* but it works in a pinch I did this to prevent false permanent sprite overwrites if two arrow keys are pressed simultaneously. 
move_occurred:
    if(cursor_curr!=cursor_prev)
    {
      set_sprite(&obj_buffer[cursor_prev], PLACEHOLDER_TID);
      set_sprite(&obj_buffer[cursor_curr], O_TID);
      oam_copy(oam_mem, obj_buffer, 9);
    }
  }
  // Make sure A key is no longer being registered as pressed before returning to main game loop.
  while(key_pressed(_A));
}

static void draw_win_animation(int32 dx, int32 dy, uint32 row, uint32 column)
{
  int x, y;
  if(row==0U && column != 0U)
  {
    // Keeping here JIC. Might not need it: int col = (int)column;
    x = (column==1) ? 14 : (column==2) ? 108 : 186;
    y = 0;
    while(y<SCREEN_HEIGHT-8)
    {
      vsync();
      play_win_animation_noise();
      OBJ_ATTRIBUTE *cursor = &obj_buffer[obj_count];
      obj_set_attribute(cursor, 0, 0x4000, (((0&0b1111)<<12) | ((0b00&0b11)<<10) | (BOARD_TID&0x3FF)));
      obj_set_pos(cursor, x, y);
      x+=dx;
      y+=dy;
      obj_count++;
      oam_copy(oam_mem, obj_buffer, obj_count);

    }

  }
  else if(row !=0U && column == 0U)
  {
    x = 0;
    y = (row==1) ? 8 : (row==2) ? 72 : 136;
    while(x<SCREEN_WIDTH-8)
    {
      vsync();
      play_win_animation_noise();
      OBJ_ATTRIBUTE *cursor = &obj_buffer[obj_count];
      obj_set_attribute(cursor, 0, 0x4000, (((0&0b1111)<<12) | ((0b00&0b11)<<10) | (BOARD_TID&0x3FF)));
      obj_set_pos(cursor, x, y);
      x+=dx;
      y+=dy;
      obj_count++;
      oam_copy(oam_mem, obj_buffer, obj_count);
    }
  }
  else
  {
    //Invalid params.
    return;
  }

  
  
}




static uint8 check_for_winner(void)
{
  uint8 sprites[9];
  uint8 winner = 5;
  for(int i=0;i<9;i++)
    sprites[i] = get_sprite(obj_buffer[i]);
  if(sprites[0]!=PLACEHOLDER_TID)
  {
    if(!(sprites[0]^sprites[1]) & !(sprites[1]^sprites[2]))
    {
      winner =  sprites[0];
      draw_win_animation(5, 0, 1, 0);
    }
    if(!(sprites[0]^sprites[4]) & !(sprites[4]^sprites[8]))
    {
      winner =  sprites[0];
      draw_win_animation(5, 4, 1, 0);
    }
    if(!(sprites[0]^sprites[3]) & !(sprites[3]^sprites[6]))
    {
      winner =  sprites[0];
      draw_win_animation(0, 5, 0, 1);
    }
  }
  if(sprites[1]!=PLACEHOLDER_TID)
  {
    if(!(sprites[1]^sprites[4]) & !(sprites[4]^sprites[7]))
    {
      winner =  sprites[1];
      draw_win_animation(0, 5, 0, 2);

    }
  }
  if(sprites[2]!=PLACEHOLDER_TID)
  {
    if(!(sprites[2]^sprites[5]) & !(sprites[5]^sprites[8]))
    {
      winner =  sprites[2];
      draw_win_animation(0, 5, 0, 2);
    }
    if(!(sprites[2]^sprites[4]) & !(sprites[4]^sprites[6]))
    {
      winner =  sprites[2];
      draw_win_animation(-5, 4, 0, 3);
      
    }
  }
  if(sprites[3]!=PLACEHOLDER_TID)
  {
    if(!(sprites[3]^sprites[4]) & !(sprites[4]^sprites[5]))
    {
      winner =  sprites[3];
      draw_win_animation(5, 0, 2, 0);
    }
  }
  if(sprites[6]!=PLACEHOLDER_TID)
  {
    if(!(sprites[6]^sprites[7]) & !(sprites[7]^sprites[8]))
    {
      winner =  sprites[6];
      draw_win_animation(5, 0, 3, 0);
    }
  }
  uint8 board_filled = 1;
  for(int i = 0; i < 9; i++)
    if(sprites[i] == PLACEHOLDER_TID)
    {
      board_filled = 0;
      break;
    }
    if(!board_filled && winner == 5) return 0;
    if(board_filled) return 1;

    if(winner == X_TID) return 2;
    if(winner == O_TID) return 3;
}

/**
 * @brief Tic tac toe round loop.
 * @return uint8 1 = tie, 2 = player 1 wins (X player wins), 3 = player 2 wins (O player wins)
*/
uint8 game_loop(void)
{

  game_init();
  uint8 winner = 0;
  while(winner==0)
  {
    player_1s_turn();
    winner = check_for_winner();
    if(winner!=0) break;
    player_2s_turn();
    winner = check_for_winner();
  }
  return winner;

}

