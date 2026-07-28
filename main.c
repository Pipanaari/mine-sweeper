#include "raylib.h"
#include <string.h>
#include <stdio.h>

void init(int bombCount, int gridWidth, int gridHeight, char tiles[], char activity[], int* dead) {
  memset(tiles, '0', gridWidth * gridHeight);
  memset(activity, 0, gridWidth * gridHeight);
  int *bombs = LoadRandomSequence(bombCount, 0, gridWidth * gridHeight - 1);

  *dead = 0;

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

void drawTile(Texture texture, int x, int y) {
  DrawTexturePro(
    texture, 
    (Rectangle){.height = texture.width, .width = texture.height, .x = 0, .y = 0},
    (Rectangle){.height = 48, .width = 48, .x = x, .y = y},
    (Vector2){.x = 0, .y = 0},
    0,
    WHITE
  );
}

Texture2D loadTexture(const char *fileName) {
  Image image = LoadImage(fileName);                // Loaded in CPU memory (RAM)
  Texture2D texture = LoadTextureFromImage(image);  // Image converted to texture, GPU memory (VRAM)
  UnloadImage(image);                               // Once image has been converted to texture and uploaded to VRAM, it can be unloaded from RAM

  return texture;
}

int main(void) {
    int gridWidth = 10, gridHeight = 10, cellSize = 48, gap = 0, bombCount = 10, dead = 0;
    char tiles[gridWidth * gridHeight];
    char activity[gridWidth * gridHeight];

    const int screenWidth = (cellSize + gap) * gridWidth - gap;
    const int screenHeight = (cellSize + gap) * gridHeight - gap;


    InitWindow(screenWidth, screenHeight, "Mine Sweeper");

    SetTargetFPS(60);

    init(bombCount, gridWidth, gridHeight, tiles, activity, &dead);

    Texture2D blank = loadTexture("textures/tile-unknown.png");
    Texture2D tile0 = loadTexture("textures/tile-0.png");
    Texture2D tile1 = loadTexture("textures/tile-1.png");
    Texture2D tile2 = loadTexture("textures/tile-2.png");
    Texture2D tile3 = loadTexture("textures/tile-3.png");
    Texture2D tile4 = loadTexture("textures/tile-4.png");
    Texture2D tile5 = loadTexture("textures/tile-5.png");
    Texture2D tile6 = loadTexture("textures/tile-6.png");
    Texture2D tile7 = loadTexture("textures/tile-7.png");
    Texture2D tile8 = loadTexture("textures/tile-8.png");
    Texture2D flag = loadTexture("textures/tile-flag.png");
    Texture2D bomb = loadTexture("textures/tile-bomb.png");
    Texture2D explode = loadTexture("textures/tile-bomb-detonated.png");
    Texture2D not_bomb = loadTexture("textures/tile-bomb-mismarked.png");


    while (!WindowShouldClose()) {
      if (IsKeyPressed(KEY_R)) {
        init(bombCount, gridWidth, gridHeight, tiles, activity, &dead);
      }

      BeginDrawing();

      ClearBackground(RAYWHITE);

      int mouseX = GetMouseX();
      int mouseY = GetMouseY();

      int mouseCellX = mouseX / (cellSize + gap);
      int mouseCellY = mouseY / (cellSize + gap);

      if (mouseX % (cellSize + gap) > cellSize || mouseCellX >= gridWidth) mouseCellX = -1;
      if (mouseY % (cellSize + gap) > cellSize || mouseCellY >= gridHeight) mouseCellY = -1;

      if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && dead == 0) {
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
      if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) && dead == 0) {
        if (mouseCellY != -1 && mouseCellX != -1 && activity[mouseCellY * gridWidth + mouseCellX] == 0) {
          activity[mouseCellY * gridWidth + mouseCellX] = 'F';
        }
        else if (mouseCellY != -1 && mouseCellX != -1 && activity[mouseCellY * gridWidth + mouseCellX] == 'F'){
          activity[mouseCellY * gridWidth + mouseCellX] = 0;
        }
      }

      for (int y = 0; y < gridHeight; y++) {
        for (int x = 0; x < gridWidth; x++) {
          drawTile(blank, x * (cellSize + gap), y * (cellSize + gap));

          if (activity[y * gridWidth + x] == 'F') {
            if (dead == 1) {
              if (tiles[y * gridWidth + x] != 'B') {
                drawTile(not_bomb, x * (cellSize + gap), y * (cellSize + gap));
              }
            }
            else {
              drawTile(flag, x * (cellSize + gap), y * (cellSize + gap));
            }
          }
          if (activity[y * gridWidth + x] == 0 && dead == 1) {
            if (tiles[y * gridWidth + x] == 'B') {
              drawTile(bomb, x * (cellSize + gap), y * (cellSize + gap));
            }
          }
          if (activity[y * gridWidth + x] == 'C') {
            if (tiles[y * gridWidth + x] == 'B') {
              dead = 1;
              drawTile(explode, x * (cellSize + gap), y * (cellSize + gap));
            }
            else {
              DrawRectangle(x * (cellSize + gap), y * (cellSize + gap), cellSize, cellSize, GRAY);
            }
            if (tiles[y * gridWidth + x] == '0') {
              // DrawText((char []){ tiles[y * gridWidth + x], '\0' }, x * (cellSize + gap), y * (cellSize + gap), cellSize, BLACK);
              drawTile(tile0, x * (cellSize + gap), y * (cellSize + gap));
            }
            if (tiles[y * gridWidth + x] == '1') {
              drawTile(tile1, x * (cellSize + gap), y * (cellSize + gap));
            }
            if (tiles[y * gridWidth + x] == '2') {
              drawTile(tile2, x * (cellSize + gap), y * (cellSize + gap));
            }
            if (tiles[y * gridWidth + x] == '3') {
              drawTile(tile3, x * (cellSize + gap), y * (cellSize + gap));
            }
            if (tiles[y * gridWidth + x] == '4') {
              drawTile(tile4, x * (cellSize + gap), y * (cellSize + gap));
            }
            if (tiles[y * gridWidth + x] == '5') {
              drawTile(tile5, x * (cellSize + gap), y * (cellSize + gap));
            }
            if (tiles[y * gridWidth + x] == '6') {
              drawTile(tile6, x * (cellSize + gap), y * (cellSize + gap));
            }
            if (tiles[y * gridWidth + x] == '7') {
              drawTile(tile7, x * (cellSize + gap), y * (cellSize + gap));
            }
            if (tiles[y * gridWidth + x] == '8') {
              drawTile(tile8, x * (cellSize + gap), y * (cellSize + gap));
            }
          }
        }
      }

      EndDrawing();
    }

    CloseWindow();
    return 0;
}
