#ifndef BUTTON_H
#define BUTTON_H
#include "GameWindow.h"
#include "Sprite.h"
#include <SDL.h>
#include <SDL_ttf.h>

using namespace std;

class Button
{
    public:
        Button(GameWindow* passed_gamewindow, int x, int y, int w, int h, string buttonText, string buttonpath ,float* passed_CameraX, float* passed_CameraY, int  passed_frameX, int  passed_frameY, int fontsize = 24);
        ~Button();
        void Update();
        void Draw();
        void Click();
        void Hover();
        void NotHover();

        bool Buttonclick(bool has_animation);

        string buttontext;

        Sprite* buttonTexture;
        Sprite* MouseLocation;
    private:

        bool Clicked;

        float *CameraX;
        float *CameraY;

        GameWindow* gamewindow;

        SDL_Rect textRect;
        SDL_Texture* text;
        SDL_Surface* message;

        int frameX,frameY;

        Mix_Chunk* soundeffect;
};
#endif // BUTTON_H
