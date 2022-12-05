#include "game/multiplayer_game_hopper.hpp"

#include "cseries/cseries.hpp"
#include "memory/module.hpp"

HOOK_DECLARE(0x00549610, multiplayer_game_hopper_update);
HOOK_DECLARE(0x00549620, multiplayer_game_hoppers_get_current_hopper_configuration);
HOOK_DECLARE(0x00549630, multiplayer_game_hoppers_get_hopper_configuration);
//HOOK_DECLARE(0x00549640, multiplayer_game_hoppers_pick_random_game_collection);
HOOK_DECLARE(0x00549650, multiplayer_game_is_playable);
HOOK_DECLARE(0x00545710, multiplayer_game_hopper_check_required_files);

static c_hopper_configuration hopper_configuration;

void __cdecl multiplayer_game_hopper_update()
{
    hopper_configuration.hopper_name.set("TEST HOPPER");
}

c_hopper_configuration* __cdecl multiplayer_game_hoppers_get_current_hopper_configuration()
{
    return &hopper_configuration;
}

c_hopper_configuration* __cdecl multiplayer_game_hoppers_get_hopper_configuration(word hopper_identifier)
{
    return &hopper_configuration;
}

bool __cdecl multiplayer_game_hoppers_pick_random_game_collection(long player_count, long valid_map_mask, s_game_hopper_picked_game_collection* game_collection_out)
{
    return false;
}

e_session_game_start_error __cdecl multiplayer_game_is_playable(word hopper_identifier, bool is_matchmaking, bool check_hopper, c_network_session_membership const* session_membership, word* out_player_error_mask)
{
    is_matchmaking = false;
    check_hopper = false;

    e_session_game_start_error result = _session_game_start_error_none;
    HOOK_INVOKE(result =, multiplayer_game_is_playable, hopper_identifier, is_matchmaking, check_hopper, session_membership, out_player_error_mask);
    return result;
}

e_session_game_start_error __cdecl multiplayer_game_hopper_check_required_files(bool check_hopper, bool valid_hopper_identifier)
{
    check_hopper = false;
    valid_hopper_identifier = false;

    e_session_game_start_error result = _session_game_start_error_none;
    //HOOK_INVOKE(result =, multiplayer_game_hopper_check_required_files, check_hopper, valid_hopper_identifier);
    return result;
}

