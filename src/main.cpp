#include "raylib.h"
#include "game.h"

int windowHeight = 700;
int windowWidth = 700;
int main()
{
    Color backgroundColor={29,29,27,255};
    InitWindow(windowWidth, windowHeight, "Game");
    Game game;
    SetTargetFPS(60);
    while (WindowShouldClose() == false)
    {
        game.Update();
        game.HandleInput();
        BeginDrawing();
        ClearBackground(backgroundColor);
        game.Draw();
        EndDrawing();
    }
    CloseWindow();
    return 0;
}