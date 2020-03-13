#include "Button.h"

Button::Button(GameWindow* passed_gamewindow, int x, int y, int w, int h, string passed_buttonText, string buttonpath, float* passed_CameraX, float* passed_CameraY, int passed_frameX, int passed_frameY, int fontsize)
{
    frameX = passed_frameX;
    frameY = passed_frameY;

    gamewindow = passed_gamewindow;

    CameraX = passed_CameraX;
    CameraY = passed_CameraY;

    buttontext = passed_buttonText;

    Clicked = false;

    MouseLocation = new Sprite(gamewindow->GetRenderer(),"/", 0, 0, 25, 25, 0, CameraX,CameraY, CollisionBox(0,0,5,5), true);
    buttonTexture = new Sprite(gamewindow->GetRenderer(),buttonpath, x-w/2, y, w, h, 0, CameraX, CameraY, CollisionBox(0,0,w,h), false);
    buttonTexture->SetUpAnimation(frameX,frameY);

    TTF_Init();
    TTF_Font* font;
    font = TTF_OpenFont("assets/fonts/fofbb_reg.ttf", fontsize);
    if ( font == NULL )
	{
		std::cout << " Failed to load font : " << SDL_GetError() << std::endl;
	}

    SDL_Color textcolor = { 255, 255, 255, 255 };
    message = TTF_RenderText_Blended(font, buttontext.c_str(), textcolor);
	text = SDL_CreateTextureFromSurface(gamewindow->GetRenderer(), message );
	textRect = { static_cast<int>(x-buttontext.length()*fontsize/4), y+h/4, w, h};

    SDL_QueryTexture( text, NULL, NULL, &textRect.w, &textRect.h );

    soundeffect = Mix_LoadWAV("assets/sounds/click.wav");

}

Button::~Button()
{
    delete buttonTexture;
    delete MouseLocation;

}

void Button::Update()
{
    int MouseX;
    int MouseY;
    SDL_GetMouseState(&MouseX,&MouseY);
    MouseLocation->SetPosition(MouseX - *CameraX, MouseY - *CameraY);
}

void Button::Hover()
{
    buttonTexture->PlayAnimation(0,frameX-1,0,0);
}
void Button::Click()
{
    buttonTexture->PlayAnimation(0,frameX-1,2,0);
}
void Button::NotHover()
{
    buttonTexture->PlayAnimation(0,frameX-1,1,0);
}

void Button::Draw()
{
    Update();
    buttonTexture->Draw();
    MouseLocation->Draw();
    SDL_RenderCopy(gamewindow->GetRenderer(), text, NULL, &textRect);
}

bool Button::Buttonclick(bool has_animation)
{
    if(buttonTexture->collision.check_collision(MouseLocation->collision.CollisionRect))
    {
        if(gamewindow->GetMainEvent()->type == SDL_MOUSEBUTTONDOWN && !Clicked)
        {
            if(has_animation)
                Click();
            Mix_PlayChannel(-1, soundeffect, 0);
            Clicked = true;
            return true;
        }
        else
            if(has_animation)
                Hover();
    }
    else
        if(has_animation)
            NotHover();
    if(gamewindow->GetMainEvent()->type == SDL_MOUSEBUTTONUP)
        Clicked = false;

    return false;

}
