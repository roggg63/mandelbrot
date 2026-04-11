#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

const static int max_iter = 255;

static float max_x = 2.0;
static float min_x = -2.0;
static float max_y = 1.5;
static float min_y = -1.5;

int Get_Mandelbrot_color(int iter) {
    int color = 1;
    if (iter == max_iter) {
        color = 2;
    }
    else if (iter < max_iter / 3) {
        color = 3;
    }
    else if (iter < 2 * (max_iter / 3)) {
        color = 4;
    }
    else {
        color = 5;
    }
    return color;
}


int main () {
    const int screen_width = 650;
    const int screen_height = 650;

    float scale_x = (max_x - min_x) / screen_width;
    float scale_y = (max_y - min_y) / screen_height;

    long long total = 0;

    for (int y = 0; y < screen_height; y++) {
        float y__0 = min_y + y * scale_y;
        float y_0[4] = {y__0, y__0, y__0, y__0};
        for (int x = 0; x < screen_width; x+=4) {
            float x_0[4] = {};
            for (int i = 0; i < 4; i++) {
                x_0[i] = min_x + (x + i) * scale_x;
            }

            int iters[4] = {};

            float cur_x[4] = {};
            float cur_y[4] = {};

            for (int iter = 0; iter < max_iter; iter++) {
                float cur_x2[4] = {};
                float cur_y2[4] = {};
                float sum2[4] = {};
                int rad_f[4] = {};

                #pragma clang loop vectorize(enable) unroll(disable)
                for (int i = 0; i < 4; i++) {
                    cur_x2[i] = cur_x[i] * cur_x[i];
                    cur_y2[i] = cur_y[i] * cur_y[i];
                    sum2[i] = cur_x2[i] + cur_y2[i];
                    rad_f[i] = (float) (sum2[i] <= 4.0f);
                }

                if (!((int)rad_f[0] || (int)rad_f[1] || (int)rad_f[2] || (int)rad_f[3])) {
                    break;
                }

                #pragma clang loop vectorize(enable) unroll(disable)
                for (int i = 0; i < 4; i++) {
                    float new_cur_y = 2.0f*cur_x[i]*cur_y[i] + y_0[i];
                    float new_cur_x = cur_x2[i] - cur_y2[i] + x_0[i];

                    cur_y[i] = rad_f[i] * new_cur_y + (1.0f - rad_f[i]) * cur_y[i];
                    cur_x[i] = rad_f[i] * new_cur_x + (1.0f - rad_f[i]) * cur_x[i];

                    iters[i] += (int)rad_f[i];
                }

                for (int i = 0; i < 4; i++) {
                    int color_i = Get_Mandelbrot_color(iters[i]);
                    total += color_i;
                }
            }
        }
    }
    printf("total: %lld", total);
}
