#include "pch.h"
#include "network_selected_spawner.h"
#include "rage/invoker/natives.h"
#include "../network_players.h"
#include "gui/util/panels.h"
#include "network_selected_player.h"
#include "rage/classes/enums.h"
#include "menu/util/control.h"
#include <array>
using namespace base::gui;
using namespace menu::players::selected::spawner::vars;

namespace menu::players::selected::spawner::vars {
	variables m_vars;

	float deg_to_rad(float deg) {
		double radian = (3.14159265359 / 180) * deg;
		return (float)radian;
	}

	Vehicle spawn_remote_vehicle(std::uint32_t hash) {
		if (!STREAMING::IS_MODEL_VALID(hash)) {
			return NULL;
		}
		control::requestModel(hash);
		float forward = 5.f;
		Vector3 coords = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(players::vars::m_vars.m_selected_player), false);
		float heading = ENTITY::GET_ENTITY_HEADING(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(players::vars::m_vars.m_selected_player));
		float x = forward * sin(deg_to_rad(heading)) * -1.f;
		float y = forward * cos(deg_to_rad(heading));
		*(unsigned short*)patterns::set_this_thread_networked = 0x9090; // we set this thread as networked so we can spawn the vehicle / u can add rage classes and use excute under thread instead of this
		Vehicle the_vehicle = VEHICLE::CREATE_VEHICLE(hash, { coords.x + x, coords.y + y, coords.z }, heading, NETWORK::NETWORK_IS_SESSION_ACTIVE(), false, false);
		std::int32_t net_id = NETWORK::VEH_TO_NET(the_vehicle);
		*(unsigned short*)patterns::set_this_thread_networked = 0x0574; // We restore it so we don't get detected 
		bool isFlyingVehicle = VEHICLE::IS_THIS_MODEL_A_PLANE(hash) || VEHICLE::IS_THIS_MODEL_A_HELI(hash);
		NETWORK::NETWORK_FADE_IN_ENTITY(the_vehicle, true, 0);
		NETWORK::SET_NETWORK_ID_EXISTS_ON_ALL_MACHINES(net_id, TRUE);
		if (ENTITY::IS_ENTITY_VISIBLE_TO_SCRIPT(NETWORK::NET_TO_VEH(net_id))) {

			float speed = ENTITY::GET_ENTITY_SPEED(the_vehicle);
			VEHICLE::SET_VEHICLE_FORWARD_SPEED(NETWORK::NET_TO_VEH(net_id), speed);
			VEHICLE::SET_VEHICLE_ENGINE_ON(NETWORK::NET_TO_VEH(net_id), TRUE, TRUE, TRUE);
			DECORATOR::DECOR_SET_INT(NETWORK::NET_TO_VEH(net_id), "MPBitset", (1 << 10));
			return the_vehicle;
		}
		return NULL;
	}
}

namespace menu {
	void network_players_spawner_menu::render() {
		renderer::addPlayerSubmenu(&players::vars::m_vars.m_selected_player, rage::joaat("selected_player_spawner"), [](pcore* core) {
			player_info_panel(players::vars::m_vars.m_selected_player);
			
			for (std::int32_t i = 0; i < 23; i++) {
				core->addOption(submenuOption(rage::engine::get_vehicle_class_name(i))
					.addClick([=] { m_vars.m_selected_vehicle_class = i; })
					.setTarget(rage::engine::get_vehicle_class_name(m_vars.m_selected_vehicle_class)));
			}
		});

		renderer::addPlayerSubmenu(&players::vars::m_vars.m_selected_player, rage::joaat(rage::engine::get_vehicle_class_name(m_vars.m_selected_vehicle_class)), [](pcore* core) {
			if (patterns::hash_list != nullptr) {
				for (std::int32_t i = 0; i < patterns::hash_list->capacity; i++) {
					std::uint64_t info = patterns::hash_list->get(i);
					if (info != NULL) {
						if ((*(BYTE*)(info + 157) & 0x1F) == 5) {
							std::string make_ptr = (char*)((uintptr_t)info + 0x2A4);
							std::string model_ptr = (char*)((uintptr_t)info + 0x298);
							if (VEHICLE::GET_VEHICLE_CLASS_FROM_NAME(rage::joaat(model_ptr)) == m_vars.m_selected_vehicle_class) {
								std::stringstream ss;
								std::string make(make_ptr);
								std::string model(model_ptr);
								if (make[0] || model[0]) {
									make = HUD::GET_FILENAME_FOR_AUDIO_CONVERSATION(make.c_str());
									model = HUD::GET_FILENAME_FOR_AUDIO_CONVERSATION(model.c_str());
									if (model != "NULL") {
										ss << model;
									}
									else {
										ss << "Unknown";
									}
								}
								core->addOption(vehicleOption((ss.str().c_str()))
									.addClick([=] { spawn_remote_vehicle(*(std::uint32_t*)(info + 0x18)); }));
							}
						}
					}
				}
			}
		});
	}

	void network_players_spawner_menu::update() {
		render();


	}
}