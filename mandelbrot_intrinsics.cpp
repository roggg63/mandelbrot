#include "mandelbrot.h"

void open_graphic_window() {
    const int screen_width = 650;
    const int screen_height = 650;

    //const int x_0 = screen_width / 2;
    //const int y_0 = screen_height / 2;

    SetConfigFlags(FLAG_WINDOW_HIGHDPI);
    SetTargetFPS(120);

    InitWindow(screen_width, screen_height, "Mandelbrot visualisation");

    float scale_x = (max_x - min_x) / screen_width;
    float scale_y = (max_y - min_y) / screen_height;

    Texture2D texture = { 0 };

    while (!WindowShouldClose()) {
        int image_changed = 0;
        Image image = GenImageColor(screen_width, screen_height, BLACK);

        if (IsKeyDown(KEY_LEFT)) {
            min_x -= 0.05;
            max_x -= 0.05;
            image_changed = 1;
        }
        if (IsKeyDown(KEY_RIGHT)) {
            min_x += 0.05;
            max_x += 0.05;
            image_changed = 1;
        }
        if (IsKeyDown(KEY_DOWN)) {
            min_y += 0.05;
            max_y += 0.05;
            image_changed = 1;
        }
        if (IsKeyDown(KEY_UP)) {
            min_y -= 0.05;
            max_y -= 0.05;
            image_changed = 1;
        }
        if (IsKeyPressed(KEY_Z)){
            float zoom = 0.8;
            float center_x = (min_x + max_x) / 2.0;
            float center_y = (min_y + max_y) / 2.0;
            float new_width = (max_x - min_x) * zoom;
            float new_height = (max_y - min_y) * zoom;
            min_x = center_x - new_width / 2.0;
            max_x = center_x + new_width / 2.0;
            min_y = center_y - new_height / 2.0;
            max_y = center_y + new_height / 2.0;
            scale_x = (max_x - min_x) / screen_width;
            scale_y = (max_y - min_y) / screen_height;
            image_changed = 1;
        }
        if (IsKeyPressed(KEY_X)){
            float zoom = 1.2;
            float center_x = (min_x + max_x) / 2.0;
            float center_y = (min_y + max_y) / 2.0;
            float new_width = (max_x - min_x) * zoom;
            float new_height = (max_y - min_y) * zoom;
            min_x = center_x - new_width / 2.0;
            max_x = center_x + new_width / 2.0;
            min_y = center_y - new_height / 2.0;
            max_y = center_y + new_height / 2.0;
            scale_x = (max_x - min_x) / screen_width;
            scale_y = (max_y - min_y) / screen_height;
            image_changed = 1;
        }

        if (image_changed == 1) {
            UnloadTexture(texture);
        }

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
                    Color color = Get_Mandelbrot_color(final_iters[i]);
                    ImageDrawPixel(&image, x + i, y, color);
                }
            }
        }

        Texture2D texture = LoadTextureFromImage(image);
        UnloadImage(image);

        BeginDrawing();
        ClearBackground(BLACK);
            //for (int i = -x_0; i < x_0; i++) {
            //    for (int j = -y_0; j < y_0; j++) {
            //        int new_i = i + x_0;
            //        int new_j = j + y_0;
            //
            //        if (i * i + j * j <= radius*radius) {
            //            DrawPixel(new_i, new_j, RED);
            //        }
            //        else {
            //            DrawPixel(new_i, new_j, BLUE);
            //        }
            //    }
            //}
            DrawTexture(texture, 0, 0, WHITE);
            DrawFPS(10, 10);
        EndDrawing();
    }

    UnloadTexture(texture);
    CloseWindow();
}

Color Get_Mandelbrot_color(int iter) {
    Color color = WHITE;
    if (iter == max_iter) {
        color = BLACK;
    }
    else if (iter < max_iter / 3) {
        color = WHITE;
    }
    else if (iter < 2 * (max_iter / 3)) {
        color = GREEN;
    }
    else {
        color = BLUE;
    }
    return color;
}

int main() {
    open_graphic_window();

    return 0;
}


//g++ patch.cpp -std=gnu++11 -I/opt/homebrew/include -L/opt/homebrew/lib -lraylib
