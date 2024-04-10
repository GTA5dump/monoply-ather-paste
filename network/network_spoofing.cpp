#include "pch.h"
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

#include "network_spoofing.h"
#include "spoofing/network_spoofing_host_token.h"
#include "spoofing/network_spoofing_name.h"

#include "rage/classes/CNetworkPlayerMgr.h"
#include "rage/classes/CNetGamePlayer.h"
#include "rage/classes/NetObject.h"


using namespace base::gui;
using namespace menu::network::spoofing::vars;

namespace menu::network::spoofing::vars {
	variables m_vars;

	const char* ping[] = {
		"Very Low", "Low", "Medium", "High", "Very High"
	}; int ping_id = 0;

	const char* face_direction[] = {
		"Left",
		"Right",
		"Back"
	};

	const char* static_type[] = {
		"Relative",
		"Static"
	};

	const char* axis[] = {
		"Pitch",
		"Roll",
		"Yaw"
	}; int axis_id = 0;

	static bool active;
	void do_spoof() {
		MISC::DISPLAY_ONSCREEN_KEYBOARD(true, (char*)"Input", (char*)"", (char*)"", (char*)"", (char*)"", (char*)"", 20);
		while (MISC::UPDATE_ONSCREEN_KEYBOARD() == 0) {
			active = true;
			renderer::getRenderer()->reset_keys();
			util::fiber::go_to_main();
		}
		active = false;
		if (!MISC::GET_ONSCREEN_KEYBOARD_RESULT())
			return;
		m_vars.m_model_name = MISC::GET_ONSCREEN_KEYBOARD_RESULT();
	}

	constexpr const static auto blip_types = std::to_array({
		"Random",
		"ON FOOT",
		"TANK",
		"PLAYER JET",
		"PLAYER PLANE",
		"PLAYER HELI",
		"PLAYER GUNCAR",
		"PLAYER BOAT",
		"ROCKET VOLTIC",
		"TECHNICAL",
		"RUINER 2000",
		"DUNE BUGGY",
		"PHANTOM WEDGE",
		"ARMORED BOXVILLE",
		"WASTELANDER",
		"QUAD",
		"APC",
		"OPPRESSOR MK 1",
		"HALF TRACK",
		"DUNE FAV",
		"WEAPONIZED TAMPA",
		"AA TRAILER",
		"ALPHA Z1",
		"BOMBUSHKA",
		"HAVOK",
		"HOWARD",
		"HUNTER",
		"MICROLIGHT",
		"MOGUL",
		"MOLOTOK",
		"NOKOTA",
		"PYRO",
		"ROGUE",
		"STARLING",
		"SEABREEZE",
		"TULA",
		"STROMBERG",
		"DELUXO",
		"THRUSTER",
		"KHANJALI",
		"RIOT VAN",
		"VOLATOL",
		"BARRAGE",
		"AKULA",
		"CHERNOBOG",
		"AVENGER",
		"TURRETED LIMO",
		"SEA SPARROW",
		"CARACARA",
		"PARTY BUS",
		"TERRORBYTE",
		"MENACER",
		"SCRAMJET",
		"POUNDER CUSTOM",
		"MULE CUSTOM",
		"SPEEDO CUSTOM",
		"OPPRESSOR MK 2",
		"STRIKEFORCE",
		"ARENA BRUISER",
		"ARENA BRUTUS",
		"ARENA CERBERUS",
		"ARENA DEATHBIKE",
		"ARENA DOMINATOR",
		"ARENA IMPALER",
		"ARENA IMPERATOR",
		"ARENA ISSI",
		"ARENA SASQUATCH",
		"ARENA SCARAB",
		"ARENA SLAMVAN",
		"ARENA ZR380",
		"MINI SUB",
		"SPARROW",
		"FOLDING WING JET",
		"GANG BIKE",
		"MILITARY QUAD",
		"SQUADDIE",
		"CAYO DINGHY",
		"WINKY",
		"PATROL BOAT",
		"ANNIHILATOR",
		"KART RETRO",
		"KART MODERN",
		"MILITARY TRUCK",
		"SUBMARINE",
		"CHAMPION",
		"BUFFALO STX",
		"DEITY",
		"JUBILEE",
		"GRANGER 3600LX",
		"PATRIOT MILSPEC",
		"ARMS DEALING AIR",
		"BRICKADE 6X6",
	});

	const auto player_states = std::to_array({
		"Playing",
		"Dead",
		"Arrested",
		"Failed Mission",
		"Left Game",
		"Respawning",
		"In Cutscene"
	});
	inline const char* regions[] = {
		"CIS", "Africa", "East", "Europe", "China", "Australia", "West", "Japan", "Unknown"
	}; inline int region_id = 0;

	inline const char* langs[] = {
		"English", "French", "German", "Italian", "Spanish ", "Portuguese ", "Polish", "Russian", "Korean", "Chinese", "Japanese", "Spanish (Mexico)", "Chinese (Simpified)"
	}; inline int lang_id = 0;

	inline void spoof_member_id() {
		MISC::DISPLAY_ONSCREEN_KEYBOARD(true, (char*)"Input", (char*)"", (char*)"", (char*)"", (char*)"", (char*)"", 20);
		while (MISC::UPDATE_ONSCREEN_KEYBOARD() == 0) {
			active = true;
			renderer::getRenderer()->reset_keys();
			util::fiber::go_to_main();
		}
		active = false;
		if (!MISC::GET_ONSCREEN_KEYBOARD_RESULT())
			return;
		m_vars.m_member_id = std::atoi(MISC::GET_ONSCREEN_KEYBOARD_RESULT());
	}
	inline void spoof_crew_id() {
		MISC::DISPLAY_ONSCREEN_KEYBOARD(true, (char*)"Input", (char*)"", (char*)"", (char*)"", (char*)"", (char*)"", 20);
		while (MISC::UPDATE_ONSCREEN_KEYBOARD() == 0) {
			active = true;
			renderer::getRenderer()->reset_keys();
			util::fiber::go_to_main();
		}
		active = false;
		if (!MISC::GET_ONSCREEN_KEYBOARD_RESULT())
			return;
		m_vars.m_crew_id = std::atoi(MISC::GET_ONSCREEN_KEYBOARD_RESULT());
	}
	inline void spoof_crew_color() {
		MISC::DISPLAY_ONSCREEN_KEYBOARD(true, (char*)"Input", (char*)"", (char*)"", (char*)"", (char*)"", (char*)"", 20);
		while (MISC::UPDATE_ONSCREEN_KEYBOARD() == 0) {
			active = true;
			renderer::getRenderer()->reset_keys();
			util::fiber::go_to_main();
		}
		active = false;
		if (!MISC::GET_ONSCREEN_KEYBOARD_RESULT())
			return;
		m_vars.m_crew_color = std::atoi(MISC::GET_ONSCREEN_KEYBOARD_RESULT());
	}
	inline void spoof_crew_members() {
		MISC::DISPLAY_ONSCREEN_KEYBOARD(true, (char*)"Input", (char*)"", (char*)"", (char*)"", (char*)"", (char*)"", 20);
		while (MISC::UPDATE_ONSCREEN_KEYBOARD() == 0)
		{
			active = true;
			renderer::getRenderer()->reset_keys();
			util::fiber::go_to_main();
		}
		active = false;
		if (!MISC::GET_ONSCREEN_KEYBOARD_RESULT())
			return;
		m_vars.m_crew_members = std::atoi(MISC::GET_ONSCREEN_KEYBOARD_RESULT());
	}
	inline void spoof_crew_time() {
		MISC::DISPLAY_ONSCREEN_KEYBOARD(true, (char*)"Input", (char*)"", (char*)"", (char*)"", (char*)"", (char*)"", 20);
		while (MISC::UPDATE_ONSCREEN_KEYBOARD() == 0) {
			active = true;
			renderer::getRenderer()->reset_keys();
			util::fiber::go_to_main();
		}
		active = false;
		if (!MISC::GET_ONSCREEN_KEYBOARD_RESULT())
			return;
		m_vars.m_crew_time = std::atoi(MISC::GET_ONSCREEN_KEYBOARD_RESULT());
	}
	inline void spoof_crew_tag() {
		MISC::DISPLAY_ONSCREEN_KEYBOARD(true, (char*)"Input", (char*)"", (char*)"", (char*)"", (char*)"", (char*)"", 20);
		while (MISC::UPDATE_ONSCREEN_KEYBOARD() == 0) {
			active = true;
			renderer::getRenderer()->reset_keys();
			util::fiber::go_to_main();
		}
		active = false;
		if (!MISC::GET_ONSCREEN_KEYBOARD_RESULT())
			return;
		m_vars.m_crew_tag = MISC::GET_ONSCREEN_KEYBOARD_RESULT();
	}
}


namespace menu {

	void network_spoofing_menu::render() {
		renderer::addSubmenu("Spoofing", "Network Spoofing", [](core* core) {
			core->addOption(submenuOption("Name")
				.setTarget("Spoofing Name"));

			core->addOption(submenuOption("Host Token")
				.setTarget("Spoofing Host Token"));

			core->addOption(submenuOption("Model")
				.setTarget("Spoofing Model"));

			core->addOption(submenuOption("Crew")
				.setTarget("Spoofing Crew"));

			core->addOption(submenuOption("Blips")
				.setTarget("Spoofing Blips"));

			core->addOption(submenuOption("Account")
				.setTarget("Spoofing Account"));

			core->addOption(submenuOption("Player State")
				.setTarget("Spoofing Player State"));

			core->addOption(submenuOption("Player Position")
				.setTarget("Spoofing Player Position"));

			core->addOption(submenuOption("Player Rotation")
				.setTarget("Spoofing Player Rotation"));

			core->addOption(submenuOption("Vehicle Rotation")
				.setTarget("Spoofing Vehicle Rotation"));

			core->addOption(submenuOption("Session Attributes")
				.setTarget("Spoofing Session Attributes"));

			core->addOption(toggleScrollOption<const char*, int>("Ping")
				.addToggle(&m_vars.m_toggle_ping)
				.addScroll(&ping).setPosition(&ping_id));

			core->addOption(toggleOption("Movement Animation")
				.addToggle(&m_vars.m_movement_animation));

			core->addOption(toggleOption("Rockstar Developer")
				.addToggle(&m_vars.m_dev));

			core->addOption(toggleOption("Rockstar Tester")
				.addToggle(&m_vars.m_tester));

			core->addOption(toggleOption("Cheater")
				.addToggle(&m_vars.m_cheater));
		});


		renderer::addSubmenu("Player Position", "Spoofing Player Position", [](core* core) {
			core->addOption(numberOption<int>("X")
				.addNumber(&m_vars.x).addMin(INT_MIN).addMax(INT_MAX));

			core->addOption(numberOption<int>("Y")
				.addNumber(&m_vars.y).addMin(INT_MIN).addMax(INT_MAX));

			core->addOption(numberOption<int>("Z")
				.addNumber(&m_vars.z).addMin(INT_MIN).addMax(INT_MAX));
			});

		renderer::addSubmenu("Account", "Spoofing Account", [](core* core) {
			core->addOption(numberOption<int>("Rank")
				.addNumber(&m_vars.m_rank).addMin(0).addMax(8000));

			core->addOption(numberOption<int>("KD")
				.addNumber(&m_vars.m_kd).addMin(0).addMax(INT_MAX));
		});

		renderer::addSubmenu("Crew", "Spoofing Crew", [](core* core) {
			core->addOption(submenuOption("Member ID")
				.setTarget("spoofing_crew_member_id"));

			core->addOption(submenuOption("Crew ID")
				.setTarget("spoofing_crew_id"));

			core->addOption(submenuOption("Crew Color")
				.setTarget("spoofing_crew_color"));

			core->addOption(submenuOption("Crew Member Count")
				.setTarget("spoofing_crew_member_count"));

			core->addOption(submenuOption("Crew Creation Time")
				.setTarget("spoofing_crew_creation_time"));

			core->addOption(submenuOption("Crew Tag")
				.setTarget("spoofing_crew_tag"));
		});

		renderer::addSubmenu("Spoof Crew Tag", "spoofing_crew_tag", [](core* core) {
			core->addOption(buttonOption("Input Crew Tag")
				.addClick(spoof_crew_tag));

			core->addOption(buttonOption("Reset to Default")
				.addClick([=] { m_vars.m_spoof_crew_tag = false; }));

			std::string preview = std::format("Apply: {}", m_vars.m_crew_tag.c_str());

			core->addOption(buttonOption(preview.c_str())
				.addClick([=] { m_vars.m_spoof_crew_tag = true; }));
			});
		renderer::addSubmenu("Spoof Crew Creation Time", "spoofing_crew_creation_time", [](core* core) {
			core->addOption(buttonOption("Input Crew Creation Time")
				.addClick(spoof_crew_time));

			core->addOption(buttonOption("Reset to Default")
				.addClick([=] { m_vars.m_spoof_crew_time = false; }));

			std::string preview = std::format("Apply: {}", std::to_string(m_vars.m_crew_time).c_str());

			core->addOption(buttonOption(preview.c_str())
				.addClick([=] { m_vars.m_spoof_crew_time = true; }));
			});
		renderer::addSubmenu("Spoof Crew Member Count", "spoofing_crew_member_count", [](core* core) {
			core->addOption(buttonOption("Input Crew Member Count")
				.addClick(spoof_crew_members));

			core->addOption(buttonOption("Reset to Default")
				.addClick([=] { m_vars.m_spoof_crew_members = false; }));

			std::string preview = std::format("Apply: {}", std::to_string(m_vars.m_crew_members).c_str());

			core->addOption(buttonOption(preview.c_str())
				.addClick([=] { m_vars.m_spoof_crew_members = true; }));
			});
		renderer::addSubmenu("Spoof Crew Color", "spoofing_crew_color", [](core* core) {
			core->addOption(buttonOption("Input Crew Color")
				.addClick(spoof_crew_color));

			core->addOption(buttonOption("Reset to Default")
				.addClick([=] { m_vars.m_spoof_crew_color = false; }));

			std::string preview = std::format("Apply: {}", std::to_string(m_vars.m_crew_color).c_str());

			core->addOption(buttonOption(preview.c_str())
				.addClick([=] { m_vars.m_spoof_crew_color = true; }));
			});
		renderer::addSubmenu("Spoof Crew ID", "spoofing_crew_id", [](core* core) {
			core->addOption(buttonOption("Input Crew ID")
				.addClick(spoof_crew_id));

			core->addOption(buttonOption("Reset to Default")
				.addClick([=] { m_vars.m_spoof_crew_id = false; }));

			std::string preview = std::format("Apply: {}", std::to_string(m_vars.m_crew_id).c_str());

			core->addOption(buttonOption(preview.c_str())
				.addClick([=] { m_vars.m_spoof_crew_id = true; }));
			});
		renderer::addSubmenu("Spoof Crew Member ID", "spoofing_crew_member_id", [](core* core) {
			core->addOption(buttonOption("Input Member ID")
				.addClick(spoof_member_id));

			core->addOption(buttonOption("Reset to Default")
				.addClick([=] { m_vars.m_spoof_member_id = false; }));

			std::string preview = std::format("Apply: {}", std::to_string(m_vars.m_member_id).c_str());

			core->addOption(buttonOption(preview.c_str())
				.addClick([=] { m_vars.m_spoof_member_id = true; }));
			});

		renderer::addSubmenu("Session Attributes", "Spoofing Session Attributes", [](core* core) {
			core->addOption(toggleScrollOption<const char*, int>("Region")
				.addToggle(&m_vars.m_spoof_region).addScroll(&regions).setPosition(&region_id).addClick([=] {  }));

			core->addOption(toggleScrollOption<const char*, int>("Language")
				.addToggle(&m_vars.m_spoof_lang).addScroll(&langs).setPosition(&lang_id).addClick([=] {  }));


			core->addOption(numberOption<int>("Player Count")
				.addNumber(&m_vars.player_count).addMin(1).addMax(32));
		});

		renderer::addSubmenu("Player State", "Spoofing Player State", [](core* core) {
			core->addOption(toggleOption("Toggle")
				.addToggle(&m_vars.m_spoof_state));

			if (m_vars.m_spoof_state) {
				core->addOption(breakOption("State Types"));

				for (int i = 0; i < player_states.size(); i++) {
					core->addOption(buttonOption(player_states[i])
						.addClick([=] {} ));
				}
			}
		});

		renderer::addSubmenu("Blips", "Spoofing Blips", [](core* core) {
			core->addOption(toggleOption("Toggle")
				.addToggle(&m_vars.m_spoof_blip));

			if (m_vars.m_spoof_blip) {
				core->addOption(breakOption("Blip Types"));

				for (int i = 0; i < blip_types.size(); i++) {
					core->addOption(buttonOption(blip_types[i])
						.addClick([=] { m_vars.m_blip_type = i; }));
				}
			}
		});

		renderer::addSubmenu("Model", "Spoofing Model", [](core* core) {
			core->addOption(keyboardOption("Input Model")
				.addClick([=] { do_spoof(); }));

			core->addOption(buttonOption("Reset to Default")
				.addClick([=] {  m_vars.m_model_name = " "; }));

			std::string preview = std::format("Apply: {}", m_vars.m_model_name);
			
			core->addOption(buttonOption(preview.c_str())
				.addClick([=] { }));

		});

		renderer::addSubmenu("Vehicle Rotation", "Spoofing Vehicle Rotation", [](core* core) {
			core->addOption(toggleNumberOption<float>("Spinbot")
				.addToggle(&m_vars.m_vehicle_spinbot)
				.addNumber(&m_vars.m_vehicle_spinbot_speed).addMin(0.f).addMax(100.f).setPrecision(0).addStep(1));

			core->addOption(scrollOption<const char*, int>("Spinbot Axis")
				.addScroll(&axis).setPosition(&axis_id));

			core->addOption(toggleScrollOption<const char*, int>("Jitter")
				.addToggle(&m_vars.m_vehicle_jitter)
				.addScroll(&axis).setPosition(&m_vars.m_vehicle_jitter_axis));

			core->addOption(toggleOption("Relative")
				.addToggle(&m_vars.m_vehicle_relative));

			core->addOption(toggleOption("Static")
				.addToggle(&m_vars.m_vehicle_static));

			if (m_vars.m_vehicle_static) {
				core->addOption(breakOption("Static"));

				core->addOption(toggleNumberOption<float>("Pitch")
					.addToggle(&m_vars.m_vehicle_static_toggle.x)
					.addNumber(&m_vars.m_vehicle_static_value.x).addMin(0.f).addMax(360.f).setPrecision(0).addStep(1));

				core->addOption(toggleNumberOption<float>("Roll")
					.addToggle(&m_vars.m_vehicle_static_toggle.y)
					.addNumber(&m_vars.m_vehicle_static_value.y).addMin(0.f).addMax(360.f).setPrecision(0).addStep(1));

				core->addOption(toggleNumberOption<float>("Yaw")
					.addToggle(&m_vars.m_vehicle_static_toggle.z)
					.addNumber(&m_vars.m_vehicle_static_value.z).addMin(0.f).addMax(360.f).setPrecision(0).addStep(1));
			}
		});

		renderer::addSubmenu("Player Rotation", "Spoofing Player Rotation", [](core* core) {
			core->addOption(toggleNumberOption<float>("Spinbot")
				.addToggle(&m_vars.m_ped_spinbot)
				.addNumber(&m_vars.m_ped_spinbot_speed).addMin(0.f).addMax(14.f).setPrecision(0).addStep(1)
				.addClick([] { m_vars.m_ped_face_direction = false; m_vars.m_ped_jitter = false; }));

			core->addOption(toggleScrollOption<const char*, int>("Face Direction")
				.addToggle(&m_vars.m_ped_face_direction)
				.addScroll(&face_direction).setPosition(&m_vars.m_ped_face_direction_val)
				.addClick([] { m_vars.m_ped_spinbot = false; m_vars.m_ped_jitter = false; }));

			core->addOption(toggleOption("Jitter")
				.addToggle(&m_vars.m_ped_jitter)
				.addClick([] { m_vars.m_ped_spinbot = false; m_vars.m_ped_face_direction = false; }));

			core->addOption(toggleOption("Relative")
				.addToggle(&m_vars.m_ped_relative));
		});

	}

	void network_spoofing_menu::update() {
		render();
		getNetworkSpoofingHostTokenMenu()->update();
		getNetworkSpoofingNameMenu()->update();

		if (m_vars.m_ped_spinbot) {
			static float value = 0.f;
			value += m_vars.m_ped_spinbot_speed;

			if (value > 180.f) value = -180.f + (value - 180.f);
			m_vars.m_ped_hook_value = { value * 0.0174532924f, value * 0.0174532924f };
			return;
		}

		if (m_vars.m_ped_jitter) {
			static float value = 0.f;
			value += 40.f;
			if (value > 180.f) value = -180.f + (value - 180.f);

			m_vars.m_ped_hook_value = { value * 0.0174532924f, value * 0.0174532924f };
		}

		if (PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), false)) {
			m_vars.m_vehicle_hook_value.x = ENTITY::GET_ENTITY_ROTATION(PLAYER::PLAYER_PED_ID(), 2).x;
			m_vars.m_vehicle_hook_value.y = ENTITY::GET_ENTITY_ROTATION(PLAYER::PLAYER_PED_ID(), 2).y;
			m_vars.m_vehicle_hook_value.z = ENTITY::GET_ENTITY_ROTATION(PLAYER::PLAYER_PED_ID(), 2).z;

			if (m_vars.m_vehicle_static) {
				if (m_vars.m_vehicle_static_toggle.x) m_vars.m_vehicle_hook_value.x = m_vars.m_vehicle_static_value.x + (m_vars.m_vehicle_relative ? ENTITY::GET_ENTITY_ROTATION(PLAYER::PLAYER_PED_ID(), 2).x : 0.f);
				if (m_vars.m_vehicle_static_toggle.y) m_vars.m_vehicle_hook_value.y = m_vars.m_vehicle_static_value.y + (m_vars.m_vehicle_relative ? ENTITY::GET_ENTITY_ROTATION(PLAYER::PLAYER_PED_ID(), 2).y : 0.f);
				if (m_vars.m_vehicle_static_toggle.z) m_vars.m_vehicle_hook_value.z = m_vars.m_vehicle_static_value.z + (m_vars.m_vehicle_relative ? ENTITY::GET_ENTITY_ROTATION(PLAYER::PLAYER_PED_ID(), 2).z : 0.f);
			}

			if (m_vars.m_vehicle_spinbot) {
				static float value = 0.f;
				value += m_vars.m_vehicle_spinbot_speed;
				if (value > 360.f) value = (value - 360.f);

				switch (m_vars.m_vehicle_spinbot_axis) {
				case 0:
					m_vars.m_vehicle_hook_value.x = value;
					break;

				case 1:
					m_vars.m_vehicle_hook_value.y = value;
					break;

				case 2:
					m_vars.m_vehicle_hook_value.z = value;
					break;
				}
			}

			if (m_vars.m_vehicle_jitter) {
				static float value = 0.f;
				value += MISC::GET_RANDOM_FLOAT_IN_RANGE(5.f, 100.f);
				if (value > 360.f) value = (value - 360.f);

				switch (m_vars.m_vehicle_jitter_axis) {
				case 0:
					m_vars.m_vehicle_hook_value.x = value;
					break;

				case 1:
					m_vars.m_vehicle_hook_value.y = value;
					break;

				case 2:
					m_vars.m_vehicle_hook_value.z = value;
					break;
				}
			}

			m_vars.m_vehicle_hook_value.x *= 0.0174532924f;
			m_vars.m_vehicle_hook_value.y *= 0.0174532924f;
			m_vars.m_vehicle_hook_value.z *= 0.0174532924f;
		}

		
	}

}