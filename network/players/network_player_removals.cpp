#include "pch.h"
#include "network_player_removals.h"
#include "rage/invoker/natives.h"
#include "../network_players.h"
#include "gui/util/panels.h"
#include "network_selected_player.h"
#include "rage/classes/enums.h"
#include "menu/util/control.h"
#include "gui/util/notify.h"
#include <array>
#include "rage/classes/netGameEvent.h"
using namespace base::gui;
using namespace menu::players::selected::removals::vars;

class CRCEvent : public rage::netGameEvent
{
public:
	std::uint16_t m_unk; // 0x30
};

class REPORTMYSELFEVENT : public rage::netGameEvent
{
public:
	std::uint16_t m_unk; // 0x30
};

class CASHSPAWNEVENT : public rage::netGameEvent
{
public:
	std::uint16_t m_unk; // 0x30
};

namespace menu::players::selected::removals::vars {
	variables m_vars;

	void delete_entity(Entity ent) {
		ENTITY::DETACH_ENTITY(ent, 1, 1);
		ENTITY::SET_ENTITY_VISIBLE(ent, false, false);
		NETWORK::NETWORK_SET_ENTITY_ONLY_EXISTS_FOR_PARTICIPANTS(ent, true);
		ENTITY::SET_ENTITY_COORDS_NO_OFFSET(ent, { 0, 0, 0 }, 0, 0, 0);
		ENTITY::SET_ENTITY_AS_MISSION_ENTITY(ent, 1, 1);
		ENTITY::DELETE_ENTITY(&ent);
	}

	void script_crash(Player player) {
		menu::notify::stacked("Initiating player crash");
		std::int64_t Args1[] = { (std::int64_t)eRemoteEvent::Crash, (std::int64_t)PLAYER::PLAYER_ID() };
		patterns::trigger_script_event(1, Args1, sizeof(Args1) / sizeof(Args1[0]), 1 << player);

		std::int64_t Args2[] = { (std::int64_t)eRemoteEvent::Crash2, (std::int64_t)PLAYER::PLAYER_ID(), rand() % INT64_MAX };
		patterns::trigger_script_event(1, Args2, sizeof(Args2) / sizeof(Args2[0]), 1 << player);

		std::int64_t Args3[] = { (std::int64_t)eRemoteEvent::Crash3, (std::int64_t)PLAYER::PLAYER_ID(), rand() % INT64_MAX, rand() % INT64_MAX };
		patterns::trigger_script_event(1, Args3, sizeof(Args3) / sizeof(Args3[0]), 1 << player);

		std::int64_t Args4[] = { (std::int64_t)eRemoteEvent::NotificationCrash1, (std::int64_t)PLAYER::PLAYER_ID() };
		patterns::trigger_script_event(1, Args4, sizeof(Args4) / sizeof(Args4[0]), 1 << player);

		std::int64_t Args5[] = { (std::int64_t)eRemoteEvent::NotificationCrash2, (std::int64_t)PLAYER::PLAYER_ID() };
		patterns::trigger_script_event(1, Args5, sizeof(Args5) / sizeof(Args5[0]), 1 << player);

		menu::notify::stacked("Player crash sent");
	}

	void vehicle_crash(Player player) {
		Ped ped = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(player);
		if (PED::IS_PED_IN_ANY_VEHICLE(ped, 1)) {

			Vehicle vehicle = PED::GET_VEHICLE_PED_IS_IN(ped, FALSE);
			TASK::TASK_VEHICLE_TEMP_ACTION(ped, vehicle, 15, 100);
			TASK::TASK_VEHICLE_TEMP_ACTION(ped, vehicle, 16, 100);
			TASK::TASK_VEHICLE_TEMP_ACTION(ped, vehicle, 17, 100);
			TASK::TASK_VEHICLE_TEMP_ACTION(ped, vehicle, 18, 100);
		}
		else {
			menu::notify::stacked("Player must be in a vehicle");
		}
	}

	void sync_crash(Player player) {
		uint32_t ctx = 0xFF;
		const auto Ped = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(player);
		const auto Coords = ENTITY::GET_ENTITY_COORDS(Ped, TRUE);
		if (!INTERIOR::IS_VALID_INTERIOR(INTERIOR::GET_INTERIOR_FROM_ENTITY(Ped)) && ENTITY::DOES_ENTITY_EXIST(Ped))
		{
			{
				const auto Hash = rage::joaat("adder");
				while (!STREAMING::HAS_MODEL_LOADED(Hash))
				{
					STREAMING::REQUEST_MODEL(Hash);
					util::fiber::go_to_main();
				}
				const auto ModelInfo = patterns::get_model_info(Hash, &ctx);
				if (ModelInfo != NULL)
				{
					*(std::int32_t*)(ModelInfo + 0x340) = 8;
					*(unsigned short*)patterns::set_this_thread_networked = 0x9090; // We restore it so we don't get detected 
					const auto Vehicle = VEHICLE::CREATE_VEHICLE(Hash, Coords, 0.f, TRUE, FALSE, FALSE);
					*(unsigned short*)patterns::set_this_thread_networked = 0x0574; // We restore it so we don't get detected 
					DECORATOR::DECOR_SET_INT(Vehicle, "MPBitset", 0);
					ENTITY::SET_ENTITY_SHOULD_FREEZE_WAITING_ON_COLLISION(Vehicle, TRUE);
					const auto NID = NETWORK::VEH_TO_NET(Vehicle);
					if (NETWORK::NETWORK_GET_ENTITY_IS_NETWORKED(Vehicle))
						NETWORK::SET_NETWORK_ID_EXISTS_ON_ALL_MACHINES(NID, TRUE);
					VEHICLE::SET_VEHICLE_IS_STOLEN(Vehicle, FALSE);
					if (ENTITY::DOES_ENTITY_EXIST(Vehicle))
					{
						ENTITY::SET_ENTITY_VISIBLE(Vehicle, FALSE, NULL);
						VEHICLE::SET_VEHICLE_ON_GROUND_PROPERLY(Vehicle, FALSE);
						VEHICLE::SET_VEHICLE_FORWARD_SPEED(Vehicle, 100.f);
						util::fiber::go_to_main();
						delete_entity(Vehicle);
					}
					*(std::int32_t*)(ModelInfo + 0x340) = 0;
				}

				STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(Hash);
			}
			{
				const auto Hash = rage::joaat("blazer");
				while (!STREAMING::HAS_MODEL_LOADED(Hash))
				{
					STREAMING::REQUEST_MODEL(Hash);
					util::fiber::go_to_main();
				}
				const auto ModelInfo = patterns::get_model_info(Hash, &ctx);
				if (ModelInfo != NULL)
				{
					*(std::int32_t*)(ModelInfo + 0x340) = 11;
					*(unsigned short*)patterns::set_this_thread_networked = 0x9090; // We restore it so we don't get detected 
					const auto Vehicle = VEHICLE::CREATE_VEHICLE(Hash, Coords, 0.f, TRUE, FALSE, FALSE);
					*(unsigned short*)patterns::set_this_thread_networked = 0x0574; // We restore it so we don't get detected 
					DECORATOR::DECOR_SET_INT(Vehicle, "MPBitset", 0);
					ENTITY::SET_ENTITY_SHOULD_FREEZE_WAITING_ON_COLLISION(Vehicle, TRUE);
					const auto NID = NETWORK::VEH_TO_NET(Vehicle);
					if (NETWORK::NETWORK_GET_ENTITY_IS_NETWORKED(Vehicle))
						NETWORK::SET_NETWORK_ID_EXISTS_ON_ALL_MACHINES(NID, TRUE);
					VEHICLE::SET_VEHICLE_IS_STOLEN(Vehicle, FALSE);
					if (ENTITY::DOES_ENTITY_EXIST(Vehicle))
					{
						ENTITY::SET_ENTITY_VISIBLE(Vehicle, FALSE, NULL);
						VEHICLE::SET_VEHICLE_ON_GROUND_PROPERLY(Vehicle, FALSE);
						VEHICLE::SET_VEHICLE_FORWARD_SPEED(Vehicle, 100.f);
						util::fiber::go_to_main();
						delete_entity(Vehicle);
					}
					*(std::int32_t*)(ModelInfo + 0x340) = 3;
				}

				STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(Hash);
			}
			{
				const auto Hash = rage::joaat("dinghy");
				while (!STREAMING::HAS_MODEL_LOADED(Hash))
				{
					STREAMING::REQUEST_MODEL(Hash);
					util::fiber::go_to_main();
				}
				const auto ModelInfo = patterns::get_model_info(Hash, &ctx);
				if (ModelInfo != NULL)
				{
					*(std::int32_t*)(ModelInfo + 0x340) = 1;
					*(unsigned short*)patterns::set_this_thread_networked = 0x9090; // We restore it so we don't get detected 
					const auto Vehicle = VEHICLE::CREATE_VEHICLE(Hash, Coords, 0.f, TRUE, FALSE, FALSE);
					*(unsigned short*)patterns::set_this_thread_networked = 0x0574; // We restore it so we don't get detected 
					DECORATOR::DECOR_SET_INT(Vehicle, "MPBitset", 0);
					ENTITY::SET_ENTITY_SHOULD_FREEZE_WAITING_ON_COLLISION(Vehicle, TRUE);
					const auto NID = NETWORK::VEH_TO_NET(Vehicle);
					if (NETWORK::NETWORK_GET_ENTITY_IS_NETWORKED(Vehicle))
						NETWORK::SET_NETWORK_ID_EXISTS_ON_ALL_MACHINES(NID, TRUE);
					VEHICLE::SET_VEHICLE_IS_STOLEN(Vehicle, FALSE);
					if (ENTITY::DOES_ENTITY_EXIST(Vehicle))
					{
						ENTITY::SET_ENTITY_VISIBLE(Vehicle, FALSE, NULL);
						VEHICLE::SET_VEHICLE_ON_GROUND_PROPERLY(Vehicle, FALSE);
						VEHICLE::SET_VEHICLE_FORWARD_SPEED(Vehicle, 100.f);
						util::fiber::go_to_main();
						delete_entity(Vehicle);
					}
					*(std::int32_t*)(ModelInfo + 0x340) = 13;
				}

				STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(Hash);
			}
		}
	}

	void pickup_crash(Player player) {
		Vector3 entity = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(player), 0);
		const auto Hash = rage::joaat("vw_prop_vw_colle_alien");
		while (!STREAMING::HAS_MODEL_LOADED(Hash)) {
			STREAMING::REQUEST_MODEL(Hash);
			util::fiber::go_to_main();
		}
		OBJECT::CREATE_AMBIENT_PICKUP(0x2C014CA6, { entity.x, entity.y, entity.z + 0.5f }, 0, 2500, Hash, FALSE, TRUE);
		STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(Hash);
	}
	void model_crash(Player player) {
		Player playa = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(player);
		STREAMING::REQUEST_MODEL(0x856cfb02);
		while (!STREAMING::HAS_MODEL_LOADED(0x856cfb02)) SYSTEM::WAIT(0);
		Vector3 fposition = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(playa, { 0.0, 5.0, 0.0 });
		PED::CREATE_PED(1, 0x856cfb02, fposition, ENTITY::GET_ENTITY_HEADING(playa), TRUE, TRUE);
	}

	void host_kick(Player player) {
		if (player != PLAYER::PLAYER_ID())
			NETWORK::NETWORK_SESSION_KICK_PLAYER(player);
	}

	void script_kick(Player player) {
		const size_t arg_count = 15;
		int64_t args[arg_count] = { (int64_t)eRemoteEvent::InteriorControl, (int64_t)PLAYER::PLAYER_PED_ID(), (int64_t)(int)-1 };

		patterns::trigger_script_event(1, args, arg_count, 1 << player);
	}

	void network_bail_kick(Player player) {
		const size_t arg_count = 3;
		int64_t args[arg_count] = { (int64_t)eRemoteEvent::NetworkBail,
			(int64_t)PLAYER::PLAYER_ID(),
			scr_globals::gpbd_fm_3.as<GPBD_FM_3*>()->Entries[patterns::get_net_player(player)->m_player_id].ScriptEventReplayProtectionCounter };

		patterns::trigger_script_event(1, args, arg_count, 1 << player);
	}



}

namespace menu {
	void network_players_removals_menu::render() {
		renderer::addPlayerSubmenu(&players::vars::m_vars.m_selected_player, rage::joaat("selected_player_removals"), [](pcore* core) {
			player_info_panel(players::vars::m_vars.m_selected_player);
			core->addOption(buttonOption("Session host Kick")
				.addClick([] { host_kick(players::vars::m_vars.m_selected_player); }));

			core->addOption(buttonOption("Script Host Kick")
				.addClick([] {
					if (!rage::engine::force_host(RAGE_JOAAT("freemode"))) {
						return;
					}
					if (ENTITY::DOES_ENTITY_EXIST(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(players::vars::m_vars.m_selected_player))) {
						*scr_globals::gsbd_kicking.at(players::vars::m_vars.m_selected_player, 1).as<bool*>() = true;
					}
					}));

			core->addOption(buttonOption("Bail Kick")
				.addClick([] { network_bail_kick(players::vars::m_vars.m_selected_player); }));

			core->addOption(buttonOption("Script Kick")
				.addClick([] { script_kick(players::vars::m_vars.m_selected_player); }));

			core->addOption(buttonOption("Sync Crash")
				.addClick([] { util::fiber::pool::add([] { sync_crash(players::vars::m_vars.m_selected_player); }); }));

			core->addOption(buttonOption("Task Crash")
				.addClick([] { util::fiber::pool::add([] { vehicle_crash(players::vars::m_vars.m_selected_player); }); }));

			core->addOption(buttonOption("Script Crash")
				.addClick([] { util::fiber::pool::add([] { script_crash(players::vars::m_vars.m_selected_player); }); }));

			core->addOption(buttonOption("Model Crash")
				.addClick([] { util::fiber::pool::add([] { model_crash(players::vars::m_vars.m_selected_player); }); }));

			core->addOption(buttonOption("Pickup Crash")
				.addClick([] { util::fiber::pool::add([] { pickup_crash(players::vars::m_vars.m_selected_player); }); })); 

		

			});

	}

	void network_players_removals_menu::update() {
		render();
	}
}