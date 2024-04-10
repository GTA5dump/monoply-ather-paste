#pragma once
#include "gui/options/core.h"
#include "gui/options/submenu.h"
#include "gui/options/option.h"
#include "gui/options/button.h"
#include "gui/options/submenu_option.h"
#include "gui/options/number.h"
#include "gui/options/toggle.h"
#include "gui/options/scroll.h"
#include "menu/util/players.h"

namespace menu {
	class network_players_abusive_menu {
	public:
		void render();
		void update();
	};

	static network_players_abusive_menu* getNetworkPlayersAbusiveMenu() {
		static network_players_abusive_menu instance;
		return &instance;
	}

	namespace players::selected::abusive::vars {
		struct variables {
			bool m_freeze;
			bool m_explode;
			bool m_kick_from_vehicle;
			bool m_set_on_fire;
			bool m_taze;
			bool m_ragdoll;
			bool m_glitch_physics;
			bool m_blame;
			bool m_rain_rockets;
			bool m_shake_cam;
			bool m_clone;
			bool m_vehicle_ram;
			bool m_always_wanted;
			bool m_hostile_peds;

			int m_kick_from_vehicle_timer;
			int m_taze_timer;
			int m_ragdoll_timer;
			int m_glitch_physics_timer;
			int m_rain_rockets_timer;
			int m_always_wanted_timer;
			int m_set_on_fire_timer;
			int m_hostile_peds_timer;

			bool m_sound_spam;

			bool g_particle_man;
			float g_particle_man_scale{ 0.2 };

			bool g_explosion_loop{ false };
			bool g_visible{ false };
			float g_camera_shake{ 1.0f };
			int g_radius{ 100 };
			bool g_audible{ true };

			bool m_deny_vehicles;
			bool m_deny_planes;
			bool m_deny_helis;
			bool m_deny_boats;
			bool m_deny_aiming;
			bool m_deny_shooting;
			bool m_deny_reload;
			bool m_deny_cover;
			bool m_deny_weapon_switch;
			bool m_deny_skydiving;
			bool m_deny_jumping;
			bool m_deny_swimming;
			bool m_deny_sprinting;
			bool m_deny_walking;
			bool m_deny_ragdolling;
			bool m_deny_climbing;
		};

		extern variables m_vars;
	}
}