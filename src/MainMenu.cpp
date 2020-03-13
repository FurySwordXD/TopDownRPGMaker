#include "MainMenu.h"

MainMenu::MainMenu(GameWindow* passed_gamewindow,float *passed_CameraX, float *passed_CameraY, int passed_Screenwidth, int passed_Screenheight)
{
    gamewindow = passed_gamewindow;
    CameraX = passed_CameraX;
    CameraY = passed_CameraY;

    Screenheight = passed_Screenheight;
    Screenwidth = passed_Screenwidth;

    levelname = "";
    savedlevelnames="";
    savedlevelscount = 0;

    creategamebutton = new Button(gamewindow, Screenwidth/2, 350, 250, 55, "Create Map", "assets/sprites/button.png", CameraX, CameraY, 1, 3);
    playbutton = new Button(gamewindow, Screenwidth/2, 275, 250, 55, "Play Map", "assets/sprites/button.png", CameraX, CameraY, 1, 3);
    deletegamebutton = new Button(gamewindow, Screenwidth/2, 425, 250, 55, "Delete Map", "assets/sprites/button.png", CameraX, CameraY, 1, 3);
    exitbutton = new Button(gamewindow, Screenwidth/2, 500, 250, 55, "Exit", "assets/sprites/button.png", CameraX, CameraY, 1, 3);
    quitbutton = new Button(gamewindow, Screenwidth/2, 425, 250, 55, "Quit", "assets/sprites/button.png", CameraX, CameraY, 1, 3);
    resumebutton = new Button(gamewindow, Screenwidth/2, 275, 250, 55, "Resume", "assets/sprites/button.png", CameraX, CameraY, 1, 3);

    background = new Sprite(gamewindow->GetRenderer(),"assets/sprites/background.png", 0, 0, Screenwidth, Screenheight, 0, CameraX, CameraY, CollisionBox(0,0,0,0), false);
    pausebg = new Sprite(gamewindow->GetRenderer(),"assets/sprites/layer.png", 0, 0, Screenwidth, Screenheight, 0, CameraX, CameraY, CollisionBox(0,0,0,0), false);

    levelnametextfield = new TextField(gamewindow, Screenwidth/2 - 350/2, 150, 350, 75, CameraX, CameraY, "LevelName:");

    press = false;

    TTF_Init();
    font = TTF_OpenFont("assets/fonts/fofbb_reg.ttf", 50);

    textcolor = { 255, 255, 255 };
    textRect = {Screenwidth/2-325/2,25,325,75};
    textRect2 = {Screenwidth/2-325/2,100,325,75};

    gamenamemessage = TTF_RenderText_Blended(font, "Top Down RPG", textcolor);
    gamenametext = SDL_CreateTextureFromSurface(gamewindow->GetRenderer(), gamenamemessage);

    pausemessage = TTF_RenderText_Blended(font, "Game Paused!", textcolor);
    pausetext = SDL_CreateTextureFromSurface(gamewindow->GetRenderer(), pausemessage);

    deathmessage = TTF_RenderText_Blended(font, "You Have Died!", textcolor);
    deathtext = SDL_CreateTextureFromSurface(gamewindow->GetRenderer(), deathmessage);

    fstream savedlevelfile("savedlevels.txt", ios::in);
    if(savedlevelfile.good())
    {   cout<<"Saved levels exist";
        string level;

        while(savedlevelfile>>level)
        {
            savedlevelnames = savedlevelnames + level.c_str();
            savedlevelbuttons.push_back(new Button(gamewindow, Screenwidth-110, 250+(savedlevelscount*25), 75, 25, level, "assets/sprites/button.png", CameraX, CameraY, 1, 3, 12));
            savedlevelscount++;
        }
    }
    savedlevelfile.close();

}
MainMenu::~MainMenu()
{
    delete creategamebutton;
    delete deletegamebutton;
    delete playbutton;
    delete exitbutton;
    delete background;
    delete pausebg;
    delete quitbutton;
    delete resumebutton;
    delete levelnametextfield;
    for(vector<Button*>::iterator i = savedlevelbuttons.begin(); i!=savedlevelbuttons.end(); i++)
        delete (*i);
}

void MainMenu::Draw()
{
    background->Draw();
    levelnametextfield->Draw();

    creategamebutton->Draw();
    playbutton->Draw();
    exitbutton->Draw();
    deletegamebutton->Draw();
    savedlevelbuttons[0]->Draw();
    for(vector<Button*>::iterator i = savedlevelbuttons.begin(); i!=savedlevelbuttons.end(); i++)
        (*i)->Draw();
    levelname = levelnametextfield->inputtext;

    SDL_RenderCopy(gamewindow->GetRenderer(), gamenametext, NULL, &textRect);

}
void MainMenu::DrawPausemenu()
{
    pausebg->Draw();
    resumebutton->Draw();
    quitbutton->Draw();
    SDL_RenderCopy(gamewindow->GetRenderer(), pausetext, NULL, &textRect2);
}
void MainMenu::DrawDeathmenu()
{
    pausebg->Draw();
    quitbutton->Draw();
    SDL_RenderCopy(gamewindow->GetRenderer(), deathtext, NULL, &textRect2);
}

Button* MainMenu::GetPlayButton()
{
    return playbutton;
}
Button* MainMenu::GetExitButton()
{
    return exitbutton;
}
Button* MainMenu::GetCreategameButton()
{
    return creategamebutton;
}
Button* MainMenu::GetDeletegameButton()
{
    return deletegamebutton;
}
Button* MainMenu::Getresumebutton()
{
    return resumebutton;
}
Button* MainMenu::Getquitbutton()
{
    return quitbutton;
}
void MainMenu::DeleteCharacter()
{
    levelname = levelname.substr(0, levelname.length() - 1);
}


