#include "pch.h"
#include "network_players_abusive.h"
#include "rage/invoker/natives.h"
#include "../network_players.h"
#include "gui/util/panels.h"
#include "network_selected_player.h"
#include "rage/classes/enums.h"
#include "gui/util/timer.h"
#include "menu/util/control.h"

using namespace base::gui;
using namespace menu::players::selected::abusive::vars;

namespace menu::players::selected::abusive::vars {
	variables m_vars;

	auto& selectedPlayer = players::vars::m_vars.m_selected_player;

    const char* stats[] = {
       "Griefing", "Game Exploits", "Exploits", "Punished", "Cheat", "Bad Sport", "Bad Crew Name", "Bad Crew Motto", "Bad Crew Status", "Bad Crew Emblem"
    }; inline std::size_t stats_id;

    const char* SparticleDict[12] = {
        "scr_clown_appears", "scr_clown_death", "scr_exp_clown", "scr_indep_firework_starburst", "scr_indep_firework_fountain", "scr_indep_firework_shotburst", "scr_firework_xmas_spiral_burst_rgw", "scr_alien_teleport", "scr_alien_disintegrate", "scr_fbi_mop_drips", "scr_clown_bul", "scr_rcpap1_camera"
    }; inline std::size_t Sparticle_id = 0;

    const char* Sparticles[12] = {
        "Purple Cloud 1", "Purple Cloud 2", "Purple Cloud 3", "Firework Starburst", "Firework Fountain", "Firework Shotburst", "Xmas Firework Burst", "Teleport", "Disintegration", "Drip", "Flower Jet", "Flash"
    };


    int explosion_ids[39] = {
        1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39
    }; inline int explosion_id = 0;

    const char* explosion_type[] = {
   "Grenade", "Grenade Launcher", "Sticky Bomb", "Molotov", "Tank Shell", "Octane", "Car", "Plane", "Petrol Pump", "Bike",
   "Steam", "Flame", "Water", "Gas", "Boat", "Ship", "Truck", "Bullet", "Smoke Launcher", "Smoke Grenade", "BZ Gas", "Flae",
   "Gas Can", "Extinguisher", "Programmable Ar", "Train", "Barrel", "Propane", "Blimp", "Flame Explosion", "Tanker", "Rocket",
   "Vehicle Bullet", "Gas Tank", "Bird Crap", "Snow Ball"
    };

    bool is_ped_shooting(Ped ped) {
        Vector3 coords = ENTITY::GET_ENTITY_COORDS(ped, 1);
        return PED::IS_PED_SHOOTING_IN_AREA(ped, coords, coords, true, true);
    }

    void kick_from_vehicle(Player player) {
        Ped vic = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(player);
        control::request_control(vic);
        patterns::clear_ped_tasks_network(patterns::get_net_player(player)->m_player_info->m_ped, true);
    }
    void taze(Player player) {
        int currentAttempt = 0;
        int maxAttempts = 20;
        while (!ENTITY::IS_ENTITY_DEAD(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(player), 0))
        {
            if (currentAttempt >= maxAttempts)
            {
                break;
            }
            else
            {
                Vector3 destination = PED::GET_PED_BONE_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(player), 0x0, { 0.0f, 0.0f, 0.0f });
                Vector3 origin = PED::GET_PED_BONE_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(player), 0xDEAD, { 0.0f, 0.0f, 0.2f});
                Hash tazerHash = MISC::GET_HASH_KEY("WEAPON_STUNGUN");
                MISC::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(origin, destination, 1, 0, tazerHash, PLAYER::PLAYER_PED_ID(), false, false, 1);
                currentAttempt++;
            }
            util::fiber::go_to_main();
        }
    }
    void ragdoll(Player player) {
        if (auto ped = patterns::get_net_player(player)->m_player_info->m_ped)
            if (auto net_object = ped->m_net_object)
                patterns::request_ragdoll(net_object->m_object_id);
    }
    constexpr size_t patch_size = 24;
    static inline std::once_flag once_flag;
    static inline std::array<byte, patch_size> backup;
    static inline void setup_backup()
    {
        memcpy(backup.data(), patterns::world_model_spawn_bypass, patch_size);
    }
    inline Object SpawnObject(Hash hash, float x, float y, float z, bool is_networked = true)
    {
        STREAMING::REQUEST_MODEL(hash);
        for (int i = 0; i < 100 && !STREAMING::HAS_MODEL_LOADED(hash); i++)
        {
            util::fiber::go_to_main();
        }
        if (!STREAMING::HAS_MODEL_LOADED(hash))
        {

            return 0;
        }

        std::call_once(once_flag, setup_backup);
        memset(patterns::world_model_spawn_bypass, 0x90, patch_size);

        const auto object = OBJECT::CREATE_OBJECT(hash, { x, y, z }, is_networked, false, false);

        memcpy(patterns::world_model_spawn_bypass, backup.data(), patch_size);

        STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(hash);

        return object;
    }
    void glitch_physics(Player player) {

        uint32_t model = 0xd5c0bc07;
        Object guitar = SpawnObject(model, ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(player), 1).x, ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(player), 1).y, ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(player), 1).z, true);
        if (ENTITY::DOES_ENTITY_EXIST(guitar)) {
            ENTITY::SET_ENTITY_VISIBLE(guitar, false, false);
            ENTITY::ATTACH_ENTITY_TO_ENTITY(guitar, PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(player), PED::GET_PED_BONE_INDEX(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(player), 23553), { 0.5f, -0.2f, 0.f }, { 0.f, 75.0f, 0.f }, true, true, false, true, 1, true, 0);
            ENTITY::SET_ENTITY_COLLISION(guitar, true, true);
        }
    }
    void rain_rockets(Player player) {
        int currentAttempt = 0;
        int maxAttempts = 20;
        while (!ENTITY::IS_ENTITY_DEAD(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(player), 0))
        {
            if (currentAttempt >= maxAttempts)
            {
                break;
            }
            else
            {
                uint32_t model = 0x13579279;
                Vector3 start_position = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(player), 1);
                start_position.x += MISC::GET_RANDOM_FLOAT_IN_RANGE(-20.f, 20.f);
                start_position.y += MISC::GET_RANDOM_FLOAT_IN_RANGE(-20.f, 20.f);
                Hash tazerHash = MISC::GET_HASH_KEY("WEAPON_STUNGUN");
                MISC::SHOOT_SINGLE_BULLET_BETWEEN_COORDS_IGNORE_ENTITY({ start_position.x, start_position.y, start_position.z + MISC::GET_RANDOM_FLOAT_IN_RANGE(50.f, 150.f) }, { start_position.x, start_position.y, start_position.z }, 250, 0, model, PLAYER::PLAYER_PED_ID(), true, false, 1000.f, PLAYER::PLAYER_PED_ID(), 0);
                currentAttempt++;
            }
            util::fiber::go_to_main();
        }
    }
    void set_wanted_level(Player player, int level) {
        PLAYER::REPORT_CRIME(patterns::get_net_player(player)->m_player_id, 8, PLAYER::GET_WANTED_LEVEL_THRESHOLD(level));
    }
    void set_on_fire(Player player) {
        Player fireloopedplayer = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(player);
        Vector3 Pos = ENTITY::GET_ENTITY_COORDS(fireloopedplayer, true);
        FIRE::ADD_EXPLOSION({Pos.x, Pos.y, Pos.z - 2}, 12, 5, true, 0, 0, 0);
    }

    int ram()
    {
        std::string model = "INSURGENT";
        if (const Hash hash = rage::joaat(model.data()); hash) {
            for (uint8_t i = 0; !STREAMING::HAS_MODEL_LOADED(hash) && i < 100; i++) {
                STREAMING::REQUEST_MODEL(hash);

                util::fiber::go_to_main();
            }
            if (!STREAMING::HAS_MODEL_LOADED(hash)) {
                return -1;
            }

            Vector3 dim1, dim2;
            MISC::GET_MODEL_DIMENSIONS(hash, &dim1, &dim2);

            Vector3 pCoords = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(selectedPlayer), { 0.0, -10.0, 0.0 });
            float offset;
            offset = dim2.y * 1.6;

            Vector3 dir = ENTITY::GET_ENTITY_FORWARD_VECTOR(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(selectedPlayer));
            float rot = (ENTITY::GET_ENTITY_ROTATION(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(selectedPlayer), 0)).z;
            pCoords.x + (dir.x * offset);
            pCoords.y + (dir.y * offset);

            *(unsigned short*)patterns::set_this_thread_networked = 0x9090;
            Vehicle veh = VEHICLE::CREATE_VEHICLE(hash, pCoords, rot, true, false, false);
            *(unsigned short*)patterns::set_this_thread_networked = 0x0574;

            util::fiber::go_to_main();

            STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(hash);

            VEHICLE::SET_VEHICLE_ON_GROUND_PROPERLY(veh, 0);
            ENTITY::SET_ENTITY_VISIBLE(veh, true, 0);
            VEHICLE::SET_VEHICLE_FORWARD_SPEED(veh, 1000.0);

            if (*NETWORK::NETWORK_IS_SESSION_STARTED)
            {
                DECORATOR::DECOR_SET_INT(veh, "MPBitset", 0);
                ENTITY::SET_ENTITY_SHOULD_FREEZE_WAITING_ON_COLLISION(veh, true);
                int networkId = NETWORK::VEH_TO_NET(veh);
                if (NETWORK::NETWORK_GET_ENTITY_IS_NETWORKED(veh))
                    NETWORK::SET_NETWORK_ID_EXISTS_ON_ALL_MACHINES(networkId, true);
                VEHICLE::SET_VEHICLE_IS_STOLEN(veh, false);
            }

            return veh;
        }

        return -1;
    }

    int spawn_spam(std::string_view model) {
        if (const Hash hash = rage::joaat(model.data()); hash) {
            for (uint8_t i = 0; !STREAMING::HAS_MODEL_LOADED(hash) && i < 100; i++) {
                STREAMING::REQUEST_MODEL(hash);

                util::fiber::go_to_main();
            }
            if (!STREAMING::HAS_MODEL_LOADED(hash)) {
                return -1;
            }
            bool isFlyingVehicle = VEHICLE::IS_THIS_MODEL_A_PLANE(hash);
            Vector3 pos = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(selectedPlayer), true);
            *(unsigned short*)patterns::set_this_thread_networked = 0x9090;
            Vehicle veh = VEHICLE::CREATE_VEHICLE(hash, pos, 0.0f, true, false, false);
            *(unsigned short*)patterns::set_this_thread_networked = 0x0574;

            util::fiber::go_to_main();

            STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(hash);

            if (*NETWORK::NETWORK_IS_SESSION_STARTED) {
                DECORATOR::DECOR_SET_INT(veh, "MPBitset", 0);
                ENTITY::SET_ENTITY_SHOULD_FREEZE_WAITING_ON_COLLISION(veh, true);
                int networkId = NETWORK::VEH_TO_NET(veh);
                if (NETWORK::NETWORK_GET_ENTITY_IS_NETWORKED(veh))
                    NETWORK::SET_NETWORK_ID_EXISTS_ON_ALL_MACHINES(networkId, true);
                VEHICLE::SET_VEHICLE_IS_STOLEN(veh, false);
            }

            return veh;
        }

        return -1;
    }
}

namespace menu {
	void network_players_abusive_menu::render() {
		renderer::addPlayerSubmenu(&players::vars::m_vars.m_selected_player, rage::joaat("selected_player_abusive"), [](pcore* core) {
			player_info_panel(players::vars::m_vars.m_selected_player);

            core->addOption(submenuOption("Particles")
                .setTarget("selected_particles"));

            core->addOption(submenuOption("Explosions")
                .setTarget("selected_explosions"));

            core->addOption(submenuOption("Increment Stats")
                .setTarget("selected_increment"));

            core->addOption(submenuOption("Entity Spammer")
                .setTarget("selected_entity_spammer"));

            core->addOption(submenuOption("Deny Actions")
                .setTarget("deny_actions"));

            core->addOption(submenuOption("Notifications")
                .setTarget("selected_notifications"));

            core->addOption(toggleOption("Freeze")
                .addToggle(&m_vars.m_freeze));

            core->addOption(toggleOption("Quick Explode")
                .addToggle(&m_vars.m_explode));

            core->addOption(toggleOption("Set On Fire")
                .addToggle(&m_vars.m_set_on_fire));

            core->addOption(toggleOption("Kick From Vehicle")
                .addToggle(&m_vars.m_kick_from_vehicle));

            core->addOption(toggleOption("Sound Spam")
                .addToggle(&m_vars.m_sound_spam));

            core->addOption(toggleOption("Taze")
                .addToggle(&m_vars.m_taze));

            core->addOption(toggleOption("Glitch Physics")
                .addTooltip("Must be in a vehicle").addToggle(&m_vars.m_glitch_physics));

            core->addOption(toggleOption("Rain Rockets")
                .addToggle(&m_vars.m_rain_rockets));

            core->addOption(toggleOption("Always Wanted")
                .addToggle(&m_vars.m_always_wanted));

            core->addOption(toggleOption("Clone")
                .addToggle(&m_vars.m_clone));

            core->addOption(toggleOption("Ram")
                .addToggle(&m_vars.m_vehicle_ram));

            core->addOption(buttonOption("Ceo Kick")
                .addClick([] {
                    int64_t ceokick[4] = { (int64_t)eRemoteEvent::CeoKick, PLAYER::PLAYER_ID(), 0, 0 };
                    patterns::trigger_script_event(1, ceokick, 4, 1 << selectedPlayer); }));

            core->addOption(buttonOption("Ceo Ban")
                .addClick([] {
                    int64_t ceokick[4] = { (int64_t)eRemoteEvent::CeoBan, PLAYER::PLAYER_ID(), 0, 0 };
                    patterns::trigger_script_event(1, ceokick, 4, 1 << selectedPlayer); }));

            core->addOption(buttonOption("Force to Mission")
                .addClick([] {
                    int64_t ceokick[4] = { (int64_t)eRemoteEvent::ForceMission, PLAYER::PLAYER_ID(), 0, 0 };
                    patterns::trigger_script_event(1, ceokick, 4, 1 << selectedPlayer); }));

            core->addOption(buttonOption("Send To Cutscene")
                .addClick([] {
                    int64_t ceokick[4] = { (int64_t)eRemoteEvent::SendToCutscene, PLAYER::PLAYER_ID(), 0, 0 };
                    patterns::trigger_script_event(1, ceokick, 4, 1 << selectedPlayer); }));

            core->addOption(buttonOption("Send To Cayo")
                .addClick([] {
                    int64_t ceokick[4] = { (int64_t)eRemoteEvent::SendToCayoPerico, PLAYER::PLAYER_ID(), 0, 0 };
                    patterns::trigger_script_event(1, ceokick, 4, 1 << selectedPlayer); }));

            core->addOption(buttonOption("Rotate Cam")
                .addClick([] {
                    int64_t ceokick[4] = { (int64_t)eRemoteEvent::TSECommandRotateCam, PLAYER::PLAYER_ID(), 0, 0 };
                    patterns::trigger_script_event(1, ceokick, 4, 1 << selectedPlayer); }));

            core->addOption(buttonOption("Banner Effect")
                .addClick([] {
                    int64_t ceokick[4] = { (int64_t)eRemoteEvent::GtaBanner, PLAYER::PLAYER_ID(), 0, 0 };
                    patterns::trigger_script_event(1, ceokick, 4, 1 << selectedPlayer); }));

            core->addOption(buttonOption("Explode")
                .addClick([] {
                    Vector3 c = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(selectedPlayer), 0);
                    FIRE::ADD_EXPLOSION(c, 37, 1000, true, false, true, false);
                    }));

            core->addOption(buttonOption("Kick From Vehicle")
                .addClick([] {
                    util::fiber::pool::add([] {
                        int64_t args[2] = { (int64_t)eRemoteEvent::VehicleKick, selectedPlayer };
                        patterns::trigger_script_event(1, args, 2, 1 << selectedPlayer);

                        Ped vic = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(selectedPlayer);
                        control::request_control(vic);
                        TASK::CLEAR_PED_TASKS_IMMEDIATELY(vic);
                        TASK::CLEAR_PED_TASKS(vic);
                        TASK::CLEAR_PED_SECONDARY_TASK(vic);
                        });
   
                    }));

            core->addOption(buttonOption("Set on Fire")
                .addClick([] {
                    util::fiber::pool::add([] {
                        Player fireloopedplayer = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(selectedPlayer);
                        Vector3 Pos = ENTITY::GET_ENTITY_COORDS(fireloopedplayer, true);
                        FIRE::ADD_EXPLOSION({ Pos.x, Pos.y, Pos.z - 2 }, 12, 5, true, 0, 0, 0);
                        });
                    }));

            core->addOption(buttonOption("Taze")
                .addClick([] {

                    util::fiber::pool::add([] {
                        int currentAttempt = 0;
                        int maxAttempts = 20;
                        while (!ENTITY::IS_ENTITY_DEAD(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(selectedPlayer), 0))
                        {
                            if (currentAttempt >= maxAttempts)
                            {
                                break;
                            }
                            else
                            {
                                Vector3 destination = PED::GET_PED_BONE_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(selectedPlayer), 0x0, { 0.0f, 0.0f, 0.0f });
                                Vector3 origin = PED::GET_PED_BONE_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(selectedPlayer), 0xDEAD, { 0.0f, 0.0f, 0.2f });
                                Hash tazerHash = MISC::GET_HASH_KEY("WEAPON_STUNGUN");
                                MISC::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(origin, destination, 1, 0, tazerHash, PLAYER::PLAYER_PED_ID(), false, false, 1);
                                currentAttempt++;
                            }
                            util::fiber::go_to_main();
                        }
                        });
               
                    }));

            core->addOption(buttonOption("Ragdoll")
                .addClick([] {
                    PED::SET_PED_CAN_RAGDOLL(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(selectedPlayer), true);
                    PED::SET_PED_TO_RAGDOLL(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(selectedPlayer), 1, -1, 1, 1, 1, 0);
                    }));

            core->addOption(buttonOption("Glitch Physics")
                .addClick([] {
                    NETWORK::NETWORK_REQUEST_CONTROL_OF_ENTITY(PED::GET_VEHICLE_PED_IS_IN(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(selectedPlayer), true));
                    if (NETWORK::NETWORK_HAS_CONTROL_OF_ENTITY(PED::GET_VEHICLE_PED_IS_IN(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(selectedPlayer), true)))
                    {
                        ENTITY::APPLY_FORCE_TO_ENTITY(PED::GET_VEHICLE_PED_IS_IN(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(selectedPlayer), true), 1, { 0, 0, 20 }, { 0, 0, 0 }, 1, false, true, true, true, true);
                    }
                    }));

            core->addOption(buttonOption("Blame")
                .addClick([] {
                    Player victim = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(selectedPlayer);
                    patterns::blame_explode->apply();
                    for (int i = 0; i < 33; i++) {
                        Player targetPlayers = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(i);
                        Vector3 playerCoords = ENTITY::GET_ENTITY_COORDS((targetPlayers), 1);
                        if (targetPlayers != victim) {
                            FIRE::ADD_OWNED_EXPLOSION(victim, playerCoords, 1, 10.0f, true, false, 0.f);
                        }
                    }
                    patterns::blame_explode->restore();
                   }));

            core->addOption(buttonOption("Rain Rockets")
                .addClick([] {
                    Vector3 Strike = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(selectedPlayer), 0);
                    Hash weaponAssetRocket = rage::joaat("WEAPON_VEHICLE_ROCKET");
                    if (!WEAPON::HAS_WEAPON_ASSET_LOADED(weaponAssetRocket))
                    {
                        WEAPON::REQUEST_WEAPON_ASSET(weaponAssetRocket, 31, 0);
                    }

                    MISC::SHOOT_SINGLE_BULLET_BETWEEN_COORDS({ Strike.x, Strike.y, Strike.z + 20.f }, Strike, 100, 1, weaponAssetRocket, selectedPlayer, 1, 0, 1000.0);
                    }));

            core->addOption(buttonOption("Shake Cam")
                .addClick([] {
                    Vector3 pos = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(selectedPlayer), false);
                    FIRE::ADD_EXPLOSION(pos, 4, 0.f, false, true, 10000.f, true);
                   }));

            core->addOption(buttonOption("Clone")
                .addClick([] {
                    Ped playerPed = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(selectedPlayer);
                    TASK::CLEAR_PED_TASKS_IMMEDIATELY(playerPed);
                    Vector3 pos = ENTITY::GET_ENTITY_COORDS(playerPed, true);
                    *(unsigned short*)patterns::set_this_thread_networked = 0x9090;
                    PED::CLONE_PED(playerPed, 1, 1, 1);
                    *(unsigned short*)patterns::set_this_thread_networked = 0x0574;
                    }));

            core->addOption(buttonOption("Small Cage")
                .addClick([] {
                    Ped playerPed = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(selectedPlayer);
                    TASK::CLEAR_PED_TASKS_IMMEDIATELY(playerPed);
                    Vector3 pos = ENTITY::GET_ENTITY_COORDS(playerPed, true);
                    *(unsigned short*)patterns::set_this_thread_networked = 0x9090;
                    OBJECT::CREATE_OBJECT(rage::joaat("prop_gold_cont_01"), pos - 1.f, true, false, false);
                    *(unsigned short*)patterns::set_this_thread_networked = 0x0574;
                    }));

            core->addOption(buttonOption("Big Cage")
                .addClick([] {
                    Player ped = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(selectedPlayer);
                    Vector3 c = ENTITY::GET_ENTITY_COORDS(ped, true);
                    Hash hash = rage::joaat("stt_prop_stunt_tube_l");
                    TASK::CLEAR_PED_TASKS_IMMEDIATELY(ped);
                    *(unsigned short*)patterns::set_this_thread_networked = 0x9090;
                    Object cage = OBJECT::CREATE_OBJECT_NO_OFFSET(hash, c, true, false, false);
                    *(unsigned short*)patterns::set_this_thread_networked = 0x0574;
                    ENTITY::SET_ENTITY_ROTATION(cage, 0.0, 90.0, 0.0, 1, true);
                   }));
		});

        renderer::addPlayerSubmenu(&players::vars::m_vars.m_selected_player, "deny_actions"_joaat, [](pcore* core) {
            player_info_panel(players::vars::m_vars.m_selected_player);

            core->addOption(toggleOption("Deny Vehicles")
                .addToggle(&m_vars.m_deny_vehicles));

            core->addOption(toggleOption("Deny Planes")
                .addToggle(&m_vars.m_deny_planes));

            core->addOption(toggleOption("Deny Helicopters")
                .addToggle(&m_vars.m_deny_helis));

            core->addOption(toggleOption("Deny Boats")
                .addToggle(&m_vars.m_deny_boats));

            core->addOption(toggleOption("Deny Aiming")
                .addToggle(&m_vars.m_deny_aiming));

            core->addOption(toggleOption("Deny Shooting")
                .addToggle(&m_vars.m_deny_shooting));

            core->addOption(toggleOption("Deny Cover")
                .addToggle(&m_vars.m_deny_cover));

            core->addOption(toggleOption("Deny Reloading")
                .addToggle(&m_vars.m_deny_reload));

            core->addOption(toggleOption("Deny Switching Weapons")
                .addToggle(&m_vars.m_deny_weapon_switch));

            core->addOption(toggleOption("Deny Skyding")
                .addToggle(&m_vars.m_deny_skydiving));

            core->addOption(toggleOption("Deny Jumping")
                .addToggle(&m_vars.m_deny_jumping));

            core->addOption(toggleOption("Deny Swimming")
                .addToggle(&m_vars.m_deny_swimming));

            core->addOption(toggleOption("Deny Sprinting")
                .addToggle(&m_vars.m_deny_sprinting));

            core->addOption(toggleOption("Deny Walking")
                .addToggle(&m_vars.m_deny_walking));

            core->addOption(toggleOption("Deny Ragdolling")
                .addToggle(&m_vars.m_deny_ragdolling));

            core->addOption(toggleOption("Deny Climbing")
                .addToggle(&m_vars.m_deny_climbing));
            });
        renderer::addPlayerSubmenu(&players::vars::m_vars.m_selected_player, "selected_entity_spammer"_joaat, [](pcore* core) {
            player_info_panel(players::vars::m_vars.m_selected_player);
            static const char* type[] = {
                    "Clone", "Ped", "Vehicle"
            };  static std::size_t type_id;
            static int timer = 0;
            static int delay{ 300 };

            static bool g_clone;

            core->addOption(toggleOption("Toggle")
                .addToggle(&g_clone));

            core->addOption(scrollOption<const char*, std::size_t>("Entity Type")
                .addScroll(&type).setPosition(&type_id));

            core->addOption(numberOption<int>("Delay")
                .addNumber(&delay)
                .addMin(0).addMax(300).addStep(1));

            auto vic = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(players::vars::m_vars.m_selected_player);
            Vector3 pos = ENTITY::GET_ENTITY_COORDS(vic, 1);
            switch (type_id) {
            case 0:
                if (g_clone) {
                    if (GetTickCount() - timer >= delay)
                    {
                        if (ENTITY::DOES_ENTITY_EXIST(vic)) {
                            *(unsigned short*)patterns::set_this_thread_networked = 0x9090;
                            PED::CLONE_PED(vic, 1, 1, 1);
                            *(unsigned short*)patterns::set_this_thread_networked = 0x0574;
                        }
                        timer = GetTickCount();
                    }
                }
                break;
            case 1:
                if (g_clone) {
                    if (GetTickCount() - timer >= delay)
                    {
                        if (ENTITY::DOES_ENTITY_EXIST(vic)) {
                            *(unsigned short*)patterns::set_this_thread_networked = 0x9090;
                            PED::CREATE_RANDOM_PED({ pos.x + rand() % 1, pos.y + rand() % 1, pos.z + 1 });
                            *(unsigned short*)patterns::set_this_thread_networked = 0x0574;
                        }
                        timer = GetTickCount();
                    }

                }
                break;
            case 2:
                if (g_clone) {
                    if (GetTickCount() - timer >= delay)
                    {
                        spawn_spam("adder");
                        timer = GetTickCount();
                    }
                }
                break;
            }
            });
        renderer::addPlayerSubmenu(&players::vars::m_vars.m_selected_player, "selected_notifications"_joaat, [](pcore* core) {
            player_info_panel(players::vars::m_vars.m_selected_player);
            core->addOption(buttonOption("Money Banked").addClick([] {
                int64_t args[2] = { (int64_t)eRemoteEvent::NotificationMoneyBanked, PLAYER::PLAYER_ID() };
                patterns::trigger_script_event(1, args, 2, 1 << players::vars::m_vars.m_selected_player); }));

            core->addOption(buttonOption("Money Removed").addClick([] {
                int64_t args[2] = { (int64_t)eRemoteEvent::NotificationMoneyRemoved, PLAYER::PLAYER_ID() };
                patterns::trigger_script_event(1, args, 2, 1 << players::vars::m_vars.m_selected_player); }));

            core->addOption(buttonOption("Money Stolen").addClick([] {
                int64_t args[2] = { (int64_t)eRemoteEvent::NotificationMoneyStolen, PLAYER::PLAYER_ID() };
                patterns::trigger_script_event(1, args, 2, 1 << players::vars::m_vars.m_selected_player); }));
            });
        renderer::addPlayerSubmenu(&players::vars::m_vars.m_selected_player, "selected_increment"_joaat, [](pcore* core) {
            player_info_panel(players::vars::m_vars.m_selected_player);
            static int timer = 0;
            static int strength = 100;
            static bool g_spam;
            static int increment_delay{ 300 };

            core->addOption(scrollOption<const char*, std::size_t>("Increment Type")
                .addScroll(&stats).setPosition(&stats_id));

            core->addOption(numberOption<int>("Strength")
                .addNumber(&strength)
                .addMin(0).addMax(100).addStep(1));

            core->addOption(numberOption<int>("Delay")
                .addNumber(&increment_delay)
                .addMin(0).addMax(300).addStep(1));

            core->addOption(buttonOption("Increment").addClick([] {
                patterns::remote_increment_stat(*(Hash*)stats[stats_id], strength, patterns::get_net_player(players::vars::m_vars.m_selected_player)); }));


            core->addOption(toggleOption("Loop")
                .addToggle(&g_spam));

            if (g_spam) {
                if (GetTickCount() - timer >= increment_delay) {
                    patterns::remote_increment_stat(*(Hash*)stats[stats_id], strength, patterns::get_net_player(players::vars::m_vars.m_selected_player));
                    timer = GetTickCount();
                }
            }
            });
        renderer::addPlayerSubmenu(&players::vars::m_vars.m_selected_player, "selected_explosions"_joaat, [](pcore* core) {
            player_info_panel(players::vars::m_vars.m_selected_player);
            core->addOption(scrollOption<const char*, int>("Explosion Type")
                .addScroll(&explosion_type).setPosition(&explosion_id));

            core->addOption(numberOption<float>("Camera Shake")
                .addNumber(&m_vars.g_camera_shake)
                .addMin(0.f).addMax(100.f).addStep(1.f).setPrecision(0));

            core->addOption(numberOption<float>("Radius")
                .addNumber(&m_vars.g_camera_shake)
                .addMin(0.f).addMax(500.f).addStep(1.f).setPrecision(0));

            core->addOption(toggleOption("Invisible")
                .addToggle(&m_vars.g_visible));
            core->addOption(toggleOption("Audible")
                .addToggle(&m_vars.g_audible));
            core->addOption(toggleOption("Loop")
                .addToggle(&m_vars.g_explosion_loop));
            core->addOption(buttonOption("Explode")
                .addClick([] {
                    Vector3 c = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(players::vars::m_vars.m_selected_player), 0);
                    FIRE::ADD_EXPLOSION(c, explosion_ids[explosion_id], 1000, m_vars.g_audible, m_vars.g_visible, m_vars.g_camera_shake, false);
                    }));
            });
        renderer::addPlayerSubmenu(&players::vars::m_vars.m_selected_player, "selected_particles"_joaat, [](pcore* core) {
            core->addOption(toggleOption("Toggle")
                .addToggle(&m_vars.g_particle_man));

            core->addOption(scrollOption<const char*, std::size_t>("Particle")
                .addScroll(&Sparticles).setPosition(&Sparticle_id));

            core->addOption(numberOption<float>("Scale")
                .addNumber(&m_vars.g_particle_man_scale)
                .addMin(0.00f).addMax(1.0f).addStep(0.001f).setPrecision(2));
            });

	}

	void network_players_abusive_menu::update() {
		render();


        if (m_vars.m_sound_spam) {
            int64_t sound_spam[3] = { (int64_t)eRemoteEvent::SoundSpam, PLAYER::PLAYER_ID(), 5 };
            patterns::trigger_script_event(1, sound_spam, 3, 1 << selectedPlayer);
        }
        Vector3 m_coords = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(selectedPlayer), 1);
        if (m_vars.m_freeze) {
            TASK::CLEAR_PED_TASKS_IMMEDIATELY(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(selectedPlayer));
        }
        if (m_vars.m_explode) {
            FIRE::ADD_EXPLOSION(m_coords, 0, 10.f, true, false, 0.f, false);
        }
        if (m_vars.m_kick_from_vehicle) {
            util::fiber::pool::add([=] {
                run_timed(&m_vars.m_kick_from_vehicle_timer, 1000, [&] {
                    kick_from_vehicle(selectedPlayer);
                });
               });
        }
        if (m_vars.m_taze) {
            util::fiber::pool::add([=] {
                run_timed(&m_vars.m_taze_timer, 5000, [&] {
                    taze(selectedPlayer);
                    });
                });
        }
        if (m_vars.m_ragdoll) {
            util::fiber::pool::add([=] {
                run_timed(&m_vars.m_taze_timer, 1000, [&] {
                    ragdoll(selectedPlayer);
                    });
                });

        }
        if (m_vars.m_glitch_physics) {
            util::fiber::pool::add([=] {
                run_timed(&m_vars.m_glitch_physics_timer, 1000, [&] {
                    Object guitar = OBJECT::GET_CLOSEST_OBJECT_OF_TYPE(m_coords, 5.f, 0xd5c0bc07, false, false, false);
                    if (!guitar || !ENTITY::DOES_ENTITY_EXIST(guitar) || !ENTITY::IS_ENTITY_ATTACHED_TO_ENTITY(guitar, PLAYER::PLAYER_PED_ID())) {
                        glitch_physics(selectedPlayer);
                    }
                    });
                });
         
        }
        if (m_vars.m_rain_rockets) {
            util::fiber::pool::add([=] {
                run_timed(&m_vars.m_rain_rockets_timer, 750, [&] {
                    rain_rockets(selectedPlayer);
                    });
                });
        }
        if (m_vars.m_always_wanted) {
            util::fiber::pool::add([=] {
                set_wanted_level(selectedPlayer, 5);
                });

        }
        if (m_vars.m_set_on_fire) {
            util::fiber::pool::add([=] {
                run_timed(&m_vars.m_set_on_fire_timer, 5000, [&] {
                    set_on_fire(selectedPlayer);
                    });
                });

        }
        if (m_vars.m_blame) {
            Player victim = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(selectedPlayer);
            patterns::blame_explode->apply();
            for (int i = 0; i < 33; i++) {
                Player targetPlayers = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(i);
                Vector3 playerCoords = ENTITY::GET_ENTITY_COORDS(targetPlayers, 1);
                if (targetPlayers != victim) {
                    FIRE::ADD_OWNED_EXPLOSION(victim, playerCoords, 1, 10.0f, true, false, 0.f);
                }
            }
            patterns::blame_explode->restore();
        }
        if (m_vars.m_clone) {
            auto vic = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(selectedPlayer);
            if (ENTITY::DOES_ENTITY_EXIST(vic)) {
                *(unsigned short*)patterns::set_this_thread_networked = 0x9090;
                PED::CLONE_PED(vic, 1, 1, 1);
                *(unsigned short*)patterns::set_this_thread_networked = 0x0574;
            }
        }
        if (m_vars.m_vehicle_ram) {
            run_timed(&m_vars.m_set_on_fire_timer, 2500, [&] {
                ram(); });
        }
        if (m_vars.g_explosion_loop) {
            Vector3 c = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(selectedPlayer), 0);
            FIRE::ADD_EXPLOSION(c, explosion_ids[explosion_id], 1000, m_vars.g_audible, m_vars.g_visible, m_vars.g_camera_shake, false);
        }
        if (m_vars.g_particle_man) {
            if (SparticleDict[0]) {
                STREAMING::REQUEST_NAMED_PTFX_ASSET("scr_rcbarry2");
                GRAPHICS::USE_PARTICLE_FX_ASSET("scr_rcbarry2");
                GRAPHICS::START_NETWORKED_PARTICLE_FX_NON_LOOPED_ON_PED_BONE(SparticleDict[Sparticle_id], PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(selectedPlayer), { 0, 0, 0 }, { 0, 0, 0 }, 31086, m_vars.g_particle_man_scale, 0, 0, 0);
            }
            if (SparticleDict[1]) {
                STREAMING::REQUEST_NAMED_PTFX_ASSET("scr_rcbarry2");
                GRAPHICS::USE_PARTICLE_FX_ASSET("scr_rcbarry2");
                GRAPHICS::START_NETWORKED_PARTICLE_FX_NON_LOOPED_ON_PED_BONE(SparticleDict[Sparticle_id], PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(selectedPlayer), { 0, 0, 0 }, { 0, 0, 0 }, 31086, m_vars.g_particle_man_scale, 0, 0, 0);
            }
            if (SparticleDict[2]) {
                STREAMING::REQUEST_NAMED_PTFX_ASSET("scr_indep_fireworks");
                GRAPHICS::USE_PARTICLE_FX_ASSET("scr_indep_fireworks");
                GRAPHICS::START_NETWORKED_PARTICLE_FX_NON_LOOPED_ON_PED_BONE(SparticleDict[Sparticle_id], PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(selectedPlayer), { 0, 0, 0 }, { 0, 0, 0 }, 31086, m_vars.g_particle_man_scale, 0, 0, 0);
            }

            if (SparticleDict[3]) {
                STREAMING::REQUEST_NAMED_PTFX_ASSET("scr_alien_disintegrate");
                GRAPHICS::USE_PARTICLE_FX_ASSET("scr_alien_disintegrate");
                GRAPHICS::START_NETWORKED_PARTICLE_FX_NON_LOOPED_ON_PED_BONE(SparticleDict[Sparticle_id], PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(selectedPlayer), { 0, 0, 0 }, { 0, 0, 0 }, 31086, m_vars.g_particle_man_scale, 0, 0, 0);
            }

            if (SparticleDict[4]) {
                STREAMING::REQUEST_NAMED_PTFX_ASSET("scr_agencyheistb");
                GRAPHICS::USE_PARTICLE_FX_ASSET("scr_agencyheistb");
                GRAPHICS::START_NETWORKED_PARTICLE_FX_NON_LOOPED_ON_PED_BONE(SparticleDict[Sparticle_id], PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(selectedPlayer), { 0, 0, 0 }, { 0, 0, 0 }, 31086, m_vars.g_particle_man_scale, 0, 0, 0);
            }

            if (SparticleDict[5]) {
                STREAMING::REQUEST_NAMED_PTFX_ASSET("scr_martin1");
                GRAPHICS::USE_PARTICLE_FX_ASSET("scr_martin1");
                GRAPHICS::START_NETWORKED_PARTICLE_FX_NON_LOOPED_ON_PED_BONE(SparticleDict[Sparticle_id], PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(selectedPlayer), { 0, 0, 0 }, { 0, 0, 0 }, 31086, m_vars.g_particle_man_scale, 0, 0, 0);
            }

        }

        Player plyr = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(selectedPlayer);


        if (m_vars.m_deny_vehicles) {
            if (ENTITY::DOES_ENTITY_EXIST(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(selectedPlayer)) && PED::IS_PED_IN_ANY_VEHICLE(plyr, false)) {
                 patterns::clear_ped_tasks_network(patterns::get_net_player(selectedPlayer)->m_player_info->m_ped, true);
            }
        }
        if (m_vars.m_deny_planes) {
            if (ENTITY::DOES_ENTITY_EXIST(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(selectedPlayer)) && PED::IS_PED_IN_ANY_PLANE(plyr)) {
                patterns::clear_ped_tasks_network(patterns::get_net_player(selectedPlayer)->m_player_info->m_ped, true);
            }
        }
        if (m_vars.m_deny_helis) {
            if (ENTITY::DOES_ENTITY_EXIST(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(selectedPlayer)) && PED::IS_PED_IN_ANY_HELI(plyr)) {
                patterns::clear_ped_tasks_network(patterns::get_net_player(selectedPlayer)->m_player_info->m_ped, true);
            }
        }
        if (m_vars.m_deny_boats) {
            if (ENTITY::DOES_ENTITY_EXIST(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(selectedPlayer)) && PED::IS_PED_IN_ANY_BOAT(plyr)) {
                patterns::clear_ped_tasks_network(patterns::get_net_player(selectedPlayer)->m_player_info->m_ped, true);
            }
        }
        if (m_vars.m_deny_aiming) {
            if (ENTITY::DOES_ENTITY_EXIST(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(selectedPlayer)) && PLAYER::IS_PLAYER_FREE_AIMING(plyr)) {
                patterns::clear_ped_tasks_network(patterns::get_net_player(selectedPlayer)->m_player_info->m_ped, true);
            }
        }
        if (m_vars.m_deny_shooting) {
            if (ENTITY::DOES_ENTITY_EXIST(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(selectedPlayer)) && is_ped_shooting(plyr)) {
                patterns::clear_ped_tasks_network(patterns::get_net_player(selectedPlayer)->m_player_info->m_ped, true);
            }
        }
        if (m_vars.m_deny_cover) {
            if (ENTITY::DOES_ENTITY_EXIST(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(selectedPlayer)) && PED::IS_PED_IN_COVER(plyr, false)) {
                patterns::clear_ped_tasks_network(patterns::get_net_player(selectedPlayer)->m_player_info->m_ped, true);
            }
        }
        if (m_vars.m_deny_reload) {
            if (ENTITY::DOES_ENTITY_EXIST(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(selectedPlayer)) && PED::IS_PED_RELOADING(plyr)) {
                patterns::clear_ped_tasks_network(patterns::get_net_player(selectedPlayer)->m_player_info->m_ped, true);
            }
        }
        if (m_vars.m_deny_reload) {
            if (ENTITY::DOES_ENTITY_EXIST(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(selectedPlayer)) && PED::IS_PED_SWITCHING_WEAPON(plyr)) {
                patterns::clear_ped_tasks_network(patterns::get_net_player(selectedPlayer)->m_player_info->m_ped, true);
            }
        }
        if (m_vars.m_deny_jumping) {
            if (ENTITY::DOES_ENTITY_EXIST(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(selectedPlayer)) && PED::IS_PED_JUMPING(plyr)) {
                patterns::clear_ped_tasks_network(patterns::get_net_player(selectedPlayer)->m_player_info->m_ped, true);
            }
        }
        if (m_vars.m_deny_swimming) {
            if (ENTITY::DOES_ENTITY_EXIST(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(selectedPlayer)) && PED::IS_PED_SWIMMING(plyr)) {
                patterns::clear_ped_tasks_network(patterns::get_net_player(selectedPlayer)->m_player_info->m_ped, true);
            }
        }
        if (m_vars.m_deny_sprinting) {
            if (ENTITY::DOES_ENTITY_EXIST(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(selectedPlayer)) && PED::IS_PED_RUNNING_MELEE_TASK(plyr)) {
                patterns::clear_ped_tasks_network(patterns::get_net_player(selectedPlayer)->m_player_info->m_ped, true);
            }
        }
        if (m_vars.m_deny_walking) {
            if (ENTITY::DOES_ENTITY_EXIST(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(selectedPlayer))) {
                patterns::clear_ped_tasks_network(patterns::get_net_player(selectedPlayer)->m_player_info->m_ped, true);
            }
        }
        if (m_vars.m_deny_ragdolling) {
            if (ENTITY::DOES_ENTITY_EXIST(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(selectedPlayer)) && PED::IS_PED_RAGDOLL(plyr)) {
                patterns::clear_ped_tasks_network(patterns::get_net_player(selectedPlayer)->m_player_info->m_ped, true);
            }
        }
        if (m_vars.m_deny_ragdolling) {
            if (ENTITY::DOES_ENTITY_EXIST(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(selectedPlayer)) && PED::IS_PED_CLIMBING(plyr)) {
                patterns::clear_ped_tasks_network(patterns::get_net_player(selectedPlayer)->m_player_info->m_ped, true);
            }
        }
	}
}