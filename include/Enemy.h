#ifndef ENEMY_H
#define ENEMY_H
#include <cmath>
#include "Sprite.h"
#include "PlayerCharacter.h"
#include "GameWindow.h"

using namespace std;

class Enemy
{
    public:
        Enemy(GameWindow* passed_gamewindow, float* passed_CameraX, float *passed_CameraY, int passed_ScreenWidth, int passed_ScreenHeight, int x, int y);
        virtual ~Enemy();

        void Draw();
        void Update(int destx, int desty);
        void Collide();

        vector<FireBall*> fireball;
        Sprite* enemy;
        bool colliding;

        float speed,acceleration;
        int hp,damage;
        int maxhp;
        bool alive;

        GameObjects* tempcolobj;

        unsigned int timecheck;
        unsigned int timecheck2;
        unsigned int timecheck3;
        unsigned int timecheck4;
        unsigned int timecheck5;

        Sprite* healthbar;
        Sprite* healthbarborder;

    protected:

    private:

        GameWindow* gamewindow;

        float *CameraX;
        float *CameraY;

        int ScreenWidth;
        int ScreenHeight;

        bool ismove;

        float angle;
        double distance;

        int OffsetX,OffsetY;
        int PrevX, PrevY;
        int randx;
        int randy;

        Mix_Chunk* soundeffect;
};

#endif // ENEMY_H
