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
    game.set_current_screen(General::GameScreen::TITLE);

    Player spaceship{{0, 0,
                      static_cast<float>(Config::General::spaceship_size.x),
                      Config::General::spaceship_size.y * 1.5f},
                     TrashInfo::Type::METAL};

    spaceship.place_in_middle(game.get_bg_rec());
    spaceship.place_at_bottom(game.get_bg_rec());

    // FPS cap
    window.SetTargetFPS(60);

    window.SetExitKey(KEY_Q);

    // Delta time
    float dt{};

    std::stringstream message{};
    bool show_message{false};

    bool menu_request{false};
    //--------------------------------------------------------------------------------------

    // Game loop switch flag
    bool running{true};
    bool exit_request{false};

    while (running) {
        game.handle_input(window, &exit_request, &menu_request, &running,
                          spaceship);

        // Update
        //----------------------------------------------------------------------------------
        {

            if (!game.is_paused()) {
                dt = GetFrameTime();
            }

            if (window.IsResized()) {
                // Corrects positioning anomalies when resizing
                game.update_size(window);
                spaceship.place_at_bottom(game.get_bg_rec());
            }

            switch (game.get_current_screen()) {
                using enum General::GameScreen;
            case TITLE:
                game.update(dt, spaceship, window, &running);
                break;

            case INSTRUCTIONS:
                game.update(dt, spaceship, window, &running);
                break;

            case GAMEPLAY: {
                if (window.IsResized()) {
                    // Corrects positioning anomalies when resizing
                    // game.update_size(window);
                    spaceship.update_position(game.get_bg_rec());

                    game.set_paused(true);
                }

                if (!game.is_paused()) {
                    game.update(dt, spaceship, window, &running);

                    spaceship.update(game.get_bg_rec());
                    spaceship.handle_input(game.get_bg_rec(), dt);
                }

                if (game.did_lose())
                    game.set_paused(true);
            } break;

            default:
                break;
            }

            if (IsKeyPressed(KEY_SPACE))
                game.set_current_screen(General::GameScreen::GAMEPLAY, window);
        }
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        {
            window.BeginDrawing();

            switch (game.get_current_screen()) {
                using enum General::GameScreen;
            case TITLE:
                game.draw(spaceship);
                break;

            case INSTRUCTIONS:
                game.draw(spaceship);
                break;

            case GAMEPLAY: {
                game.draw(spaceship);

                spaceship.draw();

                if (exit_request) {
                    DrawRectangle(0, 100, GetRenderWidth(), 200, BLACK);
                    message << "Are you sure you want to exit? [Y/N]";
                    show_message = true;
                } else if (game.did_lose()) {
                    DrawRectangle(0, 100, GetRenderWidth(), 200, BLACK);
                    message << "You lost! Wanna try again? [Y/N]";
                    show_message = true;
                } else if (menu_request) {
                    DrawRectangle(0, 100, GetRenderWidth(), 200, BLACK);
                    message << "Quer voltar ao menu? [Y/N]";
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
            } break;

            default:
                break;
            }

            window.EndDrawing();
        }
        //----------------------------------------------------------------------------------
    }
cleanup:

    return EXIT_SUCCESS;
}
