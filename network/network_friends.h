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
	class network_friends_menu {
	public:
		void render();
		void update();
	};

	static network_friends_menu* getNetworkFriendsMenu() {
		static network_friends_menu instance;
		return &instance;
	}

	namespace network::friends::vars {
		struct variables {

		};

		extern variables m_vars;
	}
}