#include "raylib.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

Texture2D counter_blank;
Texture2D counter_0;
Texture2D counter_1;
Texture2D counter_2;
Texture2D counter_3;
Texture2D counter_4;
Texture2D counter_5;
Texture2D counter_6;
Texture2D counter_7;
Texture2D counter_8;
Texture2D counter_9;

void init(int bombCount, int gridWidth, int gridHeight, char tiles[], char activity[], int* dead, int* flagsLeft) {
  memset(tiles, '0', gridWidth * gridHeight);
  memset(activity, 0, gridWidth * gridHeight);
  int *bombs = LoadRandomSequence(bombCount, 0, gridWidth * gridHeight - 1);

  *dead = 0;
  *flagsLeft = bombCount;

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

void clickTile(int tileX, int tileY, int gridWidth, int gridHeight, char tiles[], char activity[], int* dead) {
  int unvisitedTilesLeft = 1, unvisitedPositions[gridWidth * gridHeight];

  int pos = tileY * gridWidth + tileX;
  activity[pos] = 'C';
  unvisitedPositions[0] = pos;

  while (unvisitedTilesLeft) {
    pos = unvisitedPositions[--unvisitedTilesLeft];

    if (tiles[pos] == 'B') *dead = 1;
    if (tiles[pos] != '0') continue;

    int x = pos % gridWidth;
    int y = pos / gridWidth;

    for (int dy = -1; dy <= 1; dy++) {
      for (int dx = -1; dx <= 1; dx++) {
        if (y + dy < 0 || y + dy == gridHeight || x + dx < 0 || x + dx == gridWidth) continue;

        pos = (y + dy) * gridWidth + (x + dx);
        if (activity[pos] == 'C') continue;

        activity[pos] = 'C';
        if (tiles[pos] == '0') unvisitedPositions[unvisitedTilesLeft++] = pos;
        if (tiles[pos] == 'B') *dead = 1;
      }
    }
  }
}

void drawTile(Texture texture, int x, int y) {
  DrawTexturePro(
    texture, 
    (Rectangle){.height = texture.height, .width = texture.width, .x = 0, .y = 0},
    (Rectangle){.height = 48, .width = 48, .x = x, .y = y},
    (Vector2){.x = 0, .y = 0},
    0,
    WHITE
  );
}

void drawCounterTile(Texture texture, int x, int y) {
  DrawTexturePro(
    texture, 
    (Rectangle){.height = texture.height, .width = texture.width, .x = 0, .y = 0},
    (Rectangle){.height = texture.height * 3, .width = texture.width * 3, .x = x, .y = y},
    (Vector2){.x = 0, .y = 0},
    0,
    WHITE
  );
}

void drawCounter(int x, int y, double value, int digits) {
  for (int i = 0; i < digits; i++) {
    double num = remainder(floor(value / pow(10, digits - i - 1)), 10);
    // Fix remainders under 10; For example 8 % 10 == -2
    if (num < 0) {
      num += 10;
    }

    if (value == 0.0 && i == digits - 1) drawCounterTile(counter_0, x + i * 39, y);
    else if (value < pow(10, digits - i - 1)) drawCounterTile(counter_blank, x + i * 39, y);
    else if (num == 0.0) drawCounterTile(counter_0, x + i * 39, y);
    else if (num == 1.0) drawCounterTile(counter_1, x + i * 39, y);
    else if (num == 2.0) drawCounterTile(counter_2, x + i * 39, y);
    else if (num == 3.0) drawCounterTile(counter_3, x + i * 39, y);
    else if (num == 4.0) drawCounterTile(counter_4, x + i * 39, y);
    else if (num == 5.0) drawCounterTile(counter_5, x + i * 39, y);
    else if (num == 6.0) drawCounterTile(counter_6, x + i * 39, y);
    else if (num == 7.0) drawCounterTile(counter_7, x + i * 39, y);
    else if (num == 8.0) drawCounterTile(counter_8, x + i * 39, y);
    else if (num == 9.0) drawCounterTile(counter_9, x + i * 39, y);
  }
}


Texture2D loadTexture(const char *fileName) {
  Image image = LoadImage(fileName);                // Loaded in CPU memory (RAM)
  Texture2D texture = LoadTextureFromImage(image);  // Image converted to texture, GPU memory (VRAM)
  UnloadImage(image);                               // Once image has been converted to texture and uploaded to VRAM, it can be unloaded from RAM

  if (!IsTextureValid(texture)) {
    printf("Error loading texture \"%s\"", fileName);
    exit(EXIT_FAILURE);
  }

  return texture;
}

int main(void) {
    int gridWidth = 20, gridHeight = 10, cellSize = 48, gap = 0, bombCount = 40, dead = 0, flagsLeft = bombCount;
    int bottomMargin = 119;
    char tiles[gridWidth * gridHeight];
    char activity[gridWidth * gridHeight];

    const int screenWidth = (cellSize + gap) * gridWidth - gap;
    const int screenHeight = (cellSize + gap) * gridHeight - gap + bottomMargin;


    InitWindow(screenWidth, screenHeight, "Mine Sweeper");

    Image icon = LoadImage("textures/icon.png");
    SetWindowIcon(icon);

    SetTargetFPS(60);

    init(bombCount, gridWidth, gridHeight, tiles, activity, &dead, &flagsLeft);

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

    counter_blank = loadTexture("textures/counter-empty.png");
    counter_0 = loadTexture("textures/counter-0.png");
    counter_1 = loadTexture("textures/counter-1.png");
    counter_2 = loadTexture("textures/counter-2.png");
    counter_3 = loadTexture("textures/counter-3.png");
    counter_4 = loadTexture("textures/counter-4.png");
    counter_5 = loadTexture("textures/counter-5.png");
    counter_6 = loadTexture("textures/counter-6.png");
    counter_7 = loadTexture("textures/counter-7.png");
    counter_8 = loadTexture("textures/counter-8.png");
    counter_9 = loadTexture("textures/counter-9.png");

    while (!WindowShouldClose()) {
      if (IsKeyPressed(KEY_R)) {
        init(bombCount, gridWidth, gridHeight, tiles, activity, &dead, &flagsLeft);
      }

      BeginDrawing();

      ClearBackground((Color){.r = 192, .g = 192, .b = 192, .a = 1});

      int mouseX = GetMouseX();
      int mouseY = GetMouseY();

      int mouseCellX = mouseX / (cellSize + gap);
      int mouseCellY = mouseY / (cellSize + gap);

      int prevActivity = activity[mouseCellY * gridWidth + mouseCellX];

      if (mouseX % (cellSize + gap) > cellSize || mouseCellX >= gridWidth) mouseCellX = -1;
      if (mouseY % (cellSize + gap) > cellSize || mouseCellY >= gridHeight) mouseCellY = -1;

      if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && dead == 0) {
        if (mouseCellY != -1 && mouseCellX != -1 && activity[mouseCellY * gridWidth + mouseCellX] != 'F') {
          clickTile(mouseCellX, mouseCellY, gridWidth, gridHeight, tiles, activity, &dead);
        }
      }
      if (IsMouseButtonReleased(MOUSE_BUTTON_RIGHT) && dead == 0) {
        if (flagsLeft > 0 && mouseCellY != -1 && mouseCellX != -1 && activity[mouseCellY * gridWidth + mouseCellX] == 0) {
          activity[mouseCellY * gridWidth + mouseCellX] = 'F';
          flagsLeft--;
        }
        else if (mouseCellY != -1 && mouseCellX != -1 && activity[mouseCellY * gridWidth + mouseCellX] == 'F'){
          activity[mouseCellY * gridWidth + mouseCellX] = 0;
          flagsLeft++;
        }
      }

      for (int y = 0; y < gridHeight; y++) {
        for (int x = 0; x < gridWidth; x++) {
          drawTile(blank, x * (cellSize + gap), y * (cellSize + gap));

          if (activity[y * gridWidth + x] == 'F') {
            if (dead == 1 && (tiles[y * gridWidth + x] != 'B')) {
              drawTile(not_bomb, x * (cellSize + gap), y * (cellSize + gap));
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
              drawTile(explode, x * (cellSize + gap), y * (cellSize + gap));
            }
            else {
              DrawRectangle(x * (cellSize + gap), y * (cellSize + gap), cellSize, cellSize, GRAY);
            }
            if (tiles[y * gridWidth + x] == '0') {
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

      // Preview and open neighboring tiles when clicking
      if (dead == 0 && prevActivity == 'C') {
        int howManyFlagsNearCursor = '0';
        // We need to calculate howManyFlagsNearCursor when mouse is released
        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) || IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
          for (int dy = -1; dy <= 1; dy++) {
            for (int dx = -1; dx <= 1; dx++) {
              int y = mouseCellY + dy;
              int x = mouseCellX + dx;
              if (y < 0 || y == gridHeight || x < 0 || x == gridWidth) continue;

              int pos = y * gridWidth + x;
              if (activity[pos] == 0) {
                drawTile(tile0, x * (cellSize + gap), y * (cellSize + gap));
              }
              if (activity[pos] == 'F') {
                howManyFlagsNearCursor++;
              }
            }
          }
        }

        // If flags near cursor is same as the clicked tile number, click all neighboring tiles
        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON) && howManyFlagsNearCursor == tiles[mouseCellY * gridWidth + mouseCellX]) {
          for (int dy = -1; dy <= 1; dy++) {
            for (int dx = -1; dx <= 1; dx++) {
              int y = mouseCellY + dy;
              int x = mouseCellX + dx;
              if (y < 0 || y == gridHeight || x < 0 || x == gridWidth) continue;

              int pos = y * gridWidth + x;
              if (activity[pos] == 'F') continue;
              clickTile(x, y, gridWidth, gridHeight, tiles, activity, &dead);
              if (tiles[pos] == 'B') dead = 1;
            }
          }
        }
      }

      if (dead == 0 && IsMouseButtonDown(MOUSE_LEFT_BUTTON) && activity[mouseCellY * gridWidth + mouseCellX] == 0) {
        drawTile(tile0, mouseCellX * (cellSize + gap), mouseCellY * (cellSize + gap));
      }

      drawCounter(cellSize / 2, gridHeight * cellSize + cellSize / 2, flagsLeft, 3);

      EndDrawing();
    }

    CloseWindow();
    return 0;
}
