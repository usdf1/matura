#include <enet/enet.h>
#include <iostream>
#include <unordered_map>
#include <vector>
#include "SDL.h"
#include "Player.h"
#include <sstream>
#include "Globals.h"
#include "Bullet.h"
#include "Collider.h"
#include "Box.h"
#include <cstdlib>
#include <ctime>
#include <random>

class NetworkClient {
public:
    NetworkClient(const std::string& serverIP, uint16_t port) {
        if (enet_initialize() != 0) {
            std::cerr << "Enet se ni inicializiral." << std::endl;
            exit(EXIT_FAILURE);
        }

        client = enet_host_create(nullptr, 1, 2, 0, 0);
        if (client == nullptr) {
            std::cerr << "Enet client se ni naredil." << std::endl;
            exit(EXIT_FAILURE);
        }

        enet_address_set_host(&address, serverIP.c_str());
        address.port = port;

        peer = enet_host_connect(client, &address, 2, 0);
        if (peer == nullptr) {
            std::cerr << "ni peerov." << std::endl;
            exit(EXIT_FAILURE);
        }

        if (enet_host_service(client, &event, 5000) > 0 && event.type == ENET_EVENT_TYPE_CONNECT) {
            std::cout << "Povezava uspešna.\n";
        }
        else {
            std::cerr << "Povezava neuspešna." << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    ~NetworkClient() {
        enet_host_destroy(client);
        enet_deinitialize();
    }

    void sendPlayerPosition(int id, float x, float y, int angle, int Bangle, bool shieldOn, int state, int carS, int tankS) {
        std::stringstream ss;
        ss << id << ":" << x << ":" << y << ":" << angle << ":" << Bangle << ":" << shieldOn << ":" << state << ":" << carS << ":" << tankS;
        std::cout << "Posle se  " << carS  << "za id " << id << std::endl;
        sendPacket(ss.str());
    }

    void sendPlayerInfo(std::string ime, std::string geslo) {
        std::stringstream ss;
        ss << "LOGIN" << ":" << ime << ":" << geslo;
        sendPacket(ss.str());
    }

    void sendPlayerLogIn(std::string ime, std::string geslo) {
        std::stringstream ss;
        ss << "PRIJAVA" << ":" << ime << ":" << geslo;
        sendPacket(ss.str());
    }

    void sendStop() {
        std::stringstream ss;
        ss << "STOP";
        sendPacket(ss.str());
    }

    void sendWait() {
        std::stringstream ss;
        ss << "WAITING";
        sendPacket(ss.str());
    }

    void sendTankWait() {
        std::stringstream ss;
        ss << "TANKS_WAITING";
        sendPacket(ss.str());
    }

    void sendWaitSpomin() {
        std::stringstream ss;
        ss << "SPOMIN_WAITING";
        //Game::GetInstance()->YourTurn = 1;
        sendGetSeed();

        sendPacket(ss.str());
    }

    void sendDelete(int id) {
        std::stringstream ss;
        ss << "DELETE:" << id;
        sendPacket(ss.str());
    }

    void sendDeleteBox(int id) {
        std::stringstream ss;
        ss << "BOX_DELETE:" << id;
        sendPacket(ss.str());
    }

    void sendGetSeed() {
        std::stringstream ss;
        ss << "SEED_REQUEST:";
        sendPacket(ss.str());
    }

    void sendEndTurn() {
        std::stringstream ss;
        ss << "END_TURN:";
        sendPacket(ss.str());
    }

    void sendTurnMsg() {
        std::stringstream ss;
        ss << "TURN" << ":" << turnedColumn1 << ":" << turnedRow1 << ":" << turnedColumn2 << ":" << turnedRow2;
        sendPacket(ss.str());
    }

    void sendTocke() {
        std::stringstream ss;
        ss << "TOCKE" << ":" << Game::GetInstance()->SpominTocka << ":" << currentPlayerId;
        sendPacket(ss.str());
    }

    void sendSkin() {
        std::stringstream ss;
        ss << "SKIN" << ":" << carSkin << ":" << tankSkin << ":" << Game::GetInstance()->PlayerNameReal;
        std::cout << "poslal je skin " << carSkin << "     " << tankSkin << std::endl;
        sendPacket(ss.str());
    }

    void handleNetworkEvents() {
        while (enet_host_service(client, &event, 0) > 0) {
            if (event.type == ENET_EVENT_TYPE_RECEIVE) {
                std::string receivedData((char*)event.packet->data);

                if (receivedData.substr(0, 3) == "ID:") {
                    std::stringstream ss(receivedData.substr(3));

                    int playerID;
                    int xPos, yPos;
                    char delimiter;

                    ss >> playerID >> delimiter >> xPos >> delimiter >> yPos;

                    if (ss && delimiter == ':') { // Ensure the packet was parsed successfully
                        std::cout << "Assigned Player ID: " << playerID
                            << " with starting position: (" << xPos << ", " << yPos << ")" << std::endl;

                        currentPlayerId = playerID;

                        // Initialize the player's position
                        players[currentPlayerId] = Player(currentPlayerId, xPos, yPos);

                        std::cout << "Global ID: " << currentPlayerId << " initialized at (" << xPos << ", " << yPos << ")" << std::endl;
                    }
                }

                else if (receivedData.rfind("DISCONNECT:", 0) == 0) {
                    int id = std::stoi(receivedData.substr(11));
                    if (players.find(id) != players.end()) {
                        players.erase(id);
                        std::cout << "Player " << id << " disconnected and removed.\n";
                    }
                }

                else if (receivedData.rfind("DELETE:", 0) == 0) {
                    std::stringstream ss(receivedData);
                    std::string prefix;
                    std::getline(ss, prefix, ':');
                    int id;
                    ss >> id;

                    // Check if the bullet exists in the map before erasing
                    auto it = bullets.find(id);
                    if (it != bullets.end()) {
                        bullets.erase(it);
                        std::cout << "Deleted bullet " << id << std::endl;
                    }
                    // Do not call sendDelete here, as the server is already managing deletion.
                    return;
                }

                else if (receivedData.rfind("BOX_DELETE:", 0) == 0) {
                    std::stringstream ss(receivedData);
                    std::string prefix;
                    std::getline(ss, prefix, ':');
                    int id;
                    ss >> id;

                    auto it = std::find_if(boxes.begin(), boxes.end(), [id](const Box& box) {
                        return box.ID == id;
                        });

                    if (it != boxes.end()) {
                        boxes.erase(it); // Erase the box from the vector
                    }

                    return;
                }

                else if (receivedData.rfind("CAKA:", 0) == 0) {
                    PLayersWaiting = std::stoi(receivedData.substr(5));
                }

                else if (receivedData.rfind("CAKA_TANKE:", 0) == 0) {
                    PLayersWaitingForTanks = std::stoi(receivedData.substr(11));
                }

                else if (receivedData.rfind("SPOMIN_CAKA:", 0) == 0) {
                    PLayersWaitingForSpomin = std::stoi(receivedData.substr(12));
                    if(PLayersWaitingForSpomin == 1 && Game::GetInstance()->clickedSpomin == 1)
                        Game::GetInstance()->YourTurn = 1;
                }

                else if (receivedData.rfind("SEED:", 0) == 0) {
                    seed = std::stoi(receivedData.substr(5));
                }

                else if (receivedData.rfind("END_TURN", 0) == 0) {
                    if (Game::GetInstance()->YourTurn == 1) {
                        Game::GetInstance()->YourTurn = 0;
                        break;
                    } 
                    else if (Game::GetInstance()->YourTurn == 0) {
                        Game::GetInstance()->YourTurn = 1;
                        break;
                    }
                }

                else
                {
                    handleReceivedPacket(receivedData);
                }
                enet_packet_destroy(event.packet);
            }
        }
    }

    void updatePlayerPosition(int id, float x, float y, double angle, double Bangle, bool shieldOn, int state, int _carSkin, int _tankSkin) {

            players[id].SetPosition(x, y, angle, Bangle, shieldOn, state, _carSkin, _tankSkin); // updata tega zdej
            std::cout << _carSkin << " skin bi mogu bit tak za id " << id << std::endl;
    }

    void renderPlayers() {
        for (auto& playerPair : players) {
           if (playerPair.second.ID != 0)
                playerPair.second.Draw(playerPair.first);
        }
    }

    void updateBullets(float deltaTime) {
        for (auto& pair : bullets) {
            pair.second.update(deltaTime);
            // Optionally, remove bullets that are off-screen or expired.
        }
    }

    void renderTanks() {
        for (auto& playerPair : players) {
            if (playerPair.second.ID != 0)
                playerPair.second.DrawTanks(playerPair.first);
        }
    }

    void renderBullets() {
        for (auto& pair : bullets) {
            const Bullet& bullet = pair.second;
            pair.second.Draw();
        }
    }

    void renderBoxes() {
        for (int i = 0; i < boxes.size(); i++) {
            boxes[i].Draw();
        }
    }

    void Disconnect() {
        if (peer != nullptr) {
            std::stringstream ss;
            ss << "DISCONNECT:" << currentPlayerId;
            sendPacket(ss.str());

            enet_peer_disconnect_now(peer, 0);
            enet_host_flush(client);
        }
    }

    bool CheckPlayerCollision(int id) {
        for (auto& playerPair : players) {
            if (bullets[id].ownerID != playerPair.first) {
                float scaledWidth = playerPair.second.tank_width * Game::GetInstance()->tank_scale;
                float scaledHeight = playerPair.second.tank_height * Game::GetInstance()->tank_scale;

                if (playerPair.second.Position.X + scaledWidth >= bullets[id].x &&
                    bullets[id].x + bullets[id].width >= playerPair.second.Position.X &&
                    playerPair.second.Position.Y + scaledHeight >= bullets[id].y &&
                    bullets[id].y + bullets[id].height >= playerPair.second.Position.Y) {

                    std::cout << playerPair.second.ID << std::endl;

                    if (playerPair.first == currentPlayerId && Game::GetInstance()->shield == 0) {
                        std::cout << "mrtu si :(" << std::endl;
                        Game::GetInstance()->Is_Hit = 1;

                    }

                    return true;
                }
            }
        }
        return false;
    }

    int CheckBoxes(){
        const int boxWidth = 32;
        const int boxHeight = 32;

        for (auto& playerPair : players) {
            // Scale the player's dimensions.
            float scaledWidth = playerPair.second.tank_width * Game::GetInstance()->tank_scale;
            float scaledHeight = playerPair.second.tank_height * Game::GetInstance()->tank_scale;
            float playerX = playerPair.second.Position.X;
            float playerY = playerPair.second.Position.Y;

            // Check against each box.
            for (const auto& box : boxes) {
                // Standard AABB collision check:
                if (playerX + scaledWidth >= box.x &&
                    box.x + boxWidth >= playerX &&
                    playerY + scaledHeight >= box.y &&
                    box.y + boxHeight >= playerY) {

                    std::cout << "Player " << playerPair.second.ID
                        << " collided with Box " << box.ID << std::endl;

                    sendDeleteBox(box.ID);

                    // You can output additional information or handle the collision event here.
                    return box.type;
                }
            }
        }

        return 0;
    }

    bool CheckTankCollision(int id, Collider gozd) {
        float scaledWidth = gozd.width; /* *Game::GetInstance()->tank_scale; */
        float scaledHeight = gozd.height; /* *Game::GetInstance()->tank_scale; */

                if (gozd.x + scaledWidth >= bullets[id].x &&
                    bullets[id].x + bullets[id].width >= gozd.x &&
                    gozd.y + scaledHeight >= bullets[id].y &&
                    bullets[id].y + bullets[id].height >= gozd.y) {

                    return true;
                }
        return false;
    }

    void checkCollision(Collider col) {
        for (auto it = bullets.begin(); it != bullets.end(); ) {
            if (CheckPlayerCollision(it->second.id)) {
                int bulletID = it->second.id;  // Save the bullet ID.
                it = bullets.erase(it);
                sendDelete(bulletID);
            }
            else {
                ++it; 
            }
        }

        for (auto it = bullets.begin(); it != bullets.end(); ) {
            bool collided = false;
            // Iterate over each collidable tile for this bullet.
            for (int i = 0; i < col.border_tank.size(); i++) {
                if (CheckTankCollision(it->second.id, col.border_tank[i])) {
                    int bulletID = it->second.id;  // Save the bullet ID.
                    it = bullets.erase(it);         // Erase bullet and get valid iterator.
                    sendDelete(bulletID);
                    collided = true;
                    break; // Stop checking other tiles for this bullet.
                }
            }
            // Only advance the iterator if no collision occurred.
            if (!collided) {
                ++it;
            }
        }
    }


    void fireBullet() {
        // Assumes you have the current player's ID, position, and angle.
        // Here we use players[currentPlayerId].Position.X/Y and players[currentPlayerId].angle
        std::stringstream ss;
        ss << "FIRE:" << currentPlayerId << ":"
            << players[currentPlayerId].Position.X + (players[currentPlayerId].tank_width / 2) - 2 << ":"
            << players[currentPlayerId].Position.Y + (players[currentPlayerId].tank_height / 2) - 2 << ":"
            << tankAngle;
        sendPacket(ss.str());
    }

    void spawnBox(Collider collider) {
        int type = (std::rand() % 2) + 1;
        std::uniform_int_distribution<> xDist(850, 950);
        std::uniform_int_distribution<> yDist(450, 550);
        int x = xDist(gen);
        int y = yDist(gen);
        if (!collider.CheckForBox(x, y, 32,32)) {
            std::stringstream ss;
            ss << "BOX:" << ":"
                << x << ":"
                << y << ":"
                << type;
            sendPacket(ss.str());
            
        }
        else 
            LastBox = SDL_GetTicks();
        
        
    }

    std::unordered_map<int, Bullet> bullets;
    std::vector<Box> boxes;

private:

    

    void handleReceivedPacket(std::string receivedData) {

        if (receivedData == "STOP") {
            //Game::GetInstance()->m_State = -1;
            AvtiOver = 1;
            movementStartTime = 0;
            PLayersWaiting = 0;
            players[currentPlayerId].angle = 0;
            std::cout << "Sprejet STOP, STATE GRE NA STARTING SCREEN" << std::endl;
        }


        else if (receivedData == "REGISTER_SUCCESS") {
            std::cout << "POVEZANO-------------------" << std::endl;
            Game::GetInstance()->Exists = 0;
            Game::GetInstance()->m_State = 0;
            Game::GetInstance()->PlayerName = "";
            Game::GetInstance()->PlayerPassword = "";
            Game::GetInstance()->SecretPassword = "";
        }

        else if (receivedData.rfind("LOGIN_SUCCESS:", 0) == 0) {
            std::stringstream ss(receivedData);
            std::string prefix, username, carSkinValue, tankSkinValue;

            std::getline(ss, prefix, ':');  // "LOGIN_SUCCESS"
            std::getline(ss, username, ':');
            std::getline(ss, carSkinValue, ':');
            std::getline(ss, tankSkinValue, ':');

            std::cout << "PRIJAVA USPESNA" << std::endl;

            Game::GetInstance()->PlayerPasswordReal = Game::GetInstance()->PlayerPassword;
            Game::GetInstance()->PlayerNameReal = Game::GetInstance()->PlayerName;
            Game::GetInstance()->m_State = 0;
            Game::GetInstance()->Exists = 0;
            Game::GetInstance()->PlayerName = "";
            Game::GetInstance()->PlayerPassword = "";
            Game::GetInstance()->SecretPassword = "";
            Game::GetInstance()->zeObstaja = 0;

            // Set global skin variables
            carSkin = carSkinValue;
            tankSkin = tankSkinValue;

            std::cout << "Car Skin: " << carSkin << std::endl;
            std::cout << "Tank Skin: " << tankSkin << std::endl;
        }

        else if (receivedData == "INVALID_CREDENTIALS") {
            std::cout << "NEVELJAVNO IME ALI GESLO" << std::endl;
            Game::GetInstance()->Exists = 1;
            Game::GetInstance()->PlayerName = "";
            Game::GetInstance()->PlayerPassword  = "";
            Game::GetInstance()->SecretPassword = "";
        }

        else if (receivedData == "USERNAME_EXISTS") {
            std::cout << "ze obstaja -------------------" << std::endl;
            Game::GetInstance()->zeObstaja = 1;
            Game::GetInstance()->SecretPassword = "";
            Game::GetInstance()->Exists = 1;

        }

        else if (receivedData == "SKIN_UPDATE_FAILED") {
            std::cout << "ni ipdatal skina" << std::endl;
        }

        else if (receivedData == "SKIN_UPDATE_SUCCESS") {
            std::cout << "updatal je skin yey" << std::endl;
        }

        else if (receivedData.rfind("BULLET_SPAWN:", 0) == 0) {
            // Expected format: "BULLET_SPAWN:<bulletID>:<ownerID>:<x>:<y>:<angle>"
            std::stringstream ss(receivedData);
            std::string token;
            std::getline(ss, token, ':');
            int bulletID, ownerID;
            float x, y;
            double angle;
            char delimiter;
            ss >> bulletID >> delimiter >> ownerID >> delimiter >> x >> delimiter >> y >> delimiter >> angle;
            if (ss) {
                Bullet bullet(bulletID, ownerID, x, y, angle);
                bullets[bulletID] = bullet;
                std::cout << "Bullet spawned: ID " << bulletID << ", fired by player " << ownerID << std::endl;
            }
            return;
        }

        else if (receivedData.rfind("BOX_SPAWN:", 0) == 0) {
            // Expected format: "BULLET_SPAWN:<bulletID>:<ownerID>:<x>:<y>:<angle>"
            std::stringstream ss(receivedData);
            std::string token;
            std::getline(ss, token, ':'); // Discard "BULLET_SPAWN"
            int ID;
            float x, y;
            int type;
            char delimiter;
            ss >> ID >> delimiter >> x >> delimiter >> y >> delimiter >> type;
            if (ss) {
                Box box(ID, x, y, type);
                boxes.push_back(box);
                std::cout << "Box :) spawned: ID " << ID << ", At  " << x << " and " << y << std::endl;
            }
            return;
        }

        else if (receivedData.rfind("TURN:", 0) == 0) {
            std::stringstream ss(receivedData);
            std::string token;
            std::getline(ss, token, ':');
            char delimiter;
            ss >> turnedColumn1 >> delimiter >> turnedRow1 >> delimiter >> turnedColumn2 >> delimiter >> turnedRow2;

            //std::cout << "Paket Sprejet: " << turnedColumn1 << "  " << turnedRow1 << "  " << turnedColumn2 << "  " << turnedRow2 << std::endl;
            return;
        }

        else if (receivedData.rfind("TOCKE:", 0) == 0) {
            std::stringstream ss(receivedData);
            std::string token;
            std::getline(ss, token, ':');
            int score;
            int ID;
            char delimiter;
          
            ss >> score >> delimiter >> ID;

            if (ID != currentPlayerId) {
                Game::GetInstance()->NasprotnikTocke = score;
            }

            return;
        }

        else if (receivedData == "START" && Game::GetInstance()->clicked == 1) {
            PLayersWaiting = 0;
            Game::GetInstance()->clicked = 0;
            Game::GetInstance()->m_State = 1;
            players[currentPlayerId].Position.X = (((currentPlayerId + 1) * 32 + (6 * 32)) - 32) + 6;
            players[currentPlayerId].Position.Y = 16 * 32 + 8;
            Game::GetInstance()->canMove = 0;
            movementStartTime = SDL_GetTicks();
        }

        else if (receivedData == "SPOMIN_START" && Game::GetInstance()->clickedSpomin == 1) {
            Game::GetInstance()->SpominStarted = 1;
        }

        else if (receivedData.rfind("TANKS_START:", 0) == 0 && Game::GetInstance()->clickedTanks == 1) {
            std::string posData = receivedData.substr(12); // skip "TANKS_START:"
            size_t commaPos = posData.find(',');
            if (commaPos != std::string::npos) {
                int spawnX = std::stoi(posData.substr(0, commaPos));
                int spawnY = std::stoi(posData.substr(commaPos + 1));
                // Set the player's spawn position
                players[currentPlayerId].Position.X = spawnX;
                players[currentPlayerId].Position.Y = spawnY;

                // Start the game state, etc.
                Game::GetInstance()->m_State = 2;
                movementStartTime = SDL_GetTicks();
            }
            Game::GetInstance()->clickedTanks = 0;
            Game::GetInstance()->m_State = 2;
            movementStartTime = SDL_GetTicks();
            LastBox = SDL_GetTicks();
        }

        else {
            std::stringstream ss(receivedData);
            int id;
            float x, y;
            double angle, Bangle;
            char delimiter;
            bool shieldOn;
            int state;
            int _carSkin, _tankSkin;


            while (ss >> id >> delimiter >> x >> delimiter >> y >> delimiter >> angle >> delimiter >> Bangle >> delimiter >> shieldOn >> delimiter >> state >> delimiter >> _carSkin >> delimiter >> _tankSkin) {

                updatePlayerPosition(id, x, y, angle, Bangle, shieldOn, state, _carSkin, _tankSkin);
            }
        }
    }

    void sendPacket(const std::string& message) {
        ENetPacket* packet = enet_packet_create(message.c_str(), message.size() + 1, ENET_PACKET_FLAG_RELIABLE);
        enet_peer_send(peer, 0, packet);
        enet_host_flush(client);
    }

    ENetHost* client;
    ENetAddress address;
    ENetPeer* peer;
    ENetEvent event;

};