#include "pch.h"
#include "network_players_friendly.h"
#include "rage/invoker/natives.h"
#include "../network_players.h"
#include "gui/util/panels.h"
#include "network_selected_player.h"
#include "rage/classes/enums.h"

using namespace base::gui;
using namespace menu::players::selected::friendly::vars;

namespace menu::players::selected::friendly::vars {
	variables m_vars;

	inline const char* commend[] = {
	   "Friendly", "Helpful"
	};
	inline const char* commend_stat[] = {
		"MPPLY_FRIENDLY", "MPPLY_HELPFUL"
	}; inline std::size_t commend_id = 0;

	inline const char* drop_pickup[] = {
		"Health", "Armor", "Money", "Parachute", "RPG"
	}; inline std::size_t drop_pickup_id = 0;

	inline const char* message_color[] = {
		"~s~White", "~u~Black", "~r~Red", "~g~Green", "~b~Blue", "~p~Purple", "~y~Yellow", "~o~Orange", "~c~Grey", "~h~Bold"
	}; inline std::size_t message_color_id = 1;

	void copy_outfit(Player player) {
		for (int i = 0; i < 12; i++) {
			PED::SET_PED_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID(), i, PED::GET_PED_DRAWABLE_VARIATION(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(player), i), PED::GET_PED_TEXTURE_VARIATION(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(player), i), PED::GET_PED_PALETTE_VARIATION(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(player), i));
			for (int i = 0; i < 2; i++) {
				util::fiber::go_to_main();
			}
		}
	}

	void clear_wanted_player(Player target)
	{
		constexpr size_t arg_count = 3;
		int64_t args[arg_count] = {
			static_cast<int64_t>(eRemoteEvent::ClearWantedLevel),
			PLAYER::PLAYER_ID(),
			*menu::scr_globals::gpbd_fm_3.at(target, menu::scr_globals::size::gpbd_fm_3).at(510).as<int*>()
		};

		patterns::trigger_script_event(1, args, arg_count, 1 << target);
	}

	void give_remote_otr(Player target)
	{
		constexpr size_t arg_count = 7;
		int64_t args[arg_count] = {
			static_cast<int64_t>(eRemoteEvent::RemoteOffradar),
			(int64_t)PLAYER::PLAYER_ID(),
			(int64_t)(NETWORK::GET_NETWORK_TIME() + 1),
			0,
			true,
			false,
			* menu::scr_globals::gpbd_fm_3.at(target, menu::scr_globals::size::gpbd_fm_3).at(510).as<int64_t*>()
		};

		patterns::trigger_script_event(1, args, arg_count, 1 << target);
	}

	void give_all_weapons(Player player) {
		int Weapons[] = { 0x92A27487, 0x958A4A8F, 0xF9E6AA4B, 0x84BD7BFD, 0x8BB05FD7, 0x440E4788, 0x4E875F73, 0xF9DCBF2D, 0xD8DF3C3C, 0x99B507EA, 0xDD5DF8D9, 0xDFE37640, 0x678B81B1, 0x19044EE0, 0xCD274149, 0x94117305, 0x3813FC08, 0x1B06D571,  0xBFE256D4, 0x5EF9FEC4, 0x22D8FE39, 0x3656C8C1, 0x99AEEB3B, 0xBFD21232, 0x88374054 ,0xD205520E, 0x83839C4, 0x47757124, 0xDC4DB296, 0xC1B3C3D1, 0xCB96392F, 0x97EA20B8, 0xAF3696A1, 0x2B5EF5EC, 0x917F6C8C, 0x57A4368C, 0x45CD9CF3, 0x13532244, 0x2BE6766B, 0x78A97CD0, 0xEFE7E2DF, 0x0A3D4D34, 0xDB1AA450, 0xBD248B55, 0x476BF155, 0x1D073A89, 0x555AF99A, 0x7846A318, 0xE284C527, 0x9D61E50F, 0xA89CB99E, 0x3AABBBAA, 0xEF951FBB, 0x12E82D3D, 0x5A96BA4, 0xBFEFFF6D, 0x394F415C, 0x83BF0278, 0xFAD1F1C9, 0xAF113F99, 0xC0A3098D, 0x969C3D67, 0x7F229F94, 0x84D6FAFD, 0x624FE830, 0x9D1F17E6, 0xC78D71B4, 0x9D07F764, 0x7FD62962, 0xDBBD7280, 0x61012683, 0x05FC3C11, 0x0C472FE2, 0xA914799, 0xC734385A, 0x6A6C02E0, 0xB1CA77B1, 0xA284510B, 0x4DD2DC56, 0x42BF8A85, 0x7F7497E5, 0x6D544C99, 0x63AB0442, 0x0781FE4A, 0xB62D1F67, 0xDB26713A, 0x93E220BD, 0xA0973D5E, 0x24B17070, 0x2C3731D9, 0xAB564B93, 0x787F0BB, 0xBA45E8B8, 0x23C9F95C, 0xFDBC8A50, 0x497FACC3, 0x34A67B97, 0xFBAB5776, 0x060EC506, 0xBA536372, 0x184140A1 };
		for (int i = 0; i < (sizeof(Weapons) / 4); i++) {
			if (!WEAPON::HAS_PED_GOT_WEAPON(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(player), Weapons[i], false))
				WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(player), Weapons[i], 9999, 1);

		}
		if (!WEAPON::HAS_PED_GOT_WEAPON(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(player), rage::joaat("WEAPON_TACTICALRIFLE"), false))
			WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(player), rage::joaat("WEAPON_TACTICALRIFLE"), 9999, 1);
	}

	void give_all_attachments(Player player) {
		std::vector<Hash> pistolMk2{ 1329061674, 2396306288, 1140676955, 1709866683, 568543123, 2467084625 };
		std::vector<Hash> smgMk2{ 190476639, 2076495324, 2681951826, 3842157419, 1038927834, 1303784126, 2774849419, 966612367 };
		std::vector<Hash> assltRifleMk2{ 1675665560, 2640679034, 2076495324, 1108334355, 77277509, 3328927042, 2805810788, 1303784126, 1447477866, 3115408816 };
		std::vector<Hash> carbineRifleMk2{ 0x420FD713, 0x7BC4CDDC, 0x9D65907A, 0x837445AA, 0xE50C424D };
		std::vector<Hash> combatMgMk2{ 1475288264, 2640679034, 1108334355, 1060929921, 3328927042, 1303784126, 3051509595, 3607349581 };
		std::vector<Hash> heavySniperMk2{ 2313935527, 2193687427, 3159677559, 3061846192, 776198721, 2890063729, 1764221345, 277524638, 1815270123 };
		std::vector<Hash> WeaponHash{ 0xBFE256D4, 0x78A97CD0, 0xDBBD7280, 0x394F415C, 0xFAD1F1C9, 0xA914799 };
		//now lets give them
		for (Hash hash : pistolMk2)
			WEAPON::GIVE_WEAPON_COMPONENT_TO_PED(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(player), rage::joaat("WEAPON_PISTOL_MK2"), hash);

		for (Hash hash : smgMk2)
			WEAPON::GIVE_WEAPON_COMPONENT_TO_PED(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(player), rage::joaat("WEAPON_SMG_MK2"), hash);

		for (Hash hash : assltRifleMk2)
			WEAPON::GIVE_WEAPON_COMPONENT_TO_PED(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(player), rage::joaat("WEAPON_ASSAULTRIFLE_MK2"), hash);

		for (Hash hash : carbineRifleMk2)
			WEAPON::GIVE_WEAPON_COMPONENT_TO_PED(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(player), rage::joaat("WEAPON_CARBINERIFLE_MK2"), hash);

		for (Hash hash : combatMgMk2)
			WEAPON::GIVE_WEAPON_COMPONENT_TO_PED(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(player), rage::joaat("WEAPON_COMBATMG_MK2"), hash);

		for (Hash hash : heavySniperMk2)
			WEAPON::GIVE_WEAPON_COMPONENT_TO_PED(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(player), rage::joaat("WEAPON_HEAVYSNIPER_MK2"), hash);

		for (Hash hash : WeaponHash)
			WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(player), hash, 9999, 0);
	}

}

namespace menu {
	void network_players_friendly_menu::render() {
		renderer::addPlayerSubmenu(&players::vars::m_vars.m_selected_player, rage::joaat("selected_player_friendly"), [](pcore* core) {
			player_info_panel(players::vars::m_vars.m_selected_player);

			core->addOption(toggleOption("Demi Godmode")
				.addToggle(&m_vars.m_demi_godmode));

			core->addOption(toggleOption("Disable Police")
				.addToggle(&m_vars.m_disable_police));

			core->addOption(toggleOption("Off The Radar")
				.addToggle(&m_vars.m_off_radar));

			core->addOption(toggleOption("Kill Killers")
				.addToggle(&m_vars.m_kill_killers));

			core->addOption(toggleOption("Force Visible")
				.addToggle(&m_vars.m_force_visible));

			core->addOption(toggleOption("Weapon Rain")
				.addToggle(&m_vars.m_weapon_rain));

			core->addOption(toggleOption("Set Waypoint")
				.addToggle(&m_vars.m_set_waypoint));

			core->addOption(buttonOption("Copy Outfit")
				.addClick([] { copy_outfit(players::vars::m_vars.m_selected_player); }));

			core->addOption(scrollOption<const char*, std::size_t>("Drop Pickup")
				.addScroll(&drop_pickup).setPosition(&drop_pickup_id)
				.addClick([] { switch (drop_pickup_id) {
				if (*patterns::is_session_started) {
					case 0: patterns::give_pickup_rewards(players::vars::m_vars.m_selected_player, REWARD_HEALTH); break;
					case 1: patterns::give_pickup_rewards(players::vars::m_vars.m_selected_player, REWARD_ARMOUR); break;
					case 2: patterns::give_pickup_rewards(players::vars::m_vars.m_selected_player, REWARD_MONEY_VARIABLE); break;
					case 3: patterns::give_pickup_rewards(players::vars::m_vars.m_selected_player, REWARD_PARACHUTE); break;
					case 4: patterns::give_pickup_rewards(players::vars::m_vars.m_selected_player, REWARD_WEAPON_RPG); break;
				}
			}}));

			core->addOption(scrollOption<const char*, std::size_t>("Commend")
				.addScroll(&commend).setPosition(&commend_id)
				.addClick([] { patterns::remote_increment_stat(*(Hash*)commend_stat[commend_id], 100, patterns::get_net_player(players::vars::m_vars.m_selected_player)); }));

			core->addOption(buttonOption("Give All Weapons")
				.addClick([] { give_all_weapons(players::vars::m_vars.m_selected_player); }));

			core->addOption(buttonOption("Give All Attachments")
				.addClick([] { give_all_attachments(players::vars::m_vars.m_selected_player); }));
		});

	}

	void network_players_friendly_menu::update() {
		render();

		static int offRadarPlayer = 0;
		static int neverWantedPlayer = 0;
		if (m_vars.m_demi_godmode) {
			if (*patterns::is_session_started) {
				patterns::give_pickup_rewards(players::vars::m_vars.m_selected_player, REWARD_HEALTH);
				patterns::give_pickup_rewards(players::vars::m_vars.m_selected_player, REWARD_ARMOUR);
			}
		}
		if (m_vars.m_disable_police) {
			clear_wanted_player(players::vars::m_vars.m_selected_player);
		}
		if (m_vars.m_off_radar) {
			give_remote_otr(players::vars::m_vars.m_selected_player);
		}
		if (m_vars.m_kill_killers) {
			Entity killer = PED::GET_PED_SOURCE_OF_DEATH(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(players::vars::m_vars.m_selected_player));
			if (killer) {
				if (PED::IS_PED_A_PLAYER(killer)) {
					if (!ENTITY::IS_ENTITY_DEAD(killer, 1)) {
						Vector3 coords = ENTITY::GET_ENTITY_COORDS(killer, true);
						FIRE::ADD_EXPLOSION(coords, 1, 100.f, true, false, 0.f, false);
					}
				}
			}
		}
		if (m_vars.m_force_visible) {
			if (!ENTITY::IS_ENTITY_VISIBLE(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(players::vars::m_vars.m_selected_player))) {
				NETWORK::SET_ENTITY_LOCALLY_VISIBLE(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(players::vars::m_vars.m_selected_player));
				ENTITY::SET_ENTITY_VISIBLE(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(players::vars::m_vars.m_selected_player), true, true);
			}
		}

		static int m_money_delay;
		if (m_vars.m_weapon_rain) {
			util::fiber::pool::add([=] {
				Player p = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(players::vars::m_vars.m_selected_player);
				if ((timeGetTime() - m_money_delay) > 300) // Time between drops
				{
					float dx = ENTITY::GET_ENTITY_COORDS(p, 1).x;
					float dy = ENTITY::GET_ENTITY_COORDS(p, 1).y;
					float dz = ENTITY::GET_ENTITY_COORDS(p, 1).z;
					float randX = MISC::GET_RANDOM_INT_IN_RANGE(-2, 2);
					float randY = MISC::GET_RANDOM_INT_IN_RANGE(-2, 2);
					float randZ = MISC::GET_RANDOM_INT_IN_RANGE(-2, 2);
					float offsetX = dx + randX;
					float offsetY = dy + randY;
					float offsetZ = dz + randZ;
					STREAMING::REQUEST_MODEL(REWARD_WEAPON_MG);
					if (!STREAMING::HAS_MODEL_LOADED(REWARD_WEAPON_MG)) {
						util::fiber::go_to_main();
					}
					else {
						//*g_pointers->should_sync_money_rewards = true;
						*(unsigned short*)patterns::set_this_thread_networked = 0x9090;
						OBJECT::CREATE_AMBIENT_PICKUP(REWARD_WEAPON_MG, { ENTITY::GET_ENTITY_COORDS(p, 1).x, ENTITY::GET_ENTITY_COORDS(p, 1).y, ENTITY::GET_ENTITY_COORDS(p, 1).z + 30 }, 1, 2500, REWARD_WEAPON_MG, false, true);
						*(unsigned short*)patterns::set_this_thread_networked = 0x0574;
						//*g_pointers->should_sync_money_rewards = false;
						STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(REWARD_WEAPON_MG);

						m_money_delay = timeGetTime();
					}
				}
				});
		}
		if (m_vars.m_set_waypoint) {
			Player p = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(players::vars::m_vars.m_selected_player);
			HUD::SET_NEW_WAYPOINT({ ENTITY::GET_ENTITY_COORDS(p, 1).x, ENTITY::GET_ENTITY_COORDS(p, 1).y } );
		}
	}
}