#include "raylib.h"
#include <string.h>
#include <stdio.h>

int main(void) {
    const int screenWidth = 800;
    const int screenHeight = 450;

    int gridWidth = 10, gridHeight = 10, cellSize = 40, gap = 1, bombCount = 10;
    int tiles[gridWidth * gridHeight];
    memset(tiles, '0', sizeof tiles);

    InitWindow(screenWidth, screenHeight, "Mine Sweeper");

    SetTargetFPS(60);

    int *bombs = LoadRandomSequence(bombCount, 0, gridWidth * gridHeight - 1);

    for (int i = 0; i < bombCount; i++) {

      int x = bombs[i] % gridWidth;
      int y = bombs[i] / gridWidth;

      tiles[bombs[i]] = 'B';
      for (int j = -1; j < 2; j++) {
        for (int k = -1; k < 2; k++) {
          if (y + j < 0 || y + j == gridHeight || x + k < 0 || x + k == gridWidth) continue;

          int pos = (y + j) * gridWidth + (x + k);
          if (tiles[pos] != 'B') tiles[pos]++;
        }
      }
    }

    while (!WindowShouldClose()) {
      BeginDrawing();

      ClearBackground(RAYWHITE);

      for (int y = 0; y < gridHeight; y++) {
        for (int x = 0; x < gridWidth; x++) {
          if (tiles[y * gridWidth + x] == 'B') {
            DrawRectangle(x * (cellSize + gap), y * (cellSize + gap), cellSize, cellSize, RED);
          }
          else {
            DrawRectangle(x * (cellSize + gap), y * (cellSize + gap), cellSize, cellSize, BLUE);
          }

          char tile[2];
          tile[0] = tiles[y * gridWidth + x];
          DrawText(tile, x * (cellSize + gap), y * (cellSize + gap), cellSize, BLACK);
        }
      }


      EndDrawing();
    }

    CloseWindow();
    return 0;
}
