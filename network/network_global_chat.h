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
	class network_chat_menu {
	public:
		void render();
		void update();
	};

	static network_chat_menu* getNetworkChatMenu() {
		static network_chat_menu instance;
		return &instance;
	}

	namespace network::chat::vars {
		struct variables {
			bool is_team;
			bool m_spam;
			int delay = 300;
			int timer = 0;
			const char* message = "monopolymenu.xyz";

			bool m_mocking;
			bool m_log_chat_messages;
			bool m_log_text_messages;
			bool m_bypass_profanity_filter;
		};

		extern variables m_vars;
	}
}