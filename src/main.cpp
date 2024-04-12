#include "Game.h"
#include "Player.h"
#include "Trash.h"
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

    float spaceship_w{50};
    Player spaceship{{game.get_bg_rec().x + (game.get_bg_rec().width / 2) -
                          spaceship_w / 2.f,
                      game.get_bg_rec().y + game.get_bg_rec().height -
                          (spaceship_w * 1.5f) / 2.f,
                      spaceship_w, spaceship_w * 1.5f}};

    // FPS cap
    window.SetTargetFPS(60);

    window.SetExitKey(KEY_Q);

    // Delta time
    float dt{};
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
            dt = GetFrameTime();

            if (window.IsResized())
                game.update(window);
            spaceship.update(game.get_bg_rec());

            spaceship.handle_input(game.get_bg_rec(), dt);
        }
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        {
            window.BeginDrawing();

            window.ClearBackground(RAYWHITE);

            game.get_bg_rec().Draw(BLUE);
            spaceship.get_hitbox().Draw(RED);

            if (exit_request) {
                DrawRectangle(0, 100, GetRenderWidth(), 200, BLACK);
                DrawText("Are you sure you want to exit? [Y/N]", 40, 180, 30,
                         WHITE);
            }

            window.EndDrawing();
        }
        //----------------------------------------------------------------------------------
    }
cleanup:

    return EXIT_SUCCESS;
}
