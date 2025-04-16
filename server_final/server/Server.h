#include "enet/enet.h"
#include <iostream>
#include <unordered_map>
#include <string>
#include <sstream>
#include <queue>
#include "Database.h"
#include "Bullet.h"
#include <vector>
#include "Box.h"

class NetworkServer {
public:

    int PlayersWaiting = 0;
    int PlayersWaitingForTanks = 0;
    int PlayersWaitingForSpomin = 0;

    struct PlayerData {
        int id;
        float x;
        float y;
        double angle, Bangle;

        bool shieldOn;

        int state;

        int carSkin, tankSkin;

        PlayerData() = default;
        PlayerData(int playerId, float posX, float posY, double playerAngle = 0.0, double BarrelAngle = 0.0, bool _shieldOn = 0, int _state = 0, int _car = 0, int _tank = 0)
            : id(playerId), x(posX), y(posY), angle(playerAngle), Bangle(BarrelAngle), shieldOn(_shieldOn),  state(_state), carSkin (_car), tankSkin (_tank){}

    };
    
    std::vector<ENetPeer*> waitingPlayers;

    NetworkServer() {
        if (enet_initialize() != 0) {
            std::cerr << "An error occurred while initializing ENet." << std::endl;
            exit(EXIT_FAILURE);
        }

        if (!initializeDatabase()) {
            std::cerr << "Database initialization failed!" << std::endl;
            exit(EXIT_FAILURE);
        }

        address.host = ENET_HOST_ANY;
        address.port = 1234;
        server = enet_host_create(&address, 32, 2, 0, 0);
        if (server == nullptr) {
            std::cerr << "An error occurred while trying to create an ENet server." << std::endl;
            exit(EXIT_FAILURE);
        }
        std::cout << "Server started.\n";
    }

    ~NetworkServer() {
        enet_host_destroy(server);
        enet_deinitialize();
        closeDatabase();
    }

    void handleNetworkEvents() {
        ENetEvent event;
        while (enet_host_service(server, &event, 0) > 0) {
            switch (event.type) {
            case ENET_EVENT_TYPE_CONNECT: {
                int playerID = assignPlayerID();

                event.peer->data = new int(playerID);

                int Xp = (((playerID + 1) * 32 + (6 * 32)) - 32) + 6;
                int Yp = 16 * 32 + 8;

                players[playerID] = PlayerData(playerID, Xp, Yp, 0.0);

                std::stringstream ss;
                ss << "ID:" << playerID << ":" << Xp << ":" << Yp << ":0";
                std::string message = ss.str();
                ENetPacket* packet = enet_packet_create(message.c_str(), message.size() + 1, ENET_PACKET_FLAG_RELIABLE);
                enet_peer_send(event.peer, 0, packet);

                std::cout << "ID: " << playerID << " na poziciji (" << Xp << ", " << Yp << ")." << std::endl;
                break;
            }

            case ENET_EVENT_TYPE_RECEIVE:
                handleReceivedPacket(event);
                enet_packet_destroy(event.packet);
                break;

            case ENET_EVENT_TYPE_DISCONNECT:
                std::cout << "Client disconnected.\n";
                removePlayerFromMap(event); 
                break;
            }
        }
    }



private:

    std::unordered_map<int, PlayerData> players;

    std::queue<int> availableIDs; 
    int currentPlayerID = 1;  

    std::unordered_map<int, Bullet> bullets; 
    std::vector<Box> boxes;
    int nextBulletID = 1;
    int nextBoxID = 1;

    ENetAddress address;
    ENetHost* server;

    int assignPlayerID() {
        if (!availableIDs.empty()) {
            int availableID = availableIDs.front();
            availableIDs.pop();
            std::cout << "Reusing ID: " << availableID << std::endl;
            return availableID;
        }
        else {
            std::cout << "Assigning new ID: " << currentPlayerID << std::endl;
            return currentPlayerID++;
        }
    }

    void removePlayerFromMap(ENetEvent& event) {
        int* playerID = static_cast<int*>(event.peer->data);
        if (playerID != nullptr) {
            std::cout << "Player " << *playerID << " disconnected.\n";

            players.erase(*playerID);
            availableIDs.push(*playerID);

            // pošle paketke
            std::stringstream ss;
            ss << "DISCONNECT:" << *playerID;
            std::string message = ss.str();

            ENetPacket* packet = enet_packet_create(message.c_str(), message.size() + 1, ENET_PACKET_FLAG_RELIABLE);
            for (size_t i = 0; i < server->peerCount; ++i) {
                ENetPeer* peer = &server->peers[i];
                if (peer->state == ENET_PEER_STATE_CONNECTED) {
                    enet_peer_send(peer, 0, packet);
                }
            }

            delete playerID;
            event.peer->data = nullptr;
        }
    }

    void spawnBullet(int playerID, float x, float y, double angle) {
        int bulletID = nextBulletID++;
        Bullet bullet(bulletID, playerID, x, y, angle);
        bullets[bulletID] = bullet;

        std::stringstream ss;
        ss << "BULLET_SPAWN:" << bulletID << ":" << playerID << ":" << x << ":" << y << ":" << angle;
        std::string message = ss.str();
        ENetPacket* packet = enet_packet_create(message.c_str(), message.size() + 1, ENET_PACKET_FLAG_RELIABLE);

        for (size_t i = 0; i < server->peerCount; ++i) {
            ENetPeer* peer = &server->peers[i];
            if (peer->state == ENET_PEER_STATE_CONNECTED) {
                enet_peer_send(peer, 0, packet);
            }
        }
    }

    void spawnBox( float x, float y, int type) {
        int BoxID = nextBoxID++;
        Box box(BoxID, x, y, type);
        boxes.push_back(box);

        std::stringstream ss;
        ss << "BOX_SPAWN:" << BoxID << ":" << x << ":" << y << ":" << type;
        std::string message = ss.str();
        ENetPacket* packet = enet_packet_create(message.c_str(), message.size() + 1, ENET_PACKET_FLAG_RELIABLE);

        for (size_t i = 0; i < server->peerCount; ++i) {
            ENetPeer* peer = &server->peers[i];
            if (peer->state == ENET_PEER_STATE_CONNECTED) {
                enet_peer_send(peer, 0, packet);
            }
        }
    }

    void handleReceivedPacket(ENetEvent& event) {
        std::string data((char*)event.packet->data);

        if (data == "STOP") {
            PlayersWaiting = 0;
            sendStopMessage();
        }

        if (data.rfind("FIRE:", 0) == 0) {
          
            std::stringstream ss(data);
            std::string prefix;
            std::getline(ss, prefix, ':'); 
            int playerID;
            float x, y;
            double angle;
            char delimiter;
            ss >> playerID >> delimiter >> x >> delimiter >> y >> delimiter >> angle;
            if (ss) {
                spawnBullet(playerID, x, y, angle);
            }
            return; 
        }

        if (data.rfind("BOX:", 0) == 0) {
            std::stringstream ss(data);
            std::string prefix;
            std::getline(ss, prefix, ':'); 
            int x, y;
            int type;
            char delimiter;
            ss >> delimiter >> x >> delimiter >> y >> delimiter >> type;
            if (ss) {
                spawnBox(x, y, type);
            }

            return; 
        }

        if (data == "WAITING") {
            PlayersWaiting++;
            std::cout << PlayersWaiting << std::endl;
            if (PlayersWaiting >= 2) {
                sendStart();
            }
            else {
                sendWaiting();
            }
            
        }

        if (data == "SPOMIN_WAITING") {
            PlayersWaitingForSpomin++;
            std::cout << PlayersWaitingForSpomin << std::endl;
            if (PlayersWaitingForSpomin >= 2) {
                sendStartSpomin();
            }
            else {
                sendWaitingSpomin();
            }

        }

        if (data == "TANKS_WAITING") {
            waitingPlayers.push_back(event.peer);
            std::cout << "Players waiting: " << waitingPlayers.size() << std::endl;

            if (waitingPlayers.size() >= 2) {
                
                int spawn1X = 192, spawn1Y = 160;
                int spawn2X = 1504, spawn2Y = 832;

                
                std::stringstream ss1;
                ss1 << "TANKS_START:" << spawn1X << "," << spawn1Y;
                std::string message1 = ss1.str();
                ENetPacket* packet1 = enet_packet_create(message1.c_str(), message1.size() + 1, ENET_PACKET_FLAG_RELIABLE);
                enet_peer_send(waitingPlayers[0], 0, packet1);

                
                std::stringstream ss2;
                ss2 << "TANKS_START:" << spawn2X << "," << spawn2Y;
                std::string message2 = ss2.str();
                ENetPacket* packet2 = enet_packet_create(message2.c_str(), message2.size() + 1, ENET_PACKET_FLAG_RELIABLE);
                enet_peer_send(waitingPlayers[1], 0, packet2);

                std::cout << "Game starting: assigned spawn positions." << std::endl;

                
                waitingPlayers.clear();
            }
            else {
              
                std::stringstream ss;
                ss << "CAKA_TANKE:" << waitingPlayers.size();
                std::string message = ss.str();
                ENetPacket* packet = enet_packet_create(message.c_str(), message.size() + 1, ENET_PACKET_FLAG_RELIABLE);
                enet_peer_send(event.peer, 0, packet);
                std::cout << "Waiting for more players..." << std::endl;
            }

        }

        if (data.rfind("LOGIN:", 0) == 0) { 
            std::stringstream ss(data);
            std::string prefix, ime, geslo;
            char delimiter;

            std::getline(ss, prefix, ':');
            std::getline(ss, ime, ':');
            std::getline(ss, geslo, ':');

            if (!ime.empty() && !geslo.empty()) {
                if (registerUser(ime, geslo)) {
                  
                    sendMsgToPeer(event.peer, "REGISTER_SUCCESS");
                }
                else {
                   
                    sendMsgToPeer(event.peer, "USERNAME_EXISTS");
                }
            }
            
            
        }

        if (data.rfind("DELETE:", 0) == 0) {
            std::stringstream ss(data);
            std::string prefix;
            std::getline(ss, prefix, ':');  
            int id;
            ss >> id;
            bullets.erase(id);
            std::cout << "IZBRISAN BULLET " << id << std::endl;
            sendDelete(id);
            return;  
        }

        if (data.rfind("SEED_REQUEST:", 0) == 0) {
            sendSeed();
            return;
        }

        if (data.rfind("BOX_DELETE:", 0) == 0) {
            std::stringstream ss(data);
            std::string prefix;
            std::getline(ss, prefix, ':');
            int id;
            ss >> id;
            bullets.erase(id);
            sendDeleteBox(id);
            return;
        }

        if (data.rfind("HIT:", 0) == 0) {
            std::stringstream ss(data);
            std::string prefix;
            std::getline(ss, prefix, ':');
            int id;
            ss >> id;
            bullets.erase(id);
            std::cout << "IZBRISAN  " << id << std::endl;
            sendDelete(id);
            return;
        }

        if (data.rfind("TURN:", 0) == 0) {
            ENetPacket* packet = enet_packet_create(data.c_str(), data.size() + 1, ENET_PACKET_FLAG_RELIABLE);
            enet_host_broadcast(server, 0, packet);
            enet_host_flush(server);
            std::cout << "Broadcasted " << data << std::endl;
            return;
        }

        if (data.rfind("TOCKE:", 0) == 0) {
           
            ENetPacket* packet = enet_packet_create(data.c_str(), data.size() + 1, ENET_PACKET_FLAG_RELIABLE);
            enet_host_broadcast(server, 0, packet);
            enet_host_flush(server);
            std::cout << "Broadcasted " << data << std::endl;
            return;
        }

        else if (data.rfind("END_TURN:", 0) == 0) {
            sendEndTurn();
            return;
        }

        if (data.rfind("PRIJAVA:", 0) == 0) {
            std::stringstream ss(data);
            std::string prefix, username, password;
            std::getline(ss, prefix, ':');
            std::getline(ss, username, ':');
            std::getline(ss, password, ':');

            if (!username.empty() && !password.empty()) {
                if (loginUser(username, password)) {
                    std::string carSkin, tankSkin;
                    if (getSkins(username, carSkin, tankSkin)) {
                        std::stringstream response;
                        response << "LOGIN_SUCCESS:" << username << ":" << carSkin << ":" << tankSkin;
                        sendMsgToPeer(event.peer, response.str());
                    }
                    else {
                        sendMsgToPeer(event.peer, "LOGIN_SUCCESS:SKINS_NOT_FOUND");
                    }
                }
                else {
                    sendMsgToPeer(event.peer, "INVALID_CREDENTIALS");
                }
            }
            return;
        }

        else if (data.rfind("SKIN:", 0) == 0) {
            std::stringstream ss(data);
            std::string prefix, carSkin, tankSkin, playerName;
            std::getline(ss, prefix, ':');
            std::getline(ss, carSkin, ':');
            std::getline(ss, tankSkin, ':');
            std::getline(ss, playerName, ':');

            std::cout << carSkin << " to je avto skin ki bi ga mogu met player     " << playerName << std::endl;

            if (updateSkins(playerName, carSkin, tankSkin)) {

                sendMsgToPeer(event.peer, "SKIN_UPDATE_SUCCESS");
            }
            else {
                sendMsgToPeer(event.peer, "SKIN_UPDATE_FAILED");
            }

            return;
        }

        else {
            std::stringstream ss(data);
            int id;
            float x, y;
            double angle, Bangle;
            char delimiter;
            bool shield;
            int state;
            int carSkin, tankSkin;

            ss >> id >> delimiter >> x >> delimiter >> y >> delimiter >> angle >> delimiter >> Bangle >> delimiter >> shield >> delimiter >> state >> delimiter >> carSkin >> delimiter >> tankSkin;

            if (ss && delimiter == ':') {
                players[id] = { id, x, y, angle, Bangle, shield, state, carSkin, tankSkin};
                broadcastPlayerPositions();
                //std::cout << carSkin << " skin za avto " << id << " id za avto " << std::endl;
            }
        }
    }

    void sendMsgToPeer(ENetPeer* peer, const std::string& message) {
        ENetPacket* packet = enet_packet_create(message.c_str(), message.size() + 1, ENET_PACKET_FLAG_RELIABLE);
        enet_peer_send(peer, 0, packet);
    }

    void sendStopMessage() {
        std::string message = "STOP";
        ENetPacket* packet = enet_packet_create(message.c_str(), message.size() + 1, ENET_PACKET_FLAG_RELIABLE);
        for (size_t i = 0; i < server->peerCount; ++i) {
            ENetPeer* peer = &server->peers[i];
            if (peer->state == ENET_PEER_STATE_CONNECTED) {
                enet_peer_send(peer, 0, packet);
            }
        }
        std::cout << "Broadcasted STOP message to all clients." << std::endl;
    }

    void sendStart() {
        std::string message = "START";
        PlayersWaiting = 0;
        ENetPacket* packet = enet_packet_create(message.c_str(), message.size() + 1, ENET_PACKET_FLAG_RELIABLE);
        for (size_t i = 0; i < server->peerCount; ++i) {
            ENetPeer* peer = &server->peers[i];
            if (peer->state == ENET_PEER_STATE_CONNECTED) {
                enet_peer_send(peer, 0, packet);
            }
        }
        PlayersWaiting = 0;
        std::cout << "IGEA SE ZACENJA" << std::endl;
    }

    void sendStartSpomin() {
        std::string message = "SPOMIN_START";
        PlayersWaitingForSpomin = 0;
        ENetPacket* packet = enet_packet_create(message.c_str(), message.size() + 1, ENET_PACKET_FLAG_RELIABLE);
        for (size_t i = 0; i < server->peerCount; ++i) {
            ENetPeer* peer = &server->peers[i];
            if (peer->state == ENET_PEER_STATE_CONNECTED) {
                enet_peer_send(peer, 0, packet);
            }
        }
        PlayersWaitingForSpomin = 0;
        std::cout << "IGEA SPOMINA SE ZACENJA" << std::endl;
    }

    void sendTankStart() {
        std::string message = "TANKS_START";
        PlayersWaitingForTanks = 0;
        ENetPacket* packet = enet_packet_create(message.c_str(), message.size() + 1, ENET_PACKET_FLAG_RELIABLE);
        for (size_t i = 0; i < server->peerCount; ++i) {
            ENetPeer* peer = &server->peers[i];
            if (peer->state == ENET_PEER_STATE_CONNECTED) {
                enet_peer_send(peer, 0, packet);
            }
        }
        PlayersWaitingForTanks = 0;
        std::cout << "IGEA TANKOV SE ZACENJA" << std::endl;
    }

    void sendEndTurn() {
        std::string message = "END_TURN";
        ENetPacket* packet = enet_packet_create(message.c_str(), message.size() + 1, ENET_PACKET_FLAG_RELIABLE);
        for (size_t i = 0; i < server->peerCount; ++i) {
            ENetPeer* peer = &server->peers[i];
            if (peer->state == ENET_PEER_STATE_CONNECTED) {
                enet_peer_send(peer, 0, packet);
            }
        }

    }

    void sendWaiting() {
        std::stringstream ss;
        ss << "CAKA:" << PlayersWaiting;
        std::string message = ss.str();
        ENetPacket* packet = enet_packet_create(message.c_str(), message.size() + 1, ENET_PACKET_FLAG_RELIABLE);
        for (size_t i = 0; i < server->peerCount; ++i) {
            ENetPeer* peer = &server->peers[i];
            if (peer->state == ENET_PEER_STATE_CONNECTED) {
                enet_peer_send(peer, 0, packet);
            }
        }
        std::cout << "TOK PLAYERJEV CAKA" << PlayersWaiting << std::endl;
    }

    void sendWaitingTanks() {
        std::stringstream ss;
        ss << "CAKA_TANKE:" << PlayersWaitingForTanks;
        std::string message = ss.str();
        ENetPacket* packet = enet_packet_create(message.c_str(), message.size() + 1, ENET_PACKET_FLAG_RELIABLE);
        for (size_t i = 0; i < server->peerCount; ++i) {
            ENetPeer* peer = &server->peers[i];
            if (peer->state == ENET_PEER_STATE_CONNECTED) {
                enet_peer_send(peer, 0, packet);
            }
        }
        std::cout << "TOK PLAYERJEV CAKA" << PlayersWaitingForTanks << std::endl;
    }

    void sendWaitingSpomin() {
        std::stringstream ss;
        ss << "SPOMIN_CAKA:" << PlayersWaitingForSpomin;
        std::string message = ss.str();
        ENetPacket* packet = enet_packet_create(message.c_str(), message.size() + 1, ENET_PACKET_FLAG_RELIABLE);
        for (size_t i = 0; i < server->peerCount; ++i) {
            ENetPeer* peer = &server->peers[i];
            if (peer->state == ENET_PEER_STATE_CONNECTED) {
                enet_peer_send(peer, 0, packet);
            }
        }
        std::cout << "TOK PLAYERJEV CAKA NA SPOMIN " << PlayersWaitingForSpomin << std::endl;
    }

    void sendDelete(int id) {
        std::stringstream ss;
        ss << "DELETE:" << id;
        std::string message = ss.str();
        ENetPacket* packet = enet_packet_create(message.c_str(), message.size() + 1, ENET_PACKET_FLAG_RELIABLE);
        for (size_t i = 0; i < server->peerCount; ++i) {
            ENetPeer* peer = &server->peers[i];
            if (peer->state == ENET_PEER_STATE_CONNECTED) {
                enet_peer_send(peer, 0, packet);
            }
        }
        
    }

    void sendDeleteBox(int id) {
        std::stringstream ss;
        ss << "BOX_DELETE:" << id;
        std::string message = ss.str();
        ENetPacket* packet = enet_packet_create(message.c_str(), message.size() + 1, ENET_PACKET_FLAG_RELIABLE);
        for (size_t i = 0; i < server->peerCount; ++i) {
            ENetPeer* peer = &server->peers[i];
            if (peer->state == ENET_PEER_STATE_CONNECTED) {
                enet_peer_send(peer, 0, packet);
            }
        }

    }

    void sendSeed() {
        std::stringstream ss;
        unsigned int seed = static_cast<unsigned int>(std::time(nullptr));
        ss << "SEED:" << seed;
        std::string message = ss.str();
        ENetPacket* packet = enet_packet_create(message.c_str(), message.size() + 1, ENET_PACKET_FLAG_RELIABLE);
        for (size_t i = 0; i < server->peerCount; ++i) {
            ENetPeer* peer = &server->peers[i];
            if (peer->state == ENET_PEER_STATE_CONNECTED) {
                enet_peer_send(peer, 0, packet);
            }
        }

    }


    void broadcastPlayerPositions() {
        std::stringstream ss;
        for (const auto& player : players) {
            ss << player.second.id << ":" << player.second.x << ":" << player.second.y << ":" << player.second.angle << ":" << player.second.Bangle << ":" << player.second.shieldOn << ":" << player.second.state <<  ":" << player.second.carSkin << ":" << player.second.tankSkin << "\n";
        }
        std::string message = ss.str();
        ENetPacket* packet = enet_packet_create(message.c_str(), message.size() + 1, ENET_PACKET_FLAG_RELIABLE);
        for (size_t i = 0; i < server->peerCount; ++i) {
            ENetPeer* peer = &server->peers[i];
            if (peer->state == ENET_PEER_STATE_CONNECTED) {
                enet_peer_send(peer, 0, packet);
            }
        }
    }
};

