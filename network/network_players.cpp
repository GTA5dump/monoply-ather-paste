#include "pch.h"
#include "network_players.h"
#include "rage/invoker/natives.h"
#include "players/network_selected_player.h"
#include "menu/util/players.h"
#include "gui/util/timer.h"
#include "menu/util/globals.h"
using namespace menu::renderer;





bool if_your_gay_look_at_this = true;
float big_tits_man_with_smalldog_as_his_dick = 2.05f;


using namespace base::gui;
using namespace menu::players::vars;

namespace menu::players::vars {
	variables m_vars;
}

const char* sort_types[] = {
	"Player ID", "Alphabetical"
}; std::size_t sort_types_id = 0;

namespace menu {

	void network_players_menu::render() {
		renderer::addSubmenu("Players", "Network Players", [](core* core) {
			if (*patterns::is_session_started) {
                 #define SCORE_BOARD_HEADSHOT_GLOBAL 1680805 + 2
				const char* GPic = "CHAR_MULTIPLAYER";
				const int HeadIndex = SCORE_BOARD_HEADSHOT_GLOBAL;
			
				/*core->addOption(toggleNumberOption<float>("Run Speed")
					.addToggle(&if_your_gay_look_at_this).addNumber(&big_tits_man_with_smalldog_as_his_dick)
					.addMin(-100.0f).addMax(100.f).addStep(0.03f).setPrecision(2));*/

				core->addOption(scrollOption<const char*, std::size_t>("Sort Players")
					.addScroll(&sort_types).setPosition(&sort_types_id));
				
				core->addOption(breakOption("Players"));

				for (uint32_t i = 0; i < 32; i++) {
					if (auto ped = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(i)) {
						if (ENTITY::DOES_ENTITY_EXIST(ped)) {
							std::string name = PLAYER::GET_PLAYER_NAME(i);

							auto plyr = patterns::get_net_player(i);
							
							for (int i = 0; i <= 150; i += 5)
							{
								uint64_t* base = script_global(HeadIndex + i).as<uint64_t*>();
								int playerId = static_cast<int>(*base);

								if (playerId == -1)
								{
									break;  // Stop iterating if playerId is -1
								}

								uint64_t* logo = script_global(HeadIndex + i + 1).as<uint64_t*>();
								int logos = static_cast<int>(*logo);
								GPic = reinterpret_cast<const char*>(PED::GET_PEDHEADSHOT_TXD_STRING(logos));

								auto size = render::get_sprite_scale(0.032);
								auto your_mom_is_a_hoe = menu::renderer::getRenderer();
								/*render::draw_sprite({ GPic, GPic }, { your_mom_is_a_hoe->m_position.x + (your_mom_is_a_hoe->m_width / 17.14f), your_mom_is_a_hoe->m_draw_base_y + (your_mom_is_a_hoe->m_option.m_height / 0.13f) - (render::get_text_height(2, 0.4) / 1.725f), }, { size.x, size.y }, { 255, 255, 255, 255 }, 0.f);*/

								// You might want to add a delay or update logic here to avoid displaying multiple headshots in the same frame.
							}


					

							if (plyr->IsNetworkHost()) {
								name.append(" ~p~[H]");
							}

							if (plyr->m_is_spectating) {
								name.append(" ~p~[S]");
							}

							if (plyr->m_is_cheater) {
								name.append(" ~p~[M]");
							}

							if (i == PLAYER::PLAYER_ID())
								name.append(" ~p~[ME]");

							if (i == NETWORK::NETWORK_GET_HOST_OF_SCRIPT("Freemode", 4294967295, 0))
								name.append(" ~p~[SH]");
							

							core->addOption(submenuOption(name.c_str())
								.addClick([=] { m_vars.m_selected_player = i; })
								.setTarget("selected_player"));
						}
					}
				}
			}
			
			
		});
	}


	

	void network_players_menu::update() {
		render();
		getNetworkSelectedPlayerMenu()->update();
	}

}