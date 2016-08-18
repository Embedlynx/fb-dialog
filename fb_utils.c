/* Raspberry Pi Fast Put Pixel, Draw X Line, Draw Line,
 * Draw Rectangle, Draw Circle, Draw Char, and Draw String
 * in Frame Buffer Example using C (no asm)
 * By radioing/mates 2016, CC BY 4.0
 */

#include "fb_utils.h"

/*******************************************************************
 * Optimized draw functions for 888 color mode (4 bytes per pixel) *
 *******************************************************************/
#define PUTPIXEL(a) *(uint32_t *)a = (uint32_t)color;
#define XMOVEPOW 2

void putPixel888(int x, int y, uint32_t color,
                        char *fbuffer, const int bytesperline)
{
    uint8_t * addr = fbuffer;
    addr += y*bytesperline + (x << XMOVEPOW);
    PUTPIXEL(addr);
}

void drawXLine888(int x, int y, uint32_t dx,
                  uint32_t color,
                  char *fbuffer,
                  const int bytesperline)
{
    uint8_t * addr = fbuffer + y*bytesperline + (x << XMOVEPOW);
    int cnt = dx >> 4;
    switch (dx & 0x0F) {
        do {
#undef PUTPIX
#define PUTPIX(n) case n: \
                      PUTPIXEL(addr); \
                      addr += (1 << XMOVEPOW);
            PUTPIX(15)
            PUTPIX(14)
            PUTPIX(13)
            PUTPIX(12)
            PUTPIX(11)
            PUTPIX(10)
            PUTPIX(9)
            PUTPIX(8)
            PUTPIX(7)
            PUTPIX(6)
            PUTPIX(5)
            PUTPIX(4)
            PUTPIX(3)
            PUTPIX(2)
            PUTPIX(1)
            PUTPIX(0)
            if (--cnt < 0) return;
        } while (1);
        return;
    }
}

void drawLine888(int x1, int y1, int x2, int y2,
                 uint32_t color,
                 char *fbuffer,
                 const int bytesperline)
{
    int xmove, ymove;
    int dx, dy;
    int accum;
    int cnt;
    uint8_t * addr = fbuffer;

    /* (x1, y1) must be the topmost point */
    if ((dy = y2 - y1) < 0) {
        dy = -dy;
        y1 = y2;
        dx = x2; x2 = x1; x1 = dx;
    }
    addr += y1*bytesperline + (x1 << XMOVEPOW);
    if ((dx = x2 - x1) < 0) {
        dx = -dx;
        xmove = -(1 << XMOVEPOW);
    } else {
        xmove = 1 << XMOVEPOW;
    }
    /* here: dx >= 0, dy >= 0, addr, xmove */
    while (1) {
        if (dy == 0) {
            /* horizontal lines */
            break;
        }
        if (dx == 0) {
            /* vertical lines */
            xmove = bytesperline;
            dx = dy;
            break;
        }
        if (dx == dy) {
            /* 45 deg lines */
            xmove += bytesperline;
            break;
        }
        /* the rest...*/
        ymove = bytesperline;
        dx <<= 1;
        dy <<= 1;
        if (dx > dy) {
            /* left-left-down and right-right-down */
            cnt = xmove; xmove = ymove; ymove = cnt;
            cnt = dx; dx = dy; dy = cnt;
        }
        /* left-down-down and right-rown-down */
        accum = dy >> 1;
        cnt = dy >> 5;
        switch (accum & 0x0F) {
            do {
#undef PUTPIX
#define PUTPIX(n) case n: \
                      PUTPIXEL(addr); \
                      addr += ymove; \
                      accum -= dx; \
                      if (accum < 0) { \
                          accum += dy; \
                          addr += xmove; \
                      }
                PUTPIX(15)
                PUTPIX(14)
                PUTPIX(13)
                PUTPIX(12)
                PUTPIX(11)
                PUTPIX(10)
                PUTPIX(9)
                PUTPIX(8)
                PUTPIX(7)
                PUTPIX(6)
                PUTPIX(5)
                PUTPIX(4)
                PUTPIX(3)
                PUTPIX(2)
                PUTPIX(1)
                PUTPIX(0)
                if (--cnt < 0) return;
            } while (1);
        }
    }
    /* special care for horizontal/vertical/45 deg lines
       as they are statistically significant objects */
    cnt = dx >> 4;
    switch (dx & 0x0F) {
        do {
#undef PUTPIX
#define PUTPIX(n) case n: \
                      PUTPIXEL(addr); \
                      addr += xmove;
            PUTPIX(15)
            PUTPIX(14)
            PUTPIX(13)
            PUTPIX(12)
            PUTPIX(11)
            PUTPIX(10)
            PUTPIX(9)
            PUTPIX(8)
            PUTPIX(7)
            PUTPIX(6)
            PUTPIX(5)
            PUTPIX(4)
            PUTPIX(3)
            PUTPIX(2)
            PUTPIX(1)
            PUTPIX(0)
            if (--cnt < 0) return;
        } while (1);
        return;
    }
    return;
}

void drawRect888(int x1, int y1, int x2, int y2,
                 uint32_t color,
                 char *fbuffer,
                 const int bytesperline)
{
    int dx, dy;
    int ds, da, cnt;
    uint8_t * addr = fbuffer;

    if ((dx = x2 - x1) < 0) {
        dx = -dx;
        x1 = x2;
    }
    if ((dy = y2 - y1) < 0) {
        dy = -dy;
        y1 = y2;
    }
    addr += y1*bytesperline + (x1 << XMOVEPOW);
    /* here: dx >= 0, dy >= 0, addr */
    da = bytesperline - ((dx + 1) << XMOVEPOW);
    ds = dx >> 4;
    dx = dx & 0x0F;
    do {
        cnt = ds;
        switch (dx) {
            do {
#undef PUTPIX
#define PUTPIX(n) case n: \
                      PUTPIXEL(addr); \
                      addr += (1 << XMOVEPOW);
                PUTPIX(15)
                PUTPIX(14)
                PUTPIX(13)
                PUTPIX(12)
                PUTPIX(11)
                PUTPIX(10)
                PUTPIX(9)
                PUTPIX(8)
                PUTPIX(7)
                PUTPIX(6)
                PUTPIX(5)
                PUTPIX(4)
                PUTPIX(3)
                PUTPIX(2)
                PUTPIX(1)
                PUTPIX(0)
            } while (--cnt >= 0);
        }
        addr += da;
    } while (--dy >= 0);
}

void drawCircle888(int x, int y, int r,
                   uint32_t color,
                   char *fbuffer,
                   const int bytesperline)
{
  int dx = r;
  int dy = 0;
  int accum = dx - (dy << 1) - 1;
  uint8_t * addr;

  while( dy <= dx ) {
#undef PUTPIX
#define PUTPIX(x, y) addr = fbuffer + (y)*bytesperline + ((x) << XMOVEPOW); \
                     PUTPIXEL(addr);
    PUTPIX( dx + x,  dy + y)
    PUTPIX( dy + x,  dx + y)
    PUTPIX(-dx + x,  dy + y)
    PUTPIX(-dy + x,  dx + y)
    PUTPIX(-dx + x, -dy + y)
    PUTPIX(-dy + x, -dx + y)
    PUTPIX( dx + x, -dy + y)
    PUTPIX( dy + x, -dx + y)
    dy++;
    if ((accum -= (dy << 1) - 1) < 0) {
        dx--;
        accum += dx << 1;
    }
  }
}

void drawFCircle888(int x, int y, int r,
                    uint32_t color,
                    char *fbuffer,
                    const int bytesperline)
{
  int dx = r;
  int dy = 0;
  int accum = dx - (dy << 1) - 1;
  uint8_t * addr;

  while( dy <= dx ) {
#undef PUTPIX
#define PUTPIX(x, y) addr = fbuffer + (y)*bytesperline + ((x) << XMOVEPOW); \
                     PUTPIXEL(addr);
    drawXLine888( dx + x, -dy + y, dy << 1, color, fbuffer, bytesperline);
    drawXLine888(-dx + x, -dy + y, dy << 1, color, fbuffer, bytesperline);
    drawXLine888( dy + x, -dx + y, dx << 1, color, fbuffer, bytesperline);
    drawXLine888(-dy + x, -dx + y, dx << 1, color, fbuffer, bytesperline);
    dy++;
    if ((accum -= (dy << 1) - 1) < 0) {
        dx--;
        accum += dx << 1;
    }
  }
}

void drawChar816_888(int x, int y, uint32_t color, uint8_t *chrmask,
                    char *fbuffer, const int bytesperline)
{
    uint8_t mask;
    int dx = 0;
    uint8_t * addr = (uint8_t *)fbuffer;
    addr += y*bytesperline + (x << XMOVEPOW);
    do {
        mask = chrmask[dx];
#undef PUTPIX
#define PUTPIX(m, a) if (mask & m) PUTPIXEL(a); \
                       a += (1 << XMOVEPOW);
        PUTPIX(0x80, addr)
        PUTPIX(0x40, addr)
        PUTPIX(0x20, addr)
        PUTPIX(0x10, addr)
        PUTPIX(0x08, addr)
        PUTPIX(0x04, addr)
        PUTPIX(0x02, addr)
        if (mask & 0x01) PUTPIXEL(addr);
        addr += bytesperline - 7*(1 << XMOVEPOW);
    } while (++dx < 16);
}

/*******************************************************************
 * Optimized draw functions for 565 color mode (2 bytes per pixel) *
 *******************************************************************/
#undef PUTPIXEL
#undef XMOVEPOW
#define PUTPIXEL(a) *(uint16_t *)a = (uint16_t)color;
#define XMOVEPOW 1

void putPixel565(int x, int y, uint32_t color,
                        char *fbuffer, const int bytesperline)
{
    uint8_t * addr = fbuffer;
    addr += y*bytesperline + (x << XMOVEPOW);
    PUTPIXEL(addr);
}

void drawXLine565(int x, int y, uint32_t dx,
                  uint32_t color,
                  char *fbuffer,
                  const int bytesperline)
{
    uint8_t * addr = fbuffer + y*bytesperline + (x << XMOVEPOW);
    int cnt = dx >> 4;
    switch (dx & 0x0F) {
        do {
#undef PUTPIX
#define PUTPIX(n) case n: \
                      PUTPIXEL(addr); \
                      addr += (1 << XMOVEPOW);
            PUTPIX(15)
            PUTPIX(14)
            PUTPIX(13)
            PUTPIX(12)
            PUTPIX(11)
            PUTPIX(10)
            PUTPIX(9)
            PUTPIX(8)
            PUTPIX(7)
            PUTPIX(6)
            PUTPIX(5)
            PUTPIX(4)
            PUTPIX(3)
            PUTPIX(2)
            PUTPIX(1)
            PUTPIX(0)
            if (--cnt < 0) return;
        } while (1);
        return;
    }
}

void drawLine565(int x1, int y1, int x2, int y2,
                 uint32_t color,
                 char *fbuffer,
                 const int bytesperline)
{
    int xmove, ymove;
    int dx, dy;
    int accum;
    int cnt;
    uint8_t * addr = fbuffer;

    /* (x1, y1) must be the topmost point */
    if ((dy = y2 - y1) < 0) {
        dy = -dy;
        y1 = y2;
        dx = x2; x2 = x1; x1 = dx;
    }
    addr += y1*bytesperline + (x1 << XMOVEPOW);
    if ((dx = x2 - x1) < 0) {
        dx = -dx;
        xmove = -(1 << XMOVEPOW);
    } else {
        xmove = 1 << XMOVEPOW;
    }
    /* here: dx >= 0, dy >= 0, addr, xmove */
    while (1) {
        if (dy == 0) {
            /* horizontal lines */
            break;
        }
        if (dx == 0) {
            /* vertical lines */
            xmove = bytesperline;
            dx = dy;
            break;
        }
        if (dx == dy) {
            /* 45 deg lines */
            xmove += bytesperline;
            break;
        }
        /* the rest...*/
        ymove = bytesperline;
        dx <<= 1;
        dy <<= 1;
        if (dx > dy) {
            /* left-left-down and right-right-down */
            cnt = xmove; xmove = ymove; ymove = cnt;
            cnt = dx; dx = dy; dy = cnt;
        }
        /* left-down-down and right-rown-down */
        accum = dy >> 1;
        cnt = dy >> 5;
        switch (accum & 0x0F) {
            do {
#undef PUTPIX
#define PUTPIX(n) case n: \
                      PUTPIXEL(addr); \
                      addr += ymove; \
                      accum -= dx; \
                      if (accum < 0) { \
                          accum += dy; \
                          addr += xmove; \
                      }
                PUTPIX(15)
                PUTPIX(14)
                PUTPIX(13)
                PUTPIX(12)
                PUTPIX(11)
                PUTPIX(10)
                PUTPIX(9)
                PUTPIX(8)
                PUTPIX(7)
                PUTPIX(6)
                PUTPIX(5)
                PUTPIX(4)
                PUTPIX(3)
                PUTPIX(2)
                PUTPIX(1)
                PUTPIX(0)
                if (--cnt < 0) return;
            } while (1);
        }
    }
    /* special care for horizontal/vertical/45 deg lines
       as they are statistically significant objects */
    cnt = dx >> 4;
    switch (dx & 0x0F) {
        do {
#undef PUTPIX
#define PUTPIX(n) case n: \
                      PUTPIXEL(addr); \
                      addr += xmove;
            PUTPIX(15)
            PUTPIX(14)
            PUTPIX(13)
            PUTPIX(12)
            PUTPIX(11)
            PUTPIX(10)
            PUTPIX(9)
            PUTPIX(8)
            PUTPIX(7)
            PUTPIX(6)
            PUTPIX(5)
            PUTPIX(4)
            PUTPIX(3)
            PUTPIX(2)
            PUTPIX(1)
            PUTPIX(0)
            if (--cnt < 0) return;
        } while (1);
        return;
    }
    return;
}

void drawRect565(int x1, int y1, int x2, int y2,
                 uint32_t color,
                 char *fbuffer,
                 const int bytesperline)
{
    int dx, dy;
    int ds, da, cnt;
    uint8_t * addr = fbuffer;

    if ((dx = x2 - x1) < 0) {
        dx = -dx;
        x1 = x2;
    }
    if ((dy = y2 - y1) < 0) {
        dy = -dy;
        y1 = y2;
    }
    addr += y1*bytesperline + (x1 << XMOVEPOW);
    /* here: dx >= 0, dy >= 0, addr */
    da = bytesperline - ((dx + 1) << XMOVEPOW);
    ds = dx >> 4;
    dx = dx & 0x0F;
    do {
        cnt = ds;
        switch (dx) {
            do {
#undef PUTPIX
#define PUTPIX(n) case n: \
                      PUTPIXEL(addr); \
                      addr += (1 << XMOVEPOW);
                PUTPIX(15)
                PUTPIX(14)
                PUTPIX(13)
                PUTPIX(12)
                PUTPIX(11)
                PUTPIX(10)
                PUTPIX(9)
                PUTPIX(8)
                PUTPIX(7)
                PUTPIX(6)
                PUTPIX(5)
                PUTPIX(4)
                PUTPIX(3)
                PUTPIX(2)
                PUTPIX(1)
                PUTPIX(0)
            } while (--cnt >= 0);
        }
        addr += da;
    } while (--dy >= 0);
}

void drawCircle565(int x, int y, int r,
                   uint32_t color,
                   char *fbuffer,
                   const int bytesperline)
{
  int dx = r;
  int dy = 0;
  int accum = dx - (dy << 1) - 1;
  uint8_t * addr;

  while( dy <= dx ) {
#undef PUTPIX
#define PUTPIX(x, y) addr = fbuffer + (y)*bytesperline + ((x) << XMOVEPOW); \
                     PUTPIXEL(addr);
    PUTPIX( dx + x,  dy + y)
    PUTPIX( dy + x,  dx + y)
    PUTPIX(-dx + x,  dy + y)
    PUTPIX(-dy + x,  dx + y)
    PUTPIX(-dx + x, -dy + y)
    PUTPIX(-dy + x, -dx + y)
    PUTPIX( dx + x, -dy + y)
    PUTPIX( dy + x, -dx + y)
    dy++;
    if ((accum -= (dy << 1) - 1) < 0) {
        dx--;
        accum += dx << 1;
    }
  }
}

void drawFCircle565(int x, int y, int r,
                    uint32_t color,
                    char *fbuffer,
                    const int bytesperline)
{
  int dx = r;
  int dy = 0;
  int accum = dx - (dy << 1) - 1;
  uint8_t * addr;

  while( dy <= dx ) {
#undef PUTPIX
#define PUTPIX(x, y) addr = fbuffer + (y)*bytesperline + ((x) << XMOVEPOW); \
                     PUTPIXEL(addr);
    drawXLine565( -dx + x, -dy + y, dx << 1, color, fbuffer, bytesperline);
    drawXLine565( -dx + x,  dy + y, dx << 1, color, fbuffer, bytesperline);
    drawXLine565( -dy + x, -dx + y, dy << 1, color, fbuffer, bytesperline);
    drawXLine565( -dy + x,  dx + y, dy << 1, color, fbuffer, bytesperline);
    dy++;
    if ((accum -= (dy << 1) - 1) < 0) {
        dx--;
        accum += dx << 1;
    }
  }
}

void drawChar816_565(int x, int y, uint32_t color, uint8_t *chrmask,
                     char *fbuffer, const int bytesperline)
{
    uint8_t mask;
    int dx = 0;
    uint8_t * addr = (uint8_t *)fbuffer;
    addr += y*bytesperline + (x << XMOVEPOW);
    do {
        mask = chrmask[dx];
#undef PUTPIX
#define PUTPIX(m, a) if (mask & m) PUTPIXEL(a); \
                       a += (1 << XMOVEPOW);
        PUTPIX(0x80, addr)
        PUTPIX(0x40, addr)
        PUTPIX(0x20, addr)
        PUTPIX(0x10, addr)
        PUTPIX(0x08, addr)
        PUTPIX(0x04, addr)
        PUTPIX(0x02, addr)
        if (mask & 0x01) PUTPIXEL(addr);
        addr += bytesperline - 7*(1 << XMOVEPOW);
    } while (++dx < 16);
}

/*****************************
 * Color composing functions *
 *****************************/
/* 16 bits per pixel, rgb:565 */
uint32_t makeColor565(uint8_t r, uint8_t g, uint8_t b)
{
    return (r >> 3) << 11 | (g >> 2) << 5 | (b >> 3);
}

/* 32 bits per pixel, rgba:8888 */
uint32_t makeColor888rgba(uint8_t r, uint8_t g, uint8_t b)
{
    return r << 16 | g << 8 | b;
}

/* 32 bits per pixel, bgra:8888 */
uint32_t makeColor888bgra(uint8_t r, uint8_t g, uint8_t b)
{
    return b << 16 | g << 8 | r;
}


/* DrawStr - works through wrapper drawStr816() */
void drawStr816(int x, int y, uint32_t color, char * str, uint8_t *font,
                    char *fbuffer, const int bytesperline)
{
    while (*str) {
        drawChar816(x, y, color, font + (*str << 4), fbuffer, bytesperline);
        x += 8;
        str++;
    };
}

int init_fb(struct fb *fb)
{
    int fb_handle = open("/dev/fb0", O_RDWR);
    if (fb_handle == -1) {
        perror("Impossible to open \"/dev/fb0\" for read and write.");
        return 1;
    }
    if (ioctl(fb_handle, FBIOGET_VSCREENINFO, &fb->var_screen_info)) {
        perror("Impossible to read framebuffer parameters.");
        return 1;
    }
    if (ioctl(fb_handle, FBIOGET_FSCREENINFO, &fb->fix_screen_info)) {
        perror("Impossible to read mode parameters.");
        return 1;
    }
    fb->screen = (uint8_t *)mmap(0, fb->fix_screen_info.smem_len, PROT_READ | PROT_WRITE, MAP_SHARED, fb_handle, 0);
    if (fb->screen == MAP_FAILED) {
        close(fb_handle);
        perror("Impossible to mmap framebuffer.");
        return 1;
    }
    fb->fb_handle = fb_handle;
    fb->w = fb->var_screen_info.xres;
    fb->h = fb->var_screen_info.yres;
    /* set proper drawLine and makeColor functions */
    if (fb->fix_screen_info.type == FB_TYPE_PACKED_PIXELS && fb->fix_screen_info.visual == FB_VISUAL_TRUECOLOR) {
        if (fb->var_screen_info.bits_per_pixel == 16) {
            makeColor = makeColor565;
            putPixel = putPixel565;
            drawXLine = drawXLine565;
            drawLine = drawLine565;
            drawRect = drawRect565;
            drawCircle = drawCircle565;
            drawFCircle = drawFCircle565;
            drawChar816 = drawChar816_565;
        } else if (fb->var_screen_info.bits_per_pixel == 32) {
            makeColor = (fb->var_screen_info.red.offset == 16 ? makeColor888rgba : makeColor888bgra);
            putPixel = putPixel888;
            drawXLine = drawXLine888;
            drawLine = drawLine888;
            drawRect = drawRect888;
            drawCircle = drawCircle888;
            drawFCircle = drawFCircle888;
            drawChar816 = drawChar816_888;
        }
    }
    memset(fb->screen, 0, fb->fix_screen_info.smem_len);
    return 0;
}

void fb_destroy(struct fb *fb) 
{
    munmap(fb->screen, fb->fix_screen_info.smem_len);
    close(fb->fb_handle);
}
