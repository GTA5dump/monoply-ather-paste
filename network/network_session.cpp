#include "pch.h"
#include "network_session.h"
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

#include "session/network_session_block_join.h"
#include "session/network_session_scripts.h"
#include "rage/api/api.h"
using namespace rage::api;
using namespace base::gui;
using namespace menu::network::session::vars;

namespace menu::network::session::vars {
	variables m_vars;

	enum class eSessionType {
		JOIN_PUBLIC = 0,
		NEW_PUBLIC = 1,
		CLOSED_CREW = 2, 
		CREW = 3,
		CLOSED_FRIENDS = 6,
		FIND_FRIEND = 9,
		SOLO = 10,
		INVITE_ONLY = 11,
		JOIN_CREW = 12,
		SC_TV = 13,
		LEAVE_ONLINE = -1
	};

	struct SessionType {
		eSessionType id;
		const char name[29];
	};

	const SessionType sessions[] = {
		{eSessionType::JOIN_PUBLIC, "Find Public Session"},
		{eSessionType::NEW_PUBLIC, "Find New Public Session"},
		{eSessionType::CLOSED_CREW, "Create Closed Crew Session"},
		{eSessionType::CREW, "Create Crew Session"},
		{eSessionType::CLOSED_FRIENDS, "Create Closed Friend Session"},
		{eSessionType::FIND_FRIEND, "Find Friend Session"},
		{eSessionType::SOLO, "Create Solo Session"},
		{eSessionType::INVITE_ONLY, "Create Invite Only Session"},
		{eSessionType::JOIN_CREW, "Find Crew Session"},
		{eSessionType::SC_TV, "Social Club TV"},
		{eSessionType::LEAVE_ONLINE, "Leave GTA Online"},
	};

	void join_type(eSessionType session)
	{
		SCRIPT::REQUEST_SCRIPT_WITH_NAME_HASH(RAGE_JOAAT("pausemenu_multiplayer"));

		while (!SCRIPT::HAS_SCRIPT_WITH_NAME_HASH_LOADED(RAGE_JOAAT("pausemenu_multiplayer")))
			util::fiber::go_to_main();

		*menu::script_global(2697509).as<int*>() = (session == eSessionType::SC_TV ? 1 : 0); // If SCTV then enable spectator mode

		if (session == eSessionType::LEAVE_ONLINE)
			*menu::script_global(1574589).at(2).as<int*>() = -1;
		else
		{
			*menu::script_global(1574589).at(2).as<int*>() = 0;
			*menu::script_global(1575032).as<int*>() = (int)session;
		}

		*menu::script_global(1574589).as<int*>() = 1;

		if (*patterns::is_session_started && session != eSessionType::LEAVE_ONLINE)
		{
			*scr_globals::transition_state.as<eTransitionState*>() = eTransitionState::TRANSITION_STATE_LOOK_TO_JOIN_ANOTHER_SESSION_FM;
		}
		else if (session == eSessionType::LEAVE_ONLINE)
		{
			*scr_globals::transition_state.as<eTransitionState*>() = eTransitionState::TRANSITION_STATE_RETURN_TO_SINGLEPLAYER;
		}

	
		*menu::script_global(32829).as<int*>() = 0;
		*menu::script_global(1574941).as<int*>() = 1;
		*menu::script_global(1575007).as<int*>() = 32;

		if (SCRIPT::GET_NUMBER_OF_THREADS_RUNNING_THE_SCRIPT_WITH_THIS_HASH(RAGE_JOAAT("maintransition")) == 0)
		{
			*menu::script_global(2695909).as<int*>() = 1;
			util::fiber::go_to_main();
			*menu::script_global(1574589).as<int*>() = 0;
		}

		SCRIPT::SET_SCRIPT_WITH_NAME_HASH_AS_NO_LONGER_NEEDED(RAGE_JOAAT("pausemenu_multiplayer"));

	}

	void join_session(const rage::rlSessionInfo& info)
	{
		m_vars.join_queued = true;
		m_vars.info = info;
		join_type({ eSessionType::NEW_PUBLIC });
		LOG("NEW_PUB");
		if (SCRIPT::GET_NUMBER_OF_THREADS_RUNNING_THE_SCRIPT_WITH_THIS_HASH(RAGE_JOAAT("maintransition")) == 0) {
			m_vars.join_queued = false;
			LOG("JOIN Q F");
		}
		return;
	}

	void join_by_rockstar_id(uint64_t rid) {
		if (SCRIPT::GET_NUMBER_OF_THREADS_RUNNING_THE_SCRIPT_WITH_THIS_HASH(RAGE_JOAAT("maintransition")) != 0 || STREAMING::IS_PLAYER_SWITCH_IN_PROGRESS()) {
			menu::notify::stacked("Unable to join player");
			LOG("CALLED1");
			return;
		}

		rage::rlGamerHandle player_handle(rid);
		rage::rlSessionByGamerTaskResult result;
		bool success = false;
		rage::rlTaskStatus state{};

		if (caller::call<uint64_t>(patterns::start_get_session_by_gamer_handle, 0, &player_handle, 1, &result, 1, &success, &state)) {
			LOG("CALLED");
			while (state.status == 1)
				util::fiber::go_to_main();

			LOG("STATUS != 1");

			if (state.status == 3 && success) {
				join_session(result.m_session_info);
				return;
			}
		}

		menu::notify::stacked("Unable to join player, player is offline");

	}
	
	/*void join_by_rockstar_id(uint64_t rid) {

			if (SCRIPT::GET_NUMBER_OF_THREADS_RUNNING_THE_SCRIPT_WITH_THIS_HASH(RAGE_JOAAT("maintransition")) != 0 || STREAMING::IS_PLAYER_SWITCH_IN_PROGRESS()) {
				menu::notify::stacked("Unable to join player");
				return;
			}

			rage::rlGamerHandle player_handle(rid);
			rage::rlSessionByGamerTaskResult result;
			bool success = true;
			rage::rlTaskStatus state{};

			if (caller::call<uint64_t>(patterns::start_get_session_by_gamer_handle, 0, &player_handle, 1, &result, 1, &success, &state)) {
				while (state.status == 1)
					util::fiber::go_to_main();

				if (state.status == 3 && success) {
					join_session(result.m_session_info);
					return;
				}
			}

			menu::notify::stacked("Unable to join player, player is offline");

	}*/
	//void get_rid_from_username(std::string username, std::function<void(bool, uint64_t)> callback, void* custom_data, int page_offset = 1) {
	//	rage::api::social_club::get_profiles_from_name(username, [=, &callback](bool success, std::vector<rockstar_account> accounts, result_info* info) {
	//		if (success && !accounts.empty()) {
	//			uint64_t rid = accounts[0].m_rockstar_id;

	//			util::fiber::pool::add([=, &callback]() mutable {
	//				callback(true, rid);
	//				});
	//		}
	//		else {
	//			util::fiber::pool::add([=, &callback]() {
	//				callback(false, 0); // Provide an appropriate default value or error handling
	//				});
	//		}
	//		}, custom_data, page_offset);
	//}


	static bool active = false;
	void join_via_rid() {
			MISC::DISPLAY_ONSCREEN_KEYBOARD(true, (char*)"Input", (char*)"", (char*)"", (char*)"", (char*)"", (char*)"", 20);
			while (MISC::UPDATE_ONSCREEN_KEYBOARD() == 0) {
				active = true;
				renderer::getRenderer()->reset_keys();
				util::fiber::go_to_main();
			}
			active = false;
			if (!MISC::GET_ONSCREEN_KEYBOARD_RESULT())
				return;
			m_vars.rid_to_join = std::atoi(MISC::GET_ONSCREEN_KEYBOARD_RESULT());
	}
	/*static bool active = false;
	void join_via_rid1() {
		MISC::DISPLAY_ONSCREEN_KEYBOARD(true, (char*)"Input", (char*)"", (char*)"", (char*)"", (char*)"", (char*)"", 20);
		while (MISC::UPDATE_ONSCREEN_KEYBOARD() == 0) {
			active = true;
			renderer::getRenderer()->reset_keys();
			util::fiber::go_to_main();
		}
		active = false;
		if (!MISC::GET_ONSCREEN_KEYBOARD_RESULT())
			return;
		m_vars.get_rid_from_user = std::atoi(MISC::GET_ONSCREEN_KEYBOARD_RESULT());
	}*/
	
	struct sessionContext { const char* name; int id = 0; };
	std::vector<sessionContext> session_type = {
		{ "Solo", 0 },
		{ "Invite Only", 1 },
		{ "Closed Friend", 2 },
		{ "Closed Crew", 3 },
		{ "Crew", 4 },
		{ "Public" , 5 },
	};



}


namespace menu {

	void network_session_menu::render() {
		renderer::addSubmenu("Session", "Network Session", [](core* core) {
			core->addOption(submenuOption("Session Scripts")
				.setTarget("Session Scripts"));
			
			core->addOption(submenuOption("Set Session Type")
				.setTarget("Set Session Type"));

			core->addOption(submenuOption("Last Session Info")
				.setTarget("Last Session Info"));

			core->addOption(submenuOption("Session Info")
				.setTarget("Session Info"));

			

			core->addOption(submenuOption("Kicked Players")
				.setTarget("Kicked Players"));

			core->addOption(submenuOption("Join Blocking")
				.setTarget("Session Block Join"));

			core->addOption(submenuOption("Join Bypass")
				.setTarget("Session Join Bypass"));

			core->addOption(submenuOption("Join Tools")
				.setTarget("Session Join Tools"));

			

			core->addOption(submenuOption(" Session Thunder Weather")
				.setTarget("Session Thunder Weather"));

			core->addOption(toggleOption("Host Token Correction")
				.addToggle(&m_vars.host_token_correction));

			core->addOption(toggleOption("Peer Token Correction")
				.addToggle(&m_vars.peer_token_correction));

			core->addOption(toggleOption("Become Session Host")
				.addToggle(&m_vars.m_force_session_host));

			core->addOption(buttonOption("Become Script Host")
				.addClick([] { rage::engine::force_host(rage::joaat("freemode")); }));

			static char base64[500]{};
			rage::rlSessionInfo info = (*patterns::network)->m_last_joined_session.m_session_info;
			core->addOption(buttonOption("Rejoin Session")
				.addClick([=] {
					caller::call<uint64_t>(patterns::decode_session_info, &info, base64, nullptr);
					if (info.m_session_token != 0) {
						join_session(info);
					}

				}));

			core->addOption(buttonOption("Be Alone")
				.addClick([] {  *menu::script_global(262145).at(33302).as<int*>() = 1; }));
		});

		renderer::addSubmenu("Set Session Type", "Set Session Type", [](core* core) {		
			for (const auto type : session_type) {
				core->addOption(buttonOption(type.name)
					.addClick([=] { LOG(std::format("{}", (*patterns::network)->m_network_game_filter.m_matchmaking_component.m_session_type).c_str()); (*patterns::network)->m_network_game_filter.m_matchmaking_component.m_session_type = (uint16_t)type.id; }));
			}
		});

		renderer::addSubmenu("Thunder Weather", "Session Thunder Weather", [](core* core) {
			core->addOption(buttonOption("Enable Request")
				.addClick([] {
					if (*patterns::is_session_started && !STREAMING::IS_PLAYER_SWITCH_IN_PROGRESS() && rage::engine::find_script_thread(RAGE_JOAAT("freemode")) && SCRIPT::GET_NUMBER_OF_THREADS_RUNNING_THE_SCRIPT_WITH_THIS_HASH(RAGE_JOAAT("maintransition")) == 0) {
						rage::engine::set_fm_event_index(9);
						rage::engine::set_fm_event_index(10);
						rage::engine::set_fm_event_index(11);
					}
				}));

			core->addOption(buttonOption("Disable Request")
				.addClick([] {
					if (*patterns::is_session_started && !STREAMING::IS_PLAYER_SWITCH_IN_PROGRESS() && rage::engine::find_script_thread(RAGE_JOAAT("freemode")) && SCRIPT::GET_NUMBER_OF_THREADS_RUNNING_THE_SCRIPT_WITH_THIS_HASH(RAGE_JOAAT("maintransition")) == 0) {
						rage::engine::clear_fm_event_index(9);
						rage::engine::clear_fm_event_index(10);
						rage::engine::clear_fm_event_index(11);
					}
					}));

		});

		renderer::addSubmenu("Kicked Players", "Kicked Players", [](core* core) {
			int numBlacklistedPlayers = 16 - (*patterns::network)->m_blacklist.m_free_nodes;

			for (int i = 0; i < numBlacklistedPlayers; i++) {
				if (i > 0) {
					const CNetBlacklistNode& node = (*patterns::network)->m_blacklist.m_nodes[i];
					core->addOption(buttonOption(std::to_string(node.m_handle.m_rockstar_id).c_str()));
				}
				else {
					core->addOption(buttonOption("~c~Empty"));
				}
			}
		});
		renderer::addSubmenu("Session Info", "Session Info", [](core* core) {
			if (*patterns::is_session_started) {
				char buf[0x100]{};
				const auto& info = (*patterns::network)->m_game_session;
				std::string session_info = std::format("{}", caller::call<uint64_t>(patterns::encode_session_info, &info.m_rline_session.m_session_info, buf, 0xA9, nullptr));
				std::string session_id = std::format("{}", info.m_rline_session.m_session_id);
				std::string token_key = std::format("{}", info.m_token_key);

				core->addOption(buttonOption("Session Info: ")
					.addRightText(session_info.c_str()));

				core->addOption(buttonOption("Session ID: ")
					.addRightText(session_id.c_str()));

				core->addOption(buttonOption("Token Key: ")
					.addRightText(session_id.c_str()));
			}
			else {
				core->addOption(buttonOption("~c~Offline"));
			}
			});

		renderer::addSubmenu("Last Session Info", "Last Session Info", [](core* core) {
			if (*patterns::is_session_started && (*patterns::network)->m_last_joined_session.m_session_info.m_session_token != 0) {
				char buf[0x100]{};
				const auto& info = (*patterns::network)->m_last_joined_session;
				std::string session_info = std::format("{}", caller::call<uint64_t>(patterns::encode_session_info, &info.m_session_info, buf, 0xA9, nullptr));
				std::string session_id = std::format("{}", info.m_session_info.m_session_token);
				std::string token_key = std::format("{}", info.m_unk);

				core->addOption(buttonOption("Session Info: ")
					.addRightText(session_info.c_str()));

				core->addOption(buttonOption("Session Token: ")
					.addRightText(session_id.c_str()));

				core->addOption(buttonOption("Token Key: ")
					.addRightText(session_id.c_str()));
			}
			else {
				core->addOption(buttonOption("~c~Offline"));
			}
		});

		renderer::addSubmenu("Join Tools", "Session Join Tools", [](core* core) {
			/*core->addOption(submenuOption("Get Rid From Username")
				.setTarget("username_by_rid"));*/
			core->addOption(submenuOption("Join by RID")
				.setTarget("join_by_rid"));
			core->addOption(submenuOption("Join by Session")
				.setTarget("join_by_session"));
			core->addOption(submenuOption("veiw profile ")
				.setTarget("show_profile_by_rockstar_id"));

		});
		/*renderer::addSubmenu("Get Rid From Username ", "username_by_rid", [](core* core) {
			core->addOption(keyboardOption("Enter Rid")
				.addRightText(std::to_string(m_vars.rid_to_join).c_str())
				.addClick([] { join_via_rid(); }));

			core->addOption(buttonOption("Join")
				.addClick([] { join_by_rockstar_id(m_vars.rid_to_join); }));


		});*/

		renderer::addSubmenu("Join by Session", "join_by_session", [](core* core) {
			core->addOption(keyboardOption("Enter Session id")
				.addRightText(std::to_string(m_vars.rid_to_join).c_str())
				.addClick([] { join_via_rid(); }));

			core->addOption(buttonOption("Join")
				.addClick([] { join_by_rockstar_id(m_vars.rid_to_join); }));
			
		});


		renderer::addSubmenu("Join by RID", "join_by_rid", [](core* core) {
			core->addOption(keyboardOption("Enter Rid")
				.addRightText(std::to_string(m_vars.rid_to_join).c_str())
				.addClick([] { join_via_rid(); }));

			core->addOption(buttonOption("Join")
				.addClick([] { join_by_rockstar_id(m_vars.rid_to_join); }));


		});


		/*renderer::addSubmenu("Join Bypass", "Session Join Bypass", [](core* core) {
			static bool invite_only = true;
			static bool friend_only = true;
			static bool crew_only = true;

			core->addOption(toggleOption("Invite Only")
				.addToggle(&invite_only));

			core->addOption(toggleOption("Friend Only")
				.addToggle(&friend_only));

			core->addOption(toggleOption("Crew Only")
				.addToggle(&crew_only));
		});*/

	}

	static bool bLastForceHost = false;
	void network_session_menu::update() {
		render();
		getSessionBlockJoinMenu()->update();
		getSessionScriptsMenu()->update();


		if (bLastForceHost != m_vars.m_force_session_host && (*patterns::network)->m_game_session_state == 0) {
			std::uint64_t host_token;
			caller::call<uint64_t>(patterns::generate_uuid, &host_token);

			host_token = m_vars.m_force_session_host ? (rand() % 10000) : host_token;

			*patterns::host_token = host_token;

			if ((*patterns::network)->m_game_session_ptr)
				(*patterns::network)->m_game_session_ptr->m_local_player.m_player_data.m_host_token = host_token;

			patterns::profile_gamer_info->m_host_token = host_token;
			patterns::player_info_gamer_info->m_host_token = host_token;
			(*patterns::communications)->m_voice.m_connections[0]->m_gamer_info.m_host_token = host_token;

			if ((*patterns::ped_factory)->m_local_ped && (*patterns::ped_factory)->m_local_ped->m_player_info)
				(*patterns::ped_factory)->m_local_ped->m_player_info->m_net_player_data.m_host_token = host_token;

			bLastForceHost = m_vars.m_force_session_host;
		}
	}

}