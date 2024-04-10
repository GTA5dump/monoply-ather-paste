#include "pch.h"
#include "network_tunables.h"
#include "rage/invoker/natives.h"
#include "gui/util/timer.h"
#include "menu/util/globals.h"
#include "rage/classes/enums.h"
#include "menu/util/scr_global.h"
#include "gui/render.h"
using namespace base::gui;
using namespace menu::network::tunables::vars;

namespace menu::network::tunables::vars {
	variables m_vars;

}


namespace menu {

	void network_tunables_menu::render() {
		renderer::addSubmenu("Tunables", "Network Tunables", [](core* core) {
			core->addOption(buttonOption("Remove Transaction Pending")
				.addClick([] { *menu::script_global(4536678).as<int*>() = 0; *menu::script_global(4536679).as<int*>() = 0; *menu::script_global(4536677).as<int*>() = 0; }));

			core->addOption(buttonOption("Refresh Interior")
				.addClick([] { INTERIOR::REFRESH_INTERIOR(INTERIOR::GET_INTERIOR_FROM_ENTITY(PLAYER::PLAYER_PED_ID())); }));

			core->addOption(buttonOption("Force Cloud Save")
				.addClick([] { STATS::STAT_SAVE(0, 0, 3, 0); }));

			core->addOption(toggleOption("Disable Idle Kick")
				.addToggle(&m_vars.m_idle_kick));

			core->addOption(toggleOption("Disable Stat Caps")
				.addToggle(&m_vars.m_idle_kick));

			core->addOption(toggleOption("Disable Error Banner")
				.addToggle(&m_vars.m_disable_transaction_error));

			core->addOption(toggleOption("Disable Authorities Cooldown")
				.addToggle(&m_vars.m_disable_bribe_cops_cooldown));

			core->addOption(toggleOption("Disable Ghost Organization")
				.addToggle(&m_vars.m_disable_ghost_org_cooldown));

			core->addOption(toggleOption("Disable BST Cooldown")
				.addToggle(&m_vars.m_disable_bst_cooldown));

			core->addOption(toggleOption("Disable Ceo Call Vehicle Cooldown")
				.addToggle(&m_vars.m_disable_ceo_call_cooldown));

			core->addOption(toggleOption("Allow Missions in Private Sessions")
				.addToggle(&m_vars.m_freemode_bypass));

			core->addOption(toggleOption("Bypass Casino Region Lock")
				.addToggle(&m_vars.m_bypass_country_restrictions));

			core->addOption(toggleOption("Despawn Bypass")
				.addToggle(&m_vars.m_despwan_bypass));

			core->addOption(toggleOption("Game Bypass")
				.addToggle(&m_vars.m_game_bypass));
		});
	}


	void network_tunables_menu::update() {
		render();

		if (m_vars.m_disable_transaction_error) {
			*menu::script_global(4536678).as<int*>() = 0;
			*menu::script_global(4536679).as<int*>() = 0;
			*menu::script_global(4536677).as<int*>() = 0;
		}

		if (m_vars.m_disable_bribe_cops_cooldown) {
			*menu::script_global(262145).at(13011).as<int*>() = 1;
		}

		if (m_vars.m_disable_ghost_org_cooldown) {
			*menu::script_global(262145).at(13010).as<int*>() = 1;
		}

		if (m_vars.m_disable_bst_cooldown) {
			*menu::script_global(262145).at(13009).as<int*>() = 1;
		}

		if (m_vars.m_disable_ceo_call_cooldown) {
			*menu::script_global(262145).at(13005).as<int*>() = 1;
		}

		if (m_vars.m_despwan_bypass) {
			*menu::script_global(4540731).as<int*>() = 1; 
		}

		if (m_vars.m_freemode_bypass) {
			*menu::script_global(2697098).as<int*>() = 1;
		}

		if (m_vars.m_game_bypass) {
			*menu::script_global(1669211).as<int*>() = 1;
		}

		if (m_vars.m_disable_stat_caps) {
			*menu::script_global(262145).at(167).as<int*>() = 1;
		}

		if (m_vars.m_idle_kick) {
			*menu::script_global(1654054).at(1155).as<int*>() = 0;//idle kick
			*menu::script_global(262145).at(87).as<int*>() = 120000;//idle kick
			*menu::script_global(262145).at(88).as<int*>() = 300000;//idle kick
			*menu::script_global(262145).at(89).as<int*>() = 600000;//idle kick
			*menu::script_global(262145).at(90).as<int*>() = 900000;//idle kick

			*menu::script_global(1654054).at(1171).as<int*>() = 0;//idle kick
			*menu::script_global(262145).at(8420).as<int*>() = 30000;//idle kick
			*menu::script_global(262145).at(8421).as<int*>() = 60000;//idle kick
			*menu::script_global(262145).at(8422).as<int*>() = 90000;//idle kick
			*menu::script_global(262145).at(8423).as<int*>() = 120000;//idle kick
		}
	}

}