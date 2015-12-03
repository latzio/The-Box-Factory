/*
 * SDL OpenGL Tutorial.
 * (c) Michael Vance, 2000
 * briareos@lokigames.com
 *
 * Distributed under terms of the LGPL.
 */

#include "Game.h"
#include <SDL/SDL.h>
#include <GL/gl.h>

#include <stdio.h>
#include <stdlib.h>

#include <iostream>

static int width = 1024;
static int height = 768;
static GLboolean should_rotate = GL_TRUE;

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

static void handle_key_press(SDL_keysym* keysym, bool pressed, Game* bf)
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
        should_rotate = !should_rotate;
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

static void draw_screen(Game* bf)
{
    bf->walk_gl();

    SDL_GL_SwapBuffers();
}

static void setup_bf_opengl(int width, int height)
{
    // Set up the rendering context, define display lists etc.:
    std::cout << "Initialize GL called." << std::endl;

    glViewport(0, 0, width, height);
}

int main(int argc, char* argv[])
{
    /* Information about the current video settings. */
    const SDL_VideoInfo* info = NULL;

    /* Color depth in bits of our window. */
    int bpp = 0;
    /* Flags we will pass into SDL_SetVideoMode. */
    int flags = 0;

    /* First, initialize SDL's video subsystem. */
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        /* Failed, exit. */
        fprintf(stderr, "Video initialization failed: %s\n",
                SDL_GetError());
        quit_tutorial(1);
    }

    /* Let's get some video information. */
    info = SDL_GetVideoInfo();

    if (!info) {
        /* This should probably never happen. */
        fprintf(stderr, "Video query failed: %s\n",
                SDL_GetError());
        quit_tutorial(1);
    }

    bpp = info->vfmt->BitsPerPixel;

    /*
     * Now, we want to setup our requested
     * window attributes for our OpenGL window.
     * We want *at least* 5 bits of red, green
     * and blue. We also want at least a 16-bit
     * depth buffer.
     *
     * The last thing we do is request a double
     * buffered window. '1' turns on double
     * buffering, '0' turns it off.
     *
     * Note that we do not use SDL_DOUBLEBUF in
     * the flags to SDL_SetVideoMode. That does
     * not affect the GL attribute state, only
     * the standard 2D blitting setup.
     */
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    /*
     * We want to request that SDL provide us
     * with an OpenGL window, in a fullscreen
     * video mode.
     */
    flags = SDL_OPENGL; // | SDL_FULLSCREEN;

    /*
     * Set the video mode
     */
    if (SDL_SetVideoMode(width, height, bpp, flags) == 0) {
        /*
         * This could happen for a variety of reasons,
         * including DISPLAY not being set, the specified
         * resolution not being available, etc.
         */
        fprintf(stderr, "Video mode set failed: %s\n",
                SDL_GetError());
        quit_tutorial(1);
    }

    std::cout << "Initialzing " << argv[0] << ". GL Ver: " << glGetString(GL_VERSION) << ", GLSL Ver: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

    // Create one player game now.
    Game bf(1);

    /*
     * At this point, we should have a properly setup
     * double-buffered window for use with OpenGL.
     */
    //setup_opengl( width, height );
    setup_bf_opengl(width, height);
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
        draw_screen(&bf);
    }

    /*
     * EXERCISE:
     * Record timings using SDL_GetTicks() and
     * and print out frames per second at program
     * end.
     */

    /* Never reached. */
    return 0;
}

