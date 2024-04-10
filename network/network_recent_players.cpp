#include "pch.h"
#include "network_recent_players.h"
#include "rage/invoker/natives.h"
#include "gui/util/timer.h"
#include "menu/util/globals.h"
#include "rage/engine.h"
#include "gui/util/notify.h"
#include "rage/classes/rlSessionByGamerTaskResult.h"
#include "rage/classes/rlTaskStatus.h"
#include "util/caller.h"
#include "rage/classes/CPlayerInfo.h"
#include "rage/classes/CPed.h"
#include "gui/util/panels.h"
#include "util/http.h"
#include "util/json.h"
using namespace base::gui;
using namespace menu::network::recent_players::vars;

namespace menu::network::recent_players::vars {
    variables m_vars;


    uint32_t selectedPlayer;
}


namespace menu {

    void network_recent_players_menu::render() {

        renderer::addSubmenu("Recent Players", "Network Recent Players", [&](core* core) {
			for (auto player : m_vars.m_joined) {
				if (player->GetName() && strcmp(player->GetName(), "") != 0) {
					core->addOption(submenuOption(player->GetName())
						.addClick([=] { selectedPlayer = player->m_player_id; }).setTarget("Selected Recent Player"));
				}
			}
        });

		renderer::addPlayerSubmenu(&selectedPlayer, "Selected Recent Player"_joaat, [&](pcore* core) {
			recent_info_panel(selectedPlayer);

            core->addOption(buttonOption("Join Player"));

            core->addOption(buttonOption("Invite to Session"));

            core->addOption(buttonOption("View Profile")
                .addClick([=] {
                    if (*patterns::is_session_started) {
                        auto net_player = patterns::get_net_player(selectedPlayer);
                        auto rid = net_player->GetGamerInfo()->m_gamer_handle_2.m_rockstar_id;
                        if (rid > 0) {
                            Any netHandle[13];
                            NETWORK::NETWORK_HANDLE_FROM_MEMBER_ID(std::to_string(rid).c_str(), netHandle, 13);
                            NETWORK::NETWORK_SHOW_PROFILE_UI(netHandle);
                        }
                    }
                   }));
		});

    }

    void network_recent_players_menu::update() {
        render();

		for (std::int32_t i = 0; i < 32; ++i) {
			std::string name = PLAYER::GET_PLAYER_NAME(i);

			auto netPlayer = patterns::get_net_player(i);

			if (name == "**Invalid**" || name == PLAYER::GET_PLAYER_NAME(PLAYER::PLAYER_ID())) {}
			else {
				if (std::find(m_vars.m_joined.begin(), m_vars.m_joined.end(), netPlayer) != m_vars.m_joined.end()) {}
				else {
					m_vars.m_joined.push_back(netPlayer);
				}
			}
		}
    }

}