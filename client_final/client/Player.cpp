#include "Player.h"
#include "Keyboard.h"
#include "Globals.h"
#include <cmath>

std::unordered_map<int, Player> players;  


int Player::GetAngle(int w_angle) {
    int diff = w_angle - angle;

    if (diff > 180) {
        diff -= 360;
    }
    else if (diff < -180) {
        diff += 360;
    }

    if (diff > 0) {
        angle += 4;
    }
    else if (diff < 0) {
        angle -= 4;
    }

    if (std::abs(diff) <= 3) {
        angle = w_angle;
    }

    if (angle < 0) {
        angle += 360;
    }

    else if (angle >= 360) {
        angle -= 360;
    }

    return angle;
}

void Player::startMove(Collider col)
{
    //std::cout << damage << std::endl;
    // Calculate movement offsets based on the current angle.
    float radians = (angle - 90) * M_PI / 180.0;
    float dx = std::cos(radians) * speed;
    float dy = std::sin(radians) * speed;

    // Start with the current position.
    Vector2D newPos = players[currentPlayerId].Position;

    // --- Horizontal Movement ---
    // Check for collision along the X axis only.
    if (col.GetCollision(newPos, width, height, angle, dx, 0).empty()) {
        newPos.X += dx;
    }
    else {
        // If a collision occurs, push the player slightly in the opposite direction.
        newPos.X += (dx > 0 ? -2.0f : 2.0f);
        max = 2;
        damage += 0.05;
    }

    // --- Vertical Movement ---
    // Check for collision along the Y axis only, using the updated horizontal position.
    if (col.GetCollision(newPos, width, height, angle, 0, dy).empty()) {
        newPos.Y += dy;
    }
    else {
        newPos.Y += (dy > 0 ? -2.0f : 2.0f);
        max = 2;
        damage += 0.05;
    }

    // --- Screen Boundary Clamping ---
    // Ensure the new position keeps the entire player within the screen bounds.
    if (newPos.X < 0)
        newPos.X = 0;
    else if (newPos.X + width > SCREEN_WIDTH)
        newPos.X = SCREEN_WIDTH - width;

    if (newPos.Y < 0)
        newPos.Y = 0;
    else if (newPos.Y + height > SCREEN_HEIGHT)
        newPos.Y = SCREEN_HEIGHT - height;

    // Update the player's position.
    players[currentPlayerId].Position = newPos;



    for (auto& playerPair : players) {
        if (playerPair.first != currentPlayerId) {
            if (CheckCollisosWithPlayer(playerPair.second)) {
                
                float knockbackAmount = 6.0f;
                resolveCollision(playerPair.second, knockbackAmount);
            
            }
        }
            
    }

    
}


void Player::Pospesi(Collider col) {
    max = col.GetSpeed(Position, width, height);
    if (speed < max) {
        speed += 0.7; // Increase speed gradually
        if (speed > max) speed = max; // Clamp to maxSpeed
    }
    else if (speed > max) {
        speed -= 0.4;
        if (speed < max) speed = max;
    }  
}

void Player::Upocasni() {
    if (speed > 0) {
        speed -= 0.3; // Decrease speed gradually
        if (speed < 0) speed = 0; // Clamp to 0
    }
}

void Player::Draw(int ID)
{
    if (state == Game::GetInstance()->m_State) {

        if (ID != 0 && CarS == 0) {
            TextureManager::GetInstance()->DrawAngleFrames("avto", Position.X, Position.Y, 32, 64, 0, 0, Game::GetInstance()->car_scale, angle);
        }
		
        if (ID != 0 && CarS == 1) {
            TextureManager::GetInstance()->DrawAngleFrames("avto", Position.X, Position.Y, 32, 64, 1, 0, Game::GetInstance()->car_scale, angle);
        }

        if (ID != 0 && CarS == 2) {
            TextureManager::GetInstance()->DrawAngleFrames("avto", Position.X, Position.Y, 32, 64, 2, 0, Game::GetInstance()->car_scale, angle);
        }

        if (ID != 0 && CarS == 3) {
            TextureManager::GetInstance()->DrawAngleFrames("avto", Position.X, Position.Y, 32, 64, 0, 1, Game::GetInstance()->car_scale, angle);
        }

        if (ID != 0 && CarS == 4) {
            TextureManager::GetInstance()->DrawAngleFrames("avto", Position.X, Position.Y, 32, 64, 1, 1, Game::GetInstance()->car_scale, angle);
        }

        if (ID != 0 && CarS == 5) {
            TextureManager::GetInstance()->DrawAngleFrames("avto", Position.X, Position.Y, 32, 64, 2, 1, Game::GetInstance()->car_scale, angle);
        }

        if (ID != 0 && CarS == 6) {
            TextureManager::GetInstance()->DrawAngleFrames("avto", Position.X, Position.Y, 32, 64, 0, 2, Game::GetInstance()->car_scale, angle);
        }

        if (ID != 0 && CarS == 7) {
            TextureManager::GetInstance()->DrawAngleFrames("avto", Position.X, Position.Y, 32, 64, 1, 2, Game::GetInstance()->car_scale, angle);
        }

    }
    

}

void Player::DrawTanks(int ID)
{

    if (state == Game::GetInstance()->m_State) {
        if (TankS == 0 && ID != 0) {
            TextureManager::GetInstance()->DrawAngleFrames("tank", Position.X, Position.Y, 33, 33, 0, 0, Game::GetInstance()->tank_scale, angle);
            TextureManager::GetInstance()->DrawAngleFrames("barrel", Position.X - 1, Position.Y - 4, 35, 37, 0, 0, Game::GetInstance()->tank_scale, barrelAngle);
        }

        else if (TankS == 1 && ID != 0) {
            TextureManager::GetInstance()->DrawAngleFrames("tank", Position.X, Position.Y, 33, 33, 1, 0, Game::GetInstance()->tank_scale, angle);
            TextureManager::GetInstance()->DrawAngleFrames("barrel", Position.X - 1, Position.Y - 4, 35, 37, 1, 0, Game::GetInstance()->tank_scale, barrelAngle);
        }

        else if (TankS == 2 && ID != 0) {
            TextureManager::GetInstance()->DrawAngleFrames("tank", Position.X, Position.Y, 33, 33, 2, 0, Game::GetInstance()->tank_scale, angle);
            TextureManager::GetInstance()->DrawAngleFrames("barrel", Position.X - 1, Position.Y - 4, 35, 37, 2, 0, Game::GetInstance()->tank_scale, barrelAngle);
        }

        else if (TankS == 3 && ID != 0) {
            TextureManager::GetInstance()->DrawAngleFrames("tank", Position.X, Position.Y, 33, 33, 0, 1, Game::GetInstance()->tank_scale, angle);
            TextureManager::GetInstance()->DrawAngleFrames("barrel", Position.X - 1, Position.Y - 4, 35, 37, 3, 0, Game::GetInstance()->tank_scale, barrelAngle);
        }

        else if (TankS == 4 && ID != 0) {
            TextureManager::GetInstance()->DrawAngleFrames("tank", Position.X, Position.Y, 33, 33, 1, 1, Game::GetInstance()->tank_scale, angle);
            TextureManager::GetInstance()->DrawAngleFrames("barrel", Position.X - 1, Position.Y - 4, 35, 37, 4, 0, Game::GetInstance()->tank_scale, barrelAngle);
        }

        else if (TankS == 5 && ID != 0) {
            TextureManager::GetInstance()->DrawAngleFrames("tank", Position.X, Position.Y, 33, 33, 2, 1, Game::GetInstance()->tank_scale, angle);
            TextureManager::GetInstance()->DrawAngleFrames("barrel", Position.X - 1, Position.Y - 4, 35, 37, 5, 0, Game::GetInstance()->tank_scale, barrelAngle);
        }

        else if (TankS == 6 && ID != 0) {
            TextureManager::GetInstance()->DrawAngleFrames("tank", Position.X, Position.Y, 33, 33, 0, 2, Game::GetInstance()->tank_scale, angle);
            TextureManager::GetInstance()->DrawAngleFrames("barrel", Position.X - 1, Position.Y - 4, 35, 37, 0, 1, Game::GetInstance()->tank_scale, barrelAngle);
        }

        else if (TankS == 7 && ID != 0) {
            TextureManager::GetInstance()->DrawAngleFrames("tank", Position.X, Position.Y, 33, 33, 1, 2, Game::GetInstance()->tank_scale, angle);
            TextureManager::GetInstance()->DrawAngleFrames("barrel", Position.X - 1, Position.Y - 4, 35, 37, 1, 1, Game::GetInstance()->tank_scale, barrelAngle);
        }

        if (shieldOn == 1) {
            TextureManager::GetInstance()->Draw("scit", Position.X - 12, Position.Y - 8, 66, 66);
        }
    }
}

void Player::Update(Collider col)
{
    if(Game::GetInstance()->m_State == 1)
    {
        if (damage < 5) {
            if (Keyboard::GetInstance()->GetKeyDown(SDL_SCANCODE_SPACE)) {
                Pospesi(col);
            }
            else {
                Upocasni();
            }
            startMove(col);
        }
        if (CheckCheckpoints(col))
            laps = AddLaps(col);
    }

    if(Game::GetInstance()->m_State == 2)
    {
        float rad = (angle - 90) * M_PI / 180.0;
        float dx = 0.0f;
        float dy = 0.0f;
        Vector2D newPos = players[currentPlayerId].Position;

        if (Keyboard::GetInstance()->GetKeyDown(SDL_SCANCODE_D)) {
            angle += tankAngleSpeed;
        }
        else if (Keyboard::GetInstance()->GetKeyDown(SDL_SCANCODE_A)) {
            angle -= tankAngleSpeed;
        }


        else if (Keyboard::GetInstance()->GetKeyDown(SDL_SCANCODE_W)) {
            dx = std::cos(rad) * tankSpeed;
            dy = std::sin(rad) * tankSpeed;
        }

        if (col.GetCollisionTanks(newPos, width, height, angle, dx, 0).empty()) {
            newPos.X += dx;
        }

        if (col.GetCollisionTanks(newPos, width, height, angle, 0, dy).empty()) {
            newPos.Y += dy;
        }

        if (Keyboard::GetInstance()->GetKeyDown(SDL_SCANCODE_L)) {
            tankAngle += 1;
        }
        else if (Keyboard::GetInstance()->GetKeyDown(SDL_SCANCODE_K)) {
            tankAngle -= 1;
        }

        if (tankAngle > 360) tankAngle = 0;
        if (tankAngle < 0) tankAngle = 360;

        players[currentPlayerId].Position = newPos;

    }

    Position.X = players[currentPlayerId].XPos();
    Position.Y = players[currentPlayerId].YPos();
    width = players[currentPlayerId].width;
    height = players[currentPlayerId].height;

}

void Player::Clean()
{
}

int Player::AddLaps(Collider col) {
    bool collidedWithFinishLine = false;

    for (int i = 0; i < col.cilj.size(); i++) {
        if (Position.X + width >= col.cilj[i].x &&
            col.cilj[i].x + col.cilj[i].width >= Position.X &&
            Position.Y + height >= col.cilj[i].y &&
            col.cilj[i].y + col.cilj[i].height >= Position.Y) {

            collidedWithFinishLine = true;

            if (!CrossedTheLine) {
                CrossedTheLine = true;
                for (int t = 0; t < 3; t++) {
                    checkpoints[t] = 0;
                }

                return laps + 1;
            }
        }
    }

    if (!collidedWithFinishLine) {
        CrossedTheLine = false; 
    }

    return laps;
}

bool Player::CheckCheckpoints(Collider col)
{
    for (int i = 0; i < col.fullCheckpoint.size(); i++) {
        if (Position.X + width >= col.fullCheckpoint[i].x &&
            col.fullCheckpoint[i].x + col.fullCheckpoint[i].width >= Position.X &&
            Position.Y + height >= col.fullCheckpoint[i].y &&
            col.fullCheckpoint[i].y + col.fullCheckpoint[i].height >= Position.Y) {

            if (checkpoints[0] == 0 && i == 0) {
                checkpoints[0] = 1;
            }

            if (checkpoints[0] == 1 && i == 1) {
                checkpoints[1] = 1; 
            }

            if (checkpoints[1] == 1 && i == 2 && checkpoints[0] == 1) {
                checkpoints[2] = 1;
            }
            
        }
    }

    for (int i = 0; i < 3; i++) {
        if (checkpoints[i] == 0) {
            return 0;
        }
            
    }

    return 1;
}

void Player::StartPosition()
{
    players[currentPlayerId].Position.X = (((currentPlayerId + 1) * 32 + (6 * 32)) - 32) + 6;;
    players[currentPlayerId].Position.Y = 16 * 32 + 8;
}

bool Player::CheckCollisosWithPlayer(Player other) {
    // Calculate the scaled dimensions for both cars.
    float myScaledWidth = width * Game::GetInstance()->car_scale;
    float myScaledHeight = height * Game::GetInstance()->car_scale;
    float otherScaledWidth = other.width * Game::GetInstance()->car_scale;
    float otherScaledHeight = other.height * Game::GetInstance()->car_scale;

    // Use the scaled dimensions in the AABB test.
    if (other.Position.X + otherScaledWidth >= Position.X &&
        Position.X + myScaledWidth >= other.Position.X &&
        other.Position.Y + otherScaledHeight >= Position.Y &&
        Position.Y + myScaledHeight >= other.Position.Y) {
        return true;
    }
    return false;
}


void Player::resolveCollision(Player& other, float knockbackAmount) {
    // Compute your forward vector from your angle.

    std::cout << "Collision resolved for your car (ID " << ID << ") using knockback amount " << knockbackAmount << std::endl;
}

void Player::NumSkins()
{
    if (carSkin == "red")
        CarS = 0;

    if (carSkin == "blue")
        CarS = 1;

    if (carSkin == "yellow")
        CarS = 2;

    if (carSkin == "white")
        CarS = 3;

    if (carSkin == "orange")
        CarS = 4;

    if (carSkin == "green")
        CarS = 5;

    if (carSkin == "black")
        CarS = 6;

    if (carSkin == "slo")
        CarS = 7;

    //------------------------------------------

    if (tankSkin == "red")
        TankS = 2;

    if (tankSkin == "blue")
        TankS = 0;

    if (tankSkin == "lightgreen")
        TankS = 3;

    if (tankSkin == "orangeM")
        TankS = 4;

    if (tankSkin == "whiteM")
        TankS = 5;

    if (tankSkin == "brownM")
        TankS = 6;

    if (tankSkin == "military")
        TankS = 7;

    if (tankSkin == "greenM")
        TankS = 1;

}
