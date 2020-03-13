#include "Environment.h"

using namespace std;

Environment::Environment(GameWindow* passed_gamewindow, int passed_ScreenWidth, int passed_ScreenHeight, float *passed_CameraX, float *passed_CameraY, string passed_levelname)
{
    ScreenWidth = passed_ScreenWidth;
    ScreenHeight = passed_ScreenHeight;
    CameraX = passed_CameraX;
    CameraY = passed_CameraY;
    gamewindow = passed_gamewindow;
    Mode = GamePlay;
    click = false;
    press = false;
    block = false;
    levelname = passed_levelname;
    height = width = 50;

    PlaceBox = new Sprite(gamewindow->GetRenderer(), "assets/sprites/mapobjects/outline.png", 0, 0, 50, 50, 0, CameraX, CameraY, CollisionBox(0,0,50,50), true);
    EditObjectType = type = 0;
    Objecttypetextfield = new TextField(gamewindow, 0 , ScreenHeight-75, 400, 75, CameraX, CameraY, "Object:");
    Widthtextfield = new TextField(gamewindow, 0 , ScreenHeight-150, 200, 75, CameraX, CameraY, "Width:");
    Heighttextfield = new TextField(gamewindow, 200 , ScreenHeight-150, 200, 75, CameraX, CameraY, "Height:");
    Leveltextfield = new TextField(gamewindow, 400 , ScreenHeight-75, 300, 75, CameraX, CameraY, "Level:");

    Widthtextfield->inputtext = "50";
    Heighttextfield->inputtext = "50";
    for(int i=0; i < 7; i++)
    {
        selecttilebutton[i] = new Button(gamewindow, ScreenWidth-34, i*60+87.5, 53, 50, "", "assets/sprites/icons.png", CameraX, CameraY, 7,1);
        selecttileframe[i] = new Sprite(gamewindow->GetRenderer(), "assets/sprites/itemFrame.png", ScreenWidth-70, i*60+80, 70, 60, 0, CameraX, CameraY, CollisionBox(0,0,0,0), false);
        selecttileframe[i]->SetUpAnimation(2,7);
    }
    Objecttypetextfield->inputmode = false;
    Widthtextfield->inputmode = false;
    Heighttextfield->inputmode = false;

    font = TTF_OpenFont("assets/fonts/fofbb_reg.ttf", 24);
    textcolor = { 255, 255, 255 };

    playerspawner = new Sprite(gamewindow->GetRenderer(), "assets/sprites/player_spawner.png", 500, 300, 50, 75, 0, CameraX, CameraY, CollisionBox(0,0,0,0), true);
}

Environment::~Environment()
{
    for(vector<GameObjects*>::iterator i = tiles.begin(); i != tiles.end(); i++)
        delete (*i);
    for(vector<Sprite*>::iterator i = enemyspawner.begin(); i != enemyspawner.end(); i++)
        delete (*i);
    for(vector<Enemy*>::iterator i = enemies.begin(); i != enemies.end(); i++)
        delete (*i);
    delete Objecttypetextfield;
    delete Widthtextfield;
    delete Heighttextfield;
    delete Leveltextfield;
    delete playerspawner;
    delete PlaceBox;
    for(int i=0; i < 7; i++)
        delete selecttilebutton[i];
}

void Environment::Draw()
{

    if(Mode == GameEdit)
    {
        for(vector<GameObjects*>::iterator i = tiles.begin(); i != tiles.end(); i++)
            if(!((*i)->block))
                (*i)->Draw();
        for(vector<GameObjects*>::iterator i = tiles.begin(); i != tiles.end(); i++)
            if((*i)->block)
                (*i)->Draw();
        for(vector<Sprite*>::iterator i = enemyspawner.begin(); i != enemyspawner.end();i++)
            (*i)->Draw();
        playerspawner->Draw();
    }
    else
    {
        for(vector<GameObjects*>::iterator i = tiles.begin(); i != tiles.end(); i++)
            if(!((*i)->block) )
                (*i)->Draw();
        for(vector<GameObjects*>::iterator i = tiles.begin(); i != tiles.end(); i++)
            if((*i)->block )
                (*i)->Draw();
        for(vector<Enemy*>::iterator i = enemies.begin(); i != enemies.end();i++)
            (*i)->Draw();
    }
    for(vector<GameObjects*>::iterator i = portal.begin(); i != portal.end();i++)
            (*i)->Draw();


}

void Environment::DrawFront()
{
    for(vector<GameObjects*>::iterator i = tiles.begin(); i != tiles.end(); i++)
        if((*i)->refer == "door_closed" || (*i)->refer == "door_open")
            (*i)->Draw();
}


void Environment::Update()
{
    if(Mode == GamePlay)
    {
        Mode =  GameEdit;
        cout<<"GameEdit Mode\n";
    }
    else
    {   Mode = GamePlay;
        cout<<"GamePlay Mode\n";
    }
}
int Environment::GetGameMode()
{
    return Mode;
}

void Environment::Savetofile()
{
    fstream savefile;
    string word;
    string savefilename = "assets/Levels/"+levelname+".txt";
    savefile.open(savefilename.c_str(), ios::out);
    int b = 0;
    for (vector<GameObjects*>::iterator i = tiles.begin(); i != tiles.end();i++)
    {
        if((*i)->block)
            b = 1;
        else
            b = 0;
        savefile << "x: " << (*i)->GetX() << "\ty: " << (*i)->GetY() <<"\tw: " << (*i)->GetW() <<"\th: " << (*i)->GetH()<< "\tr: " << (*i)->GetR() << "\tb: " << b;
        if((*i)->refer == "" && Mode == GamePlay)
            (*i)->refer = "outline";
        savefile<<"\t"<<(*i)->refer<<endl;
    }
    savefile<<"\nPlayerSpawn: \tx: "<<playerspawner->GetX()<<"\ty: "<<playerspawner->GetY()<<endl;
    for(vector<Sprite*>::iterator i = enemyspawner.begin(); i != enemyspawner.end();i++)
    {
        savefile<<"\nEnemySpawn: \tx: "<<(*i)->GetX()<<"\ty: "<<(*i)->GetY();
    }
    savefile<<endl;
    for (vector<GameObjects*>::iterator i = portal.begin(); i != portal.end();i++)
    {
        savefile<<"\nPortal: "<< "x: " << (*i)->GetX() << "\ty: " << (*i)->GetY() << "\tLevel: "<< (*i)->textdata;
        savefile<<"\t"<<(*i)->refer<<endl;
    }

    savefile.close();
    portal.clear();
    tiles.clear();
    enemyspawner.clear();
    cout<<"Game Saved!\n";
}

void Environment::Loadfromfile()
{
    while(tiles.size() > 0)
        tiles.pop_back();
    fstream loadfile;
    string line;
    string loadfilename = "assets/Levels/"+levelname+".txt";
    int TempX = 0;
    int TempY = 0;
    int TempW = 50;
    int TempH = 50;
    int TempR = 0;
    int TempB = 0;
    loadfile.open(loadfilename.c_str(), ios::in);
    if(loadfile.is_open())
    {   tiles.reserve(1000);
        cout<<"\nLoading...\n";
        while(!loadfile.eof())
        {
            getline(loadfile,line);
            istringstream iss(line);
            while (iss)
            {
                string word;
                iss>>word;
                if (word == "x:")
                {   iss>>word;
                    TempX = atoi( word.c_str() );
                }
                else if (word == "y:")
                {   iss>>word;
                    TempY = atoi( word.c_str() );
                }
                else if (word == "w:")
                {   iss>>word;
                    TempW = atoi( word.c_str() );
                }
                else if (word == "h:")
                {   iss>>word;
                    TempH = atoi( word.c_str() );
                }
                else if (word == "r:")
                {   iss>>word;
                    TempR = atoi( word.c_str() );
                }
                else if (word == "b:")
                {   iss>>word;
                    TempB = atoi( word.c_str() );
                    if(TempB == 1)
                        block = true;
                    else
                        block = false;
                }
                else
                {
                    if(word != "")
                    {   imagecheck = new Sprite(gamewindow->GetRenderer(), "assets/sprites/mapobjects/"+word+".png", 0,0,0,0,0, CameraX, CameraY, CollisionBox(0,0,0,0), true);
                        if(imagecheck->GetImage() != NULL)
                        {
                            cout<<word<<"\n";
                            if(word == "outline" && Mode == GamePlay)
                                word = "";
                            tiles.push_back(new GameObjects(gamewindow, "assets/sprites/mapobjects/"+word+".png", TempX, TempY, TempW, TempH, TempR, CameraX, CameraY, word, block));
                            SDL_Delay(5);
                        }
                    }
                }
                if(word == "PlayerSpawn:")
                {
                    iss>>word;
                    if(word == "x:")
                    {
                        iss>>word;
                        playerspawner->SetX(atoi(word.c_str()));
                    }
                    iss>>word;
                    if(word == "y:")
                    {
                        iss>>word;
                        playerspawner->SetY(atoi(word.c_str()));
                    }

                }
                if(word == "EnemySpawn:")
                {
                    int x=0,y=0;
                    iss>>word;
                    if(word == "x:")
                    {
                        iss>>word;
                        x = atoi(word.c_str());
                    }
                    iss>>word;
                    if(word == "y:")
                    {
                        iss>>word;
                        y = atoi(word.c_str());
                    }
                    enemyspawner.push_back(new Sprite(gamewindow->GetRenderer(), "assets/sprites/enemy_spawner.png", x, y, 50, 75, 0, CameraX, CameraY, CollisionBox(0,0,0,0), true));
                }
                if(word == "Portal:")
                {
                    int x=0,y=0;
                    string level;
                    iss>>word;
                    if (word == "x:")
                    {   iss>>word;
                        x = atoi( word.c_str() );
                        cout<<x;
                    }
                    iss>>word;
                    if (word == "y:")
                    {   iss>>word;
                        y = atoi( word.c_str() );
                        cout<<y;
                    }
                    iss>>word;
                    if(word == "Level:")
                    {
                        iss>>word;
                        level = word;
                        cout<<level;
                    }
                    iss>>word;
                    if(word != "")
                    {
                        cout<<word<<"\n";
                        portal.push_back(new GameObjects(gamewindow, "assets/sprites/mapobjects/"+word+".png", x, y, 100, 100, 0, CameraX, CameraY, word, false, level));
                        //SDL_Delay(20);
                    }
                }
            }
        }
        if(loadfile.eof())
        {
            cout<<"Game Loaded!"<<endl;
        }
    }
    else
        cout<<"File Doesn't Exist!\n";
    loadfile.close();
}

vector<GameObjects*> Environment::GetTile()
{
        return tiles;
}

void Environment::EditMode()
{
    int MouseX, MouseY;
    SDL_GetMouseState(&MouseX,&MouseY);

    int placeboxX = floor(((MouseX - *CameraX )/50)) * 50;
    int placeboxY = floor(((MouseY - *CameraY )/50)) * 50;

    PlaceBox->SetPosition(placeboxX,placeboxY);
    PlaceBox->SetW(width);
    PlaceBox->SetH(height);
    PlaceBox->Draw();

    for(int i =0 ; i < 7; i++)
    {
        selecttileframe[i]->Draw();
        selecttilebutton[i]->Draw();
        selecttilebutton[i]->buttonTexture->PlayAnimation(i,i,0,0);
    }

    for(int i =0 ; i < 7; i++)
    {
        if(selecttilebutton[i]->Buttonclick(false))
        {
                    EditObjectType = i+1;
                    cout <<EditObjectType<<endl;
                    if(EditObjectType == 6)
                    {
                        tiletype = "Door_Closed";
                        Objecttypetextfield->inputtext = tiletype;
                    }
                    else
                    {
                        tiletype = "";
                        Objecttypetextfield->inputtext = tiletype;
                    }

        }
    }

    if(gamewindow->GetMainEvent()->key.keysym.sym == SDLK_q && (Objecttypetextfield->inputmode == false && Widthtextfield->inputmode == false && Heighttextfield->inputmode == false && Leveltextfield->inputmode == false))
        EditObjectType = 0;

    switch(EditObjectType)
    {
        case 1:
            TextBox();
            block = false;
            selecttileframe[0]->PlayAnimation(1,0,0,0);
            for(int i =0 ; i < 7; i++)
                if(i != 0)
                   selecttileframe[i]->PlayAnimation(0,0,0,0);
            break;

        case 2:
            block = true;
            selecttileframe[1]->PlayAnimation(1,0,0,0);
            for(int i =0 ; i < 7; i++)
                if(i != 1)
                    selecttileframe[i]->PlayAnimation(0,0,0,0);
            break;

        case 3:
            TextBox();
            block = false;
            selecttileframe[2]->PlayAnimation(1,0,0,0);
            for(int i =0 ; i < 7; i++)
                if(i != 2)
                   selecttileframe[i]->PlayAnimation(0,0,0,0);
            break;

        case 4:
            TextBox();
            block = true;
            selecttileframe[3]->PlayAnimation(1,0,0,0);
            for(int i =0 ; i < 7; i++)
                if(i != 3)
                   selecttileframe[i]->PlayAnimation(0,0,0,0);
            break;

        case 5:
            tiletype = "";
            playerspawner->SetPosition(placeboxX, placeboxY);
            block = false;
            selecttileframe[4]->PlayAnimation(1,0,0,0);
            for(int i =0 ; i < 7; i++)
                if(i != 4)
                   selecttileframe[i]->PlayAnimation(0,0,0,0);
            if(gamewindow->GetMainEvent()->type == SDL_MOUSEBUTTONDOWN && *CameraX + placeboxX < ScreenWidth - 100)
            {
                EditObjectType = 0;
            }
            break;

        case 6:
            block = false;
            Objecttypetextfield->Draw();
            Leveltextfield->Draw();
            selecttileframe[5]->PlayAnimation(1,0,0,0);
            for(int i =0 ; i < 7; i++)
                if(i != 5)
                   selecttileframe[i]->PlayAnimation(0,0,0,0);
            break;

        case 7:
            block = false;
            selecttileframe[6]->PlayAnimation(1,0,0,0);
            for(int i =0 ; i < 7; i++)
                if(i != 6)
                   selecttileframe[i]->PlayAnimation(0,0,0,0);
            break;

        default:
            tiletype = "";
            for(int i =0 ; i < 7; i++)
                    selecttileframe[i]->PlayAnimation(0,0,0,0);
    }

    if(EditObjectType != 2 || EditObjectType != 6 || EditObjectType != 5 || EditObjectType != 4)
    {
        tiletype = Objecttypetextfield->inputtext;
        if(Widthtextfield->inputtext != "")
        {
            width = atoi(Widthtextfield->inputtext.c_str());
        }
        if(Heighttextfield->inputtext != "")
        {
            height = atoi(Heighttextfield->inputtext.c_str());
        }
    }
    if(EditObjectType == 2)
    {
        tiletype = "outline";
        width = 50;
        height = 50;
    }
    if(EditObjectType == 6)
    {
        portallevel = Leveltextfield->inputtext;
        width = 100;
        height = 100;
    }


    if(*CameraX + placeboxX < ScreenWidth - 100)
    {
        if(EditObjectType == 7 && gamewindow->GetMainEvent()->type == SDL_MOUSEBUTTONDOWN && !click)
        {
                click = true;
                enemyspawner.push_back(new Sprite(gamewindow->GetRenderer(), "assets/sprites/enemy_spawner.png", placeboxX, placeboxY, 50, 75, 0, CameraX, CameraY, CollisionBox(0,0,0,0), true));
                for(unsigned int i=0; i < enemyspawner.size(); i++)
                {
                    for(unsigned int j=0; j < enemyspawner.size(); j++)
                    {
                        if(enemyspawner[i]->GetX() == enemyspawner[j]->GetX() && enemyspawner[i]->GetY() == enemyspawner[j]->GetY())
                            if(i != j)
                            {
                                enemyspawner.erase(enemyspawner.begin()+i);
                                cout<<"Delete\n";
                            }

                    }
                }
        }
        if(EditObjectType == 6)
        {
            if(gamewindow->GetMainEvent()->type == SDL_MOUSEBUTTONDOWN && !click && gamewindow->GetMainEvent()->button.button == SDL_BUTTON_LEFT)
            {
                click = true;
                tilepath = "assets/sprites/mapobjects/" + tiletype + ".png";
                imagecheck = new Sprite(gamewindow->GetRenderer(), tilepath, 0,0,0,0,0, CameraX, CameraY, CollisionBox(0,0,0,0), true);
                if(imagecheck->GetImage() != NULL && portallevel != "")
                {
                    portal.push_back(new GameObjects(gamewindow, tilepath, placeboxX, placeboxY, width, height, 0, CameraX, CameraY, tiletype, false, portallevel));
                }
                for(unsigned int i=0; i < portal.size(); i++)
                    {
                        for(unsigned int j=0; j < portal.size(); j++)
                        {
                            if(portal[i]->GetX() == portal[j]->GetX() && portal[i]->GetY() == portal[j]->GetY())
                                if(i != j)
                                {
                                    portal.erase(portal.begin()+i);
                                    cout<<"Delete\n";
                                }

                        }
                    }
            }
            if(gamewindow->GetMainEvent()->type == SDL_MOUSEBUTTONDOWN && !click && gamewindow->GetMainEvent()->button.button == SDL_BUTTON_RIGHT)
            {
                for(unsigned int i=0; i < portal.size(); i++)
                    if(placeboxX == portal[i]->GetGameObject()->GetX() && placeboxY == portal[i]->GetGameObject()->GetY())
                    {
                        click = true;
                        cout<<"Delete "<<portal[i]->refer<<endl;
                        portal.erase(portal.begin()+i);
                    }
            }
        }
    }

    if(*CameraX + placeboxX < ScreenWidth - 100)
    {
        if(gamewindow->GetMainEvent()->type == SDL_MOUSEBUTTONDOWN && !click && gamewindow->GetMainEvent()->button.button == SDL_BUTTON_LEFT)
        {
                click = true;
                tilepath = "assets/sprites/mapobjects/" + tiletype + ".png";
                imagecheck = new Sprite(gamewindow->GetRenderer(), tilepath, 0,0,0,0,0, CameraX, CameraY, CollisionBox(0,0,0,0), true);
                if(imagecheck->GetImage() != NULL && (EditObjectType == 1 || EditObjectType == 2 || EditObjectType == 3 || EditObjectType == 4) )
                {
                    tiles.push_back(new GameObjects(gamewindow, tilepath , placeboxX, placeboxY, width, height,0, CameraX, CameraY, tiletype, block));
                    cout<<"Placed "<<tiletype<<endl;
                    if(EditObjectType == 3)
                    for(unsigned int i=0; i < tiles.size(); i++)
                    {
                        for(unsigned int j=0; j < tiles.size(); j++)
                        {
                            if( (!(tiles[i]->block) ) && (!(tiles[j]->block)))
                                if(tiles[i]->GetGameObject()->GetX() == tiles[j]->GetGameObject()->GetX() && tiles[i]->GetGameObject()->GetY() == tiles[j]->GetGameObject()->GetY())
                                    if(i != j)
                                    {
                                        tiles.erase(tiles.begin()+i);
                                        cout<<"Delete\n";
                                    }

                        }
                    }
                }
        }
    }
    if(gamewindow->GetMainEvent()->type == SDL_MOUSEBUTTONDOWN && gamewindow->GetMainEvent()->button.button == SDL_BUTTON_RIGHT && !click )
        for(unsigned int i=0; i < tiles.size(); i++)
            if(placeboxX == tiles[i]->GetGameObject()->GetX() && placeboxY == tiles[i]->GetGameObject()->GetY())
            {
                click = true;
                cout<<"Delete "<<tiles[i]->refer<<endl;
                tiles.erase(tiles.begin()+i);
            }
    if(EditObjectType == 7 && gamewindow->GetMainEvent()->type == SDL_MOUSEBUTTONDOWN && gamewindow->GetMainEvent()->button.button == SDL_BUTTON_RIGHT)
        for(unsigned int i=0; i < enemyspawner.size(); i++)
            if(placeboxX == enemyspawner[i]->GetX() && placeboxY == enemyspawner[i]->GetY())
            {
                enemyspawner.erase(enemyspawner.begin()+i);
                cout<<"Delete ";
            }
    if(gamewindow->GetMainEvent()->type == SDL_KEYDOWN && gamewindow->GetMainEvent()->key.keysym.sym == SDLK_r && !press)
        for(unsigned int i=0; i < tiles.size(); i++)
            if(placeboxX == tiles[i]->GetGameObject()->GetX() && placeboxY == tiles[i]->GetGameObject()->GetY())
            {
                press = true;
                cout<<"Rotate "<<tiles[i]->refer<<endl;
                tiles[i]->Rotate(90);
            }
    if(gamewindow->GetMainEvent()->type == SDL_MOUSEBUTTONDOWN && gamewindow->GetMainEvent()->button.button == SDL_BUTTON_MIDDLE && !click && EditObjectType != 0)
        for(unsigned int i=0; i < tiles.size(); i++)
            if(placeboxX == tiles[i]->GetGameObject()->GetX() && placeboxY == tiles[i]->GetGameObject()->GetY())
            {
                click = true;
                cout<<tiles[i]->refer<<endl;
                tiletype = tiles[i]->refer;
                Objecttypetextfield->inputtext = tiles[i]->refer;
                Objecttypetextfield->EditText(true);
            }
    if(gamewindow->GetMainEvent()->type == SDL_KEYUP)
        press = false;
    if(gamewindow->GetMainEvent()->type == SDL_MOUSEBUTTONUP)
        click = false;

}

void Environment::TextBox()
{
        Objecttypetextfield->Draw();
        Widthtextfield->Draw();
        Heighttextfield->Draw();

}

