#include "game/multiplayer_game_hopper.hpp"

#include "cseries/cseries.hpp"
#include "memory/module.hpp"

HOOK_DECLARE(0x00545710, multiplayer_game_hopper_check_required_files);
HOOK_DECLARE(0x00549610, multiplayer_game_hopper_update);
HOOK_DECLARE(0x00549620, multiplayer_game_hoppers_get_current_hopper_configuration);
HOOK_DECLARE(0x00549630, multiplayer_game_hoppers_get_hopper_configuration);
//HOOK_DECLARE(0x00549640, multiplayer_game_hoppers_pick_random_game_collection);
HOOK_DECLARE(0x00549650, multiplayer_game_is_playable);

static c_hopper_configuration hopper_configuration;

//.text:00544AC0 ; void __cdecl initialize_fake_hopper(s_hopper_configuration_table*, s_game_hopper_description_table*, s_game_set*, s_game_set*);
//.text:00545700 ; e_network_file_load_status __cdecl multiplayer_game_hopper_catalog_load_status();

//.text:00545710 ; e_session_game_start_error __cdecl multiplayer_game_hopper_check_required_files(bool, bool);
e_session_game_start_error __cdecl multiplayer_game_hopper_check_required_files(bool check_hopper, bool valid_hopper_identifier)
{
    check_hopper = false;
    valid_hopper_identifier = false;

    e_session_game_start_error result = _session_game_start_error_none;
    //HOOK_INVOKE(result =, multiplayer_game_hopper_check_required_files, check_hopper, valid_hopper_identifier);
    return result;
}

//.text:005457F0 ; void __cdecl multiplayer_game_hopper_client_compute_repeated_play_adjustment_weight(e_controller_index, word, long, long, long, long, qword const*, long, long*, bool*);
//.text:00545930 ; void __cdecl multiplayer_game_hopper_client_modify_repeated_play_list(e_controller_index, word, qword const*, long);
//.text:005459D0 ; sub_5459D0
//.text:00545A70 ; sub_545A70
//.text:00545B00 ; real __cdecl multiplayer_game_hopper_compute_gather_chance(long);
//.text:00545B10 ; long __cdecl multiplayer_game_hopper_compute_match_quality(c_network_session_membership const*);
//.text:00545C20 ; long __cdecl multiplayer_game_hopper_compute_skill_match_delta(long);
//.text:00545C30 ; sub_545C30
//.text:00545E80 ; bool __cdecl multiplayer_game_hopper_decode(c_bitstream*, s_hopper_configuration_table*);
//.text:00546820 ; bool __cdecl multiplayer_game_hopper_description_decode(c_bitstream*, s_game_hopper_description_table*);
//.text:005468A0 ; void __cdecl multiplayer_game_hopper_description_encode(c_bitstream*, s_game_hopper_description_table const*);
//.text:005469E0 ; void __cdecl multiplayer_game_hopper_dispose();
//.text:005469F0 ; void __cdecl multiplayer_game_hopper_encode(c_bitstream*, s_hopper_configuration_table const*);
//.text:00548210 ; e_network_file_load_status __cdecl multiplayer_game_hopper_game_set_load_status();
//.text:00548220 ; e_network_file_load_status __cdecl multiplayer_game_hopper_game_variant_load_status();
//.text:00548230 ; s_game_hopper_custom_category const* __cdecl multiplayer_game_hopper_get_category_from_index();
//.text:00548240 ; c_game_variant const* __cdecl multiplayer_game_hopper_get_current_game_variant();
//.text:00548250 ; word __cdecl multiplayer_game_hopper_get_current_hopper_identifier();
//.text:00548260 ; c_map_variant const* __cdecl multiplayer_game_hopper_get_current_map_variant();
//.text:00548270 ; utf8 const* __cdecl multiplayer_game_hopper_get_description(word);
//.text:00548280 ; word __cdecl multiplayer_game_hopper_get_hopper_identifier(long);
//.text:00548290 ; sub_548290
//.text:005482A0 ; sub_5482A0
//.text:005483A0 ; sub_5483A0
//.text:005483B0 ; void __cdecl multiplayer_game_hopper_initialize();
//.text:005483C0 ; bool __cdecl multiplayer_game_hopper_is_hopper_visible(word, c_network_session_membership const*);
//.text:005483D0 ; sub_5483D0
//.text:005483E0 ; e_network_file_load_status __cdecl multiplayer_game_hopper_map_variant_load_status();
//.text:005483F0 ; long __cdecl multiplayer_game_hopper_pack_game_set(void*, long, s_game_set const*);
//.text:00548610 ; long __cdecl multiplayer_game_hopper_pack_game_variant(void*, long, c_game_variant const*);
//.text:00548830 ; long __cdecl multiplayer_game_hopper_pack_hopper_description(void*, long, s_game_hopper_description_table const*);
//.text:00548A50 ; long __cdecl multiplayer_game_hopper_pack_hopper_file(void*, long, s_hopper_configuration_table const*);
//.text:00548C70 ; long __cdecl multiplayer_game_hopper_pack_map_variant(void*, long, c_map_variant const*);
//.text:00548E90 ; void __cdecl multiplayer_game_hopper_request_game_variant(word, char const*, s_network_http_request_hash const*);
//.text:00548EA0 ; void __cdecl multiplayer_game_hopper_request_map_variant(word, char const*, s_network_http_request_hash const*);
//.text:00548EB0 ; bool __cdecl multiplayer_game_hopper_set_active_hopper_and_request_game_set(word);
//.text:00548EC0 ; sub_548EC0
//.text:00548ED0 ; bool __cdecl multiplayer_game_hopper_unpack_game_set(void const*, long, s_game_set*);
//.text:00549050 ; bool __cdecl multiplayer_game_hopper_unpack_game_variant(void const*, long, c_game_variant*);
//.text:005491D0 ; bool __cdecl multiplayer_game_hopper_unpack_hopper_description(void const*, long, s_game_hopper_description_table*);
//.text:00549350 ; bool __cdecl multiplayer_game_hopper_unpack_hopper_file(void const*, long, s_hopper_configuration_table*);
//.text:005494D0 ; bool __cdecl multiplayer_game_hopper_unpack_map_variant(void const*, long, c_map_variant*);

//.text:00549610 ; void __cdecl multiplayer_game_hopper_update();
void __cdecl multiplayer_game_hopper_update()
{
    hopper_configuration.hopper_name.set("TEST HOPPER");
}

//.text:00549620 ; c_hopper_configuration const* __cdecl multiplayer_game_hoppers_get_current_hopper_configuration();
c_hopper_configuration* __cdecl multiplayer_game_hoppers_get_current_hopper_configuration()
{
    return &hopper_configuration;
}

//.text:00549630 ; c_hopper_configuration const* __cdecl multiplayer_game_hoppers_get_hopper_configuration(word);
c_hopper_configuration* __cdecl multiplayer_game_hoppers_get_hopper_configuration(word hopper_identifier)
{
    return &hopper_configuration;
}

//.text:00549640 ; bool __cdecl multiplayer_game_hoppers_pick_random_game_collection(long, long, s_game_hopper_picked_game_collection*);
bool __cdecl multiplayer_game_hoppers_pick_random_game_collection(long player_count, long valid_map_mask, s_game_hopper_picked_game_collection* game_collection_out)
{
    return false;
}

//.text:00549650 ; enum e_session_game_start_error __cdecl multiplayer_game_is_playable(word, bool, bool, c_network_session_membership const*, word*);
e_session_game_start_error __cdecl multiplayer_game_is_playable(word hopper_identifier, bool is_matchmaking, bool check_hopper, c_network_session_membership const* session_membership, word* out_player_error_mask)
{
    is_matchmaking = false;
    check_hopper = false;

    e_session_game_start_error result = _session_game_start_error_none;
    HOOK_INVOKE(result =, multiplayer_game_is_playable, hopper_identifier, is_matchmaking, check_hopper, session_membership, out_player_error_mask);
    return result;
}

//.text:00549870 ; bool __cdecl multiplayer_game_set_decode(c_bitstream*, s_game_set*);
//.text:00549970 ; void __cdecl multiplayer_game_set_encode(c_bitstream*, s_game_set const*);
//.text:00549B70 ; char const* __cdecl multiplayer_game_start_error_to_string(e_session_game_start_error);

