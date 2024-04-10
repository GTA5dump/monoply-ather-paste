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
	class network_spoofing_menu {
	public:
		void render();
		void update();
	};

	static network_spoofing_menu* getNetworkSpoofingMenu() {
		static network_spoofing_menu instance;
		return &instance;
	}

	namespace network::spoofing::vars {
		struct variables {
			bool m_toggle_ping;
			bool m_ped_spinbot;
			bool m_ped_jitter;
			bool m_ped_face_direction;
			bool m_ped_relative;

			int m_ped_face_direction_val;
			float m_ped_spinbot_speed = 10.f;

			math::vector2<float> m_ped_hook_value;

			const char* m_model_name = " ";

			bool m_vehicle_spinbot;
			bool m_vehicle_jitter;
			bool m_vehicle_static;
			bool m_vehicle_relative;

			float m_vehicle_spinbot_speed = 10.f;

			int m_vehicle_spinbot_axis;
			int m_vehicle_jitter_axis;

			math::vector3_<float> m_vehicle_static_value;
			math::vector3_<bool> m_vehicle_static_toggle;
			math::vector3_<float> m_vehicle_hook_value;			

			bool m_movement_animation;

			bool m_spoof_blip;
			int m_blip_type;

			bool m_spoof_state;

			bool m_spoof_lang;
			bool m_spoof_region;

			int player_count = 1;

			bool m_spoof_crew_tag;
			std::string m_crew_tag = { "monopoly" };
			int m_member_id;
			int m_crew_id;
			int m_crew_color;
			int m_crew_members;
			int m_crew_time;

			bool m_spoof_member_id;
			bool m_spoof_crew_id;
			bool m_spoof_crew_color;
			bool m_spoof_crew_members;
			bool m_spoof_crew_time;

			int m_rank;
			int m_kd;

			int x;
			int y;
			int z;

			bool m_dev;
			bool m_tester;
			bool m_cheater;
		};

		extern variables m_vars;
	}
}