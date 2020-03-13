#include "FireBall.h"
#include <cmath>

float DistanceBetween(int x1, int y1, int x2, int y2)
{
    return sqrt((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2));
}

FireBall::FireBall(GameWindow* gamewindow, string filepath,int x, int y, int destX, int destY, int Screenwidth, int Screenheight, float* CameraX, float* CameraY)
{
    initialX = x;
    initialY = y;

    velX =0;
    velY =0;

    fireball = new Sprite(gamewindow->GetRenderer(), filepath, x, y, 50, 50, 0, CameraX, CameraY, CollisionBox(0,0,40,60), true);

    fireball->SetUpAnimation(5,1);

    float angle = atan2((initialY - destY ) ,(initialX- destX )) * 180.0/3.14 +180.0;

    velY = (15.0) * sin(angle * (3.14/180.0));
    velX = (15.0) * cos(angle * (3.14/180.0));

    fireball->Rotate(angle+180);
}

FireBall::~FireBall()
{
    delete fireball;
}

void FireBall::Update()
{
    fireball->SetPosition(fireball->GetX()+velX,fireball->GetY()+velY);
    fireball->PlayAnimation(0,4,0,50);
}

bool FireBall::Distance()
{
    return (DistanceBetween(initialX,initialY,fireball->GetX(),fireball->GetY()) > 700);
}
void FireBall::Draw()
{
    fireball->Draw();
}
CollisionBox FireBall::GetHitbox()
{
    return fireball->collision;
}
