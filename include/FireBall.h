#ifndef FIREBALL_H
#define FIREBALL_H
#include "Sprite.h"
#include "GameWindow.h"

float DistanceBetween(int x1, int y1, int x2, int y2);

class FireBall
{
    public:
        FireBall(GameWindow* gamewindow, string filepath,int x, int y, int destY, int destX, int Screenwidth, int Screenheight,  float* CameraX, float* CameraY);
        virtual ~FireBall();
        void Update();
        void Draw();

        CollisionBox GetHitbox();
        Sprite* fireball;

        bool Distance();

    protected:

    private:

        int velX;
        int velY;

        int initialX;
        int initialY;

};

#endif // FIREBALL_H
