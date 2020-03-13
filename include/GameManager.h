#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H
#include <iostream>
#include <SDL.h>
#include <fstream>
#include <vector>
#include "GameWindow.h"
#include "Sprite.h"
#include "PlayerCharacter.h"
#include "Environment.h"
#include "Button.h"
#include "MainMenu.h"

class GameManager
{
     public:
        GameManager(int passed_ScreenWidth, int passed_ScreenHeight);
        ~GameManager(void);
        void GameLoop(void);
        float CameraX;
        float CameraY;


        bool check_collision(SDL_Rect a, SDL_Rect b);
        void Collision();
        void UpdateGameInput();
        int UpdateEditModeInputs();

        bool ButtonControls(Button* button, bool has_animation = true);

        void ButtonControls();
        void UpdateLevels();


    private:
        string levelname;
        vector<string> existinglevels;

        int ScreenWidth;
        int ScreenHeight;
        unsigned int timecheck;

        bool quit;
        bool pressed;
        bool started;
        bool gamecreated;
        bool loading;
        bool paused;
        bool levelload;

        Environment* Level;

        GameWindow* gamewindow;

        PlayerCharacter* playercharacter;

        MainMenu* mainmenu;

        fstream gamefile;

        Sprite* loadscreen;
        int type;

        Mix_Music* bgm;

        Sprite* cursor;

};
#endif // GAMEMANAGER_H
