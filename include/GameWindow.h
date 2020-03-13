#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_opengl.h>
#include <SDL_render.h>
#include <stdio.h>

using namespace std;

class GameWindow
{
    public:
        GameWindow(bool* quit, int ScreenWidth, int ScreenHeight);
        ~GameWindow(void);

        SDL_Renderer* GetRenderer();
        SDL_Event* GetMainEvent();

        void Begin();
        void End();
private:

        SDL_Window* window;
        SDL_Renderer* renderer;
        SDL_Event* mainEvent;



};

#endif // GAMEWINDOW_H
