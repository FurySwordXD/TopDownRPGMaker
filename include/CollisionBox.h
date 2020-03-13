#ifndef COLLISIONBOX_H
#define COLLISIONBOX_H
#include <SDL.h>

class CollisionBox
{
    public:
        CollisionBox();
        CollisionBox(int x, int y, int w, int h);
        virtual ~CollisionBox();

        void SetRectangle(int x, int y, int w, int h);
        void Setposition(int x, int y);

        SDL_Rect GetRectangle();
        SDL_Rect CollisionRect;
        bool check_collision(SDL_Rect b);

    protected:

    private:

        int OffsetX;
        int OffsetY;

};

#endif // COLLISIONBOX_H
