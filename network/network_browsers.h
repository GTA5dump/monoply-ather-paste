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

#include "rage/classes/SessionInfo.h"
namespace menu {
	class network_browsers_menu {
	public:
		void render();
		void update();
	};

	static network_browsers_menu* getNetworkBrowsersMenu() {
		static network_browsers_menu instance;
		return &instance;
	}

	namespace network::browsers::vars {
		struct variables {
			bool region_filter_enabled = true;
			int region_filter = 0;

			bool language_filter_enabled = false;
			int language_filter = 0;

			bool pool_filter_enabled = false;
			int pool_filter = 0;

			bool player_count_filter_enabled = true;
			int player_count_filter_minimum = 0;
			int player_count_filter_maximum = 32;

			int sort_method = 0;
			int sort_direction = 0;

			bool replace_game_matchmaking = false;

		};

		extern variables m_vars;
	}
}