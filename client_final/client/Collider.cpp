#include "Collider.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include "TextureManager.h"
#include <algorithm>
#include "Globals.h"

void Collider::FillColliders()
{
    std::ifstream file("assets/map");
    if (!file) {
        std::cerr << "Error opening file." << std::endl;
        return;
    }

    std::string line;
    bool isLayer1 = false;
    int row = 0;

    while (std::getline(file, line)) {
        if (line == "layer 1") {
            isLayer1 = true;
            continue;
        }

        if (isLayer1) {
            std::istringstream stream(line);
            std::string value;
            int column = 0;

            while (std::getline(stream, value, ',')) {

                if (std::stoi(value) == 14) {
                    Collider col (column * 32, row * 32);
                    border.push_back(col);
                }

                else if (std::stoi(value) == 15 || std::stoi(value) == 16 || std::stoi(value) == 17) {
                    Collider col(column * 32, row * 32);
                    cesta.push_back(col);
                    if (std::stoi(value) == 16) {
                        cilj.push_back(col);
                    }
                    if (std::stoi(value) == 17 || std::stoi(value) == 18) {
                        checkpoint.push_back(col);
                    }
                }



                column++;
            }
            row++;
        }
    }

    file.close();
    GetCheckpoints();

    std::ifstream file_tank("assets/tank_map");
    if (!file_tank) {
        std::cerr << "Error opening file." << std::endl;
        return;
    }

    std::string line2;
    isLayer1 = false;
    row = 0;

    while (std::getline(file_tank, line2)) {
        if (line2 == "layer 1") {
            isLayer1 = true;
            continue;
        }

        if (isLayer1) {
            std::istringstream stream(line2);
            std::string value;
            int column = 0;

            while (std::getline(stream, value, ',')) {

                if (std::stoi(value) == 6) {
                    Collider col(column * 32, row * 32);
                    border_tank.push_back(col);
                }



                column++;
            }
            row++;
        }
    }

    file_tank.close();
}

std::string Collider::GetCollision(Vector2D position, float width, float height, double /*angle*/, float dx, float dy)
{
    std::string collisionSides;

    // Compute the player's projected bounding box after applying dx/dy.
    float newX = position.X + dx;
    float newY = position.Y + dy;
    float playerLeft = newX;
    float playerRight = newX + width * Game::GetInstance()->car_scale;
    float playerTop = newY;
    float playerBottom = newY + height * Game::GetInstance()->car_scale;

    // Iterate over each tile that has collision.
    for (size_t i = 0; i < border.size(); i++) {
        float tileLeft = border[i].x;
        float tileRight = border[i].x + border[i].width;
        float tileTop = border[i].y;
        float tileBottom = border[i].y + border[i].height;

        // First, check if the player's projected rectangle intersects the tile.
        if (playerRight > tileLeft && playerLeft < tileRight &&
            playerBottom > tileTop && playerTop < tileBottom)
        {
            // Compute overlap distances on each side.
            float overlapLeft = playerRight - tileLeft;   // how far player intrudes from the left
            float overlapRight = tileRight - playerLeft;     // intrusion from the right
            float overlapTop = playerBottom - tileTop;     // intrusion from the top
            float overlapBottom = tileBottom - playerTop;     // intrusion from the bottom

            // Determine which overlap is smallest.
            float minOverlap = std::min({ overlapLeft, overlapRight, overlapTop, overlapBottom });

            // Choose the collision side corresponding to the minimum overlap.
            // (If you detect more than one collision, you may have to decide whether to combine
            // or prioritize one side. Here we append each unique side.)
            if (minOverlap == overlapLeft) {
                if (collisionSides.find('L') == std::string::npos)
                    collisionSides += "L";
            }
            else if (minOverlap == overlapRight) {
                if (collisionSides.find('R') == std::string::npos)
                    collisionSides += "R";
            }
            else if (minOverlap == overlapTop) {
                if (collisionSides.find('T') == std::string::npos)
                    collisionSides += "T";
            }
            else if (minOverlap == overlapBottom) {
                if (collisionSides.find('B') == std::string::npos)
                    collisionSides += "B";
            }

        }
    }
    return collisionSides;
}

std::string Collider::GetCollisionTanks(Vector2D position, float width, float height, double /*angle*/, float dx, float dy)
{
    std::string collisionSides;

    // Compute the player's projected bounding box after applying dx/dy.
    float newX = position.X + dx;
    float newY = position.Y + dy;
    float playerLeft = newX;
    float playerRight = newX + width * Game::GetInstance()->tank_scale;
    float playerTop = newY;
    float playerBottom = newY + height * Game::GetInstance()->tank_scale;

    // Iterate over each tile that has collision.
    for (size_t i = 0; i < border_tank.size(); i++) {
        float tileLeft = border_tank[i].x;
        float tileRight = border_tank[i].x + border_tank[i].width;
        float tileTop = border_tank[i].y;
        float tileBottom = border_tank[i].y + border_tank[i].height;

        // First, check if the player's projected rectangle intersects the tile.
        if (playerRight > tileLeft && playerLeft < tileRight &&
            playerBottom > tileTop && playerTop < tileBottom)
        {
            // Compute overlap distances on each side.
            float overlapLeft = playerRight - tileLeft;   // how far player intrudes from the left
            float overlapRight = tileRight - playerLeft;     // intrusion from the right
            float overlapTop = playerBottom - tileTop;     // intrusion from the top
            float overlapBottom = tileBottom - playerTop;     // intrusion from the bottom

            // Determine which overlap is smallest.
            float minOverlap = std::min({ overlapLeft, overlapRight, overlapTop, overlapBottom });

            // Choose the collision side corresponding to the minimum overlap.
            // (If you detect more than one collision, you may have to decide whether to combine
            // or prioritize one side. Here we append each unique side.)
            if (minOverlap == overlapLeft) {
                if (collisionSides.find('L') == std::string::npos)
                    collisionSides += "L";
            }
            else if (minOverlap == overlapRight) {
                if (collisionSides.find('R') == std::string::npos)
                    collisionSides += "R";
            }
            else if (minOverlap == overlapTop) {
                if (collisionSides.find('T') == std::string::npos)
                    collisionSides += "T";
            }
            else if (minOverlap == overlapBottom) {
                if (collisionSides.find('B') == std::string::npos)
                    collisionSides += "B";
            }
        }
    }
    return collisionSides;
}

float Collider::GetSpeed(Vector2D Position, int width, int height)
{
    for (int i = 0; i < cesta.size(); i++) {       

        if (Position.X + width >= cesta[i].x &&
            cesta[i].x + cesta[i].width >= Position.X &&
            Position.Y + height >= cesta[i].y &&
            cesta[i].y + cesta[i].height >= Position.Y) {

            return 13.0f - damage;
        }

    }

    if (damage < 1.5) {
        return 4.0f - damage;
    }

    else
        return 2.5f;
    
}

int Collider::AddLaps(Vector2D Position, int width, int height, int laps, bool hasCrossedFinishLine) {
    for (int i = 0; i < cilj.size(); i++) {
        if (Position.X + width >= cilj[i].x &&
            cilj[i].x + cilj[i].width >= Position.X &&
            Position.Y + height >= cilj[i].y &&
            cilj[i].y + cilj[i].height >= Position.Y) {

            if (hasCrossedFinishLine == 0) {
                hasCrossedFinishLine = true;
                return laps + 1; 
            }
        }
        else {
            hasCrossedFinishLine = false; 
        }
    }
    return laps;
}

void Collider::GetCheckpoints()
{
    Collider prvi(1024, 512, 32, 3 * 32);
    Collider drugi(1312, 864, 32, 32 * 7);
    Collider tretji(512, 928, 32, 32 * 5);

    fullCheckpoint.push_back(prvi);
    fullCheckpoint.push_back(drugi);
    fullCheckpoint.push_back(tretji);
}

bool Collider::CheckForBox(float x, float y, int boxWidth, int boxHeight)
{
    const int tileSize = 32; // Each tile is 32x32 pixels
    for (const auto& tile : border_tank) {
        // Standard AABB collision check:
        // Check if the box (x, y, boxWidth, boxHeight) intersects with the tile (tile.x, tile.y, tileSize, tileSize)
        if (x < tile.x + tileSize &&
            x + boxWidth > tile.x &&
            y < tile.y + tileSize &&
            y + boxHeight > tile.y) {
            return true; // There is an overlap.
        }
    }
    return false; // No collision with any tile.
}

void Collider::Draw()
{
    for (int i = 0; i < fullCheckpoint.size(); i++) {
        TextureManager::GetInstance()->Draw("player", fullCheckpoint[i].x, fullCheckpoint[i].y, fullCheckpoint[i].width, fullCheckpoint[i].height);
  
    }
}

