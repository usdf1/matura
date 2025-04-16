#include "Text.h"
#include <iostream>

Text::Text() : mFont(nullptr) {}

Text::~Text() {
    closeFont();
}

bool Text::loadFont(const std::string& fontPath, int fontSize) {
    mFont = TTF_OpenFont(fontPath.c_str(), fontSize);
    if (!mFont) {
        std::cerr << "Failed to load font: " << TTF_GetError() << std::endl;
        return false;
    }
    return true;
}


void Text::closeFont() {
    if (mFont) {
        TTF_CloseFont(mFont);
        mFont = nullptr;
    }
}
