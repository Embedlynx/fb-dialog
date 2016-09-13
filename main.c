/*
 * main.c 
 *
 * Copyright (C) 2016 Rangel Ivanov <rangelivanov88@gmail.com>
 *
 * This file is placed under the LGPL.  Please see the file
 * COPYING for more details.
 *
 * Misc utils for ts test programs
 */
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <tslib.h>
#include <getopt.h>

#include "fb_utils.h"
#include "testutils.h"
#include "pipe_utils.h"
#include "font.h"

#define PROGRESS_HEIGHT 50
#define PADDING 60
#define CHAR_WIDTH 8
#define PIPE_BUFFER_SIZE 6
#define MESSAGE_BOX_WIDTH 500
#define MESSAGE_BOX_HEIGHT 200

struct fb_button {
    uint32_t x;
    uint32_t y;
    uint32_t w;
    uint32_t h;

    char *text;
    uint32_t index;
};

struct fb_progress_bar {
    uint32_t x;
    uint32_t y;
    uint32_t w;
    uint32_t h;
    uint32_t max_width;

    char *text;
    char percents[PIPE_BUFFER_SIZE];
    uint32_t progress;
};

int string_width(char* string)
{
    return strlen(string) * CHAR_WIDTH;
}

struct fb_progress_bar* fb_progress_bar_new(struct fb *fb, char *text, uint32_t max_width)
{
    struct fb_progress_bar *progress = malloc(sizeof(struct fb_progress_bar));

    uint32_t progress_bar_x = fb->w / 2 - max_width / 2;
    uint32_t y = fb->h/2 - PROGRESS_HEIGHT;
    progress->x = progress_bar_x;
    progress->y = y;
    progress->w = 1;
    progress->h = PROGRESS_HEIGHT;
    progress->text = strdup(text);
    progress->max_width = max_width;
    progress->progress = 0;

    return progress;
}

void fb_progess_bar_set_progress(struct fb_progress_bar *p, int percent)
{
    if (percent > 100) {
        percent = 100;
    }

    if (p->progress == 100) {
        return;
    }
    p->progress = percent;
    p->w = percent * 0.01 * p->max_width;
    sprintf(p->percents, "%d\%%", percent);
}

void fb_progress_bar_draw(struct fb_progress_bar *p, struct fb *fb)
{
    int line_length = fb->fix_screen_info.line_length;

    drawRect(p->x, p->y, p->x + p->max_width, p->y + p->h, makeColor(0x55, 0x55, 0x55), fb->screen, line_length); 
    drawRect(p->x, p->y, p->x + p->w, p->y + p->h, makeColor(0xff, 0, 0), fb->screen, line_length); 

    int text_width = string_width(p->percents);
    int text_x = p->x  + p->max_width / 2 - text_width / 2;

    drawStr816(text_x, p->y + 18, makeColor(0xff, 0xff, 0xff), p->percents, VGA_FONT_16, fb->screen, line_length);

    text_width = string_width(p->text);
    text_x = p->x + p->max_width / 2 - text_width / 2;
    drawStr816(text_x, p->y - 20, makeColor(0xff, 0xff, 0xff), p->text, VGA_FONT_16, fb->screen, line_length);
}


void fb_progress_bar_destroy(struct fb_progress_bar *p)
{
    free(p->text);
    free(p);
}

struct fb_button* fb_button_new(uint32_t x, uint32_t y, char *text, uint32_t index)
{
    struct fb_button *btn = malloc(sizeof(struct fb_button));
    int text_width = string_width(text);
    int button_width = text_width + 40;
    btn->x = x;
    btn->y = y;
    btn->w = button_width;
    btn->h = 40;
    btn->text = strdup(text); 
    btn->index = index;

    return btn;
}

void fb_button_draw(struct fb_button *btn, struct fb *fb) 
{
    int line_length = fb->fix_screen_info.line_length;
    drawRect(btn->x, btn->y, btn->x + btn->w, btn->y + btn->h, makeColor(0x55, 0x55, 0x55), fb->screen, line_length); 

    int text_width = string_width(btn->text);

    int text_x = btn->x  + btn->w / 2 - text_width / 2;

    drawStr816(text_x, btn->y + 14, makeColor(0xff, 0xff, 0xff), btn->text, VGA_FONT_16, fb->screen, line_length);
}

void fb_button_destroy(struct fb_button *btn)
{
    free(btn->text);
    free(btn);
}

int fb_button_pressed(struct fb_button *b, int p_x, int p_y) 
{
    int pressed = p_x > b->x && p_x < (b->x + b->w)
        && p_y > b->y && p_y < (b->y + b->h);
    return pressed == 1 ? b->index : -1;
}

void display_progress(struct fb *fb, char *options)
{
    char *width = strtok(options, ",");
    char *text = strtok(NULL, ",");
    struct fb_progress_bar *progress = fb_progress_bar_new(fb, text, atoi(width));
    fb_progress_bar_draw(progress, fb);

    char data[PIPE_BUFFER_SIZE];
    int status;
    while ((status = poll_and_read(stdin, data, PIPE_BUFFER_SIZE)) > 0 ){
        int c = atoi(data);
        fb_progess_bar_set_progress(progress, c);
        fb_progress_bar_draw(progress, fb);
    }

    fb_progress_bar_destroy(progress);
}

void draw_message_text(struct fb *fb, int x, int y, char *message)
{
    int line_length = fb->fix_screen_info.line_length;
    char *new_line = "\x0A";
    int lines = 0;
    char *text = strtok(message, new_line);
    int text_width = string_width(text);
    int text_x = x + MESSAGE_BOX_WIDTH / 2 - text_width / 2;
    drawStr816(text_x, y + MESSAGE_BOX_HEIGHT/2, makeColor(0xff, 0xff, 0xff), message, VGA_FONT_16, fb->screen, line_length);

    while ((text = strtok(NULL, new_line)) > 0) {
        lines++;
        text_width = string_width(text);
        text_x = x + MESSAGE_BOX_WIDTH / 2 - text_width / 2;
        drawStr816(text_x, y + MESSAGE_BOX_HEIGHT/2 + 18 * lines, makeColor(0xff, 0xff, 0xff), text, VGA_FONT_16, fb->screen, line_length);
    }
}

void draw_message_box(struct fb *fb, char *message, int y_pos)
{
    int line_length = fb->fix_screen_info.line_length;
    int text_width = string_width(message);

    int x = fb->w / 2 - MESSAGE_BOX_WIDTH / 2;

    drawRect(x, y_pos, x + MESSAGE_BOX_WIDTH, y_pos + MESSAGE_BOX_HEIGHT, makeColor(0x55, 0x55, 0x55), fb->screen, line_length);

    draw_message_text(fb, x, y_pos, message);
}

void handle_confirm(struct tsdev *ts, struct fb *fb, char *text_for_buttons)
{
    char *btn1_text = strtok(text_for_buttons, ",");
    char *btn2_text = strtok(NULL, ",");
    char *message_text = strtok(NULL, ",");

    if (message_text) {
        draw_message_box(fb, message_text, 30);
    }
    uint32_t center_x = fb->w / 2; 
    uint32_t center_y = fb->h / 2;

    int btn1_width = string_width(btn1_text) + PADDING;
    int btn2_width = string_width(btn2_text) + PADDING;
    uint32_t button1_x = fb->w / 2 - (btn1_width + btn2_width + PADDING) / 2;

    struct fb_button *btn1 = fb_button_new(button1_x, center_y, btn1_text, 0);
    struct fb_button *btn2 = fb_button_new(button1_x + btn1_width + PADDING, center_y, btn2_text, 1);

    fb_button_draw(btn1, fb);
    fb_button_draw(btn2, fb);

    int p_x, p_y;
    int button_index = -1;
    while (1) {
        getxy(ts, &p_x, &p_y);
        button_index = fb_button_pressed(btn1, p_x, p_y);
        if (button_index != -1) {
            printf("%d\n", button_index);
            break;
        }

        button_index = fb_button_pressed(btn2, p_x, p_y);
        if (button_index != -1) {
            printf("%d\n", button_index);
            break;
        }

        fb_button_draw(btn1, fb);
        fb_button_draw(btn2, fb);
    }

    fb_button_destroy(btn1);
    fb_button_destroy(btn2);
}

int main(int argc, char **argv)
{
	struct tsdev *ts = init_ts();
    if (!ts) {
        perror("Could not initialize touch screen");
        return 1;
    }

    struct fb fb;
    if (init_fb(&fb)) {
        perror("Could not initialize framebuffer");
        return 1;
    }

    int c;

    while (1) {
        static struct option long_options[] =
            {
              /* These options don’t set a flag.
                 We distinguish them by their indices. */
              {"gauge",  required_argument, 0, 'g'},
              {"confirm",  required_argument, 0, 'c'},
              {"message",  required_argument, 0, 'm'},
              {0, 0, 0, 0}
            };
        /* getopt_long stores the option index here. */
        int option_index = 0;

        c = getopt_long(argc, argv, "g:c:m:", long_options, &option_index);

        /* Detect the end of the options. */
        if (c == -1)
            break;

        switch (c) {
            case 'g':
              display_progress(&fb, optarg);
              break;

            case 'c':
              handle_confirm(ts, &fb, optarg);
              break;

            case 'm':
              draw_message_box(&fb, optarg, 75);
              break;

            case '?':
              break;

            default:
              abort();
        }
    }

    fb_destroy(&fb);
    return 0;
}
