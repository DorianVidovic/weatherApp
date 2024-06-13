#include "gui.h"

gui::gui(int windowW, int windowH, bool res, std::string winText, Image icon)
{
#ifndef _DEBUG
    SetTraceLogLevel(LOG_NONE);
#else
    SetTraceLogLevel(LOG_ALL);
#endif 


    InitWindow(windowW, windowH, winText.c_str());

    windowWidth  = GetRenderWidth();
    windowHeight = GetRenderHeight();   

    screenW = GetMonitorWidth(0);
    screenH = GetMonitorHeight(0);

    SetWindowPosition((screenW - windowWidth) / 2, (screenH - windowHeight)/2);
    SetTargetFPS(10);

    SetWindowIcon(icon);
    //UnloadImage(icon);

}

gui::~gui()
{
    
    CloseWindow();
    for (element& currentElement : items)
    {
        UnloadImage(currentElement.img);
        UnloadTexture(currentElement.tex);
    }
    for (Font& currentFont : fonts)
    {
        UnloadFont(currentFont);
    }
}


void gui::createElement(Image imgPar, Vector2 size,Vector2 position)
{
    ImageResize(&imgPar, windowWidth*size.x, windowWidth*size.y);
    items.push_back({ imgPar, LoadTextureFromImage(imgPar),"" ,0 ,0 ,size.x ,size.y ,position.x, position.y});
}

void gui::createElement(std::string text, int fontType, int textSize, Vector2 position)
{
    Image i{};
    Texture2D e{};
    items.push_back({ i, e ,text ,fontType ,textSize ,0 ,0 ,position.x, position.y });
}

void gui::createElement(std::string text, int fontType, int textSize, int positionX, int positionY)
{
    Image i{};
    Texture2D e{};
    Vector2 position = { positionX , positionY };
    items.push_back({ i, e ,text ,fontType ,textSize ,0 ,0 ,position.x, position.y });
}

void gui::update()
{/*
    if (IsWindowResized())
    {
        windowWidth = GetRenderWidth();
        windowHeight = GetRenderHeight();

        std::cout << "\n  windowH= " << windowHeight << " - windowW= " << windowWidth
            << "\n background x=" << items[0].size.x*windowWidth  << std::endl;

        for (auto& currentElement: items)
        {
            if (!currentElement.text.empty())
            {
               // currentElement.textSize = (windowHeight * currentElement.textSize) / windowHeight;
            }
            else
            {
                ImageResize(&currentElement.img, currentElement.size.x*windowWidth, currentElement.size.y*windowHeight);
                UnloadTexture(currentElement.tex);
                currentElement.tex = LoadTextureFromImage(currentElement.img);
            }
            
        }
    }
 */


}

void gui::draw()
{
    BeginDrawing();
    ClearBackground(RAYWHITE);


    for (element& currentElement: items)
    {
        if (currentElement.text.empty())
        {
            DrawTexture(currentElement.tex, currentElement.position.x, currentElement.position.y, WHITE);
        }
        else
        {
            DrawTextEx(fonts.at(currentElement.fontType), currentElement.text.c_str(), currentElement.position, currentElement.textSize, 2, WHITE);
        }
    }
    EndDrawing();
}

bool gui::windowExit()
{
    return WindowShouldClose();
}

