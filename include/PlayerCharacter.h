#ifndef PLAYERCHARACTER_H
#define PLAYERCHARACTER_H
#include "Sprite.h"
#include "GameWindow.h"
#include "GameObjects.h"
#include "FireBall.h"
#include <iostream>
#include <SDL.h>
#include <SDL_mixer.h>
#include <iostream>
#include <cmath>
#include <vector>

using namespace std;

class PlayerCharacter
{
    public:
        PlayerCharacter(GameWindow* gamewindow, float* passed_CameraX, float *passed_CameraY, int passed_ScreenWidth, int passed_ScreenHeight, int x, int y);
        virtual ~PlayerCharacter();

        void Update();
        void Draw();
        void UpdateAnimation();
        void UpdateControls();
        void StopMove();
        void Shoot();

        int GetX();
        int GetY();

        Sprite* GetPlayer();
        void PlayerCollided();

        int speedX, speedY;
        float acceleration;
        bool colliding;

        vector<FireBall*> GetFireBall();
        vector<FireBall*> fireball;

        int hp,damage,maxhp;
        bool alive;

        Sprite* healthbar;
        Sprite* healthbarborder;

    protected:

      private:
        Sprite* player;

        float *CameraX;
        float *CameraY;

        int Screenwidth;
        int Screenheight;

        int MouseX;
        int MouseY;

        int PrevX;
        int PrevY;

        GameWindow* gamewindow;

        unsigned int timecheck1;
        unsigned int timecheck2;
        unsigned int timecheck3;
        unsigned int timecheck4;
        unsigned int timecheck5;

        bool moveleft;
        bool moveright;
        bool moveforward;
        bool movedown;
        bool ismove;
        bool shoot;

        int bottomanimnumber;

        int OffsetX,OffsetY;

        Mix_Chunk* soundeffect;
};

#endif // PLAYERCHARACTER_H
