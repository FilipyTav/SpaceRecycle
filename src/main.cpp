#include <cstdlib>
#include <raylib-cpp.hpp>
#include <string>

const std::string root_path = std::string(GetApplicationDirectory()) + "../";
namespace Rlib = raylib;

int main() {
    // Initialization
    //--------------------------------------------------------------------------------------
    Rlib::Window window{800, 400, "Template"};

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
            //
        }
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        {
            BeginDrawing();

            ClearBackground(RAYWHITE);

            if (exit_request) {
                DrawRectangle(0, 100, GetRenderWidth(), 200, BLACK);
                DrawText("Are you sure you want to exit? [Y/N]", 40, 180, 30,
                         WHITE);
            }

            DrawText("Template", GetRenderWidth() / 2, GetRenderHeight() / 2,
                     40, LIGHTGRAY);

            EndDrawing();
        }
        //----------------------------------------------------------------------------------
    }
cleanup:

    return EXIT_SUCCESS;
}
