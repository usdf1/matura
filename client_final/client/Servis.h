#pragma once

#include "SDL.h"
#include "Player.h"
#include "TextureManager.h"
#include "Globals.h"

class Servis {
public:
	Servis() {}
	Servis(int _x, int _y, int _w, int _h) {
		x = _x;
		y = _y;
		w = _w;
		h = _h;
	}

	int x, y, h, w;
	Uint32 stojis = 0;
	Uint32 del = 2000;
    Uint32 lastRepairTime = 0;

	void Draw() {
		TextureManager::GetInstance()->Draw("servis",x,y,w,h);
	}

    void CheckCollision() {
        std::cout << damage << std::endl;
        // AABB collision detection: check if the player is in the servis area.
        if (players[currentPlayerId].Position.X + players[currentPlayerId].width >= x &&
            x + w >= players[currentPlayerId].Position.X &&
            players[currentPlayerId].Position.Y + players[currentPlayerId].height >= y &&
            y + h >= players[currentPlayerId].Position.Y) {

            // Only begin repair if the car is completely stopped.
            if (speed == 0) {
                // Start the "stopped" timer if it hasn't been started already.
                if (stojis == 0) {
                    stojis = SDL_GetTicks();
                }
                Uint32 currentTime = SDL_GetTicks();

                // Check if the car has been stopped for at least 2 seconds.
                if (currentTime - stojis >= 2000) {
                    // Once waiting is done, initialize lastRepairTime if needed.
                    if (lastRepairTime == 0) {
                        lastRepairTime = currentTime;
                    }
                    // Compute the elapsed time since the last repair update.
                    Uint32 elapsed = currentTime - lastRepairTime;

                    // Define a repair rate (damage reduced per millisecond).
                    float repairRate = 0.001f;  // Adjust this value for your desired repair speed.

                    // Reduce damage based on elapsed time.
                    damage -= repairRate * elapsed;
                    if (damage < 0.0f)
                        damage = 0.0f;

                    // Update the repair timer.
                    lastRepairTime = currentTime;
                }
                else {
                    // If still within the 2-second waiting period, ensure lastRepairTime is reset.
                    lastRepairTime = 0;
                }
            }
            else {
                // If the car moves, reset both timers.
                stojis = 0;
                lastRepairTime = 0;
            }
        }
        else {
            // If the car leaves the servis area, reset the timers.
            stojis = 0;
            lastRepairTime = 0;
        }
    }



};