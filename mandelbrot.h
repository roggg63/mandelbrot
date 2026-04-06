#include <stdio.h>
#include <assert.h>
#include <raylib.h>
#include <stdlib.h>
#include <stdbool.h>
#include <arm_neon.h>

const static int max_iter = 255;

static float max_x = 2.0;
static float min_x = -2.0;
static float max_y = 1.5;
static float min_y = -1.5;

void open_graphic_window();
Color Mandelbrot_color(float x_0, float y_0);
Color Get_Mandelbrot_color(int iter);
