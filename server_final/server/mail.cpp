#include "Server.h"

int main() {
    NetworkServer network;

    while (true) {
        // 1. Handle network events
        network.handleNetworkEvents();

        // 2. Update game state
        // Handle player updates, physics, AI, etc.

        // 3. Send updates to clients
        // network.sendDataToClient(peer, "Updated game state");

        // 4. Render if applicable (if this is also handling graphics)
    }
}
