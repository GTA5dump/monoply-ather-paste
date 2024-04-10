#pragma once
#include "gui/options/core.h"
#include "gui/options/submenu.h"
#include "gui/options/option.h"
#include "gui/options/button.h"
#include "gui/options/submenu_option.h"
#include "gui/options/number.h"
#include "gui/options/toggle.h"
#include "gui/options/scroll.h"
#include "gui/options/break.h"

#include "rage/classes/CNetGamePlayer.h"
namespace menu {
	class network_recent_players_menu {
	public:
		void render();
		void update();
	};

	static network_recent_players_menu* getNetworkRecentPlayersMenu() {
		static network_recent_players_menu instance;
		return &instance;
	}

	namespace network::recent_players::vars {
		struct variables {
			std::vector<CNetGamePlayer*> m_joined;
		};

		extern variables m_vars;
	}
}