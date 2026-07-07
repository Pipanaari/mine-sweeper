#include "raylib.h"
#include <string.h>
#include <stdio.h>

void init(int bombCount, int gridWidth, int gridHeight, char tiles[], char activity[]) {
  memset(tiles, '0', gridWidth * gridHeight);
  memset(activity, 0, gridWidth * gridHeight);
  int *bombs = LoadRandomSequence(bombCount, 0, gridWidth * gridHeight - 1);

  for (int i = 0; i < bombCount; i++) {

    int x = bombs[i] % gridWidth;
    int y = bombs[i] / gridWidth;

    tiles[bombs[i]] = 'B';
    for (int j = -1; j <= 1; j++) {
      for (int k = -1; k <= 1; k++) {
        if (y + j < 0 || y + j == gridHeight || x + k < 0 || x + k == gridWidth) continue;

        int pos = (y + j) * gridWidth + (x + k);
        if (tiles[pos] != 'B') tiles[pos]++;
      }
    }
  }

  UnloadRandomSequence(bombs);
}

int main(void) {
    const int screenWidth = 800;
    const int screenHeight = 450;

    int gridWidth = 10, gridHeight = 10, cellSize = 40, gap = 1, bombCount = 10;
    char tiles[gridWidth * gridHeight];
    char activity[gridWidth * gridHeight];

    InitWindow(screenWidth, screenHeight, "Mine Sweeper");

    SetTargetFPS(60);

    init(bombCount, gridWidth, gridHeight, tiles, activity);

    while (!WindowShouldClose()) {
      if (IsKeyPressed(KEY_R)) {
        init(bombCount, gridWidth, gridHeight, tiles, activity);
      }

      BeginDrawing();

      ClearBackground(RAYWHITE);

      int mouseX = GetMouseX();
      int mouseY = GetMouseY();

      int mouseCellX = mouseX / (cellSize + gap);
      int mouseCellY = mouseY / (cellSize + gap);

      if (mouseX % (cellSize + gap) > cellSize || mouseCellX >= gridWidth) mouseCellX = -1;
      if (mouseY % (cellSize + gap) > cellSize || mouseCellY >= gridHeight) mouseCellY = -1;

      if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        if (mouseCellY != -1 && mouseCellX != -1 && activity[mouseCellY * gridWidth + mouseCellX] != 'F') {
          int unvisitedTilesLeft = 1, unvisitedPositions[gridWidth * gridHeight];

          int pos = mouseCellY * gridWidth + mouseCellX;
          activity[pos] = 'C';
          unvisitedPositions[0] = pos;

          while (unvisitedTilesLeft) {
            pos = unvisitedPositions[--unvisitedTilesLeft];

            if (tiles[pos] != '0') continue;

            int x = pos % gridWidth;
            int y = pos / gridWidth;

            for (int j = -1; j <= 1; j++) {
              for (int k = -1; k <= 1; k++) {
                if (y + j < 0 || y + j == gridHeight || x + k < 0 || x + k == gridWidth) continue;

                pos = (y + j) * gridWidth + (x + k);
                if (activity[pos] == 'C') continue;

                activity[pos] = 'C';
                if (tiles[pos] == '0') unvisitedPositions[unvisitedTilesLeft++] = pos;
              }
            }
          }
        }
      }
      if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
        if (mouseCellY != -1 && mouseCellX != -1 && activity[mouseCellY * gridWidth + mouseCellX] == 0) {
          activity[mouseCellY * gridWidth + mouseCellX] = 'F';
        }
        else if (mouseCellY != -1 && mouseCellX != -1 && activity[mouseCellY * gridWidth + mouseCellX] == 'F'){
          activity[mouseCellY * gridWidth + mouseCellX] = 0;
        }
      }

      for (int y = 0; y < gridHeight; y++) {
        for (int x = 0; x < gridWidth; x++) {
          DrawRectangle(x * (cellSize + gap), y * (cellSize + gap), cellSize, cellSize, BLUE);

          if (activity[y * gridWidth + x] == 'F'){
            DrawRectangle(x * (cellSize + gap), y * (cellSize + gap), cellSize, cellSize, GREEN);
            DrawText("F", x * (cellSize + gap), y * (cellSize + gap), cellSize, BLACK);
          }
          if (activity[y * gridWidth + x] == 'C') {
            if (tiles[y * gridWidth + x] == 'B') {
              DrawRectangle(x * (cellSize + gap), y * (cellSize + gap), cellSize, cellSize, RED);
            }
            else {
              DrawRectangle(x * (cellSize + gap), y * (cellSize + gap), cellSize, cellSize, GRAY);
            }
            if (tiles[y * gridWidth + x] != '0'){
              DrawText((char []){ tiles[y * gridWidth + x], '\0' }, x * (cellSize + gap), y * (cellSize + gap), cellSize, BLACK);
            }
          }
        }
      }


      EndDrawing();
    }

    CloseWindow();
    return 0;
}
