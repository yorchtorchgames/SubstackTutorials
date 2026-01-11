#include <SDL3/SDL.h>

int main()
{
    if(!SDL_Init(SDL_INIT_VIDEO)){
        SDL_Log("Failed to initialize video! %s", SDL_GetError());
        return 1;
    }

    // Step 1: Create a window and renderer using SDL_CreateWindowAndRenderer.
    // This function combines window and renderer creation into a single call for convenience.
    // It returns true on success, false on failure, setting window and renderer pointers accordingly.
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    if (!SDL_CreateWindowAndRenderer("Simple Game Loop", 800, 600, 0, &window, &renderer)) {
        SDL_Log("Failed to create window and renderer: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // Step 2: Set up game variables.
    // We'll have a red square that can be moved with arrow keys.
    // Position and size of the square. Using floats for position to allow subpixel precision.
    float squareX = 100.0f;
    float squareY = 100.0f;
    float squareSize = 50.0f;
    float moveSpeed = 5.0f;  // Pixels to move per frame.

    // Get window dimensions as floats for clamping.
    int windowWidthInt, windowHeightInt;
    SDL_GetWindowSize(window, &windowWidthInt, &windowHeightInt);
    float windowWidth = static_cast<float>(windowWidthInt);
    float windowHeight = static_cast<float>(windowHeightInt);

    // Movement flags (true when key is held down).
    bool movingUp = false;
    bool movingDown = false;
    bool movingLeft = false;
    bool movingRight = false;

    // Step 3: Game loop setup.
    // A game loop typically runs continuously until the user quits.
    // It consists of:
    // - Handling input/events.
    // - Updating game state (e.g., moving objects).
    // - Rendering graphics.
    // - Managing frame rate for consistency.
    //
    // For stable frame rates, we'll target 60 FPS (frames per second).
    // This means each frame should take about 16.67 ms (1000 ms / 60).
    // We'll measure time spent on the frame and delay if it's too fast.
    bool running = true;
    const float targetFrameTimeMS = 1000.0f / 60.0f;  // ~16.67 ms for 60 FPS.

    while (running) {
        // Measure start time for this frame (using high-performance counter).
        Uint64 frameStart = SDL_GetPerformanceCounter();

        // Step 4: Poll for events.
        // SDL_PollEvent checks the event queue and processes one event at a time.
        // We loop until the queue is empty.
        // Common events: quit (window close), key presses/releases.
        // Comments: Polling events is crucial to handle user input and system messages.
        // Without this, the window might become unresponsive.
        // We use SDL_Event to store the event data.
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_EVENT_QUIT:
                    // User closed the window or pressed quit.
                    running = false;
                    break;
                case SDL_EVENT_KEY_DOWN:
                    // Key pressed: Set movement flags.
                    // We check scancode for hardware-independent keys.
                    switch (event.key.scancode) {
                        case SDL_SCANCODE_W:
                            movingUp = true;
                            break;
                        case SDL_SCANCODE_S:
                            movingDown = true;
                            break;
                        case SDL_SCANCODE_A:
                            movingLeft = true;
                            break;
                        case SDL_SCANCODE_D:
                            movingRight = true;
                            break;
                        default:
                            break;
                    }
                    break;
                case SDL_EVENT_KEY_UP:
                    // Key released: Clear movement flags.
                    switch (event.key.scancode) {
                        case SDL_SCANCODE_W:
                            movingUp = false;
                            break;
                        case SDL_SCANCODE_S:
                            movingDown = false;
                            break;
                        case SDL_SCANCODE_A:
                            movingLeft = false;
                            break;
                        case SDL_SCANCODE_D:
                            movingRight = false;
                            break;
                        default:
                            break;
                    }
                    break;
                default:
                    break;
            }
        }

        // Step 5: Update game state.
        // Apply movement based on flags.
        // This happens every frame, so holding a key moves continuously.
        // We also clamp position to stay within window bounds.
        if (movingUp) squareY -= moveSpeed;
        if (movingDown) squareY += moveSpeed;
        if (movingLeft) squareX -= moveSpeed;
        if (movingRight) squareX += moveSpeed;

        // Clamp to window (prevent going off-screen).
        if (squareX < 0.0f) squareX = 0.0f;
        if (squareY < 0.0f) squareY = 0.0f;
        if (squareX + squareSize > windowWidth) squareX = windowWidth - squareSize;
        if (squareY + squareSize > windowHeight) squareY = windowHeight - squareSize;

        // Step 6: Render the frame.
        // Clear the screen to black.
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Draw the red square using SDL_FRect for floating-point precision.
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_FRect squareRect = { squareX, squareY, squareSize, squareSize };
        SDL_RenderFillRect(renderer, &squareRect);

        // Present the rendered frame to the screen.
        SDL_RenderPresent(renderer);

        // Step 7: Calculate stable frame rate.
        // Measure time elapsed for this frame.
        // Comments: To achieve a stable frame rate, we calculate how long the frame took
        // and delay if it was faster than the target. This prevents the game from running
        // too fast on powerful hardware. SDL_GetPerformanceCounter() and Frequency() provide
        // high-resolution timing. If the frame took longer than target, no delay (catch up next frame).
        Uint64 frameEnd = SDL_GetPerformanceCounter();
        float elapsedMS = (frameEnd - frameStart) / (float)SDL_GetPerformanceFrequency() * 1000.0f;

        if (elapsedMS < targetFrameTimeMS) {
            SDL_Delay((Uint32)(targetFrameTimeMS - elapsedMS));
        }
    }

    // Step 8: Cleanup.
    // Destroy resources and quit SDL.
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}