#pragma once
#include <cmath>

class Bullet {
public:
    int id;         // Unique bullet ID
    int ownerID;    // ID of the player who fired the bullet
    float x, y; 
    float width, height;    // Current position
    double angle;   // Direction (in radians) the bullet is moving
    float speed;    // Speed (pixels per second)

    Bullet() {}

    Bullet(int bulletId, int owner, float startX, float startY, double bulletAngle)
        : id(bulletId), ownerID(owner), x(startX), y(startY), angle(bulletAngle), speed(3.0f)
    {
        width = 10;
        height = 10;
    }

    // Update the bullet's position; deltaTime is in seconds.
    void update(float deltaTime) {
        float radAngle = (angle - 90) * M_PI / 180.0;
        x += speed * std::cos(radAngle) * deltaTime;
        y += speed * std::sin(radAngle) * deltaTime;

    }

    void GetAngle() {
        
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        float dx = static_cast<float>(mouseX) - x;
        float dy = static_cast<float>(mouseY) - y;
        tankAngle = std::atan2(dy, dx);
    }

    void Draw() {
        TextureManager::GetInstance()->Draw("player2", x, y, width, height);
    }
};
