#ifndef SPRITE_H
#define SPRITE_H
#include <iostream>
#include <SDL.h>
#include <cstdlib>
#include <SDL_image.h>
#include "CollisionBox.h"

using namespace std;

class Sprite
{
    public:
        Sprite(SDL_Renderer* passed_renderer, string FilePath, int x, int y, int w, int h, int r, float *passed_CameraX, float *passed_CameraY, CollisionBox passed_collision, bool passed_cameramovable);
        ~Sprite(void);

        void Draw();
        void DrawCollision();

        void SetPosition(int X, int Y);
        void SetX(int X);
        void SetY(int Y);
        void SetW(int W);
        void SetH(int H);
        void Rotate(int angle);
        int GetX();
        int GetY();
        int GetW();
        int GetH();
        void PlayAnimation(int BeginFrame, int EndFrame, int Row, float Speed);
        bool PlayAnimation_withreturn(int BeginFrame, int EndFrame, int Row, float Speed);
        void SetUpAnimation(int passed_Amount_Frame_X, int passed_Amount_Frame_Y);

        void SetFontSize(int fontSize);
        void LoadFromText(string text);
        void SetCropping(int x, int y, int w, int h);
        void SetTextColor(int r,int g,int b,int a);

        int isColliding(CollisionBox theCollider);
        CollisionBox GetCollisionRect();
        SDL_Rect GetRect();

        CollisionBox collision;

        SDL_Texture* SurfaceToTexture( SDL_Surface* surf );
        SDL_Texture* GetImage(){return image;}

        int rotationAngle;
        bool animover;

    private:
        bool cameramovable;

        SDL_Texture* image;
        SDL_Texture* collision_image;


        SDL_Rect crop;
        SDL_Rect rect;

        float *CameraX;
        float *CameraY;

        float X_pos;
        float Y_pos;

        int img_width;
        int img_height;
        int CurrentFrame;
        int animationdelay;

        int Amount_Frame_X;
        int Amount_Frame_Y;

        SDL_Renderer* renderer;

};

#endif // SPRITE_H
