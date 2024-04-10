#pragma once
#include "gui/options/core.h"
#include "gui/options/submenu.h"
#include "gui/options/option.h"
#include "gui/options/button.h"
#include "gui/options/submenu_option.h"
#include "gui/options/number.h"
#include "gui/options/toggle.h"
#include "gui/options/scroll.h"
#include "menu/util/players.h"

namespace menu {
	class network_players_vehicle_menu {
	public:
		void render();
		void update();
	};

	static network_players_vehicle_menu* getNetworkPlayersVehicleMenu() {
		static network_players_vehicle_menu instance;
		return &instance;
	}

	namespace players::selected::vehicle::vars {
		struct variables {
			bool m_godmode;
			bool m_invisible;
			bool m_disable_collision;
			bool m_slippy_wheels;
			bool m_freeze_vehicle;
		};

		extern variables m_vars;
	}
}