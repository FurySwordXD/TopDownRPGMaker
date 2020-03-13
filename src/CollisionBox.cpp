#include "CollisionBox.h"

CollisionBox::CollisionBox()
{
    OffsetX = 0;
    OffsetY = 0;
    SetRectangle(0,0,0,0);
}
CollisionBox::CollisionBox(int x, int y, int w, int h)
{
    OffsetX = x;
    OffsetY = y;
    SetRectangle(0,0,w,h);
}

CollisionBox::~CollisionBox()
{
    //dtor
}

void CollisionBox::SetRectangle(int x, int y, int w, int h)
{
    CollisionRect.x = x + OffsetX;
    CollisionRect.y = y + OffsetY;
    CollisionRect.w = w;
    CollisionRect.h = h;
}

void CollisionBox::Setposition(int x, int y)
{
    CollisionRect.x = x + OffsetX;
    CollisionRect.y = y + OffsetY;
}

SDL_Rect CollisionBox::GetRectangle()
{
    return CollisionRect;
}

bool CollisionBox::check_collision(SDL_Rect b)
{
    int leftA, leftB;
    int rightA, rightB;
    int topA, topB;
    int bottomA, bottomB;

    leftA = CollisionRect.x;
    rightA = CollisionRect.x + CollisionRect.w;
    topA = CollisionRect.y;
    bottomA = CollisionRect.y + CollisionRect.h;

    leftB = b.x;
    rightB = b.x + b.w;
    topB = b.y;
    bottomB = b.y + b.h;

    if(bottomA <= topB){
        return false;
    }
    if(topA >= bottomB){
        return false;
    }
    if(rightA <= leftB){
        return false;
    }
    if(leftA >= rightB){
        return false;
    }
    return true;
}

