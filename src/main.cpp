#include "Game.h"
#include "Player.h"
#include "Trash.h"
#include "Utils/Globals.h"
#include <cstdlib>
#include <raylib-cpp.hpp>
#include <raylib.h>
#include <sstream>
#include <string>

namespace Rlib = raylib;

int main() {
    // Initialization
    //--------------------------------------------------------------------------------------
    Rlib::Window window{800, 400, "SpaceRecycle"};

    Game game{};
    game.update_size(window);
    game.reset_enemies(1);

    float spaceship_w{50};
    Player spaceship{{game.get_bg_rec().x + (game.get_bg_rec().width / 2) -
                          spaceship_w / 2.f,
                      game.get_bg_rec().y + game.get_bg_rec().height -
                          (spaceship_w * 1.5f) / 2.f,
                      spaceship_w, spaceship_w * 1.5f},
                     TrashInfo::Type::METAL};

    // FPS cap
    window.SetTargetFPS(60);

    window.SetExitKey(KEY_ESCAPE);

    // Delta time
    float dt{};
    //--------------------------------------------------------------------------------------

    // Game loop switch flag
    bool running{true};
    bool exit_request{false};

    while (running) {
        if (window.ShouldClose()) {
            exit_request = true;
            game.set_paused(true);
        }

        if (exit_request) {
            if (IsKeyPressed(KEY_Y))
                running = false;
            else if (IsKeyPressed(KEY_N)) {
                exit_request = false;
                game.set_paused(false);
            }
        }

        if (IsKeyPressed(KEY_SPACE))
            game.reset_enemies(1);

        if (IsKeyPressed(KEY_P))
            game.set_paused(!game.is_paused());

        // Update
        //----------------------------------------------------------------------------------
        {
            if (window.IsResized()) {
                // Corrects positioning anomalies when resizing
                game.update_size(window);
                spaceship.update(game.get_bg_rec());

                game.set_paused(true);
            }

            if (!game.is_paused()) {
                dt = GetFrameTime();

                game.update(dt, spaceship);

                spaceship.update(game.get_bg_rec());

                spaceship.handle_input(game.get_bg_rec(), dt);
            }
        }
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        {
            window.BeginDrawing();

            window.ClearBackground(RAYWHITE);

            game.draw();
            std::stringstream a{"Points: "};
            a << spaceship.get_points();

            // DrawRectangle(0, 100, GetRenderWidth(), 200, BLACK);
            // DrawText(a.str().c_str(), 40, 180, 30, WHITE);
            spaceship.draw();

            if (exit_request) {
                DrawRectangle(0, 100, GetRenderWidth(), 200, BLACK);
                DrawText("Are you sure you want to exit? [Y/N]", 40, 180, 30,
                         WHITE);
            } else if (game.is_paused()) {
                DrawRectangle(0, 100, GetRenderWidth(), 200, BLACK);
                DrawText("Paused! Press P to unpause", 40, 180, 30, WHITE);
            }

            window.EndDrawing();
        }
        //----------------------------------------------------------------------------------
    }
cleanup:

    return EXIT_SUCCESS;
}
