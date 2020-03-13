#include "Sprite.h"

using namespace std;

Sprite::Sprite(SDL_Renderer* passed_renderer, string FilePath, int x, int y, int w, int h, int r, float *passed_CameraX, float *passed_CameraY, CollisionBox passed_collision, bool passed_cameramovable)
{
        collision = passed_collision;
        cameramovable = passed_cameramovable;

        CameraX = passed_CameraX;
        CameraY = passed_CameraY;

        renderer = passed_renderer;
        image = NULL;
        image = IMG_LoadTexture(renderer,FilePath.c_str());
        collision_image = NULL;

        if (image == NULL)
        {
            cout << "Couldn't Load " << FilePath.c_str() << endl;
        }

        rect.x = x;
        rect.y = y;
        rect.w = w;
        rect.h = h;

        SDL_QueryTexture(collision_image, NULL, NULL, &img_width, &img_height);
        SDL_QueryTexture(image, NULL, NULL, &img_width, &img_height);

        X_pos = x;
        Y_pos = y;

        crop.x = 0;
        crop.y = 0;
        crop.w = img_width;
        crop.h = img_height;

        CurrentFrame = 0;
        Amount_Frame_X = 0;
        Amount_Frame_Y = 0;

        animationdelay = SDL_GetTicks();

        rotationAngle = r;

        PlayAnimation(0,0,0,0);

        animover = false;

}


Sprite::~Sprite(void)
{
        SDL_DestroyTexture(image);
}

void Sprite::DrawCollision()
{
    //
}

void Sprite::Draw()
{
    SDL_Rect Position;
    if(cameramovable)
    {   Position.x = rect.x + *CameraX;
        Position.y = rect.y + *CameraY;
        collision.Setposition(rect.x + *CameraX, rect.y + *CameraY);
    }
    else
    {
        Position.x = rect.x;
        Position.y = rect.y;
        collision.Setposition(rect.x, rect.y);
    }
    Position.w = rect.w;
    Position.h = rect.h;

    SDL_RenderCopyEx(renderer, image, &crop, &Position, rotationAngle, NULL, SDL_FLIP_NONE);
    SDL_RenderCopy(renderer, collision_image, NULL, &collision.CollisionRect);
}


void Sprite::SetPosition(int X, int Y)
{
    rect.x = X;
    rect.y = Y;
}
void Sprite::SetX(int X)
{
    rect.x = X;
}
void Sprite::SetY(int Y)
{
    rect.y = Y;
}
void Sprite::SetW(int W)
{
    rect.w = W;
}
void Sprite::SetH(int H)
{
    rect.h = H;
}
int Sprite::GetX()
{
    return rect.x;
}
int Sprite::GetY()
{
    return rect.y;
}
int Sprite::GetW()
{
    return rect.w;
}
int Sprite::GetH()
{
    return rect.h;
}
void Sprite::PlayAnimation(int BeginFrame, int EndFrame, int Row, float Speed)
{
    if(animationdelay+Speed < SDL_GetTicks())
    {
        if(EndFrame <= CurrentFrame)
        {
            CurrentFrame = BeginFrame;
            animover = true;
        }
        else
        {
            CurrentFrame++;
            animover = false;
        }

        crop.x = CurrentFrame * (img_width/Amount_Frame_X);
        crop.y = Row * (img_height/Amount_Frame_Y);
        crop.w = img_width/Amount_Frame_X;
        crop.h = img_height/Amount_Frame_Y;

        animationdelay = SDL_GetTicks();
    }
}
bool Sprite::PlayAnimation_withreturn(int BeginFrame, int EndFrame, int Row, float Speed)
{
    if(animationdelay+Speed < SDL_GetTicks())
    {
        if(EndFrame <= CurrentFrame)
        {
            CurrentFrame = BeginFrame;
            return true;
        }
        else
        {
            CurrentFrame++;
        }

        crop.x = CurrentFrame * (img_width/Amount_Frame_X);
        crop.y = Row * (img_height/Amount_Frame_Y);
        crop.w = img_width/Amount_Frame_X;
        crop.h = img_height/Amount_Frame_Y;

        animationdelay = SDL_GetTicks();
    }
    return false;
}

void Sprite::SetUpAnimation(int passed_Amount_Frame_X, int passed_Amount_Frame_Y)
{
    Amount_Frame_X = passed_Amount_Frame_X;
    Amount_Frame_Y = passed_Amount_Frame_Y;
}

int Sprite::isColliding(CollisionBox theCollider)
{
    if(collision.GetRectangle().x + collision.GetRectangle().w < theCollider.GetRectangle().x || collision.GetRectangle().y + collision.GetRectangle().h < theCollider.GetRectangle().y || collision.GetRectangle().x > theCollider.GetRectangle().x + theCollider.GetRectangle().w || collision.GetRectangle().y > theCollider.GetRectangle().y + theCollider.GetRectangle().h)
        return 0;
    else
        return 1;
}
 CollisionBox Sprite::GetCollisionRect()
 {
     return collision;
 }
 SDL_Rect Sprite::GetRect()
 {
     return rect;
 }
void Sprite::Rotate(int angle)
{
     rotationAngle = angle;
}
