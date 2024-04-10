#include "pch.h"
#include "network_players_teleport.h"
#include "rage/invoker/natives.h"
#include "../network_players.h"
#include "gui/util/panels.h"
#include "network_selected_player.h"
#include "rage/classes/enums.h"
#include "menu/util/control.h"

#include <array>
using namespace base::gui;
using namespace menu::players::selected::teleport::vars;

namespace menu::players::selected::teleport::vars {
	variables m_vars;
	void tp_to_player() {
		Entity handle;
		Vector3 coords = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(players::vars::m_vars.m_selected_player), false);
		PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), false) ? handle = PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID()) : handle = PLAYER::PLAYER_PED_ID();
		ENTITY::SET_ENTITY_COORDS(handle, coords, false, false, false, false);
		if (PED::IS_PED_IN_ANY_VEHICLE(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(players::vars::m_vars.m_selected_player), FALSE)) {
			Vehicle veh = PED::GET_VEHICLE_PED_IS_IN(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(players::vars::m_vars.m_selected_player), false);
			for (int i = -1; i < 16; i++) {
				if (VEHICLE::IS_VEHICLE_SEAT_FREE(veh, i, 0)) {
					PED::SET_PED_INTO_VEHICLE(PLAYER::PLAYER_PED_ID(), veh, i);
					return;
				}
			}
		}
	}

	void tp_to_me() {
		Entity ent = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(players::vars::m_vars.m_selected_player);
		if (ENTITY::IS_ENTITY_DEAD(ent, true)) {
			return;
		}

		if (!PED::IS_PED_IN_ANY_VEHICLE(ent, true)) {
			return;
		}
		ent = PED::GET_VEHICLE_PED_IS_IN(ent, false);
		Vector3 location = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), 0);

		if (control::request_control(ent))
			ENTITY::SET_ENTITY_COORDS(ent, location, 0, 0, 0, 0);
	}

	void tp_into_veh() {
		Entity ent = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(players::vars::m_vars.m_selected_player);
		Vehicle veh = PED::GET_VEHICLE_PED_IS_IN(ent, true);
		if (!ENTITY::IS_ENTITY_A_VEHICLE(veh)) {
			return;
		}
		int seat_index = 255;
		if (VEHICLE::IS_VEHICLE_SEAT_FREE(veh, -1, true))
			seat_index = -1;
		else if (VEHICLE::IS_VEHICLE_SEAT_FREE(veh, -2, true))
			seat_index = -2;
		if (seat_index == 255) {
			return;
		}
		Vector3 location = ENTITY::GET_ENTITY_COORDS(veh, true);
		menu::control::load_ground_at_3dcoord(location);
		Ped ped = PLAYER::PLAYER_PED_ID();
		ENTITY::SET_ENTITY_COORDS(ped, location, 0, 0, 0, 0);
		util::fiber::go_to_main();
		PED::SET_PED_INTO_VEHICLE(ped, veh, seat_index);
	}

	

		void tp_to_apt(Player target, int index) {
			const size_t arg_count = 9;
			int64_t args[arg_count] = {
				(int64_t)eRemoteEvent::Teleport,
				PLAYER::PLAYER_ID(),
				(int64_t)target,
				(int64_t)(int)-1, 1, (int64_t)index, 1, 1, 1
			};

			patterns::trigger_script_event(1, args, arg_count, 1 << target);
		}

		constexpr const static auto apartment_names = std::to_array({
		"INVALID",
		"Eclipse Towers, Apt 31",
		"Eclipse Towers, Apt 9",
		"Eclipse Towers, Apt 40",
		"Eclipse Towers, Apt 5",
		"3 Alta St, Apt 10",
		"3 Alta St, Apt 57",
		"Del Perro Heights, Apt 20",
		"1162 Power St, Apt 3",
		"0605 Spanish Ave, Apt 1",
		"0604 Las Lagunas Blvd, Apt 4",
		"0184 Milton Rd, Apt 13",
		"The Royale, Apt 19",
		"0504 S Mo Milton Dr",
		"0115 Bay City Ave, Apt 45",
		"0325 South Rockford Dr",
		"Dream Tower, Apt 15",
		"2143 Las Lagunas Blvd, Apt 9",
		"1561 San Vitas St, Apt 2",
		"0112 S Rockford Dr, Apt 13",
		"2057 Vespucci Blvd, Apt 1",
		"0069 Cougar Ave, Apt 19",
		"1237 Prosperity St, Apt 21",
		"1115 Blvd Del Perro, Apt 18",
		"0120 Murrieta Heights",
		"Unit 14 Popular St",
		"Unit 2 Popular St",
		"331 Supply St",
		"Unit 1 Olympic Fwy",
		"0754 Roy Lowenstein Blvd",
		"12 Little Bighorn Ave",
		"Unit 124 Popular St",
		"0552 Roy Lowenstein Blvd",
		"0432 Davis Ave",
		"Del Perro Heights, Apt 7",
		"Weazel Plaza, Apt 101",
		"Weazel Plaza, Apt 70",
		"Weazel Plaza, Apt 26",
		"4 Integrity Way, Apt 30",
		"4 Integrity Way, Apt 35",
		"Richards Majestic, Apt 4",
		"Richards Majestic, Apt 51",
		"Tinsel Towers, Apt 45",
		"Tinsel Towers, Apt 29",
		"142 Paleto Blvd",
		"1 Strawberry Ave",
		"1932 Grapeseed Ave",
		"1920 Senora Way",
		"2000 Great Ocean Highway",
		"197 Route 68",
		"870 Route 68 Approach",
		"1200 Route 68",
		"8754 Route 68",
		"1905 Davis Ave",
		"1623 South Shambles St",
		"4531 Dry Dock St",
		"1337 Exceptionalists Way",
		"Unit 76 Greenwich Parkway",
		"Garage Innocence Blvd",
		"634 Blvd Del Perro",
		"0897 Mirror Park Blvd",
		"Eclipse Towers, Apt 3",
		"Del Perro Heights, Apt 4",
		"Richards Majestic, Apt 2",
		"Tinsel Towers, Apt 42",
		"4 Integrity Way, Apt 28",
		"4 Hangman Ave",
		"12 Sustancia Rd",
		"4584 Procopio Dr",
		"4401 Procopio Dr",
		"0232 Paleto Blvd",
		"140 Zancudo Ave",
		"1893 Grapeseed Ave",
		"3655 Wild Oats Drive",
		"2044 North Conker Avenue",
		"2868 Hillcrest Avenue",
		"2862 Hillcrest Avenue",
		"3677 Whispymound Drive",
		"2117 Milton Road",
		"2866 Hillcrest Avenue",
		"2874 Hillcrest Avenue",
		"2113 Mad Wayne Thunder Drive",
		"2045 North Conker Avenue",
		"Eclipse Towers, Penthouse Suite 1",
		"Eclipse Towers, Penthouse Suite 2",
		"Eclipse Towers, Penthouse Suite 3",
		"Private Yacht",
		"Lombank West",
		"Maze Bank West",
		"Arcadius Business Center",
		"Maze Bank Tower",
		"Rancho Clubhouse",
		"Del Perro Beach Clubhouse",
		"Pillbox Hill Clubhouse",
		"Great Chaparral Clubhouse",
		"Paleto Bay Clubhouse",
		"Sandy Shores Clubhouse",
		"La Mesa Clubhouse",
		"Downtown Vinewood Clubhouse",
		"Hawick Clubhouse",
		"Grapeseed Clubhouse",
		"Paleto Bay Clubhouse",
		"Vespucci Beach Clubhouse",
		"Office Garage 1",
		"Office Garage 2",
		"Office Garage 3",
		"Office Garage 1",
		"Office Garage 2",
		"Office Garage 3",
		"Office Garage 1",
		"Office Garage 2",
		"Office Garage 3",
		"Office Garage 1",
		"Office Garage 2",
		"Office Garage 3",
		"Vehicle Warehouse"
			});
	
}

namespace menu {
	void network_players_teleport_menu::render() {
		renderer::addPlayerSubmenu(&players::vars::m_vars.m_selected_player, rage::joaat("selected_player_teleport"), [](pcore* core) {
			player_info_panel(players::vars::m_vars.m_selected_player);

			core->addOption(submenuOption("Apartments")
				.setTarget("player_apartments"));

			core->addOption(buttonOption("Teleport To Player")
				.addClick([] { tp_to_player(); }));

			core->addOption(buttonOption("Teleport Into Vehicle")
				.addClick([] {	tp_into_veh(); }));

			core->addOption(buttonOption("Teleport To Me")
				.addClick([] { tp_to_me(); }));
		});

		renderer::addPlayerSubmenu(&players::vars::m_vars.m_selected_player, rage::joaat("player_apartments"), [](pcore* core) {
			player_info_panel(players::vars::m_vars.m_selected_player);
			static int idx;
			for (int i = 1; i < apartment_names.size(); i++) {
				core->addOption(buttonOption(apartment_names[i])
					.addClick([=] { idx = i;  tp_to_apt(players::vars::m_vars.m_selected_player, idx); }));
			}
			
		});

	}

	void network_players_teleport_menu::update() {
		render();

		
	}
}