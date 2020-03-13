#ifndef MAINMENU_H
#define MAINMENU_H
#include <SDL.h>
#include <fstream>
#include "button.h"
#include "Sprite.h"
#include <SDL_ttf.h>
#include <string>
#include <vector>
#include "TextField.h"
#include <windows.h>
#include <tchar.h>


using namespace std;

class MainMenu
{
    public:
        MainMenu(GameWindow* passed_gamewindow, float *passed_CameraX, float *passed_CameraY, int passed_Screenwidth, int passed_Screenheight);
        virtual ~MainMenu();

        Button* GetExitButton();
        Button* GetPlayButton();
        Button* GetCreategameButton();
        Button* GetDeletegameButton();
        Button* Getresumebutton();
        Button* Getquitbutton();

        void DeleteCharacter();

        void Update();

        void Draw();
        void DrawPausemenu();
        void DrawDeathmenu();

        string levelname;

        Sprite* background;
        Sprite* pausebg;
        TextField* levelnametextfield;

        vector<Button*> savedlevelbuttons;
        int savedlevelscount;

    private:
        bool press;

        Button *playbutton;
        Button *exitbutton;
        Button *creategamebutton;
        Button *deletegamebutton;
        Button *resumebutton;
        Button *quitbutton;

        float *CameraX;
        float *CameraY;

        TTF_Font *font;
        SDL_Color textcolor;

        SDL_Rect textRect;
        SDL_Rect textRect2;
        SDL_Rect textRect3;

        SDL_Texture *gamenametext;
        SDL_Surface *gamenamemessage;

        SDL_Texture *pausetext;
        SDL_Surface *pausemessage;

        SDL_Texture *deathtext;
        SDL_Surface *deathmessage;

        string savedlevelnames;
        SDL_Texture *savedlevelnamestext;
        SDL_Surface *savedlevelnamesmessage;

        GameWindow *gamewindow;

        int Screenwidth;
        int Screenheight;


};

#endif // MAINMENU_H
