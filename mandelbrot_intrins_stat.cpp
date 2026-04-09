#include <stdio.h>
#include <stdlib.h>
#include <arm_neon.h>

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


int main() {
    const int screen_width = 650;
    const int screen_height = 650;

    float scale_x = (max_x - min_x) / screen_width;
    float scale_y = (max_y - min_y) / screen_height;

    for (int y = 0; y < screen_height; y++) {
        float y_0 = min_y + y * scale_y;
        float32x4_t vect_y0 = vdupq_n_f32(y_0);
        for (int x = 0; x < screen_width; x+=4) {
            float x0_vals[4] = {
                min_x + (x + 0) * scale_x,
                min_x + (x + 1) * scale_x,
                min_x + (x + 2) * scale_x,
                min_x + (x + 3) * scale_x
            };
            float32x4_t vect_x0 = vld1q_f32(x0_vals);

            float32x4_t vect_cur_x = vdupq_n_f32(0.0f);
            float32x4_t vect_cur_y = vdupq_n_f32(0.0f);
            uint32x4_t vect_iters = vdupq_n_u32(0);

            float32x4_t vect_four = vdupq_n_f32(4.0f);
            float32x4_t vect_two  = vdupq_n_f32(2.0f);
            uint32x4_t vect_one   = vdupq_n_u32(1);

            for (int iter = 0; iter < max_iter; iter++) {
                float32x4_t vect_cur_x_2 = vmulq_f32(vect_cur_x, vect_cur_x);

                float32x4_t vect_cur_y_2 = vmulq_f32(vect_cur_y, vect_cur_y);

                float32x4_t vect_cur_x_2_p_y_2 = vaddq_f32(vect_cur_x_2, vect_cur_y_2);

                uint32x4_t vect_res = vcleq_f32(vect_cur_x_2_p_y_2, vect_four);

                vect_iters = vaddq_f32(vect_iters, vandq_u32(vect_res, vect_one));

                if (vmaxvq_u32(vect_res) == 0) {
                    break;
                }

                float32x4_t vect_cur_xy = vmulq_f32(vect_cur_x, vect_cur_y);
                float32x4_t vect_cur_2xy = vmulq_f32(vect_cur_xy, vect_two);
                float32x4_t vect_next_y = vaddq_f32(vect_cur_2xy, vect_y0);

                float32x4_t vect_x_2_sub_y_2 = vsubq_f32(vect_cur_x_2, vect_cur_y_2);
                float32x4_t vect_next_x = vaddq_f32(vect_x_2_sub_y_2, vect_x0);

                vect_cur_x = vect_next_x;
                vect_cur_y = vect_next_y;
            }

            uint32_t final_iters[4];
            vst1q_u32(final_iters, vect_iters);

            for (int i = 0; i < 4; i++) {
                int color = Get_Mandelbrot_color(final_iters[i]);
            }
        }
    }
}
