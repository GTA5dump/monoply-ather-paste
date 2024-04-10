#include "pch.h"
#include "network_global_chat.h"
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
#include "rage/classes/chatData.h"
#include "hooks/hooks.h"
using namespace base::gui;
using namespace menu::network::chat::vars;

namespace menu::network::chat::vars {
    variables m_vars;

	void draw_chat(const char* msg, const char* player_name, bool is_team) {
		int scaleform = GRAPHICS::REQUEST_SCALEFORM_MOVIE("MULTIPLAYER_CHAT");

		GRAPHICS::BEGIN_SCALEFORM_MOVIE_METHOD(scaleform, "ADD_MESSAGE");
		GRAPHICS::SCALEFORM_MOVIE_METHOD_ADD_PARAM_PLAYER_NAME_STRING(player_name); // player name
		GRAPHICS::SCALEFORM_MOVIE_METHOD_ADD_PARAM_LITERAL_STRING(msg);             // content
		GRAPHICS::SCALEFORM_MOVIE_METHOD_ADD_PARAM_TEXTURE_NAME_STRING(HUD::GET_FILENAME_FOR_AUDIO_CONVERSATION(is_team ? "MP_CHAT_TEAM" : "MP_CHAT_ALL")); // scope
		GRAPHICS::SCALEFORM_MOVIE_METHOD_ADD_PARAM_BOOL(false);                               // teamOnly
		GRAPHICS::SCALEFORM_MOVIE_METHOD_ADD_PARAM_INT((int)HudColor::HUD_COLOUR_PURE_WHITE); // eHudColour
		GRAPHICS::END_SCALEFORM_MOVIE_METHOD();

		GRAPHICS::BEGIN_SCALEFORM_MOVIE_METHOD(scaleform, "SET_FOCUS");
		GRAPHICS::SCALEFORM_MOVIE_METHOD_ADD_PARAM_INT(1);                                    // VISIBLE_STATE_DEFAULT
		GRAPHICS::SCALEFORM_MOVIE_METHOD_ADD_PARAM_INT(0);                                    // scopeType (unused)
		GRAPHICS::SCALEFORM_MOVIE_METHOD_ADD_PARAM_INT(0);                                    // scope (unused)
		GRAPHICS::SCALEFORM_MOVIE_METHOD_ADD_PARAM_PLAYER_NAME_STRING(player_name);           // player
		GRAPHICS::SCALEFORM_MOVIE_METHOD_ADD_PARAM_INT((int)HudColor::HUD_COLOUR_PURE_WHITE); // eHudColour
		GRAPHICS::END_SCALEFORM_MOVIE_METHOD();

		GRAPHICS::DRAW_SCALEFORM_MOVIE_FULLSCREEN(scaleform, 255, 255, 255, 255, 0);

		if (const auto chat_data = *patterns::chat_data; chat_data && (chat_data->m_chat_open || chat_data->m_timer_two))
			HUD::CLOSE_MP_TEXT_CHAT();
	}
	static bool active = false;
	void send_chat_message() {
		MISC::DISPLAY_ONSCREEN_KEYBOARD(true, (char*)"Input", (char*)"", (char*)"", (char*)"", (char*)"", (char*)"", 256);
		while (MISC::UPDATE_ONSCREEN_KEYBOARD() == 0) {
			active = true;
			renderer::getRenderer()->reset_keys();
			util::fiber::go_to_main();
		}
		active = false;
		if (!MISC::GET_ONSCREEN_KEYBOARD_RESULT())
			return;
		m_vars.message = MISC::GET_ONSCREEN_KEYBOARD_RESULT();
	}
}


namespace menu {

    void network_chat_menu::render() {
        renderer::addSubmenu("Global Chat", "Network Chat", [&](core* core) {
			core->addOption(submenuOption("Spammer")
				.setTarget("Chat Spammer"));

			core->addOption(toggleOption("Mocking")
				.addToggle(&m_vars.m_mocking));

			core->addOption(toggleOption("Disable Profanity Filter")
				.addToggle(&m_vars.m_bypass_profanity_filter));

			core->addOption(toggleOption("Log Chat Messages")
				.addToggle(&m_vars.m_log_chat_messages));

			core->addOption(toggleOption("Log Text Messages")
				.addToggle(&m_vars.m_log_text_messages));
        });

		renderer::addSubmenu("Spammer", "Chat Spammer", [&](core* core) {
			core->addOption(keyboardOption("Enter Message")
				.addRightText(m_vars.message)
				.addClick(send_chat_message));

			core->addOption(toggleOption("Team Chat")
				.addToggle(&m_vars.is_team));

			core->addOption(toggleOption("Spam")
				.addToggle(&m_vars.m_spam));

			core->addOption(numberOption<int>("Interval (ms)")
				.addNumber(&m_vars.delay)
				.addMin(0).addMax(300).addStep(1));

			core->addOption(buttonOption("Send Message")
				.addClick([] {
					if (const auto net_game_player = (*patterns::network_player_mgr)->m_local_net_player; net_game_player) {
						if (base::hooks::ogSendChatMessageHook(*patterns::chat_ptr,
							net_game_player->GetGamerInfo(), m_vars.message, m_vars.is_team))
							draw_chat(m_vars.message, net_game_player->GetName(), m_vars.is_team);
					}
					}));
		});
    }

    void network_chat_menu::update() {
        render();

		if (m_vars.m_spam) {
			if (GetTickCount() - m_vars.timer >= m_vars.delay)
			{
				if (const auto net_game_player = (*patterns::network_player_mgr)->m_local_net_player; net_game_player) {
					if (base::hooks::ogSendChatMessageHook(*patterns::chat_ptr,
						net_game_player->GetGamerInfo(), m_vars.message, m_vars.is_team))
						draw_chat(m_vars.message, net_game_player->GetName(), m_vars.is_team);
				}
				m_vars.timer = GetTickCount();
			}
		}
    }

}