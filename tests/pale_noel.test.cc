#include <raylib.h>

int main()
{
  InitWindow(440, 320, "Pale Noel");

  SetTargetFPS(12);

  while (!WindowShouldClose())
  {
    BeginDrawing();
    ClearBackground(BLANK);
    EndDrawing();
  }

  CloseWindow();
}
