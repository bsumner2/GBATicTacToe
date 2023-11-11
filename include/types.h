/** 
 * Authors:
 * - Jasper Vijn Caern (almost all typedefs and macro defs)
 *      (https://www.coranac.com/tonc/text/toc.htm)
 * - Burton O Sumner (Rect and bmp and word_alignedbmp typedefs)
 */

#ifndef TYPES_H
#define TYPES_H

// Macros for storing vars or code in IW/EWRAM

/** Put var into IWRAM (defaults to IWRAM) */
#define     IWRAM_DATA      __attribute__((section(".iwram")))
/** Put var into IWRAM (defaults to IWRAM) */
#define     EWRAM_DATA      __attribute__((section(".ewram")))

/** Put NON-INITIALIZED var into EWRAM */
#define     EWRAM_BSS       __attribute__((section(".sbss")))

/** Put function in IWRAM */
#define     IWRAM_CODE      __attribute__((section(".iwram"), long_call))
/** Put function in EWRAM */
#define     EWRAM_CODE      __attribute__((section(".ewram"), long_call))



// Macros for data alignment
#define ALIGN(n)        __attribute__((aligned(n)))
#define PACKED          __attribute__((packed))


typedef unsigned char                           uint8;
typedef unsigned short                          uint16;
typedef unsigned int                            uint32;

typedef signed char                             int8;
typedef signed short                            int16;
typedef signed int                              int32;


typedef uint16 bmp[];
typedef uint32 word_alignedbmp[];


/** @brief 8x8 tile square; 4 bit color depth. 32 bytes. */
typedef struct
{    uint32 data[8];     } TILE, TILE4;

/** @brief double sized tiles. (d-tiles) 8 bit color depth*/
typedef struct
{    uint32 data[16];   } TILE8;

/** @brief 32x16 tiles. */
typedef TILE    CHARBLOCK[512];

/** @brief 16x16 d-tiles */
typedef TILE8   CHARBLOCK8[256];




/*
 *typedef struct OBJ_ATTRIBUTE
 *{
 *    uint16  attribute0, attribute1, attribute2;
 *    int16   fill;
 *    
 *} OBJ_ATTRIBUTE;
 *
 * OBJ_ATTR a, b; ... b=a; <- fails due to mem_cpy
 */

/** Object attributes structure. */
typedef struct tagOBJ_ATTRIBUTE
{
    uint16  attribute0, attribute1, attribute2;
    int16   fill;
    
} ALIGN(4) OBJ_ATTRIBUTE;

//OBJ_ATTR a, b; ... b=a; doesn't fail. No memcpy; over 10 times faster.




typedef struct OBJ_AFFINE
{
    uint16  fill0[3];
    int16   pa;
    uint16  fill1[3];
    int16   pb;
    uint16  fill2[3];
    int16   pc;
    uint16  fill3[3];
    int16   pd;
} ALIGN(4) OBJ_AFFINE;



typedef struct
{
    uint32 x, y, width, height;
} Rect;



#endif
