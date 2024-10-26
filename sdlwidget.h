#ifndef SDLWIDGET_H
#define SDLWIDGET_H

#include <QWidget>
#include <SDL2/SDL.h>

class SDLWidget : public QWidget
{
    Q_OBJECT

public:
    SDLWidget(QWidget* parent = nullptr);
    ~SDLWidget();
    void render();

protected:
    void showEvent(QShowEvent* event) override;
    void paintEvent(QPaintEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;

private:
    SDL_Window* sdlWindow;
    SDL_Renderer* sdlRenderer;
    QTimer *renderTimer;
};

#endif // SDLWIDGET_H
