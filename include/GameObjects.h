#ifndef GAMEOBJECTS_H
#define GAMEOBJECTS_H
#include "Sprite.h"
#include "GameWindow.h"


class GameObjects
{
    public:
        GameObjects(GameWindow* gamewindow, string filepath, int passed_x, int passed_y, int passed_w, int passed_h, int passed_r, float *CameraX, float *CameraY, string passed_refer, bool passed_block, string txtdata = "");
        virtual ~GameObjects();

        void Draw();
        int GetX();
        int GetY();
        int GetW();
        int GetH();
        int GetR();
        void Rotate(int rot);

        Sprite* GetGameObject();
        string refer;
        string textdata;
        bool block;
    protected:

    private:
        int x,y,w,h,r;
        Sprite* gameobject;
};

#endif // GAMEOBJECTS_H
