// Using SDL2 to create an application window

#include "Game.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include <iostream>

static void quit_tutorial(int code)
{
    /*
     * Quit SDL so we can release the fullscreen
     * mode and restore the previous video settings,
     * etc.
     */
    SDL_Quit();

    /* Exit program. */
    exit(code);
}
static void handle_key_press(SDL_Keysym* keysym, bool pressed, Game* bf)
{
    Game::Action action = Game::ACTION_MOVE_LEFT;

    /*
     * We're only interested if 'Esc' has
     * been presssed.
     *
     * EXERCISE:
     * Handle the arrow keys and have that change the
     * viewing position/angle.
     */
    switch ((int)keysym->sym) {
    case SDLK_ESCAPE:
        if (pressed)
            quit_tutorial(0);
        break;
    case SDLK_SPACE:
        break;
    case SDLK_w:
        action = Game::ACTION_MOVE_UP;
        break;
    case SDLK_s:
        action = Game::ACTION_MOVE_DOWN;
        break;
    case SDLK_d:
        action = Game::ACTION_MOVE_RIGHT;
        break;
    case SDLK_a:
        action = Game::ACTION_MOVE_LEFT;
        break;
    case SDLK_i:
        action = Game::ACTION_SHOOT_UP;
        break;
    case SDLK_k:
        action = Game::ACTION_SHOOT_DOWN;
        break;
    case SDLK_l:
        action = Game::ACTION_SHOOT_RIGHT;
        break;
    case SDLK_j:
        action = Game::ACTION_SHOOT_LEFT;
        break;
    case SDLK_p:
        if (pressed)
            bf->togglePause();
        break;
    }

    if (!pressed)
        action = (Game::Action)((int)action + 1);

    bf->input(action, 0);
}
static void process_events(Game* bf)
{
    /* Our SDL event placeholder. */
    SDL_Event event;

    /* Grab all the events off the queue. */
    while (SDL_PollEvent(&event)) {

        switch (event.type) {
        case SDL_KEYDOWN:
        case SDL_KEYUP:
            /* Handle key presses. */
            handle_key_press(&event.key.keysym, event.type == SDL_KEYDOWN, bf);
            break;
        case SDL_QUIT:
            /* Handle quit requests (like Ctrl-c). */
            quit_tutorial(0);
            break;
        }

    }

}

int main(int argc, char* argv[])
{

    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* displayWindow;
    SDL_Renderer* displayRenderer;
    SDL_RendererInfo displayRendererInfo;

    /* Request opengl 3.2 context.
     * SDL doesn't have the ability to choose which profile at this time of writing,
     * but it should default to the core profile */
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

    /* Turn on double buffering with a 24bit Z buffer.
     * You may need to change this to 16 or 32 for your system */
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 2);

    SDL_CreateWindowAndRenderer(800, 600, SDL_WINDOW_OPENGL, &displayWindow, &displayRenderer);
    SDL_GetRendererInfo(displayRenderer, &displayRendererInfo);
    /*TODO: Check that we have OpenGL */
    if ((displayRendererInfo.flags & SDL_RENDERER_ACCELERATED) == 0 ||
            (displayRendererInfo.flags & SDL_RENDERER_TARGETTEXTURE) == 0) {
        /*TODO: Handle this. We have no render surface and not accelerated. */
    }

    SDL_GL_SetSwapInterval(1);

    std::cout << "Initialzing " << argv[0] << ". GL Ver: " << glGetString(GL_VERSION) << ", GLSL Ver: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

    //Display_InitGL();
    //Display_SetViewport(800, 600);

    glViewport(0, 0, 800, 600);
    // Create one player game now.
    Game bf(argc, argv);

    bf.init_gl();

    bf.play();

    /*
     * Now we want to begin our normal app process--
     * an event loop with a lot of redrawing.
     */
    while (1) {
        /* Process incoming events. */
        process_events(&bf);
        bf.tick();
        /* Draw the screen. */
        //draw_screen( &bf );
        bf.walk_gl();

        SDL_RenderPresent(displayRenderer);
    }
    // The window is open: enter program loop (see SDL_PollEvent)
    SDL_Delay(3000);  // Wait for 3000 milliseconds, for example

    // Close and destroy the window
    //SDL_DestroyWindow(window);

    // Clean up SDL2 and exit the program
    SDL_Quit();
    return 0;

}
