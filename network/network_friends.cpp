#include "pch.h"
#include "network_friends.h"
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
using namespace menu::network::friends::vars;

namespace menu::network::friends::vars {
	variables m_vars;


	uint32_t selectedFriend;
}


namespace menu {

	void network_friends_menu::render() {
        fcore* selectedFriendMenu = nullptr; 

        renderer::addSubmenu("Friends", "Network Friends", [&](core* core) {
            for (std::uint32_t i = 0; i < patterns::friend_registry->m_friend_count; i++) {
                core->addOption(submenuOption(patterns::friend_registry->get(i)->m_name)
                    .addClick([=] {
                        selectedFriend = i;
                        if (selectedFriendMenu) {
                            selectedFriendMenu->update_friend(selectedFriend);
                        }
                    }).setTarget("selected_friend_menu"));
            }
        });


        renderer::addFriendSubmenu(&selectedFriend, "selected_friend_menu"_joaat, [&](fcore* core) {
            core->update_friend(selectedFriend);
            selectedFriendMenu = core;
            friend_info_panel(selectedFriend);

            core->addOption(buttonOption("Join Player"));

            core->addOption(buttonOption("Invite to Session"));


            core->addOption(buttonOption("View Profile")
                .addClick([=] {
                    if (*patterns::is_session_started) {
                        auto net_player = patterns::get_net_player(selectedFriend);
                        auto rid = net_player->GetGamerInfo()->m_gamer_handle.m_rockstar_id;
                        if (rid > 0) {
                            Any netHandle[13];
                            NETWORK::NETWORK_HANDLE_FROM_MEMBER_ID(std::to_string(rid).c_str(), netHandle, 13);
                            NETWORK::NETWORK_SHOW_PROFILE_UI(netHandle);
                        }
                    }
                }));
        });

	}

	void network_friends_menu::update() {
		render();
	}

}