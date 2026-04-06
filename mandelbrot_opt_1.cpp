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
            for (int x = 0; x < screen_width; x+=4) {
                float x0_1 = min_x + (x + 0) * scale_x;
                float x0_2 = min_x + (x + 1) * scale_x;
                float x0_3 = min_x + (x + 2) * scale_x;
                float x0_4 = min_x + (x + 3) * scale_x;

                int it1 = 0;
                int it2 = 0;
                int it3 = 0;
                int it4 = 0;

                float cur_x1 = 0, cur_y1 = 0;
                float cur_x2 = 0, cur_y2 = 0;
                float cur_x3 = 0, cur_y3 = 0;
                float cur_x4 = 0, cur_y4 = 0;

                for (int iter = 0; iter < max_iter; iter++) {
                    float cur_x1_2 = cur_x1 * cur_x1;
                    float cur_x2_2 = cur_x2 * cur_x2;
                    float cur_x3_2 = cur_x3 * cur_x3;
                    float cur_x4_2 = cur_x4 * cur_x4;

                    float cur_y1_2 = cur_y1 * cur_y1;
                    float cur_y2_2 = cur_y2 * cur_y2;
                    float cur_y3_2 = cur_y3 * cur_y3;
                    float cur_y4_2 = cur_y4 * cur_y4;

                    int res_1 = (cur_x1_2 + cur_y1_2 <= 4.0f);
                    int res_2 = (cur_x2_2 + cur_y2_2 <= 4.0f);
                    int res_3 = (cur_x3_2 + cur_y3_2 <= 4.0f);
                    int res_4 = (cur_x4_2 + cur_y4_2 <= 4.0f);

                    if (!(res_1 || res_2 || res_3 || res_4)) {
                        break;
                    }

                    cur_y1 = res_1 ? (2.0f*cur_x1*cur_y1 + y_0) : cur_y1;
                    cur_x1 = res_1 ? (cur_x1_2 - cur_y1_2 + x0_1) : cur_x1;
                    it1 += res_1;

                    cur_y2 = res_2 ? (2.0f*cur_x2*cur_y2 + y_0) : cur_y2;
                    cur_x2 = res_2 ? (cur_x2_2 - cur_y2_2 + x0_2) : cur_x2;
                    it2 += res_2;

                    cur_y3 = res_3 ? (2.0f*cur_x3*cur_y3 + y_0) : cur_y3;
                    cur_x3 = res_3 ? (cur_x3_2 - cur_y3_2 + x0_3) : cur_x3;
                    it3 += res_3;

                    cur_y4 = res_4 ? (2.0f*cur_x4*cur_y4 + y_0) : cur_y4;
                    cur_x4 = res_4 ? (cur_x4_2 - cur_y4_2 + x0_4) : cur_x4;
                    it4 += res_4;
                }

                Color color_1 = Get_Mandelbrot_color(it1);
                Color color_2 = Get_Mandelbrot_color(it2);
                Color color_3 = Get_Mandelbrot_color(it3);
                Color color_4 = Get_Mandelbrot_color(it4);

                ImageDrawPixel(&image, x+0, y, color_1);
                ImageDrawPixel(&image, x+1, y, color_2);
                ImageDrawPixel(&image, x+2, y, color_3);
                ImageDrawPixel(&image, x+3, y, color_4);
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
