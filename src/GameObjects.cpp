#include "GameObjects.h"

using namespace std;

GameObjects::GameObjects(GameWindow* gamewindow, string filepath, int passed_x, int passed_y, int passed_w, int passed_h, int passed_r, float *CameraX, float *CameraY,  string passed_refer, bool passed_block, string txtdata)
{
    x = passed_x;
    y = passed_y;
    w = passed_w;
    h = passed_h;
    r = passed_r;
    refer = passed_refer;
    textdata = txtdata;
    block = passed_block;
    gameobject = new Sprite(gamewindow->GetRenderer(), filepath, x, y, w, h, r, CameraX, CameraY, CollisionBox(0,0,w,h), true);

}

GameObjects::~GameObjects()
{
    delete gameobject;
}

void GameObjects::Draw()
{
    gameobject->Draw();
}
int GameObjects::GetX()
{
    return x;
}
int GameObjects::GetY()
{
    return y;
}
int GameObjects::GetW()
{
    return w;
}
int GameObjects::GetH()
{
    return h;
}
int GameObjects::GetR()
{
    return r;
}

void GameObjects::Rotate(int rot)
{
    r+=rot;
    gameobject->Rotate(r);
}
Sprite* GameObjects::GetGameObject()
{
    return gameobject;
}
