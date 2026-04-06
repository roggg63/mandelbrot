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
            for (int x = 0; x < screen_width; x++) {
                float x_0 = min_x + x * scale_x;
                float y_0 = min_y + y * scale_y;

                Color color = Mandelbrot_color(x_0, y_0);

                ImageDrawPixel(&image, x, y, color);
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

Color Mandelbrot_color(float x_0, float y_0) {
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
        return BLACK;
    }
    else if (iter < max_iter / 3) {
        return WHITE;
    }
    else if (iter < 2 * (max_iter / 3)) {
        return GREEN;
    }
    return BLUE;
}

int main() {
    open_graphic_window();

    return 0;
}


//g++ patch.cpp -std=gnu++11 -I/opt/homebrew/include -L/opt/homebrew/lib -lraylib
