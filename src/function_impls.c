// Written by Burton O Sumner
#include "gba.h"

void set_video_mode3_sound_channel1(void)
{
  //~~~~~~~~~~~~~~~~~~~~~~Set draw mode in display register to VIDEO_MODE | BG_MODE = 0000 0000 0000 0011 | 0000 0100 0000 0000 = 0000 0100 0000 0011
  REG_DISPLAY = VIDEO_MODE | BG_MODE;

  //~~~~~~~~~~~~~~~~~~~~~~Set up the sound register to enable sound
  //Enable the master volume by flipping sound stat register bit number 7 to a 1, dont care about other registers so we can just overwrite with 0x0080
  REG_SOUND_STAT = MASTER_SOUND_ENABLE;
  REG_SOUND_DMG_COUNT = SOUND_DMG_BUILD_LR(SOUND_DMG_SQUARE_1, 7);
  
  //Set master volume to 100% volume
  REG_SOUND_DS_COUNT = DMG_VOL_100;

  //Turn off sweeping
  REG_SOUND1_SWEEP = SOUND1_SWEEP_OFF;
  //Envelope = Vol 12/15ths, Decaying, Max Step-Time 7, Duty 50%
  REG_SOUND1_COUNT = SOUND_SQUARE_ENVELOPE_BUILD(12, 0, 7) | SOUND_SQUARE_DUTY1_2ND;

  REG_SOUND1_FREQUENCY = 0;
}

static inline uint8 valid_channel(uint8 channel)
{
  channel &=0x0FU;
  return channel != 0U && (!(channel^SOUND_DMG_SQUARE_1) | !(channel^SOUND_DMG_SQUARE_2) | !(channel^SOUND_DMG_WAVE) | !(channel^SOUND_DMG_NOISE));
}

void set_regs(uint16 video_mode, uint16 bg_mode, uint8 sound_activate, uint8_t sound_channel)
{
  REG_DISPLAY = bg_mode|video_mode;
  if(sound_activate && sound_channel!=0)
  {
    REG_SOUND_STAT = MASTER_SOUND_ENABLE;
    REG_SOUND_DMG_COUNT = SOUND_DMG_BUILD_LR((valid_channel(sound_channel)) ? (uint8)sound_channel : SOUND_DMG_SQUARE_1, 7);
    REG_SOUND_DS_COUNT = DMG_VOL_100;

  }

}

void init_sound1_regs(uint16 envelope_build_macro, uint8 duty_macro)
{
  REG_SOUND1_COUNT = envelope_build_macro | duty_macro;
  // This is just a standin value. It will be overwritten by play_sound1_sound invocations.
  REG_SOUND1_FREQUENCY = 0;
}

Rect* init_rect(Rect *rectangle_instance_ptr, uint32 x, uint32 y, uint32 width, uint32 height)
{
  rectangle_instance_ptr->x      = x;
  rectangle_instance_ptr->y      = y;
  rectangle_instance_ptr->width  = width;
  rectangle_instance_ptr->height = height;
  return rectangle_instance_ptr;
}

void draw_rect(Rect rectange_instance, uint16 color)
{
  uint32 i, j;
  uint32 x = rectange_instance.x, y = rectange_instance.y;

  
  for(j = 0; j < rectange_instance.height; j++)
    for(i = 0; i < rectange_instance.width; i++)
      SCREENBUFFER[(x+i) + (SCREEN_WIDTH*(y+j))] = color;
  
}





/**
 * @brief Simple busy waiter to sync draw buffer updates with 
 * drawing hardware refresh rate.
 * 
 */
void vsync(void)
{
  while(REG_DISPLAY_VCOUNT>=160); //Wait until out of V-Blank, if in V-Blank.
  while(REG_DISPLAY_VCOUNT<160);  //Wait for last fame to finish drawing before setting screen buffer values for current frame.
}


// ------------------------------Tile Mode Function Defs
void oam_init(OBJ_ATTRIBUTE *obj, uint32 count)
{
  uint32 nn = count;
  uint32 *dest = (uint32*)obj;

  while(nn--)
  {
    *dest++=0x0200;
    *dest++= 0;
  }

  oam_copy(oam_mem, obj, count);
}

void oam_copy(OBJ_ATTRIBUTE *dest, const OBJ_ATTRIBUTE *src, uint32 count)
{
#if 1
  while(count--)
    *dest++ = *src++;
#else
  uint32 *destw = (uint32*)dest, *srcw = (uint32*)src;
  while(count--)
  {
    *destw++ = *srcw++;
    *destw++ = *srcw++;
  }
#endif
}
