#include "Game.h"
#include "Player.h"
#include <cstdlib>
#include <raylib-cpp.hpp>
#include <string>

const std::string root_path = std::string(GetApplicationDirectory()) + "../";
namespace Rlib = raylib;

int main() {
    // Initialization
    //--------------------------------------------------------------------------------------
    Rlib::Window window{800, 400, "SpaceRecycle"};
    Game game{};
    game.update(window);

    Player spaceship{
        {game.get_bg_rec().x + (game.get_bg_rec().width / 2) - 20 / 2.f,
         game.get_bg_rec().y + game.get_bg_rec().height - 20 / 2.f, 20, 20}};

    // FPS cap
    window.SetTargetFPS(60);

    window.SetExitKey(KEY_Q);
    //--------------------------------------------------------------------------------------

    // Game loop switch flag
    bool running{true};
    bool exit_request{false};

    while (running) {
        if (window.ShouldClose())
            exit_request = true;

        if (exit_request) {
            if (IsKeyPressed(KEY_Y))
                running = false;
            else if (IsKeyPressed(KEY_N))
                exit_request = false;
        }

        // Update
        //----------------------------------------------------------------------------------
        {
            game.update(window);
            spaceship.update(game);
        }
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        {
            window.BeginDrawing();

            window.ClearBackground(RAYWHITE);

            if (exit_request) {
                DrawRectangle(0, 100, GetRenderWidth(), 200, BLACK);
                DrawText("Are you sure you want to exit? [Y/N]", 40, 180, 30,
                         WHITE);
            }

            game.get_bg_rec().Draw(BLUE);
            spaceship.get_hitbox().Draw(RED);

            window.EndDrawing();
        }
        //----------------------------------------------------------------------------------
    }
cleanup:

    return EXIT_SUCCESS;
}
