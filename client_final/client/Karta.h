#pragma once

#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include "TextureManager.h"

class Karta {

public:
    std::string frontImg;
    std::string backImg;
    std::string Img;
	bool isFlipped;
	bool isMatched;

    int x, y;
    float width, height;


    Karta() {}
    Karta(std::string _frontImg, std::string _backImg, int _x, int _y, int w, int h) {
        frontImg = _frontImg;
        backImg = _backImg;
        x = _x;
        y = _y;
        width = w;
        height = h;
        isFlipped = false;
        isMatched = false;
        Img = backImg;
    }

    void GetImg() {
        if (isFlipped) {
            Img = frontImg;
        }
        else
            Img = backImg;
    }
    
    bool checkClicked(int mouseX, int mouseY) {
        // Check if the mouse coordinates are within the bounds of the card's rectangle.
        return (mouseX >= x &&
            mouseX <= x + width &&
            mouseY >= y &&
            mouseY <= y + height);
    }

    void flip() {
        if (!isMatched) {
            isFlipped = !isFlipped;
            GetImg();  // Update the Img string after flipping
        }
    }

    void Render() {
        TextureManager::GetInstance()->Draw(Img, x,y, width, height);
    }
};