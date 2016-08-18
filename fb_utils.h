/* Raspberry Pi Fast Put Pixel, Draw X Line, Draw Line,
 * Draw Rectangle, Draw Circle, Draw Char, and Draw String
 * in Frame Buffer Example using C (no asm)
 * By radioing/mates 2016, CC BY 4.0
 */
#ifndef _FB_UTILS_H
#define _FB_UTILS_H

#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <linux/fb.h>
#include <linux/types.h>
#include <stdlib.h>
#include <stdlib.h>
#include <string.h>

struct fb {
    struct fb_var_screeninfo var_screen_info;
    struct fb_fix_screeninfo fix_screen_info;
    uint8_t *screen;
    uint32_t w;
    uint32_t h;

    int fb_handle;
};

int init_fb(struct fb *fb);
void fb_destroy(struct fb *gb);
     
/* pointer to valid makeColor function */
typedef uint32_t (* pfMakeColor_t) (uint8_t, uint8_t, uint8_t);

/* pointer to valid putPixel function */
typedef void (* pfPutPixel_t)(int, int,
                             uint32_t,
                             char*,
                             const int);

/* pointer to valid drawXLine function */
typedef void (* pfDrawXLine_t)(int, int, uint32_t,
                             uint32_t,
                             char*,
                             const int);

/* pointer to valid drawLine function */
typedef void (* pfDrawLine_t)(int, int, int, int,
                             uint32_t,
                             char*,
                             const int);

/* pointer to valid drawRect function */
typedef void (* pfDrawRect_t)(int, int, int, int,
                             uint32_t,
                             char*,
                             const int);

/* pointer to valid drawCircle function */
typedef void (* pfDrawCircle_t)(int, int, int,
                             uint32_t,
                             char*,
                             const int);

/* pointer to valid drawFCircle function */
typedef void (* pfDrawFCircle_t)(int, int, int,
                             uint32_t,
                             char*,
                             const int);

/* pointer to valid drawFCircle function */
typedef void (* pfDrawChar816_t)(int, int,
                             uint32_t,
                             uint8_t*,
                             char*,
                             const int);
                             
/* void putPixel function */
void putPixelVoid(int x, int y,
                  uint32_t color,
                  char *screen, const int bytesPerLine);

/* void drawXLine function */
void drawXLineVoid(int x, int y, uint32_t dx,
                   uint32_t color,
                   char *screen, const int bytesPerLine);

/* void drawLine function */
void drawLineVoid(int x1, int y1, int x2, int y2,
                  uint32_t color,
                  char *screen, const int bytesPerLine);

/* void drawRect function */
void drawRectVoid(int x1, int y1, int x2, int y2,
                  uint32_t color,
                  char *screen, const int bytesPerLine);

/* void drawCircle function */
void drawCircleVoid(int x, int y, int r,
                    uint32_t color,
                    char *screen, const int bytesPerLine);

/* void drawCircle function */
void drawFCircleVoid(int x, int y, int r,
                     uint32_t color,
                     char *screen, const int bytesPerLine);

/* void drawChar816 function */
void drawChar816Void(int x, int y,
                     uint32_t color,
                     uint8_t *chrmask,
                     char *screen, const int bytesPerLine);

uint32_t makeColor888rgba(uint8_t r, uint8_t g, uint8_t b);

/* DrawStr - works through wrapper drawStr816() */
void drawStr816(int x, int y, uint32_t color, char * str, uint8_t *font,
                     char *fbuffer, const int bytesperline);

/****************************************
 * Frame buffer draw functions wrappers *
 ****************************************/
pfMakeColor_t    makeColor;
pfPutPixel_t     putPixel;
pfDrawXLine_t    drawXLine;
pfDrawLine_t     drawLine;
pfDrawRect_t     drawRect;
pfDrawCircle_t   drawCircle;
pfDrawFCircle_t  drawFCircle;
pfDrawChar816_t  drawChar816;

#endif /* _FB_UTILS_H */
