#include "sdlwidget.h"
#include <SDL2/SDL_syswm.h>
#include <QTimer>

SDLWidget::SDLWidget(QWidget* parent) : QWidget(parent), sdlWindow(nullptr), sdlRenderer(nullptr)
{
    setAttribute(Qt::WA_PaintOnScreen);
    // setAttribute(Qt::WA_NativeWindow);
    setAttribute(Qt::WA_OpaquePaintEvent);
    // setAttribute(Qt::WA_autoFillBackground);
    setAttribute(Qt::WA_NoSystemBackground);

    // Set up a timer to trigger periodic rendering
    // timer = new QTimer(this);
    // connect(timer, &QTimer::timeout, this, [=]() {
    //     update();  // Force SDL to render periodically
    // });
    // timer->start(16);  // ~60 FPS
}

SDLWidget::~SDLWidget()
{
    if (this->sdlRenderer)
    {
        SDL_DestroyRenderer(this->sdlRenderer);
    }

    if (this->sdlWindow)
    {
        SDL_DestroyWindow(this->sdlWindow);
    }
    SDL_Quit();
}

// Initialize SDL when the widget is shown
void SDLWidget::showEvent(QShowEvent* event)
{
    QWidget::showEvent(event);

    // Get the native window handle
    SDL_SysWMinfo wminfo;
    SDL_VERSION(&wminfo.version);

    // SDL must be initialized before calling this
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        qDebug() << "SDL could not initialize! SDL_Error:" << SDL_GetError();
        return;
    }

    // Create SDL window from QWidget's window handle
    this->createWinId();
    this->sdlWindow = SDL_CreateWindowFrom((void*)(winId()));
    if (this->sdlWindow == nullptr)
    {
        qDebug() << "SDL_CreateWindowFrom() failed!" << SDL_GetError();
        return;
    }
    SDL_ShowWindow(sdlWindow);
    Uint32 windowFlags = SDL_GetWindowFlags(sdlWindow);
    qDebug() << "Window flags:" << windowFlags;

    if (windowFlags & SDL_WINDOW_HIDDEN) {
        qDebug() << "SDL window is hidden";
    }

    // Create SDL Renderer
    this->sdlRenderer = SDL_CreateRenderer(this->sdlWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);    // Blue
    if (this->sdlRenderer == nullptr)
    {
        qDebug() << "SDL_CreateRenderer() failed!" << SDL_GetError();
        return;
    }

    SDL_RendererInfo info;
    SDL_GetRendererInfo(sdlRenderer, &info);
    qDebug() << "Renderer name:" << info.name;
    qDebug() << "Renderer flags:" << info.flags;
    qDebug() << "Renderer num_texture_formats:" << info.num_texture_formats;
}

// Handle resizing of the widget
void SDLWidget::resizeEvent(QResizeEvent* event)
{
    QWidget::resizeEvent(event);

    if (this->sdlRenderer)
    {
        SDL_RenderPresent(this->sdlRenderer);
    }
}

// Render using SDL in paintEvent
void SDLWidget::paintEvent(QPaintEvent *event)
{
    // if (this->sdlRenderer) {
    //     // Set the render draw color (blue) and clear the screen
    //     SDL_SetRenderDrawColor(this->sdlRenderer, 0, 0, 255, SDL_ALPHA_OPAQUE);  // Blue
    //     SDL_RenderClear(this->sdlRenderer);
    //     SDL_RenderPresent(this->sdlRenderer);  // Present the rendered content
    // }

    // this->update();
}

void SDLWidget::render() {
    // QTimer *timer = new QTimer(this);
    // connect(timer, &QTimer::timeout, this, [=]() {
    //     if (sdlRenderer) {
    //         SDL_SetRenderDrawColor(sdlRenderer, 0, 0, 255, 255);  // Blue background
    //         SDL_RenderClear(sdlRenderer);
    //         SDL_RenderPresent(sdlRenderer);
    //     }
    // });
    // timer->start(16);  // 60 FPS

    float rectX = 0;  // Initial X position of the rectangle
    int rectY = 100;  // Fixed Y position for the rectangle
    int rectWidth = 32;
    int rectHeight = 32;
    int speed = 16;  // Speed in pixels per second

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [=, &rectX]() mutable {
        if (sdlRenderer) {
            // Set the background color (blue)
            SDL_SetRenderDrawColor(sdlRenderer, 0, 0, 255, 255);
            SDL_RenderClear(sdlRenderer);

            // Set the draw color for the rectangle (white)
            SDL_SetRenderDrawColor(sdlRenderer, 255, 255, 255, 255);

            // Update the rectangle's position (move 16 pixels per second)
            rectX += speed * (16.0 / 1000.0);  // 16ms frame duration (60 FPS)

            // If the rectangle goes off the screen, reset it to the left
            if (rectX > this->width()) {
                rectX = 0;
            }

            // Create the rectangle and render it
            SDL_Rect rect = { (int)rectX, rectY, rectWidth, rectHeight };
            SDL_RenderFillRect(sdlRenderer, &rect);

            // Present the rendered content
            SDL_RenderPresent(sdlRenderer);
        }
    });

    timer->start(16);  // 60 FPS (16ms per frame)
}
