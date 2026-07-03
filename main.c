#include "raylib.h"

int main(void) {
    const int screenWidth = 800;
    const int screenHeight = 450;

    int gridWidth = 10, gridHeight = 10, cellSize = 20, gap = 1;

    InitWindow(screenWidth, screenHeight, "Mine Sweeper");

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
      BeginDrawing();


      for(int y = 0; y < gridHeight; y++){
        for(int x = 0; x < gridWidth; x++){
          DrawRectangle(x * (cellSize + gap), y * (cellSize + gap), cellSize, cellSize, BLUE);
        }
      }

      ClearBackground(RAYWHITE);
      DrawText("Hello World!", 370, 200, 20, LIGHTGRAY);

      EndDrawing();
    }

    CloseWindow();
    return 0;
}
