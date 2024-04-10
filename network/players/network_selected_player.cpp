#include "pch.h"
#include "network_selected_player.h"
#include "rage/invoker/natives.h"
#include "../network_players.h"
#include "gui/util/panels.h"

#include "network_players_friendly.h"
#include "network_players_teleport.h"
#include "network_players_abusive.h"
#include "network_players_vehicle.h"
#include "network_selected_spawner.h"
#include "network_player_removals.h"
using namespace base::gui;
using namespace menu::players::selected::vars;

namespace menu::players::selected::vars {
	variables m_vars;

	/*void view_profile() {
		if (*patterns::is_session_started) {
			auto net_player = patterns::get_net_player(players::vars::m_vars.m_selected_player);
			auto rid = net_player->GetGamerInfo()->m_gamer_handle_2.m_rockstar_id;
			if (rid > 0) {
				int64_t netHandle[13];
				NETWORK::NETWORK_HANDLE_FROM_MEMBER_ID(std::to_string(rid).c_str(), netHandle, 13);
				NETWORK::NETWORK_SHOW_PROFILE_UI(netHandle);
			}
		}
	}*/
	static bool active = false;
	void send_msg() {
		MISC::DISPLAY_ONSCREEN_KEYBOARD(true, (char*)"Input", (char*)"", (char*)"", (char*)"", (char*)"", (char*)"", 20);
		while (MISC::UPDATE_ONSCREEN_KEYBOARD() == 0) {
			active = true;
			renderer::getRenderer()->reset_keys();
			util::fiber::go_to_main();
		}
		active = false;
		if (!MISC::GET_ONSCREEN_KEYBOARD_RESULT())
			return;
		m_vars.msg_to_send = MISC::GET_ONSCREEN_KEYBOARD_RESULT();
	}

	const char* message_color[] = {
		"~s~White", "~u~Black", "~r~Red", "~g~Green", "~b~Blue", "~p~Purple", "~y~Yellow", "~o~Orange", "~c~Grey", "~h~Bold"
	}; std::size_t message_color_id = 1;
}

namespace menu {
	void network_selected_player_menu::render() {
		renderer::addPlayerSubmenu(&players::vars::m_vars.m_selected_player, rage::joaat("selected_player"), [](pcore* core) {
			player_info_panel(players::vars::m_vars.m_selected_player);
			core->addOption(submenuOption("Friendly")
				.setTarget("selected_player_friendly"));

			core->addOption(submenuOption("Teleport")
				.setTarget("selected_player_teleport"));

			core->addOption(submenuOption("Messaging")
				.setTarget("selected_player_messaging"));

			core->addOption(submenuOption("Chat Commands")
				.setTarget("selected_player_commands"));

			core->addOption(submenuOption("Abusive")
				.setTarget("selected_player_abusive"));

			core->addOption(submenuOption("Vehicle")
				.setTarget("selected_player_vehicle"));

			core->addOption(submenuOption("Spawner")
				.setTarget("selected_player_spawner"));

			core->addOption(submenuOption("Removals")
				.setTarget("selected_player_removals"));

			core->addOption(toggleOption("Spectate")
				.addToggle(&m_vars.m_spectate));

			/*core->addOption(buttonOption("View Profile")
				.addClick([] { view_profile(); }));*/
		});

		renderer::addPlayerSubmenu(&players::vars::m_vars.m_selected_player, rage::joaat("selected_player_commands"), [](pcore* core) {
			player_info_panel(players::vars::m_vars.m_selected_player);
			static bool explode;
			static bool spawn;
			static bool health;
			static bool armor;
			static bool cha_comman;
			core->addOption(toggleOption("Toggle Chat Commands")
				.addToggle(&cha_comman));

			core->addOption(breakOption("Commands"));

			core->addOption(toggleOption("/Explode")
				.addToggle(&explode));

			core->addOption(toggleOption("/Spawn")
				.addToggle(&spawn));

			core->addOption(toggleOption("/Health")
				.addToggle(&health));

			core->addOption(toggleOption("/Armor")
				.addToggle(&armor));
		});

		renderer::addPlayerSubmenu(&players::vars::m_vars.m_selected_player, rage::joaat("selected_player_messaging"), [](pcore* core) {
			player_info_panel(players::vars::m_vars.m_selected_player);
			core->addOption(keyboardOption("Enter Message")
				.addRightText(m_vars.msg_to_send)
				.addClick(send_msg));

			core->addOption(scrollOption<const char*, std::size_t>("Color")
				.addScroll(&message_color).setPosition(&message_color_id));

		/*	core->addOption(buttonOption("Send Message")
				.addClick([] {
					int64_t handle[26];
					NETWORK::NETWORK_HANDLE_FROM_PLAYER(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(players::vars::m_vars.m_selected_player), &handle[0], 13);
					if (NETWORK::NETWORK_IS_HANDLE_VALID(&handle[0], 13)) {
						NETWORK::NETWORK_SEND_TEXT_MESSAGE(m_vars.msg_to_send, &handle[0]);
				}}));*/
		});
	}

	void network_selected_player_menu::update() {
		render();
		getNetworkPlayersFriendlyMenu()->update();
		getNetworkPlayersTeleportMenu()->update();
		getNetworkPlayersAbusiveMenu()->update();
		getNetworkPlayersVehicleMenu()->update();
		getNetworkPlayersSpawnerMenu()->update();
		getNetworkPlayersRemovalsMenu()->update();

		if (m_vars.m_spectate) {
			const auto target = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(players::vars::m_vars.m_selected_player);

			NETWORK::NETWORK_SET_IN_SPECTATOR_MODE(true, target);
			HUD::SET_MINIMAP_IN_SPECTATOR_MODE(true, target);
		}
		else {
			NETWORK::NETWORK_SET_IN_SPECTATOR_MODE(false, -1);
			HUD::SET_MINIMAP_IN_SPECTATOR_MODE(false, -1);

		}
	}
}