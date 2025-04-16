#pragma once

#include <vector>
#include <algorithm>
#include <ctime>
#include "Karta.h"
#include <random> 

class Polje {
public:
    std::vector<Karta> cards;
    bool waitingForMatch = false;
    Uint32 matchTime = 0;

    Polje() {}
    Polje(std::vector<Karta> cardList) {
        cards = cardList;
    }

    void update() {
        // Flip cards based on network data (ensures clients see the flipped card)
        if (turnedColumn1 != -1 && !cards[turnedColumn1].isFlipped) {
            cards[turnedColumn1].flip();
        }
        if (turnedColumn2 != -1 && !cards[turnedColumn2].isFlipped) {
            cards[turnedColumn2].flip();
            waitingForMatch = true;
            matchTime = SDL_GetTicks();
        }

        // Handle match checking after delay
        if (waitingForMatch && SDL_GetTicks() - matchTime >= 1000) {
            checkMatch();
            waitingForMatch = false;
            turnedColumn1 = -1;
            turnedColumn2 = -1;
        }
    }

    void handleClick(int mouseX, int mouseY) {
        if (waitingForMatch) return;

        const int columns = 5;
        for (size_t i = 0; i < cards.size(); i++) {
            if (cards[i].checkClicked(mouseX, mouseY)) {
                int col = static_cast<int>(i % columns);
                int row = static_cast<int>(i / columns);

                if (turnedColumn1 == -1) {  // First card clicked
                    turnedColumn1 = i;
                    turnedRow1 = row;
                    // Send data to server (to sync across all clients)
                }
                else if (turnedColumn2 == -1 && turnedColumn1 != i) {  // Second card clicked
                    turnedColumn2 = i;
                    turnedRow2 = row;
                    // Send data to server for second flip
                }
                break;
            }
        }
    }

    void checkMatch() {
        if (turnedColumn1 != -1 && turnedColumn2 != -1) {
            if (cards[turnedColumn1].frontImg == cards[turnedColumn2].frontImg) {
                cards[turnedColumn1].isMatched = true;
                cards[turnedColumn2].isMatched = true;
                if (Game::GetInstance()->YourTurn == 1)
                    Game::GetInstance()->SpominTocka++;
            }
            else {
                cards[turnedColumn1].flip();
                cards[turnedColumn2].flip();
                if (Game::GetInstance()->YourTurn == 1) {
                    Game::GetInstance()->TurnEnded = 1;
                }
                    
            }
        }
    }

    void render() {
        for (auto& card : cards) {
            card.Render();
        }
    }

    void FillVector() {
        cards.clear();
        const int cardWidth = 100;
        const int cardHeight = 100;
        const int spacing = 10;
        const int columns = 5;
        const int rows = 4;

        std::vector<std::string> frontImageIDs = {
            "frontID_1", "frontID_2", "frontID_3", "frontID_4", "frontID_5",
            "frontID_6", "frontID_7", "frontID_8", "frontID_9", "frontID_10"
        };

        for (int i = 0; i < 10; i++) {
            std::string front = "spomin" + std::to_string(i);
            std::string back = "spomin_back";
            Karta card1(front, back, 0, 0, cardWidth, cardHeight);
            Karta card2(front, back, 0, 0, cardWidth, cardHeight);
            cards.push_back(card1);
            cards.push_back(card2);
        }

        std::default_random_engine rng(seed);
        std::shuffle(cards.begin(), cards.end(), rng);

        const int startX = 250;
        const int startY = 250;

        for (int row = 0; row < rows; row++) {
            for (int col = 0; col < columns; col++) {
                int index = row * columns + col;
                if (index >= cards.size())
                    break;
                cards[index].x = startX + col * (cardWidth + spacing);
                cards[index].y = startY + row * (cardHeight + spacing);
            }
        }
    }

};
