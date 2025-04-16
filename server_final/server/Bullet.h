#pragma once
#include <cmath>

class Bullet {
public:
    int id; 
    int ownerID;    
    float x, y;     
    double angle;   
    float speed;    


    Bullet() {}
    Bullet(int bulletId, int owner, float startX, float startY, double bulletAngle)
        : id(bulletId), ownerID(owner), x(startX), y(startY), angle(bulletAngle), speed(300.0f)
    {}

    // Update the bullet's position; deltaTime is in seconds.
    void update(float deltaTime) {
        x += speed * std::cos(angle) * deltaTime;
        y += speed * std::sin(angle) * deltaTime;
    }
};

