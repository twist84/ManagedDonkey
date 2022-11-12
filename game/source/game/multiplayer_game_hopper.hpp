#pragma once

#include "cseries/cseries.hpp"
#include "shell/shell.hpp"

struct c_network_session_membership;

extern e_session_game_start_error __cdecl multiplayer_game_hopper_check_required_files(bool check_hopper, bool valid_hopper_identifier);
extern e_session_game_start_error __cdecl multiplayer_game_is_playable(word hopper_identifier, bool is_matchmaking, bool check_hopper, c_network_session_membership const* session_membership, word* out_player_error_mask);