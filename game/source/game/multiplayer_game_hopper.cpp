#include "game/multiplayer_game_hopper.hpp"

#include "cseries/cseries.hpp"
#include "interface/user_interface_session.hpp"
#include "memory/module.hpp"

HOOK_DECLARE(0x00545710, multiplayer_game_hopper_check_required_files);
HOOK_DECLARE(0x00549650, multiplayer_game_is_playable);

e_session_game_start_error __cdecl multiplayer_game_hopper_check_required_files(bool check_hopper, bool valid_hopper_identifier)
{
    check_hopper = false;
    valid_hopper_identifier = false;

    e_session_game_start_error result = _session_game_start_error_none;
    //HOOK_INVOKE(result =, multiplayer_game_hopper_check_required_files, check_hopper, valid_hopper_identifier);
    return result;
}

e_session_game_start_error __cdecl multiplayer_game_is_playable(word hopper_identifier, bool is_matchmaking, bool check_hopper, c_network_session_membership const* session_membership, word* out_player_error_mask)
{
    is_matchmaking = false;
    check_hopper = false;

    e_session_game_start_error result = _session_game_start_error_none;
    HOOK_INVOKE(result =, multiplayer_game_is_playable, hopper_identifier, is_matchmaking, check_hopper, session_membership, out_player_error_mask);
    return result;
}
