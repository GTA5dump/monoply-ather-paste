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
	class network_players_teleport_menu {
	public:
		void render();
		void update();
	};

	static network_players_teleport_menu* getNetworkPlayersTeleportMenu() {
		static network_players_teleport_menu instance;
		return &instance;
	}

	namespace players::selected::teleport::vars{
		struct variables {
			
		};

		extern variables m_vars;
	}
}