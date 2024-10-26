#include "launcherwindow.h"

#include <QApplication>
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    LauncherWindow w;
    w.show();

    return a.exec();
}
