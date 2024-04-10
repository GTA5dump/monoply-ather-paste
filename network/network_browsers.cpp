#include "pch.h"
#include "network_browsers.h"
#include "menu/util/matchmaking_service.h"
#include "util/caller.h"
#include "gui/util/notify.h"
using namespace base::gui;
using namespace menu::network::browsers::vars;

namespace menu::network::browsers::vars {
    variables m_vars;
    static char name_buf[32];
    static char search[64];
    static char session_info[255];
    static int selected_session_idx = -1;

    struct RegionType {
        uint32_t id;
        const char name[22];
    };



    inline const char* regions[] = {
        "CIS", "Africa", "East", "Europe", "China", "Australia", "West", "Japan", "Unknown"
    }; inline int region_id = 0;

    inline const char* langs[] = {
        "English", "French", "German", "Italian", "Spanish ", "Portuguese ", "Polish", "Russian", "Korean", "Chinese", "Japanese", "Spanish (Mexico)", "Chinese (Simpified)"
    }; inline int lang_id = 0;
}


namespace menu {

    void network_browsers_menu::render() {
        renderer::addSubmenu("Browsers", "Network Browsers", [&](core* core) {

            core->addOption(toggleScrollOption<const char*, int>("Region")
                .addToggle(&m_vars.region_filter_enabled).addScroll(&regions).setPosition(&region_id).addClick([=] { m_vars.region_filter = region_id; }));

            core->addOption(toggleScrollOption<const char*, int>("Language")
                .addToggle(&m_vars.language_filter_enabled).addScroll(&langs).setPosition(&lang_id).addClick([=] { m_vars.language_filter = lang_id; }));

            core->addOption(numberOption<int>("Min Players")
                .addNumber(&m_vars.player_count_filter_minimum).addMin(0).addMax(31));

            core->addOption(numberOption<int>("Max Players")
                .addNumber(&m_vars.player_count_filter_maximum).addMin(1).addMax(32));

            core->addOption(buttonOption("Search")
                .addClick([] {
                    util::fiber::pool::add([=] {
                        selected_session_idx = -1;
                        if (!g_matchmaking_service->matchmake())
                            notify::stacked("Failed to find sessions");
                        });
                    }));

            core->addOption(breakOption(std::format("Found {} Sessions", g_matchmaking_service->get_num_found_sessions()).data()));

            if (g_matchmaking_service->get_num_found_sessions()) {
                for (int i = 0; i < g_matchmaking_service->get_num_found_sessions(); i++) {
                    auto& session = g_matchmaking_service->get_found_sessions()[i];

                    if (!session.is_valid)
                        continue;

                    core->addOption(buttonOption(std::to_string(session.info.m_session_token).c_str())
                        .addClick([=] { selected_session_idx = i; caller::call<bool>(patterns::encode_session_info, session_info, 0x7D, nullptr); }));
                }
            }
            else {
                core->addOption(buttonOption("~c~Empty"));
            }
          
        });;

    }

    void network_browsers_menu::update() {
        render();
    }

}