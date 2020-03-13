#include "Enemy.h"

double Distance(int x1, int y1, int x2, int y2)
{
    return sqrt((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2));
}

Enemy::Enemy(GameWindow* passed_gamewindow, float* passed_CameraX, float *passed_CameraY, int passed_ScreenWidth, int passed_ScreenHeight, int x, int y)
{
    CameraX = passed_CameraX;
    CameraY = passed_CameraY;
    ScreenWidth = passed_ScreenWidth;
    ScreenHeight = passed_ScreenHeight;
    gamewindow = passed_gamewindow;

    timecheck = timecheck2 = timecheck3 = timecheck4 = timecheck5 = SDL_GetTicks();
    speed = 0;
    acceleration = 0.5;
    hp = maxhp =100;
    alive = true;
    damage = 5;
    OffsetX = OffsetY = PrevX = PrevY = 0;
    distance = angle = 0;
    ismove = false;
    colliding = false;
    enemy = new Sprite(gamewindow->GetRenderer(), "assets/sprites/enemy.png", x, y, 75, 100, 0, CameraX, CameraY, CollisionBox(20,60,40,30), true);
    enemy->SetUpAnimation(8,4);

    healthbar = new Sprite(gamewindow->GetRenderer(), "assets/sprites/healthbar_indicator.png", x, y-40, 75,10,0, CameraX, CameraY, CollisionBox(0,0,0,0), true);
    healthbarborder = new Sprite(gamewindow->GetRenderer(), "assets/sprites/healthbar.png", x, y-40, 80,15,0, CameraX, CameraY, CollisionBox(0,0,0,0), true);
    soundeffect = Mix_LoadWAV("assets/sounds/shoot.wav");
    Mix_VolumeChunk(soundeffect, 15);
}

Enemy::~Enemy()
{
    Mix_FreeChunk(soundeffect);
    delete enemy;
    for(vector<FireBall*>::iterator i = fireball.begin(); i != fireball.end(); i++)
        delete (*i);
    delete healthbar;
    delete healthbarborder;
}

void Enemy::Draw()
{
    enemy->Draw();
    for(vector<FireBall*>::iterator i = fireball.begin(); i != fireball.end(); i++)
        (*i)->Draw();

    healthbarborder->Draw();
    healthbar->Draw();

    healthbar->SetPosition(enemy->GetX(),enemy->GetY()-30);
    healthbarborder->SetPosition(enemy->GetX()-2.5,enemy->GetY()-32.5);

    healthbar->SetW(75 * hp/maxhp);
}

void Enemy::Update(int passed_destx, int passed_desty)
{
    if(hp > 0)
    {
        int destx = passed_destx;
        int desty = passed_desty;
        distance = Distance(enemy->GetX(), enemy->GetY(), destx, desty);

        if(distance > 250  && distance < 500)
        {
            angle = atan2((enemy->GetY() - desty )  , (enemy->GetX() - destx) ) * 180.0/3.14 +180.0;

            if(timecheck+20 < SDL_GetTicks())
            {
                if(!colliding)
                {
                    ismove = true;
                    if(speed < 5)
                        speed += acceleration;
                    if(enemy->GetX() > enemy->GetX() - (enemy->GetX()-destx)/distance * speed)
                    {
                       PrevX = enemy->GetX() + speed;
                    }
                    if(enemy->GetX() < enemy->GetX() - (enemy->GetX()-destx)/distance * speed)
                    {
                        PrevX = enemy->GetX() - speed;
                    }
                    if(enemy->GetY() > enemy->GetY() - (enemy->GetY()-desty)/distance * speed)
                    {
                         PrevY = enemy->GetY() + speed;
                    }
                    if(enemy->GetY() < enemy->GetY() - (enemy->GetY()-desty)/distance * speed)
                    {
                        PrevY = enemy->GetY() - speed;
                    }

                    enemy->SetX(enemy->GetX() - (enemy->GetX()-destx)/distance * speed);
                    enemy->SetY(enemy->GetY() - (enemy->GetY()-desty)/distance * speed);
                    timecheck = SDL_GetTicks();
                }
                else
                {
                    speed = 0;
                }
            }
        }
        else
        {
            if(timecheck2 + 1000 < SDL_GetTicks() && distance < 300)
            {
                angle = atan2((enemy->GetY() - desty )  , (enemy->GetX() - destx) ) * 180.0/3.14 +180.0;
                Mix_PlayChannel(-1, soundeffect, 0);
                fireball.push_back(new FireBall(gamewindow, "assets/sprites/fireball_blue.png", enemy->GetX() + OffsetX, enemy->GetY() + OffsetY, destx, desty, ScreenWidth, ScreenHeight, CameraX, CameraY));
                timecheck2 = SDL_GetTicks();
            }
            ismove = false;


        }

            if(ismove)
            {

                if(45<=angle && angle<135)
                    enemy->PlayAnimation(0,3,0,150);
                else if(135<=angle && angle<225)
                    enemy->PlayAnimation(0,3,1,150);
                else if(225<=angle && angle<315)
                    enemy->PlayAnimation(0,3,3,150);
                else
                    enemy->PlayAnimation(0,3,2,150);
            }
            else
            {
                if(45<=angle && angle<135)
                    enemy->PlayAnimation(0,0,0,150);//down
                else if(135<=angle && angle<225)
                    enemy->PlayAnimation(0,0,1,150);//left
                else if(225<=angle && angle<315)
                    enemy->PlayAnimation(0,0,3,150);//up
                else
                    enemy->PlayAnimation(0,0,2,150);//right
            }

        if(45<=angle && angle<135)
        {
            OffsetX = 40;
        }//down
        else if(135<=angle && angle<225)
        {
            OffsetX= -10;
        }//left
        else if(225<=angle && angle<315)
        {
            OffsetX= -10;
            OffsetY = -10;
        }//up
        else
        {
            OffsetX = 40;
            OffsetY = -10;
        } //right
    }
    else
        if(alive)
        {
            speed = 0; ismove = false;
            if(45<=angle && angle<135)
                alive = !enemy->PlayAnimation_withreturn(4,7,0,150);//down
            else if(135<=angle && angle<225)
                alive = !enemy->PlayAnimation_withreturn(4,7,1,150);//left
            else if(225<=angle && angle<315)
                alive = !enemy->PlayAnimation_withreturn(4,7,3,150);//up
            else
                alive = !enemy->PlayAnimation_withreturn(4,7,2,150);//right
        }
    for(unsigned int i=0; i < fireball.size(); i++)
            if(fireball[i]->Distance())
                fireball.erase(fireball.begin()+i);

    if(timecheck3 + 20 < SDL_GetTicks())
    {
        for(unsigned int i=0; i<fireball.size(); i++)
        {
            fireball[i]->Update();
        }
        timecheck3 = SDL_GetTicks();
    }
    for(unsigned int i=0; i < fireball.size(); i++)
        if(fireball[i]->Distance())
            fireball.erase(fireball.begin()+i);
}

void Enemy::Collide()
{
    enemy->SetPosition(PrevX,PrevY);
    speed = 0;
    colliding = true;
    ismove = false;
}
