#include "TextField.h"

TextField::TextField(GameWindow* passed_gamewindow, int passed_x, int passed_y, int passed_w, int passed_h, float *CameraX, float *CameraY, string textfieldname)
{
    gamewindow = passed_gamewindow;

    x = passed_x;
    y = passed_y;
    w = passed_w;
    h = passed_h;

    inputtext = "";

    textfield = new Sprite(gamewindow->GetRenderer(), "assets/sprites/textField.png", x, y, w, h, 0, CameraX, CameraY, CollisionBox(0,0,w,h), false);
    MouseLocation = new Sprite(gamewindow->GetRenderer(),"assets/sprites/mapobjects/outline.", 0, 0, 25, 25, 0, CameraX,CameraY, CollisionBox(0,0,5,5), false);

    font = TTF_OpenFont("assets/fonts/fofbb_reg.ttf", 28);
    textcolor = { 255, 255, 255};
    press = false;
    click = false;
    inputmode = false;

    timecheck = SDL_GetTicks();

    textRect[0] = { (int)(x + w/2 -textfieldname.length()*8), y , w, h/2};
    message[0] = TTF_RenderText_Blended(font, textfieldname.c_str(), textcolor);
    text[0] = SDL_CreateTextureFromSurface(gamewindow->GetRenderer(), message[0]);

	string empty = "";
	message[1] = TTF_RenderText_Blended(font, empty.c_str(), textcolor);
	text[1] = SDL_CreateTextureFromSurface(gamewindow->GetRenderer(), message[1]);

    textRect[1] = {x +w/20 , (int)(y + h/2.5) , w, h/2};

    soundeffect = Mix_LoadWAV("assets/sounds/click.wav");
}

TextField::~TextField()
{
    Mix_FreeChunk(soundeffect);
    delete textfield;
    delete MouseLocation;
}

void TextField::Draw()
{
    textfield->Draw();
    Update();
    EditText(false);

    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);
    MouseLocation->SetPosition(mouseX,mouseY);
    MouseLocation->Draw();

    //SDL_FreeSurface(message[1]);

	for (int i = 0; i < 2; i++)	
		SDL_QueryTexture(text[i], NULL, NULL, &textRect[i].w, &textRect[i].h);

    SDL_RenderCopy(gamewindow->GetRenderer(), text[1], NULL, &textRect[1]);
    SDL_RenderCopy(gamewindow->GetRenderer(), text[0], NULL, &textRect[0]);
}

void TextField::EditText(bool called)
{
    if(inputmode)
    {
        SDL_StartTextInput();
        if(!press && inputtext.length() < w/20)
        {
            if(gamewindow->GetMainEvent()->type == SDL_TEXTINPUT)
            {
                press = true;
                inputtext = inputtext + gamewindow->GetMainEvent()->text.text;
                std::cout << inputtext << endl;
                message[1] = TTF_RenderText_Blended(font, displayinputtext.c_str(), textcolor);
                text[1] = SDL_CreateTextureFromSurface(gamewindow->GetRenderer(), message[1]);
            }
        }

        if(!press && inputtext.length() > 0)
            if(gamewindow->GetMainEvent()->type == SDL_KEYDOWN && gamewindow->GetMainEvent()->key.keysym.sym == SDLK_BACKSPACE)
            {
                press = true;
                inputtext = inputtext.substr(0, inputtext.length() - 1);
                message[1] = TTF_RenderText_Blended(font, displayinputtext.c_str(), textcolor);
                text[1] = SDL_CreateTextureFromSurface(gamewindow->GetRenderer(), message[1]);
            }

        if(timecheck+1000 < SDL_GetTicks())
        {
            displayinputtext = inputtext + "|";
            timecheck = SDL_GetTicks();
            message[1] = TTF_RenderText_Blended(font, displayinputtext.c_str(), textcolor);
            text[1] = SDL_CreateTextureFromSurface(gamewindow->GetRenderer(), message[1]);
        }
        else if(timecheck+500 < SDL_GetTicks())
        {
            displayinputtext = inputtext + " ";
            message[1] = TTF_RenderText_Blended(font, displayinputtext.c_str(), textcolor);
            text[1] = SDL_CreateTextureFromSurface(gamewindow->GetRenderer(), message[1]);
        }
    }
    if(!inputmode)
    {
    }

    if(gamewindow->GetMainEvent()->type == SDL_KEYUP)
        press = false;

    if(called)
    {
        displayinputtext = inputtext;
        message[1] = TTF_RenderText_Blended(font, displayinputtext.c_str(), textcolor);
        text[1] = SDL_CreateTextureFromSurface(gamewindow->GetRenderer(), message[1]);
    }
}

void TextField::Update()
{
    if(gamewindow->GetMainEvent()->type == SDL_MOUSEBUTTONDOWN && !click)
    {
        click = true;
        if(MouseLocation->collision.check_collision(textfield->collision.CollisionRect))
        {
            inputmode = true;
            Mix_PlayChannel(-1, soundeffect, 0);
        }
        else
        {
            inputmode = false;
            displayinputtext = inputtext;
            message[1] = TTF_RenderText_Blended(font, displayinputtext.c_str(), textcolor);
            text[1] = SDL_CreateTextureFromSurface(gamewindow->GetRenderer(), message[1]);
        }

    }
    if(gamewindow->GetMainEvent()->type == SDL_MOUSEBUTTONUP)
        click = false;

}
