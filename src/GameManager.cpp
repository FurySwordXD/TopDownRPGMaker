#include "GameManager.h"
#include "GameWindow.h"
#include "Sprite.h"
#include <iostream>
#include <SDL.h>
#include <vector>

using namespace std;

GameManager::GameManager(int passed_ScreenWidth, int passed_ScreenHeight)
{
    Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096);

    CameraX = 0;
    CameraY = 0;
    ScreenWidth = passed_ScreenWidth;
    ScreenHeight = passed_ScreenHeight;
    quit = false;
    started = false;
    gamewindow = new GameWindow(&quit, ScreenWidth, ScreenHeight);

    timecheck = SDL_GetTicks();
    pressed = false;
    gamecreated = false;
    loading = false;
    paused = false;
    levelload = false;

    fstream levelsave("savedlevels.txt", ios::in);
    if(levelsave.good())
    {
        string savedlevel;
        while(levelsave>>savedlevel)
        {
            existinglevels.push_back(savedlevel);
        }
        levelsave.close();

    }

    mainmenu = new MainMenu(gamewindow, &CameraX, &CameraY, ScreenWidth, ScreenHeight);
    loadscreen = new Sprite(gamewindow->GetRenderer(), "assets/loading_screen2.jpg", 0, 0, ScreenWidth, ScreenHeight, 0, &CameraX, &CameraY, CollisionBox(0,0,0,0), false);
    Level = NULL;
    playercharacter = NULL;

    bgm = Mix_LoadMUS("assets/sounds/JewelBeat_Final_Quest.wav");
    Mix_VolumeMusic(100);
    cursor = new Sprite(gamewindow->GetRenderer(), "assets/MouseCursor.png", 0, 0, 25, 25, 0, &CameraX, &CameraY, CollisionBox(0,0,0,0), false);
    cursor->Rotate(0);
    SDL_ShowCursor(0);


}


GameManager::~GameManager(void)
{
    Mix_FreeMusic(bgm);
    delete gamewindow;
    if(gamecreated)
    {
        delete Level;
        delete playercharacter;
    }
    delete mainmenu;
    delete loadscreen;
}

void GameManager::GameLoop()
{
    Uint32 start;
    const int FPS = 250;
    while (!quit && gamewindow->GetMainEvent()->type != SDL_QUIT)
    {
        start = SDL_GetTicks();
        gamewindow->Begin();
            if(!started)
            {
                levelname = mainmenu->levelname;

                if(!loading)
                {
                    timecheck = SDL_GetTicks();
                    mainmenu->Draw();
                }
				
                for(vector<Button*>:: iterator i = mainmenu->savedlevelbuttons.begin(); i!= mainmenu->savedlevelbuttons.end(); i++)
                {
                    if((*i)->Buttonclick(true))
                    {
                        mainmenu->levelnametextfield->inputtext = (*i)->buttontext;
                        mainmenu->levelnametextfield->EditText(true);
                    }
                }
                if(mainmenu->GetPlayButton()->Buttonclick(true))
                {
                    if(levelname != "savedlevels")
                    {
                        gamecreated = true;
                        loading = true;
                        Level = new Environment(gamewindow, ScreenWidth, ScreenHeight, &CameraX, &CameraY, levelname);
                        pressed = true;
                        UpdateLevels();
                    }
                }
                if(mainmenu->GetExitButton()->Buttonclick(true))
                {
                    pressed = true;
                    SDL_Delay(300);
                    quit = true;
                    SDL_Quit();
                }
                if(mainmenu->GetCreategameButton()->Buttonclick(true))
                {
                    if(levelname != "savedlevels")
                    {
                        gamecreated = true;
                        pressed = true;
                        loading = true;
                        Level = new Environment(gamewindow, ScreenWidth, ScreenHeight, &CameraX, &CameraY, levelname);
                        Level->Update();
                    }
                }
                if(mainmenu->GetDeletegameButton()->Buttonclick(true))
                {
                    if(levelname != "savedlevels")
                    {
                        string leveltodelete;
                        leveltodelete = "assets/Levels/" + levelname + ".txt";
                        remove(leveltodelete.c_str());
                        for(unsigned int i = 0; i < mainmenu->savedlevelscount; i++)
                            if(mainmenu->savedlevelbuttons[i]->buttontext == levelname)
                             {
                                cout<<"Deleted Level "<<leveltodelete<<endl;
                                cout<<mainmenu->savedlevelbuttons[i]->buttontext<<endl;
                                mainmenu->savedlevelbuttons.erase(mainmenu->savedlevelbuttons.begin()+i);
                                cout<<existinglevels[i]<<endl;
                                existinglevels.erase(existinglevels.begin()+i);
                                mainmenu->savedlevelscount--;
                                mainmenu->levelnametextfield->inputtext = "";
                                mainmenu->levelnametextfield->EditText(true);
                                levelname = mainmenu->levelname = mainmenu->levelnametextfield->inputtext;
                                UpdateLevels();
                             }
                    }
                }
                if(loading)
                {
                    loadscreen->Draw();
                    if( timecheck + 200 < SDL_GetTicks())
                    {
                        started = true;
                        loading = false;
                        Level->Loadfromfile();
                        playercharacter = new PlayerCharacter(gamewindow, &CameraX, &CameraY, ScreenWidth, ScreenHeight, Level->playerspawner->GetX(), Level->playerspawner->GetY());
                        timecheck = SDL_GetTicks();
                        if(Level->GetGameMode() == Level->GamePlay)
                        for(vector<Sprite*>::iterator i = Level->enemyspawner.begin(); i != Level->enemyspawner.end();i++)
                        {
                            Level->enemies.push_back(new Enemy(gamewindow, &CameraX, &CameraY, ScreenWidth, ScreenHeight, (*i)->GetX(), (*i)->GetY()));
                        }
                        if(Level->GetGameMode() == Level->GamePlay)
                            Mix_PlayMusic(bgm,-1);
                    }
                }
            }
            if(started)
            {

                Level->Draw();
                if(playercharacter->alive)
                    playercharacter->Draw();
                Level->DrawFront();

                if(Level->GetGameMode() == Level->GamePlay && !paused)
                {
                    SDL_StopTextInput();
                    playercharacter->Update();
                    playercharacter->Shoot();
                    playercharacter->healthbarborder->Draw();
                    playercharacter->healthbar->Draw();
                    if(playercharacter->alive)
                        for(vector<Enemy*>::iterator i = Level->enemies.begin(); i != Level->enemies.end();i++)
                        {
                            (*i)->Update(playercharacter->GetX(), playercharacter->GetY());
                            (*i)->healthbarborder->Draw();
                            (*i)->healthbar->Draw();
                        }
                    if(!playercharacter->alive)
                    {
                        paused = true;
                    }
                }
                if(Level->GetGameMode() == Level->GameEdit && !paused)
                {
                    Level->EditMode();
                    if(!(Level->Objecttypetextfield->inputmode == true || Level->Widthtextfield->inputmode == true || Level->Heighttextfield->inputmode == true || Level->Leveltextfield->inputmode == true))
                    {
                        playercharacter->Update();
                        SDL_StopTextInput();
                    }
                    else
                    {
                         playercharacter->StopMove();
                    }
                }


                Collision();

            }
            UpdateGameInput();
            gamewindow->End();
            if(gamewindow->GetMainEvent()->type == SDL_MOUSEBUTTONUP)
                pressed = false;
            if(1000/FPS > SDL_GetTicks() - start)
                SDL_Delay(1000/FPS - (SDL_GetTicks() - start));


    }
}

void GameManager::Collision()
{
    for(unsigned int i=0; i< Level->GetTile().size(); i++)
    {
        if(Level->GetTile()[i]->block)
        {   if(playercharacter->GetPlayer()->collision.check_collision(Level->GetTile()[i]->GetGameObject()->collision.CollisionRect) )
            {
                playercharacter->PlayerCollided();
                cout<<"Collided\n";
            }
            else
            {
                playercharacter->colliding = false;
            }
            for(vector<Enemy*>::iterator j = Level->enemies.begin(); j != Level->enemies.end();j++)
            {
                if((*j)->enemy->collision.check_collision(Level->GetTile()[i]->GetGameObject()->collision.CollisionRect))
                {
                    (*j)->Collide();
                    (*j)->tempcolobj = Level->GetTile()[i];
                }
                if((*j)->colliding && !(*j)->enemy->collision.check_collision((*j)->tempcolobj->GetGameObject()->collision.CollisionRect))
                {
                    (*j)->colliding = false;
                }
            }
        }
    }
    /*for(unsigned int i = 0; i < Level->enemies.size(); i++)
    {
        for(unsigned int j = 0; j < Level->enemies.size(); j++)
            if(i != j)
                if(Level->enemies[i]->enemy->collision.check_collision(Level->enemies[j]->enemy->collision.CollisionRect))
                {
                    Level->enemies[j]->Collide();
                }
                else
                    Level->enemies[j]->colliding = false;
    }*/
    for(vector<Enemy*>::iterator j = Level->enemies.begin(); j != Level->enemies.end();j++)
    {
        for(unsigned int i=0; i< playercharacter->GetFireBall().size(); i++)
        {
            if(playercharacter->fireball[i]->fireball->collision.check_collision((*j)->enemy->collision.CollisionRect))
            {
                cout<<"Damage dealt\n";
                playercharacter->fireball.erase(playercharacter->fireball.begin()+i);
                (*j)->hp -= playercharacter->damage;
            }
        }
    }
    for(vector<Enemy*>::iterator j = Level->enemies.begin(); j != Level->enemies.end();j++)
    {
        for(unsigned int k = 0; k < (*j)->fireball.size(); k++)
            if((*j)->fireball[k]->fireball->collision.check_collision(playercharacter->GetPlayer()->collision.CollisionRect))
            {
                cout<<"Damage taken\n";
                (*j)->fireball.erase((*j)->fireball.begin()+k);
                playercharacter->hp -= (*j)->damage;
            }
    }
    for(unsigned int i = 0; i < Level->enemies.size();i++)
    {
        if(Level->enemies[i]->alive == false)
            Level->enemies.erase(Level->enemies.begin()+i);
    }

    if(Level->GetGameMode() == Level->GamePlay)
        for(unsigned int i = 0; i < Level->portal.size();i++)
        {
            if(playercharacter->GetPlayer()->collision.check_collision(Level->portal[i]->GetGameObject()->collision.CollisionRect))
            {

                if(!levelload)
                {
                    timecheck = SDL_GetTicks();
                    levelload = true;
                }
                loadscreen->Draw();
                if(timecheck +100 < SDL_GetTicks())
                {
                    levelload = false;
                    delete Level;
                    Level = new Environment(gamewindow, ScreenWidth, ScreenHeight, &CameraX, &CameraY, Level->portal[i]->textdata);
                    Level->Loadfromfile();
                    playercharacter->GetPlayer()->SetPosition(Level->playerspawner->GetX(),Level->playerspawner->GetY());
                    CameraX = -playercharacter->GetPlayer()->GetX()+ ScreenWidth/2 -playercharacter->GetPlayer()->GetW()/2;
                    CameraY = -playercharacter->GetPlayer()->GetY() + ScreenHeight/2 -playercharacter->GetPlayer()->GetH()/2;
                    for(vector<Sprite*>::iterator i = Level->enemyspawner.begin(); i != Level->enemyspawner.end();i++)
                    {
                        Level->enemies.push_back(new Enemy(gamewindow, &CameraX, &CameraY, ScreenWidth, ScreenHeight, (*i)->GetX(), (*i)->GetY()));
                    }
                    timecheck = SDL_GetTicks();
                }
            }
        }
}
void GameManager::UpdateGameInput()
{
    if(gamewindow->GetMainEvent()->type == SDL_KEYDOWN && !pressed)
    {       pressed = true;
            if(gamewindow->GetMainEvent()->key.keysym.sym == SDLK_ESCAPE && started && !loading)
            {
                paused = true;

            }
    }
    if(paused)
    {
        if(playercharacter->alive)
            mainmenu->DrawPausemenu();
        else
           mainmenu->DrawDeathmenu();

        if(mainmenu->Getresumebutton()->Buttonclick(true))
                paused = false;
        if(mainmenu->Getquitbutton()->Buttonclick(true))
        {
            timecheck = SDL_GetTicks();
            Level->Savetofile();
            if(Level->GetGameMode() == Level->GameEdit)
                Level->Update();
            loading = true;
            SDL_Delay(100);
            Mix_HaltMusic();
            Mix_HaltChannel(-1);
        }

    }
    cursor->Draw();
    int MouseX,MouseY;
    SDL_GetMouseState(&MouseX,&MouseY);
    cursor->SetX(MouseX);
    cursor->SetY(MouseY);
    if(loading)
    {
        loadscreen->Draw();
        if(timecheck +500 < SDL_GetTicks())
        {
            started = false;
            loading = false;
            paused = false;
            timecheck = SDL_GetTicks();
        }
    }
    if(gamewindow->GetMainEvent()->type == SDL_KEYUP)
    {
        pressed = false;
    }

}

void GameManager::UpdateLevels()
{
        bool flag = false;
        for(vector<string>::iterator i=existinglevels.begin(); i!=existinglevels.end(); i++)
            if((*i) == levelname)
                flag = true;

        if(!flag)
        {
            existinglevels.push_back(levelname);
            mainmenu->savedlevelbuttons.push_back(new Button(gamewindow, ScreenWidth-110, 250+(mainmenu->savedlevelscount*25), 75, 25, levelname, "assets/sprites/button.png", &CameraX, &CameraY, 1, 3, 12));
            mainmenu->savedlevelscount++;
        }

        fstream levelsave("savedlevels.txt", ios::out);
        for(vector<string>::iterator i=existinglevels.begin(); i!=existinglevels.end(); i++)
            levelsave<<(*i)<<endl;
        levelsave.close();
}

