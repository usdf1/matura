#include "volan.h"
#include <cmath>
#include "TextureManager.h"
#include <iostream>
#include "Game.h"

double Volan::CalculateAngle() {
    SDL_GetMouseState(&mouseX, &mouseY);

    double deltaX = mouseX - volanCenterX;
    double deltaY = mouseY - volanCenterY;
    double currentMouseAngle = atan2(deltaY, deltaX) * (180.0 / M_PI);

    double angleDifference = currentMouseAngle - initialMouseAngle;

    angle += angleDifference;

    if (angle < 0) {
        angle += 360.0;
    }
    else if (angle >= 360.0) {
        angle -= 360.0;
    }

     initialMouseAngle = currentMouseAngle;

    return angle;
}


void Volan::CheckForHold() {
    if (IsHeld) {
        angle = CalculateAngle(); // Directly set the wheel's angle based on mouse position
    }
}


bool Volan::isMouseInsideRect()
{
    return (mouseX > Pos.x && mouseX < Pos.x + Pos.w &&
            mouseY > Pos.y && mouseY < Pos.y + Pos.h);
}

void Volan::Draw()
{
    TextureManager::GetInstance()->DrawWheel("volan", Pos.x, Pos.y, Pos.w, Pos.h, angle);
}

void Volan::OnMouseDown() {
    SDL_GetMouseState(&mouseX, &mouseY);

    if (isMouseInsideRect()) {
        IsHeld = true;

        // Set the center coordinates of the wheel
        volanCenterX = Pos.x + Pos.w / 2;
        volanCenterY = Pos.y + Pos.h / 2;

        // Calculate the initial angle of the mouse relative to the wheel center
        double deltaX = mouseX - volanCenterX;
        double deltaY = mouseY - volanCenterY;
        initialMouseAngle = atan2(deltaY, deltaX) * (180.0 / M_PI); // Store initial angle

         angle = initialMouseAngle + 90; 
    }
}

void Volan::OnMouseUp()
{
    IsHeld = 0;
}




