/** 
 * Authors:
 * - Jasper Vijn Caern
 *    (https://www.coranac.com/tonc/text/toc.htm)
 * - Burton O Sumner
 */


/** Function declarations */
#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include "types.h"
#include "registerdefs.h"

// Written by Burton O Sumner
void vsync(void);

void set_video_mode3_sound_channel1(void);

void set_regs(uint16 video_mode, uint16 bg_mode, uint8 sound_on, uint8 sound_channel);

uint8 main_menu(void);

void init_sound1_regs(uint16 envelope_build_macro, uint8 duty_macro);

uint8 game_loop(void);

Rect* init_rect(Rect *rectangle_instance_ptr, uint32 x, uint32 y, uint32 width, uint32 height);

void draw_rect(Rect rectangle_instance, uint16 color);

static inline uint16 key_pressed(uint16 key) {   return !(REG_DISPLAY_INPUT&key);  }

static inline int clamp(int value, int min, int max)
{  return (value < min) ? min : (value > max)? max : value;  }


static inline void play_sqr_wave_sound(int note, int octave, int length)
{
  
  REG_SOUND1_COUNT &= 0xFFC0; //Overwrite previously-played note's length with zeros.
  REG_SOUND1_COUNT |= ((length)&63);
  REG_SOUND1_FREQUENCY = SOUND_FREQUENCY_RESET | SOUND_RATE(note, octave);
  REG_SOUND1_FREQUENCY |= (length!=0) ? SOUND_TIMED : 0; //Else or by 0 because 1 OR 0 is 1, so basically nothing changes.
}




/** 
 * --------------------------- Tile mode stuff -------------------------------------
 * Credit: Jasper Vijn Caern
 *    (https://www.coranac.com/tonc/text/toc.htm)
 */


#define ATTRIBUTE0_BUILD(y, shape, bits_per_px, sprite_mode, mosaic_mode, blend_fx, window_fx)\
(                                                                                              \
  ((y & 0xFF) | ((sprite_mode & 0b11)<<8) | ((blend_fx & 1)<<10) | ((window_fx & 1)<<11)        \
  | ((mosaic_mode & 1)<<12) | ((bits_per_px & 0b1000)<<10) | ((shape & 0b11)<<14))               \
)


#define ATTRIBUTE1_BUILD_REGULAR(x, size, hflip, vflip)   \
((x & 0x1FF) | ((hflip & 1)<<12) | ((vflip & 1)<<13) | ((size & 0b11)<<14))

#define ATTRIBUTE1_BUILD_AFFINE(x, affine_id) \
((x & 0x1FF) | ((affine_id & 0x1F)<<9) | ((size & 0b11)<<14))

#define ATTRIBUTE2_BUILD(id, palette_bank, priority) \
((id & 0x3FF) | ((palette_bank & 0b1111)<<12) | ((priority & 3<<10)<<10))

void oam_init(OBJ_ATTRIBUTE *object, uint32 count);

void oam_copy(OBJ_ATTRIBUTE *dest, const OBJ_ATTRIBUTE *src, uint32 count);

void obj_test(void);

static inline OBJ_ATTRIBUTE *obj_set_attribute(OBJ_ATTRIBUTE *obj, uint16 attribute0, uint16 attribute1, uint16 attribute2)
{
  obj->attribute0 = attribute0;
  obj->attribute1 = attribute1;
  obj->attribute2 = attribute2;
  return obj;
}

/**
 * @brief set x and y position for the sprite.
 */
static inline void obj_set_pos(OBJ_ATTRIBUTE *obj, int x, int y)
{
  obj->attribute0 = (obj->attribute0 & 0xFF00) | y;
  obj->attribute1 = (obj->attribute1 & 0xFF00) | (uint8)clamp(x, 0, SCREEN_WIDTH);
}

/**
 * @brief Overwrites the object mode bits, in the param'd object's attribute0, with 0b10 (disable rendering aka hide) mode.
 * @details
 * 0xFFCF = 0b1111 1111 1100 1111 = Clear mask. (&'d w/attribute0 to clear Object mode bits in attribute0)
 * 0x0020 = 0b0000 0000 0010 0000 = set value (|'d w/outcome of clear_mask&attribute0 to overwrite the object mode bits with the disable rendering (hide) mode value.)
 */
static inline void obj_hide(OBJ_ATTRIBUTE *obj)
{   obj->attribute0 = (obj->attribute0 & 0xFCFF) | (0x0200);  }

/**
 * @brief Overwrites the obj mode bits to clear the hide mode value.
 * @param mode The value for the mode that will be set in place of the previous hide mode.
 */
static inline void obj_unhide(OBJ_ATTRIBUTE *obj, uint8 mode)
{   obj->attribute0 = (obj->attribute0 & 0xFCFF) | ((mode & 0b11)<<8);  }

static inline uint8 obj_is_hidden(OBJ_ATTRIBUTE *obj, uint8 mode)
{
  return (obj->attribute0 & 0x0020); 
}


static inline int clamp_sprite(int sprite_coordinate, int sprite_dimension, int window_dimension)
{  return clamp(sprite_coordinate, 0, window_dimension-sprite_dimension);  }




#endif /* FUNCTIONS_H */
