#include "pch.h"
#include "network_players_vehicle.h"
#include "rage/invoker/natives.h"
#include "../network_players.h"
#include "gui/util/panels.h"
#include "network_selected_player.h"
#include "rage/classes/enums.h"
#include "menu/util/control.h"

#include <array>
using namespace base::gui;
using namespace menu::players::selected::vehicle::vars;

namespace menu::players::selected::vehicle::vars {
	variables m_vars;

}

namespace menu {
	void network_players_vehicle_menu::render() {
		renderer::addPlayerSubmenu(&players::vars::m_vars.m_selected_player, rage::joaat("selected_player_vehicle"), [](pcore* core) {
			player_info_panel(players::vars::m_vars.m_selected_player);
			core->addOption(toggleOption("Godmode")
				.addToggle(&m_vars.m_godmode));

			core->addOption(toggleOption("Invisible")
				.addToggle(&m_vars.m_invisible));

			core->addOption(toggleOption("Disable Collission")
				.addToggle(&m_vars.m_disable_collision));

			core->addOption(toggleOption("Reduce Grip")
				.addToggle(&m_vars.m_slippy_wheels));

			core->addOption(toggleOption("Freeze")
				.addToggle(&m_vars.m_freeze_vehicle));

			core->addOption(buttonOption("Repair Vehicle")
				.addClick([] {
					Vehicle veh = PED::GET_VEHICLE_PED_IS_IN(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(players::vars::m_vars.m_selected_player), false);
					VEHICLE::SET_VEHICLE_FIXED(veh);
					VEHICLE::SET_VEHICLE_DEFORMATION_FIXED(veh);
					VEHICLE::SET_VEHICLE_DIRT_LEVEL(veh, 0); }));

			core->addOption(buttonOption("Wash Vehicle")
				.addClick([] {
					Vehicle veh = PED::GET_VEHICLE_PED_IS_IN(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(players::vars::m_vars.m_selected_player), false);
					VEHICLE::SET_VEHICLE_DIRT_LEVEL(veh, 0); }));

			core->addOption(buttonOption("Upgrade Vehicle")
				.addClick([] {
					Vehicle veh = PED::GET_VEHICLE_PED_IS_IN(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(players::vars::m_vars.m_selected_player), false);
					VEHICLE::SET_VEHICLE_MOD_KIT(veh, 0);
					for (int i = 0; i < 50; i++)
					{
						VEHICLE::SET_VEHICLE_MOD(veh, i, VEHICLE::GET_NUM_VEHICLE_MODS(veh, i) - 1, false);
					} }));

			core->addOption(buttonOption("Downgrade Vehicle")
				.addClick([] {
					Vehicle veh = PED::GET_VEHICLE_PED_IS_IN(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(players::vars::m_vars.m_selected_player), false);
					for (int i = 0; i < 50; i++)
					{
						VEHICLE::REMOVE_VEHICLE_MOD(veh, i);
					} }));

			core->addOption(buttonOption("Destroy Engine")
				.addClick([] {
					Vehicle veh = PED::GET_VEHICLE_PED_IS_IN(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(players::vars::m_vars.m_selected_player), false);
					VEHICLE::SET_VEHICLE_ENGINE_HEALTH(veh, 0); }));

			core->addOption(buttonOption("Burst Tires")
				.addClick([] {
					Vehicle veh = PED::GET_VEHICLE_PED_IS_IN(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(players::vars::m_vars.m_selected_player), false);
					VEHICLE::SET_VEHICLE_TYRES_CAN_BURST(veh, true);
					for (int i = 5; i--;)
						VEHICLE::SET_VEHICLE_TYRE_BURST(veh, i, true, 1000.f);
					}));


			core->addOption(buttonOption("Lock Doors")
				.addClick([] {
					Vehicle veh = PED::GET_VEHICLE_PED_IS_IN(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(players::vars::m_vars.m_selected_player), false);
					VEHICLE::SET_VEHICLE_DOORS_LOCKED(veh, true);
					}));

			core->addOption(buttonOption("Child Lock Doors")
				.addClick([] {
					Vehicle veh = PED::GET_VEHICLE_PED_IS_IN(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(players::vars::m_vars.m_selected_player), false);
					VEHICLE::SET_VEHICLE_DOORS_LOCKED_FOR_PLAYER(veh, PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(players::vars::m_vars.m_selected_player), true);
					}));

			core->addOption(buttonOption("Unlock All Doors")
				.addClick([] {
					Vehicle veh = PED::GET_VEHICLE_PED_IS_IN(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(players::vars::m_vars.m_selected_player), false);
					VEHICLE::SET_VEHICLE_DOORS_LOCKED_FOR_PLAYER(veh, PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(players::vars::m_vars.m_selected_player), false);
					}));

			core->addOption(buttonOption("gift car ")
				.addClick([] {
					Ped PedHandle = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(players::vars::m_vars.m_selected_player);
					Vehicle vehicle = PED::GET_VEHICLE_PED_IS_IN(PedHandle, false);
					control::request_control(vehicle);
					ENTITY::SET_ENTITY_AS_MISSION_ENTITY(vehicle, TRUE, TRUE);
					DECORATOR::DECOR_REGISTER("PV_Slot", 3);
					DECORATOR::DECOR_REGISTER("Player_Vehicle", 3);
					DECORATOR::DECOR_SET_BOOL(vehicle, "IgnoredByQuickSave", FALSE);
					DECORATOR::DECOR_SET_INT(vehicle, "Player_Vehicle", NETWORK::NETWORK_HASH_FROM_PLAYER_HANDLE(players::vars::m_vars.m_selected_player));
					VEHICLE::SET_VEHICLE_IS_STOLEN(vehicle, FALSE);
					}));
		
		});

	}

	void network_players_vehicle_menu::update() {
		render();


	}
}