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
namespace menu {
	class network_tunables_menu {
	public:
		void render();
		void update();
	};

	static network_tunables_menu* getNetworkTunablesMenu() {
		static network_tunables_menu instance;
		return &instance;
	}

	namespace network::tunables::vars {
		struct variables {
			bool m_idle_kick = true;
			bool m_despwan_bypass = true;
			bool m_freemode_bypass;
			bool m_game_bypass;
			bool m_disable_stat_caps;
			bool m_disable_ghost_org_cooldown;
			bool m_disable_bst_cooldown;
			bool m_disable_bribe_cops_cooldown;
			bool m_disable_ceo_call_cooldown;
			bool m_disable_transaction_error;
			bool m_bypass_country_restrictions;
		};

		extern variables m_vars;
	}
}