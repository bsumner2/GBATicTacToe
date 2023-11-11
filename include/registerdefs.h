/* main.c
 * Written by Burton O Sumner
 * With Partial In-class, Lecture-Guidance from Dr. Hoskins in CSCE212
 * For Tic Tac Toe Extra Credit Assignment For CSCE212 taught by Dr. Hoskins
 */
//DEFINE REGISTERS AND THEIR CONSTS HERE.
//240x160
//ARM7TDMI 16.78 MHz
//Color word is 16bits
//0bXbbbbbgggggrrrrr
//0x00000000 -> 0x00003fff (System ROM, exe but not readable)
//0x02000000 -> 0x02020000 (External RAM, 256 KB)
//0x03000000 -> 0x03007fff (Internal RAM, 32 KB)
//0x04000000 -> 0x04003fff (IO registers)
//0x05000000 -> 0x05003fff (Color Palette)
//0x06000000 -> 0x06017fff (VRAM 96 KB)
//0x07000000 -> 0x070003ff (OAM RAM)
//0x08000000 -> 0x???????? (Gamepak ROM up to 32MB)
//0x0E000000 -> 0x???????? (Gamepak RAM)



#ifndef REGISTERDEFS_H
#define REGISTERDEFS_H
#include "types.h"


// Ex usage: const uint8 array[256] ALIGN(4) = {...}; // Forces word alignment
// Ex usage: typedef struct FOO {...} ALIGN(4) FOO;   // Forces struct to be aligned to word boundaries.
// Ex usage: struct FOO {...} PACKED;                 // Forces struct members to be packed.


#define SCREEN_WIDTH                            240 
#define SCREEN_HEIGHT                           160

//#define IWRAM

#define MEMIO                                   0x04000000

#define PALETTE_MEM                             0x05000000
#define VRAM                                    0x06000000  
#define OBJ_ATTRIBUTE_MEM                       0x07000000

#define VIDEO_MODE                              0x0003
#define BG_MODE                                 0x0400


#define REG_DISPLAY                             (* (volatile uint32 *) (MEMIO))
#define REG_DISPLAY_VCOUNT                      (* (volatile uint32 *) (MEMIO + 0x0006))
#define REG_DISPLAY_INPUT                       (* (volatile uint32 *) (MEMIO + 0x0130))


#define SCREENBUFFER                            ((volatile uint16 *) (VRAM))

#define tile_mem    ( (CHARBLOCK*) VRAM)
#define tile8_mem   ( (CHARBLOCK8*) VRAM)



/*
 * Ex usage: TILE *ptr = &tile_mem[3][12]; //block 4 (lower obj block), tile 12
 * Ex usage: tile_mem[3][12] = *(TILE*)spriteData;
 */


#define oam_mem     ( (OBJ_ATTRIBUTE*) OBJ_ATTRIBUTE_MEM)
#define obj_mem     ( (OBJ_ATTRIBUTE*) OBJ_ATTRIBUTE_MEM)

#define PALETTE_BG_SIZE                         0x00200
#define PALETTE_OBJ_SIZE                        0x00200

#define PALETTE_BG_MEM                          (PALETTE_MEM)
#define PALETTE_OBJ_MEM                         (PALETTE_MEM + PALETTE_BG_SIZE)

#define PALETTE_BG_BUFFER                       ( (uint16*) PALETTE_BG_MEM)
#define PALETTE_OBJ_BUFFER                      ( (uint16*) PALETTE_OBJ_MEM)

#define OBJ_MODE            0x1000 // Enables object sprite rendering
#define OBJ_1D_ARRAY_MODE   0x0040 // VRAM acts as a 1D array.
#define OBJ_2d_ARRAY_MODE   0x0000 // VRAM acts as a 2D array.

/* 
 * NOTE: Ignore the define column. That's something I have yet to implement. It looks like a pain in the tucus, so I am deferring it to later.
 * Attribute 0
 * | F   E | D | C | B   A |  9  8 | 7  6  5  4  3  2  1  0 |
 * |  Sh   |CM |MOS|  GM   |   OM  |           Y            |
 * Bits | Name | Define                 | Description
 * 0-7  |  Y   | ATTRIBUTE0_Y#          | Y coordinate.
 * 8-9  |  OM  | ATTRIBUTE0_MODE#       | (Affine) obj mode. To hide the sprite or make it affine. 
 *      |      | ATTRIBUTE0_REG         | 00 = normal rendering
 *      |      | ATTRIBUTE0_AFF         | 01 = Affine sprite
 *      |      | ATTRIBUTE0_HIDE        | 10 = Disables rendering (i.e. Hides sprite)
 *      |      | ATTRIBUTE0_AFF_DBL     | 11 = Affine sprite using double rendering area.
 * A-B  |  GM  | ATTRIBUTE0_GFX#        | Flags for special fx.
 *      |      | ATTRIBUTE0_WIN.        | 00 = normal rendering
 *      |      | ATTRIBUTE0_BLEND       | 01 = enables alpha blending.
 *      |      |                        | 10 = Obj partof the obj window. (Sprite itself not rendered, but is mask for bgs and other sprites. Likely to be seldom used.)
 *      |      |                        | 11 = FORBIDDEN
 *  C   |  MOS | ATTRIBUTE0_MOSAIC      | Enables mosaic effect.
 *  D   |  CM  |                        | Color mode
 *      |      | ATTRIBUTE0_4BPP        | 16 colors0x
 *      |      | ATTRIBUTE0_8BPP        | 256 colors
 * E-F  |  Sh  | ATTRIBUTE0_SHAPE       | Sprite shape
 *      |      | ATTRIBUTE0_SQUARE      | 
 *      |      | ATTRIBUTE0_WIDE        |
 *      |      | ATTRIBUTE0_TALL        |
 * 
 */

/* Attribute 1
 * | F  E | D | C | B  A  9 | 8  7  6  5  4  3  2  1  0 |   
 * |  Sz  |VF |HF |    -    |            X              |
 * |  -   |       AID       |            -              |
 * Bits | Name | Define                 | Description
 * 0-8  |  Y   | ATTRIBUTE1_X#          | X coordinate.
 * 9-D  | AID  | ATTRIBUTE1_AFF#        | Specifies the OAM_AFF_ENTY this sprite will use. Valid only in Affine mode set by attribute0 bit#8 
 * C-D  |      | ATTRIBUTE1_FLIP#       | Horizontal/vertical flipping flags.
 *      |  HF  | ATTRIBUTE1_HFLIP       | As name suggests, for horizontal flipping.
 *      |  VF  | ATTRIBUTE1_VFLIP       | As name suggests, for vertical flipping. 
 * E-F  |  Sz  | ATTRIBUTE1_SIZE#       | Sprite size**. *> Together with shape bits in attribute zero bit#s E-F, determines sprite's real size.
 * 
 */

/* Attribute 2
 * | F  E  D  C | B  A | 9  8  7  6  5  4  3  2  1 0 |   
 * |     PB     |  Pr  |            TID              |
 * Bits | Name | Define                 | Description
 * 0-9  | TID  | ATTRIBUTE2_ID#         | Base tile-index of sprite. (Note: must be 512 or higher in BMP video modes)
 * A-B  |  Pr  | ATTRIBUTE2_PRIO#       | Priority: Higher priorities drawn first. (Good for layering sprites)
 * C-F  |  PB  | ATTRIBUTE1_PALBANK#    | Palette-bank: to use when in 16 color mode. Has no effect if the color mode flag at attribute 0 bit# C is set. 
 *
 */

//Bitfield macros for attribute2

/** 
 * Attribute defines that I have yet to implement. 
#define ATTRIBUTE2_ID_MASK          0x03FF
#define ATTRIBUTE2_ID_SHIFT         0
#define ATTRIBUTE2_ID(n)            ((n)<<ATTRIBUTE_2_ID_SHIFT)

#define BF_PREP(x, name)                        (((x)<<name##_SHIFT) & name##_MASK)
#define BF_GET(x, name)                         (((x)&name##_MASK)>>name##_SHIFT)
#define BF_SET(y, x, name)                      (y = ((y)&~name##_MASK) | BF_PREP(x, name))

#define BF_PREP2(x, name)                       ((x) & name##_MASK)
#define BF_GET2(y, name)                       ((y) & name##_MASK)
#define BF_SET2(y, x, name)                    (y = ((y)&~name##_MASK) | BF_PREP2(x, name))
*/

/* 
 * The attributes 
 * 
 * Ex usage:
 * Creating bitfield:
 * attr2 |= BF_PREP(id, ATTRIBUTE0_SHAPE);
 * Expands to:
 * attribute2 |= (id<<ATTRIBUTE2_ID_SSHIFT) & ATTRIBUTE2_ID_MASK;
 * 
 * Getting bitfield:
 * id = BF_GET(attribute2, ATTRIBUTE2_ID);
 * Expands to:
 * id = (attribute2 & ATTRIBUTE2_ID_MASK)>>ATTRIBUTE2_ID_SHIFT;
 * 
 * Inserting bitfield:
 * BF_SET(attribute2, id, ATTRIBUTE2_ID);
 * Expands to:
 * attribute2 = (attribute2&~ATRIBUTE2_ID_MASK) | ((id<<ATTRIBUTE2_ID_SHIFT) & ATTRIBUTE2_ID_MASK);
 */






/**
 * @brief Defining Sound Out Master Control Registers, valid modes for these registers, and functions using bitwise ops to organize the bits to write to these regs.
 * WRITTEN WITH GREAT HELP FROM TONC LIBRARY WEBSITE'S WEBPAGE ON GBA AUDIO PROGRAMMING. 
 * [Credit/Citation]: https://www.coranac.com/tonc/text/sndsqr.htm
 * 
 */

/*
 * REG_SOUND_DMG_COUNT Offset from IO Registers = 0x0080.
 * Bit number   |15|14|13|12|11|10|09|08|07|06|05|04|03|02|01|00|
 * Designations |R4|R3|R2|R1|L4|L3|L2|L1|- |   RV   |- |   LV   |
 * R4-R1 = Right speaker sound channel mode
 * L4-L1 = Left speaker sound channel mode
 * (L/R)V = (Left/Right) speaker volume
 * 
 */
#define REG_SOUND_DMG_COUNT (* (volatile uint16 *) (MEMIO + 0x0080))
#define SOUND_DMG_SQUARE_1  0x01
#define SOUND_DMG_SQUARE_2  0x02
#define SOUND_DMG_WAVE      0x04
#define SOUND_DMG_NOISE     0x08


//Macro to spit out an integer formatted to be placed into SOUND_DMG_COUNT register in a way the GBA can interpret.
#define SOUND_DMG_BUILD(_left_mode, _right_mode, _left_volume, _right_volume)   \
                ( ((_left_volume)&7) | (((_right_volume)&7)<<4) | ((_left_mode)<<8) | ((_right_mode)<< 12)  )

//Macro that uses SOUND_DMG_BUILD(...), but for mono sounds (i.e same mode and volume for both left and right speaker.) 
#define SOUND_DMG_BUILD_LR(_mode, _vol) SOUND_DMG_BUILD(_mode, _mode, _vol, _vol)

/* REG_SOUND_DS_COUNT offset from IO Register by 0x082
 * Bit Number   |15|14|13|12|11|10|09|08|07|06|05|04|03|02|01|00|
 * Designations |BF|BT|BL|BR|AF|AT|AL|AR|   -    |BV|AV|DMG Vol.|
 * (A/B)(F/T/L/R) = Direct Sound (A/B)(FIFO Reset/Timer/Left Enable/Right Enable) Respectively
 * (A/B)V = Direct Sound (A/B) Volume ratio (50% if clear | 100% if set)
 * 
 */
#define REG_SOUND_DS_COUNT  (* (volatile uint16 *) (MEMIO + 0x0082))
#define DMG_VOL_25          0x0000
#define DMG_VOL_50          0x0001
#define DMG_VOL_100         0x0002
// 0b11                    (0x0011) FORBIDDEN


/* REG_SOUND_STAT Offset from IO register by 0x084
 * Bit Number   |15|14|13|12|11|10|09|08|07|06|05|04|03|02|01|00|
 * Designations |           -           |ME|   --   |4A|3A|2A|1A|
 * ME = Master Sound Enable
 * 4/3/2/1)A = (SQUARE1)
 * 
 */
#define REG_SOUND_STAT          (* (volatile uint16 *) (MEMIO + 0x0084))
#define MASTER_SOUND_ENABLE     0x0080

/* REG_SOUND1_SWEEP
 * Bit Number   |15|14|13|12|11|10|09|08|07|06|05|04|03|02|01|00|
 * Designations |             -            |    T   | M|    N   |
 * T = Step-time: Time between sweeps measured at 128 Hz Therefore DeltaTime = T/128ms. If T = 0, sweep disabled.
 * M = Sweep mode: Sweep can either move rate up or down. (up is default)
 * N = Sweep shift number: Amount of bits to shift the Step Time until rate reaches either 0 or the max of 2047.
 * 
 */
#define REG_SOUND1_SWEEP        (* (volatile uint16 *) (MEMIO + 0x0060))
#define SOUND1_SWEEP_OFF         0x0008

/* REG_SOUND1_COUNT
 * Bit Number   |15|14|13|12|11|10|09|08|07|06|05|04|03|02|01|00|

 * Designations |    EIV    |ED|   EST  |  D  |        L        |
 * EIV = Envelope initial value, kind of akin to a volume setting. 0 = silent; 15 = full blast.
 * ED = Envelope direction; whether envelope decreases or increases with each step. (decreases by default).
 * EST = Envelope step-time: time between envelope changes such that DeltaT = EST/64sec
 * D = Duty cycle (ratio between on and off times of the sqr wave)
 * L = Sound length (Write-only. Only works if channel is timed)
 * 
 */
#define REG_SOUND1_COUNT        (* (volatile uint16 *) (MEMIO + 0x0062))
#define SOUND_SQUARE_DUTY1_8TH  0
#define SOUND_SQUARE_DUTY1_4TH  0x0040
#define SOUND_SQUARE_DUTY1_2ND  0x0080
#define SOUND_SQUARE_DUTY3_4TH  0x00C0

#define SOUND_SQUARE_ENVELOPE_BUILD(_env_vol, _env_dir, _env_max_steptime)  \
                     ( ((_env_vol & 15) << 12) | ((_env_dir & 1) << 11) | ((_env_max_steptime & 7) << 8) )


/* REG_SOUND1_FREQUENCY
 * Bit Number   |15|14|13|12|11|10|09|08|07|06|05|04|03|02|01|00|
 * Designations |Re|T |   --   |                R               |
 * R  = Sound rate = (the initial rate before sweep).
 * T  = Timed flag = Whether or not It is timed, affects whether or not S1_COUNT bits 0-6 are used, as those are the sound length bits.
 * Re = Reset flag = Resets the sound to the initial volume (and sweep) settings.
 * 
 */
#define REG_SOUND1_FREQUENCY    (* (volatile uint16 *) (MEMIO + 0x0064))
#define SOUND_FREQUENCY_RESET   0x8000
#define SOUND_TIMED             0x4000

typedef enum
{
    C_NOTE=0,   CS_NOTE,    D_NOTE,     DS_NOTE,
    E_NOTE,     F_NOTE,     FS_NOTE,    G_NOTE,
    GS_NOTE,    A_NOTE,     AS_NOTE,    B_NOTE
} enumerableSoundNote;

//Sound rates for notes at the 5th octave in traditional music
static const uint32 __sound_rates[12] = 
{
    8013, 7566, 7144, 6742, // C , C#, D , D#
    6362, 6005, 5666, 5346, // E , F , F#, G
    5048, 4766, 4499, 4246  // G#, A , A#, B
};

//Example usage : REG_SOUND1_FREQUENCY = SOUND_FREQUENCY_RESET | SND_RATE(NOTE_A, 0);
#define SOUND_RATE(note, octave) ( 2048-(__sound_rates[note]>>(4+(octave))) )







/* Input register active low.
 * 1 1 1 1 1 1 1 1 1 1
 * _ _ _ _ _ _ _ _ _ _
 *             ... B A
 */

/* If A being pressed
 * 1 1 1 1 1 1 1 1 1 0
 * &                 1
 * _ _ _ _ _ _ _ _ _ _
 * 0 0 0 0 0 0 0 0 0 0 
 *             ... B A
 * If Input Reg value&1==0, we know A is being pressed. (Basic gist of masking.)
 */

#define _A       1
#define _B       2
#define _SELECT  4
#define _START   8
#define _RIGHT   16
#define _LEFT    32
#define _UP      64
#define _DOWN    128
#define _R       256
#define _L       512



#endif
