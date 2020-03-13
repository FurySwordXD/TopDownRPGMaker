#include "PlayerCharacter.h"
#include "FireBall.h"
PlayerCharacter::PlayerCharacter(GameWindow* passed_gamewindow, float *passed_CameraX, float *passed_CameraY, int passed_ScreenWidth, int passed_ScreenHeight, int x, int y)
{
    CameraX = passed_CameraX;
    CameraY = passed_CameraY;

    Screenwidth = passed_ScreenWidth;
    Screenheight = passed_ScreenHeight;

    gamewindow = passed_gamewindow;
    speedX = speedY = 0;
    acceleration = 1;

    hp = maxhp = 200; alive = true;
    damage = 10;

    timecheck1 = SDL_GetTicks();
    timecheck2 = SDL_GetTicks();
    timecheck3 = SDL_GetTicks();
    timecheck4 = SDL_GetTicks();
    timecheck5 = SDL_GetTicks();

    moveright = false;
    moveleft = false;
    moveforward = false;
    movedown = false;
    ismove = false;
    shoot = true;
    bottomanimnumber = 0;

    player = new Sprite(gamewindow->GetRenderer(),"assets/sprites/player.png", x, y, 75, 100, 0, CameraX, CameraY, CollisionBox(20,60,40,30), true);
    player->SetUpAnimation(8,4);

    PrevX = player->GetX();
    PrevY = player->GetY();
    player->PlayAnimation(0,0,0,0);

    *CameraX = -player->GetX()+ Screenwidth/2 -player->GetW()/2;
    *CameraY = -player->GetY() + Screenheight/2 -player->GetH()/2;

    healthbar = new Sprite(gamewindow->GetRenderer(), "assets/sprites/healthbar_indicator.png", x, y-40, 100,10,0, CameraX, CameraY, CollisionBox(0,0,0,0), true);
    healthbarborder = new Sprite(gamewindow->GetRenderer(), "assets/sprites/healthbar.png", x, y-40, 110,15,0, CameraX, CameraY, CollisionBox(0,0,0,0), true);

    soundeffect = Mix_LoadWAV("assets/sounds/shoot.wav");
    Mix_VolumeChunk(soundeffect, 30);
}

PlayerCharacter::~PlayerCharacter()
{
    Mix_FreeChunk(soundeffect);
    delete player;
    for(vector<FireBall*>::iterator i = fireball.begin(); i != fireball.end(); i++)
        delete (*i);
    delete healthbar;
    delete healthbarborder;
}

void PlayerCharacter::Draw()
{
    player->Draw();

    for(vector<FireBall*>::iterator i = fireball.begin(); i != fireball.end(); i++)
        (*i)->Draw();

}
void PlayerCharacter::Update()
{
    if(hp <= 0)
    {
        float angle = atan2((*CameraY - MouseY + Screenheight/2)- *CameraY,(*CameraX - MouseX + Screenwidth/2)- *CameraX) * 180/3.14 +180;
        if(45<=angle && angle<135)
            alive = !player->PlayAnimation_withreturn(4,7,0,150);//down
        else if(135<=angle && angle<225)
            alive = !player->PlayAnimation_withreturn(4,7,1,150);//left
        else if(225<=angle && angle<315)
            alive = !player->PlayAnimation_withreturn(4,7,3,150);//up
        else
            alive = !player->PlayAnimation_withreturn(4,7,2,150);//right
    }
    else
    {

        UpdateControls();
        UpdateAnimation();

    }
    if(timecheck4+20 < SDL_GetTicks())
    {
        for(unsigned int i=0; i<fireball.size(); i++)
        {
            fireball[i]->Update();
        }
         timecheck4=SDL_GetTicks();
    }
    healthbar->SetW(100 * hp/maxhp);

}
 int PlayerCharacter::GetX()
{
    return (player->GetX() + player->GetW()/2);
}

int PlayerCharacter::GetY()
{
    return (player->GetY() + player->GetH()/2 - 15);
}

Sprite* PlayerCharacter::GetPlayer()
{
    return player;
}

void PlayerCharacter::UpdateAnimation()
{
    SDL_GetMouseState(&MouseX,&MouseY);

    float angle = atan2((*CameraY - MouseY + Screenheight/2)- *CameraY,(*CameraX - MouseX + Screenwidth/2)- *CameraX) * 180/3.14 +180;

    if(ismove)
    {
        if(45<=angle && angle<135)
            player->PlayAnimation(0,3,0,150);
        else if(135<=angle && angle<225)
            player->PlayAnimation(0,3,1,150);
        else if(225<=angle && angle<315)
            player->PlayAnimation(0,3,3,150);
        else
            player->PlayAnimation(0,3,2,150);


    }
    else
    {
        if(45<=angle && angle<135)
            player->PlayAnimation(0,0,0,150);//down
        else if(135<=angle && angle<225)
            player->PlayAnimation(0,0,1,150);//left
        else if(225<=angle && angle<315)
            player->PlayAnimation(0,0,3,150);//up
        else
            player->PlayAnimation(0,0,2,150);//right

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

void PlayerCharacter::Shoot()
{
    if(gamewindow->GetMainEvent()->type == SDL_MOUSEBUTTONDOWN && shoot == false && gamewindow->GetMainEvent()->button.button == SDL_BUTTON_LEFT)
    {
        SDL_GetMouseState(&MouseX,&MouseY);
        fireball.push_back(new FireBall(gamewindow, "assets/sprites/fireball_red.png", player->GetX() + OffsetX, player->GetY() + OffsetY, MouseX - Screenwidth/2 +player->GetX() + OffsetX, MouseY - Screenheight/2 +player->GetY() + OffsetY, Screenwidth, Screenheight, CameraX, CameraY));
        cout<<"Shoot\n";
        shoot = true;
        Mix_PlayChannel(-1, soundeffect, 0);
    }
    if(gamewindow->GetMainEvent()->type == SDL_MOUSEBUTTONUP)
    {
        shoot = false;
    }
    for(unsigned int i=0; i < fireball.size(); i++)
        if(fireball[i]->Distance())
            fireball.erase(fireball.begin()+i);
}

void PlayerCharacter::UpdateControls()
{
    switch(gamewindow->GetMainEvent()->type)
    {
        case SDL_KEYDOWN:
            switch (gamewindow->GetMainEvent()->key.keysym.sym)
                {       case SDLK_a:
                            cout<<"Left \n";
                            moveleft = true;
                            ismove = true;
                            break;
                        case SDLK_w:
                            cout<<"Forward \n";
                            moveforward = true;
                            ismove = true;
                            break;
                        case SDLK_s:
                            cout<<"Back \n";
                            movedown = true;
                            ismove = true;
                            break;
                        case SDLK_d:
                            cout<<"Right \n";
                            moveright = true;
                            ismove = true;
                            break;
                }
            break;
    }
    if(gamewindow->GetMainEvent()->type == SDL_KEYUP)
    {
                if(gamewindow->GetMainEvent()->key.keysym.sym == SDLK_a)
                {
                    moveleft = false;
                }
                if(gamewindow->GetMainEvent()->key.keysym.sym == SDLK_s)
                {
                    movedown = false;
                }
                if(gamewindow->GetMainEvent()->key.keysym.sym == SDLK_d)
                {
                    moveright = false;
                }
                if(gamewindow->GetMainEvent()->key.keysym.sym == SDLK_w)
                {
                    moveforward = false;
                }
                if(!moveforward && !movedown && !moveright && ! moveleft)
                    ismove = false;
    }
    if (timecheck1+20 < SDL_GetTicks())
    {
        if(ismove && !colliding)
        {
            if(speedX < 5)
                speedX += acceleration;
            if(speedY < 5)
                speedY += acceleration;
            if(moveleft)
            {
                player->SetX(player->GetX()-speedX);
            }
            if(moveright)
            {
                player->SetX(player->GetX()+speedX);
            }
            if(moveforward)
            {
                player->SetY(player->GetY()-speedY);
            }
            if(movedown)
            {
                player->SetY(player->GetY()+speedY);
            }
            *CameraX = -player->GetX()+ Screenwidth/2 -player->GetW()/2;
            *CameraY = -player->GetY() + Screenheight/2 -player->GetH()/2;

        }
        else
        {
            speedX = speedY = 0;
        }
        healthbarborder->SetPosition(player->GetX()-5-12,player->GetY()-32.5);
        healthbar->SetPosition(player->GetX()-12,player->GetY()-30);
        timecheck1 = SDL_GetTicks();
    }

    if (timecheck3 < SDL_GetTicks() && !colliding)
    {
        if(moveleft)
        {
            PrevX = player->GetX() + speedX;
        }
        if(moveright)
        {
            PrevX = player->GetX() - speedX;
        }
        if(moveforward)
        {
            PrevY = player->GetY() + speedY;
        }
        if(movedown)
        {
            PrevY = player->GetY() - speedY;

        }
        timecheck3 = SDL_GetTicks();
    }

}
void PlayerCharacter::StopMove()
{
    movedown = moveleft = moveforward = moveright = false;

}
void PlayerCharacter::PlayerCollided()
{
    colliding = true;
    speedX = speedY = 0;
    StopMove();
    player->SetPosition(PrevX,PrevY);

}


vector<FireBall*> PlayerCharacter::GetFireBall()
{
    return fireball;
}

