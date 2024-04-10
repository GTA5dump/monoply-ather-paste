#include <memory>
#include "discord_register.h"
#include "discord_rpc.h"

namespace menu {
    class discord_handler {
    public:
        void init();
        void tick();
        void shutdown();
    };
    inline std::unique_ptr<discord_handler> g_discord;
}