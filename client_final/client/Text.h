#pragma once

#include <SDL_ttf.h>
#include <string>

class Text {
public:
    Text();
    ~Text();

    bool loadFont(const std::string& fontPath, int fontSize);
    void closeFont();
    TTF_Font* mFont;

};