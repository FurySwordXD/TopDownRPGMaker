#ifndef TEXTFIELD_H
#define TEXTFIELD_H
#include "GameWindow.h"
#include "Sprite.h"
#include <string>
#include <SDL.h>
#include <SDL_ttf.h>

class TextField
{
    public:
        TextField(GameWindow* passed_gamewindow, int passed_x, int passed_y, int passed_w, int passed_h, float *CameraX, float *CameraY, string textfieldname);
        virtual ~TextField();

        void EditText(bool called);
        void Draw();
        void Update();

        bool inputmode;

        string inputtext;

    protected:

    private:
        bool press;
        bool click;

        int x,y,w,h;

        unsigned int timecheck;

        GameWindow* gamewindow;

        string displayinputtext;

        Sprite* textfield;
        Sprite* MouseLocation;

        SDL_Color textcolor;
        TTF_Font* font;
        SDL_Rect textRect[2];
        SDL_Texture* text[2];
        SDL_Surface* message[2];

        Mix_Chunk* soundeffect;
};

#endif // TEXTFIELD_H
