#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

const static int max_iter = 255;

static float max_x = 2.0;
static float min_x = -2.0;
static float max_y = 1.5;
static float min_y = -1.5;

int Mandelbrot_color(float x_0, float y_0) {
    float x = 0;
    float y = 0;
    int iter = 0;

    while (iter < max_iter && (x*x + y*y) <= 4.0) {
        float x_next = x*x - y*y + x_0;
        float y_next = 2.0*x*y + y_0;
        x = x_next;
        y = y_next;
        iter++;
    }

    if (iter == max_iter) {
        return 1;
    }
    else if (iter < max_iter / 3) {
        return 2;
    }
    else if (iter < 2 * (max_iter / 3)) {
        return 5;
    }
    return 4;
}



int main() {
    const int screen_width = 650;
    const int screen_height = 650;

    float scale_x = (max_x - min_x) / screen_width;
    float scale_y = (max_y - min_y) / screen_height;

    for (int y = 0; y < screen_height; y++) {
            for (int x = 0; x < screen_width; x++) {
                float x_0 = min_x + x * scale_x;
                float y_0 = min_y + y * scale_y;

                int color = Mandelbrot_color(x_0, y_0);

        }
    }
}
