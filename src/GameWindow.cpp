#include "GameWindow.h"
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_opengl.h>
#include <SDL_render.h>
#include <stdio.h>

using namespace std;

GameWindow::GameWindow(bool* quit, int ScreenWidth, int ScreenHeight)
{
    SDL_Init(SDL_INIT_EVERYTHING);
    window = NULL;
    window = SDL_CreateWindow("Fury's Project", 200, 0, ScreenWidth, ScreenHeight, SDL_WINDOW_SHOWN | SDL_WINDOW_BORDERLESS);

    if (window == NULL)
    {
        cout << "Window couldn't be created \n";
        *quit = true;
    }

    renderer = NULL;
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    mainEvent = new SDL_Event();
}


GameWindow::~GameWindow(void)
{
        SDL_DestroyWindow(window);
        SDL_DestroyRenderer(renderer);
        delete mainEvent;
}

SDL_Renderer* GameWindow::GetRenderer()
{
    return renderer;
}

SDL_Event* GameWindow::GetMainEvent()
{
    return mainEvent;
}

void GameWindow::Begin()
{
    SDL_PollEvent(mainEvent);
    SDL_RenderClear(renderer);
    SDL_UpdateWindowSurface(window);
}

void GameWindow::End()
{
    SDL_RenderPresent(renderer);
}
