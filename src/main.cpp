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
    game.reset();

    Player spaceship{{0, 0,
                      static_cast<float>(Config::General::spaceship_size.x),
                      Config::General::spaceship_size.y * 1.5f},
                     TrashInfo::Type::METAL};

    spaceship.place_in_middle(game.get_bg_rec());

    // FPS cap
    window.SetTargetFPS(60);

    window.SetExitKey(KEY_ESCAPE);

    // Delta time
    float dt{};

    std::stringstream message{};
    bool show_message{false};
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
        } else if (game.did_lose()) {
            if (IsKeyPressed(KEY_Y)) {
                game.set_paused(false);
                game.reset();
                spaceship.reset();

                spaceship.place_in_middle(game.get_bg_rec());
            } else if (IsKeyPressed(KEY_N)) {
                exit_request = true;
            }
        } else {
            if (IsKeyPressed(KEY_P))
                game.set_paused(!game.is_paused());
        }

        if (IsKeyPressed(KEY_SPACE))
            game.reset_enemies(1);

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

            if (game.did_lose())
                game.set_paused(true);
        }
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        {
            window.BeginDrawing();

            window.ClearBackground(RAYWHITE);

            game.draw();

            spaceship.draw();

            if (exit_request) {
                DrawRectangle(0, 100, GetRenderWidth(), 200, BLACK);
                message << "Are you sure you want to exit? [Y/N]";
                show_message = true;
            } else if (game.did_lose()) {
                DrawRectangle(0, 100, GetRenderWidth(), 200, BLACK);
                message << "You lost! Wanna try again? [Y/N]";
                show_message = true;
            } else if (game.is_paused()) {
                DrawRectangle(0, 100, GetRenderWidth(), 200, BLACK);
                message << "Paused! Press P to unpause";
                show_message = true;
            }

            if (show_message) {
                DrawText(message.str().c_str(), 40, 180, 30, WHITE);

                show_message = false;
                message.str("");
            }

            window.EndDrawing();
        }
        //----------------------------------------------------------------------------------
    }
cleanup:

    return EXIT_SUCCESS;
}
