#include "pch.h"
#include "network_new_session.h"
#include "rage/invoker/natives.h"
#include "gui/util/timer.h"
#include "menu/util/globals.h"

using namespace base::gui;
using namespace menu::network::new_session::vars;

namespace menu::network::new_session::vars {
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

	void join_type(eSessionType session) {
		util::fiber::pool::add([=] {
			SCRIPT::REQUEST_SCRIPT_WITH_NAME_HASH(RAGE_JOAAT("pausemenu_multiplayer"));

			while (!SCRIPT::HAS_SCRIPT_WITH_NAME_HASH_LOADED(RAGE_JOAAT("pausemenu_multiplayer")))
				util::fiber::go_to_main();

			*script_global(2697509).as<int*>() = (session == eSessionType::SC_TV ? 1 : 0); // If SCTV then enable spectator mode

			if (session == eSessionType::LEAVE_ONLINE)
				*script_global(1574589).at(2).as<int*>() = -1;
			else
			{
				*script_global(1574589).at(2).as<int*>() = 0;
				*script_global(1575032).as<int*>() = (int)session;
			}

			*script_global(1574589).as<int*>() = 1;

			if (*patterns::is_session_started && session != eSessionType::LEAVE_ONLINE)
			{
				*scr_globals::transition_state.as<eTransitionState*>() = eTransitionState::TRANSITION_STATE_LOOK_TO_JOIN_ANOTHER_SESSION_FM;
			}
			else if (session == eSessionType::LEAVE_ONLINE)
			{
				*scr_globals::transition_state.as<eTransitionState*>() = eTransitionState::TRANSITION_STATE_RETURN_TO_SINGLEPLAYER;
			}


			*script_global(32829).as<int*>() = 0;
			*script_global(1574941).as<int*>() = 1;
			*script_global(1575007).as<int*>() = 32;

			if (SCRIPT::GET_NUMBER_OF_THREADS_RUNNING_THE_SCRIPT_WITH_THIS_HASH(RAGE_JOAAT("maintransition")) == 0)
			{
				*script_global(2695909).as<int*>() = 1;
				util::fiber::go_to_main();
				*script_global(1574589).as<int*>() = 0;
			}

			SCRIPT::SET_SCRIPT_WITH_NAME_HASH_AS_NO_LONGER_NEEDED(RAGE_JOAAT("pausemenu_multiplayer"));
			});
	}
}

namespace menu {

	void network_new_session_menu::render() {
		renderer::addSubmenu("New Session", "Network New Session", [](core* core) {
			for (const auto& session_type : sessions) {
				core->addOption(buttonOption(session_type.name)
					.addClick([=] { join_type(session_type.id); }));
			}
			});
	}

	void network_new_session_menu::update() {
		render();
	}

} // namespace menu
