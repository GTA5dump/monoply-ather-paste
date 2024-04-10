
#include <chrono>
#include "handler.cpp"
#include <rage/invoker/natives.h>
#include "discord_rpc.h"
#pragma comment(lib, "discord-rpc.lib")

namespace menu {


    void discord_handler::init() {
        DiscordEventHandlers handlers;
        std::memset(&handlers, 0, sizeof(handlers));
        Discord_Initialize("941965788666019890", &handlers, 1, "0");
    }

    std::string UserType;
    std::string State;

    void discord_handler::tick() {



        static int64_t start_time{ std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count() };
        DiscordRichPresence discordPresence;
        std::memset(&discordPresence, 0, sizeof(discordPresence));
        std::string SessionType;
        srand(time(NULL));


        discordPresence.startTimestamp = start_time;
        discordPresence.largeImageKey = "image_1_";


        if (NETWORK::NETWORK_IS_SESSION_STARTED()) {
            discordPresence.state = "Online";
            discordPresence.partySize = NETWORK::NETWORK_GET_NUM_CONNECTED_PLAYERS();
            discordPresence.partyMax = 32;
        }
        else {
            discordPresence.state = "Story Mode";
        }
        discordPresence.instance = 1;
        Discord_UpdatePresence(&discordPresence);
    }

    void discord_handler::shutdown() {
        Discord_ClearPresence();
        Discord_Shutdown();
    }
}