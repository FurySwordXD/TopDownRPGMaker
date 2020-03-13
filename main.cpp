#include <iostream>
#include "GameManager.h"
#include <wtypes.h>

using namespace std;

int main(int argc, char *args[])
{
    int horizontal;
    int vertical;
    RECT desktop;
    const HWND hDesktop = GetDesktopWindow();
    GetWindowRect(hDesktop, &desktop);
    horizontal = desktop.right;
    vertical = desktop.bottom;

    GameManager gamemanager(800,600);
    gamemanager.GameLoop();
    return 0;
}
