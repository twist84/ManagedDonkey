#include "game/multiplayer_game_hopper.hpp"

#include "cseries/cseries.hpp"
#include "cseries/cseries_console.hpp"
#include "game/game.hpp"
#include "memory/module.hpp"
#include "networking/logic/network_session_interface.hpp"
#include "networking/tools/network_blf.hpp"


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
long __cdecl multiplayer_game_hopper_pack_game_variant(void* buffer, long buffer_size, c_game_variant const* game_variant)
{
    return INVOKE(0x00548610, multiplayer_game_hopper_pack_game_variant, buffer, buffer_size, game_variant);
}

//.text:00548830 ; long __cdecl multiplayer_game_hopper_pack_hopper_description(void*, long, s_game_hopper_description_table const*);
//.text:00548A50 ; long __cdecl multiplayer_game_hopper_pack_hopper_file(void*, long, s_hopper_configuration_table const*);

//.text:00548C70 ; long __cdecl multiplayer_game_hopper_pack_map_variant(void*, long, c_map_variant const*);
long __cdecl multiplayer_game_hopper_pack_map_variant(void* buffer, long buffer_size, c_map_variant const* map_variant)
{
    return INVOKE(0x00548C70, multiplayer_game_hopper_pack_map_variant, buffer, buffer_size, map_variant);
}

//.text:00548E90 ; void __cdecl multiplayer_game_hopper_request_game_variant(word, char const*, s_network_http_request_hash const*);
//.text:00548EA0 ; void __cdecl multiplayer_game_hopper_request_map_variant(word, char const*, s_network_http_request_hash const*);
//.text:00548EB0 ; bool __cdecl multiplayer_game_hopper_set_active_hopper_and_request_game_set(word);
//.text:00548EC0 ; sub_548EC0
//.text:00548ED0 ; bool __cdecl multiplayer_game_hopper_unpack_game_set(void const*, long, s_game_set*);

bool packed_game_variant_is_mcc(void const* buffer_, long bytes_read)
{
    byte const* buffer = static_cast<byte const*>(buffer_);
    void const* buffer_end = buffer + bytes_read;

    s_blf_header const* chunk_header = reinterpret_cast<s_blf_header const*>(buffer);
    while (chunk_header->chunk_type != 'msf_')
    {
        buffer += _byteswap_ulong(chunk_header->chunk_size);
        chunk_header = reinterpret_cast<s_blf_header const*>(buffer);

        if (buffer >= buffer_end)
            return false;
    }

    return chunk_header->chunk_type == 'msf_';
}

//.text:00549050 ; bool __cdecl multiplayer_game_hopper_unpack_game_variant(void const*, long, c_game_variant*);
bool __cdecl multiplayer_game_hopper_unpack_game_variant(void const* buffer, long bytes_read, c_game_variant* game_variant)
{
    if (packed_game_variant_is_mcc(buffer, bytes_read))
    {
        void const* buffer_end = static_cast<byte const*>(buffer) + bytes_read;

        s_blf_header const* chunk_header = static_cast<s_blf_header const*>(buffer);
        csmemset(game_variant, 0, sizeof(c_game_variant));

        ASSERT(0x9D == sizeof(s_blf_chunk_start_of_file) + sizeof(s_blf_chunk_author) + sizeof(s_blf_header) + sizeof(s_blf_chunk_end_of_file));
        ASSERT(bytes_read > sizeof(s_blf_chunk_start_of_file) + sizeof(s_blf_chunk_author) + sizeof(s_blf_header) + sizeof(s_blf_chunk_end_of_file));

        while (buffer < buffer_end && chunk_header->chunk_type != 'ravg')
        {
            buffer = static_cast<byte const*>(buffer) + _byteswap_ulong(chunk_header->chunk_size);
            chunk_header = static_cast<s_blf_header const*>(buffer);
        }

        if (buffer >= buffer_end)
            return false;

        long chunk_size = _byteswap_ulong(chunk_header->chunk_size) - sizeof(s_blf_header);
        byte* chunk_data = const_cast<byte*>(static_cast<byte const*>(buffer) + sizeof(s_blf_header));

        c_bitstream packet(chunk_data, chunk_size);
        packet.begin_reading();
        bool decode_succeeded = game_variant->decode_from_mcc(&packet);
        packet.finish_reading();

        bool result = decode_succeeded;
        if (decode_succeeded)
        {
            buffer = static_cast<byte const*>(buffer) + _byteswap_ulong(chunk_header->chunk_size);
            chunk_header = static_cast<s_blf_header const*>(buffer);

            if (buffer >= buffer_end)
            {
                return false;
            }
            else
            {
                // is end of file
                while (chunk_header->chunk_type != 'foe_')
                {
                    buffer = static_cast<byte const*>(buffer) + _byteswap_ulong(chunk_header->chunk_size);
                    chunk_header = static_cast<s_blf_header const*>(buffer);

                    if (buffer >= buffer_end)
                        return false;
                }

                result = false;
                if (buffer < buffer_end)
                    return true;
            }
        }

        return result;
    }

    return INVOKE(0x00549050, multiplayer_game_hopper_unpack_game_variant, buffer, bytes_read, game_variant);
}

//.text:005491D0 ; bool __cdecl multiplayer_game_hopper_unpack_hopper_description(void const*, long, s_game_hopper_description_table*);
//.text:00549350 ; bool __cdecl multiplayer_game_hopper_unpack_hopper_file(void const*, long, s_hopper_configuration_table*);

//.text:005494D0 ; bool __cdecl multiplayer_game_hopper_unpack_map_variant(void const*, long, c_map_variant*);
bool __cdecl multiplayer_game_hopper_unpack_map_variant(void const* buffer, long buffer_size, c_map_variant* map_variant)
{
    return INVOKE(0x005494D0, multiplayer_game_hopper_unpack_map_variant, buffer, buffer_size, map_variant);
}

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

bool __cdecl create_configuration_file(const char* filename, const void* file_contents, int file_size)
{
    ASSERT(file_size > 0);
    ASSERT(file_contents);

    s_file_reference info;
    file_reference_create_from_path(&info, filename, false);

    if (!file_create_parent_directories_if_not_present(&info))
    {
        c_console::write_line("create_configuration_file: unable to create parent directories: %s", filename);
        return false;
    }

    if (!file_create(&info))
    {
        c_console::write_line("create_configuration_file: unable to create file: %s", filename);
        return false;
    }

    dword error = 0;
    if (!file_open(&info, FLAG(_file_open_flag_desired_access_write), &error))
    {
        c_console::write_line("create_configuration_file: unable to open file: %s", filename);
        return false;
    }

    bool result = file_write(&info, file_size, file_contents);
    if (!file_close(&info))
    {
        c_console::write_line("create_configuration_file: unable to close file: %s", filename);
    }

    return result;
}

void __cdecl network_verify_or_load_and_use_packed_game_variant_file(char const* filename, bool load_and_use)
{
    s_file_reference info;
    if (!file_reference_create_from_path(&info, filename, 0))
    {
        c_console::write_line("networking:configuration: failed to create file reference for file '%s'", filename);
        return;
    }

    dword error = 0;
    if (!file_open(&info, FLAG(_file_open_flag_desired_access_read), &error))
    {
        c_console::write_line("networking:configuration: failed to open file '%s'", filename);
        return;
    }

    dword size = 0;
    if (!file_get_size(&info, &size))
    {
        c_console::write_line("networking:configuration: failed to determine file size for file '%s'", filename);
        file_close(&info);
        return;
    }

    byte buffer[0x400]{};
    csmemset(buffer, 0, sizeof(buffer));

    if (size > sizeof(buffer))
    {
        c_console::write_line("networking:configuration: invalid file size for '%s' (%ld bytes/%ld max)", filename, size, sizeof(buffer));
        file_close(&info);
        return;
    }

    if (!file_read(&info, size, false, buffer))
    {
        c_console::write_line("networking:configuration: failed to read from file '%s'", filename);
        file_close(&info);
        return;
    }

    c_game_variant* game_variant = new c_game_variant();
    if (!multiplayer_game_hopper_unpack_game_variant(buffer, size, game_variant))
    {
        c_console::write_line("networking:configuration: failed to unpack game variant in file '%s'", filename);

        delete game_variant;
        file_close(&info);
        return;
    }

    if (!game_engine_variant_is_valid(game_variant))
    {
        c_console::write_line("networking:configuration: game variant in file '%s' is invalid", filename);

        delete game_variant;
        file_close(&info);
        return;
    }

    c_console::write_line("networking:configuration: CONGRATULATIONS! variant file '%s' is valid", filename);

    if (load_and_use)
    {
        if (!network_squad_session_set_game_variant(game_variant))
        {
            c_console::write_line("networking:configuration: failed to set session game variant traits, probably not in a session");
        }
    }

    delete game_variant;
    file_close(&info);
}

void __cdecl network_build_game_variant(char const* filename)
{
    byte* buffer = new byte[0x600]{};
    c_static_string<256> filepath;

    c_game_variant const* game_variant = &game_options_get()->game_variant;
    long file_size = multiplayer_game_hopper_pack_game_variant(buffer, 0x600, game_variant);

    // 5:  halo3_cache_debug
    // 10: halo3_tag_test
    // 18: hf2p_game_client_cache_release, using `k_cache_file_version`
    filepath.print("game_variants\\%s_%03u.bin", filename, 18);
    if (!create_configuration_file(filepath.get_string(), buffer, file_size))
    {
        c_console::write_line("failed!");
    }

    delete[] buffer;
}

void __cdecl network_load_and_use_packed_game_variant_file(char const* filename)
{
    network_verify_or_load_and_use_packed_game_variant_file(filename, true);
}

void __cdecl network_verify_packed_game_variant_file(char const* filename)
{
    network_verify_or_load_and_use_packed_game_variant_file(filename, false);
}

void __cdecl network_build_map_variant(char const* filename)
{
    byte* buffer = new byte[0xE090]{};
    c_static_string<256> filepath;

    c_map_variant const* map_variant = &game_options_get()->map_variant;
    long file_size = multiplayer_game_hopper_pack_map_variant(buffer, 0xE090, map_variant);

    // 4:  halo3_cache_debug
    // 12: halo3_tag_test
    // 19: hf2p_game_client_cache_release, using `k_cache_file_version` + 1
    filepath.print("map_variants\\%s_%03u.bin", filename, 19);
    if (!create_configuration_file(filepath.get_string(), buffer, file_size))
    {
        c_console::write_line("failed!");
    }

    delete[] buffer;
}

void __cdecl network_verify_or_load_and_use_packed_map_variant_file(char const* filename, bool load_and_use)
{
    s_file_reference info;
    if (!file_reference_create_from_path(&info, filename, 0))
    {
        c_console::write_line("networking:configuration: failed to create file reference for file '%s'", filename);
        return;
    }

    dword error = 0;
    if (!file_open(&info, FLAG(_file_open_flag_desired_access_read), &error))
    {
        c_console::write_line("networking:configuration: failed to open file '%s'", filename);
        return;
    }

    dword size = 0;
    if (!file_get_size(&info, &size))
    {
        c_console::write_line("networking:configuration: failed to determine file size for file '%s'", filename);
        file_close(&info);
        return;
    }

    byte buffer[0x400]{};
    csmemset(buffer, 0, sizeof(buffer));

    if (size > sizeof(buffer))
    {
        c_console::write_line("networking:configuration: invalid file size for '%s' (%ld bytes/%ld max)", filename, size, sizeof(buffer));
        file_close(&info);
        return;
    }

    if (!file_read(&info, size, false, buffer))
    {
        c_console::write_line("networking:configuration: failed to read from file '%s'", filename);
        file_close(&info);
        return;
    }

    c_map_variant* map_variant = new c_map_variant();
    if (!multiplayer_game_hopper_unpack_map_variant(buffer, size, map_variant))
    {
        c_console::write_line("networking:configuration: failed to unpack map variant in file '%s'", filename);

        delete map_variant;
        file_close(&info);
        return;
    }

    if (!map_variant->validate())
    {
        c_console::write_line("networking:configuration: map variant in file '%s' is invalid", filename);

        delete map_variant;
        file_close(&info);
        return;
    }

    c_console::write_line("networking:configuration: CONGRATULATIONS! variant file '%s' is valid", filename);

    if (load_and_use)
    {
        if (!network_squad_session_set_map_variant(map_variant))
        {
            c_console::write_line("networking:configuration: failed to set session map variant traits, probably not in a session");
        }
    }

    delete map_variant;
    file_close(&info);
}

void __cdecl network_load_and_use_packed_map_variant_file(char const* filename)
{
    network_verify_or_load_and_use_packed_map_variant_file(filename, true);
}

void __cdecl network_verify_packed_map_variant_file(char const* filename)
{
    network_verify_or_load_and_use_packed_map_variant_file(filename, false);
}

