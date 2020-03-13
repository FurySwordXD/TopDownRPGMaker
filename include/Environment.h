#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H
#include "GameWindow.h"
#include "Sprite.h"
#include "GameObjects.h"
#include "Button.h"
#include <vector>
#include <fstream>
#include <sstream>
#include <cmath>
#include "TextField.h"
#include "Enemy.h"

using namespace std;

class Environment
{
    public:
        Environment(GameWindow* passed_gamewindow, int passed_ScreenWidth, int passed_ScreenHeight, float *passed_CameraX, float *passed_CameraY, string passed_levelname);
        virtual ~Environment();

        void Draw();
        void DrawFront();

        void Update();

        void Savetofile();
        void Loadfromfile();

        void EditMode();
        int GetGameMode();
        void TextBox();

        bool CheckOverlap(SDL_Rect a, SDL_Rect b);

        enum ModeType
        {
            GamePlay, GameEdit
        };
        vector<GameObjects*> GetTile();

        Sprite* PlaceBox;

        void UpdatePlaceObject(int placeboxX, int placeboxY);

        Button* selecttilebutton[7];
        Sprite* selecttileframe[7];

        Sprite* GetTextField();

        TextField* Objecttypetextfield;
        TextField* Widthtextfield;
        TextField* Heighttextfield;
        TextField* Leveltextfield;

        vector<Enemy*> enemies;

        vector<Sprite*> enemyspawner;
        vector<GameObjects*> portal;
        Sprite* playerspawner;

    protected:

    private:

        int Mode;
        int type;
        string levelname;
        string portallevel;
        bool block;

        GameWindow* gamewindow;

        vector<GameObjects*> tiles;

        int ScreenWidth;
        int ScreenHeight;

        float *CameraX;
        float *CameraY;

        bool click;
        bool press;

        int Currenttype;

        string tiletype;
        string tilepath;

        Sprite* imagecheck;

        TTF_Font* font;
        SDL_Color textcolor;

        SDL_Rect textRect;
        SDL_Texture* text;
        SDL_Surface* message;
        Sprite* textfield;

        int EditObjectType;

        int width,height;
};

#endif // ENVIRONMENT_H
