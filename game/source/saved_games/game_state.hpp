#pragma once

#include "cache/optional_cache.hpp"
#include "cseries/cseries.hpp"
#include "game/game_options.hpp"
#include "memory/secure_signature.hpp"
#include "scenario/scenario.hpp"

enum f_game_state_revert_flags
{
	_game_state_revert_by_scripting_flag = FLAG(_game_state_revert_by_scripting_bit),
	_game_state_revert_keep_playing_cinematic_outros_flag = FLAG(_game_state_revert_keep_playing_cinematic_outros_bit),

	k_no_game_state_revert_flags = 0,
};

enum f_game_state_proc_flags
{
	_flush_physics_environment_flag = FLAG(_flush_physics_environment_bit),
	_flush_networking_flag = FLAG(_flush_networking_bit),
	_rebuild_players_flag = FLAG(_rebuild_players_bit),
	_doesnt_effect_location_after_death_flag = FLAG(_doesnt_effect_location_after_death_bit),
	_restored_by_scripting_flag = FLAG(_restored_by_scripting_bit),
	_keep_playing_cinematic_outros_flag = FLAG(_keep_playing_cinematic_outros_bit),
	_dont_cancel_pending_game_save_flag = FLAG(_dont_cancel_pending_game_save_bit),
	_dont_rebuild_havok_flag = FLAG(_dont_rebuild_havok_bit),
	_use_insecure_signature_flag = FLAG(_use_insecure_signature_bit),
	_defer_writing_signature_flag = FLAG(_defer_writing_signature_bit),
	_disable_campaign_metagame_flag = FLAG(_disable_campaign_metagame_bit),
	_dont_save_gpu_effect_data_flag = FLAG(_dont_save_gpu_effect_data_bit),

	k_no_game_state_proc_flags = 0,
};

enum _game_state_test_option
{
	GAME_STATE_TEST_OPTION_DEFAULT = 0,
	GAME_STATE_TEST_OPTION_REPRO,
	GAME_STATE_TEST_OPTION_STRESS,

	k_game_state_test_option_count,
};

enum
{
	k_saved_game_storage_max_count = 2
};

class c_game_state_compressor_callback :
	public c_optional_cache_user_callback
{
};
static_assert(sizeof(c_game_state_compressor_callback) == sizeof(c_optional_cache_user_callback));

class c_restricted_memory;
class c_restricted_memory_callbacks
{
public:
	c_restricted_memory_callbacks();

	virtual unsigned int filter_size_request(unsigned int in_size);
	virtual int32 filter_base_offset(int32 base_offset);
	virtual void handle_allocation(const c_restricted_memory* manager, const char* name, const char* type_name, int32 member_index, void* primary_address, unsigned int size);
	virtual void handle_release(const c_restricted_memory* manager, int32 member_index, void* primary_address, unsigned int size);
};

class c_gamestate_deterministic_allocation_callbacks :
	public c_restricted_memory_callbacks
{
public:
	c_gamestate_deterministic_allocation_callbacks();

	virtual unsigned int filter_size_request(unsigned int in_size) override;
	virtual void handle_allocation(const c_restricted_memory* manager, const char* name, const char* type_name, int32 member_index, void* primary_address, unsigned int size) override;
	virtual void handle_release(const c_restricted_memory* manager, int32 member_index, void* primary_address, unsigned int size) override;
	virtual int32 filter_base_offset(int32 base_offset, int32 total_allocation_space);
};

class c_gamestate_nondeterministic_allocation_callbacks :
	public c_restricted_memory_callbacks
{
public:
	c_gamestate_nondeterministic_allocation_callbacks();

	virtual unsigned int filter_size_request(unsigned int in_size) override;
	virtual int32 filter_base_offset(int32 base_offset) override;
	virtual void handle_allocation(const c_restricted_memory* manager, const char* name, const char* type_name, int32 member_index, void* primary_address, unsigned int size) override;
	virtual void handle_release(const c_restricted_memory* manager, int32 member_index, void* primary_address, unsigned int size) override;
};

class c_gamestate_allocation_record_allocation_callbacks :
	public c_restricted_memory_callbacks
{
public:
	c_gamestate_allocation_record_allocation_callbacks();

	virtual void handle_allocation(const c_restricted_memory* manager, const char* name, const char* type_name, int32 member_index, void* primary_address, unsigned int size) override;
};

class c_game_state_compressor
{
public:
	void initialize();

protected:
	bool m_locked;
	bool m_lock_pending;
	char* m_scratch_buffer;
	int32 m_scratch_buffer_size;
	char* m_compressed_game_state_buffer;
	int32 m_compressed_game_state_buffer_size;
	int32 m_compressed_game_state_size;
	int32 m_status;
};
static_assert(sizeof(c_game_state_compressor) == 0x1C);

struct game_state_header
{
	uns32 allocation_size_checksum;
	int32 unused;
	char map_name[256];
	char build_number[32];
	uns32 cache_file_checksum;
	game_options options;
	s_scenario_game_state scenario_game_state;
	int32 unused2[4];
	uns8 random_salt[20];
	s_network_http_request_hash gamestate_hash;
};
static_assert(sizeof(game_state_header) == 0x24CD0);
static_assert(0x00000 == OFFSETOF(game_state_header, allocation_size_checksum));
static_assert(0x00004 == OFFSETOF(game_state_header, unused));
static_assert(0x00008 == OFFSETOF(game_state_header, map_name));
static_assert(0x00108 == OFFSETOF(game_state_header, build_number));
static_assert(0x00128 == OFFSETOF(game_state_header, cache_file_checksum));
static_assert(0x00130 == OFFSETOF(game_state_header, options));
static_assert(0x24C78 == OFFSETOF(game_state_header, scenario_game_state));
static_assert(0x24C98 == OFFSETOF(game_state_header, unused2));
static_assert(0x24CA8 == OFFSETOF(game_state_header, random_salt));
static_assert(0x24CBC == OFFSETOF(game_state_header, gamestate_hash));

struct s_game_state_globals
{
	bool initialized;
	bool allocations_locked;
	bool saved_game_valid;
	bool fallback_valid;

	int32 runtime_saved_game_storage_count;
	int32 current_saved_game_index;
	game_state_header saved_game_header[k_saved_game_storage_max_count];

	uns32 game_time_at_last_save;
	int16 number_of_bad_reverts;

	void* base_address;
	void* base_address_backup;

	byte __data499C0[0x4];

	uns32 guard_page_size;
	uns32 allocation_size_checksum;
	int32 revert_time;

	game_state_header* header;

	static _game_state_test_option game_state_test_option;
	bool loaded_from_persistent_storage;

	c_game_state_compressor game_state_compressor;
};

extern c_game_state_compressor_callback& g_game_state_compressor_optional_cache_callback;
extern c_gamestate_deterministic_allocation_callbacks g_gamestate_deterministic_allocation_callbacks;
extern c_gamestate_nondeterministic_allocation_callbacks g_gamestate_nondeterministic_allocation_callbacks;
extern c_gamestate_allocation_record_allocation_callbacks g_gamestate_allocation_record_allocation_callbacks;
extern s_game_state_globals& game_state_globals;

struct s_player_identifier;
extern bool __cdecl create_file_from_buffer(const char* file_name, const char* file_contents);
extern void __cdecl game_state_allocation_record(int32 region_index, const char* name, const char* type, int32 allocation_size);
extern void __cdecl game_state_buffer_handle_read();
extern bool __cdecl game_state_can_save_to_persistent_storage();
extern bool __cdecl game_state_compressor_lock_pending();
extern void __cdecl game_state_compressor_lock_update();
extern bool __cdecl game_state_debug_server_file_uploading_enabled(c_static_string<256>* reason);
extern void __cdecl game_state_dispose();
extern void __cdecl game_state_dispose_from_old_map();
extern void __cdecl game_state_dispose_from_old_non_bsp_zone_set(const s_game_non_bsp_zone_set* old_non_bsp_zone_set);
extern void __cdecl game_state_dispose_from_old_structure_bsp(uns32 deactivating_structure_bsp_mask);
extern const void* __cdecl game_state_get_buffer_address(int32* buffer_size);
extern c_game_state_compressor* __cdecl game_state_get_compressor();
extern bool __cdecl game_state_get_game_options_from_core(const char* core_name, game_options* options);
extern bool __cdecl game_state_get_scenario_path_from_core(const char* core_name, char* buffer, int32 buffer_length);
extern const char* __cdecl game_state_get_test_option_string();
extern bool __cdecl game_state_header_matches(const game_state_header* current_header, const game_state_header* file_header);
extern bool __cdecl game_state_header_matches_current_network_session_settings(const game_state_header* header);
extern bool __cdecl game_state_header_prepare_to_load(game_state_header* header, bool allow_networking, bool allow_change_maps, bool* out_changed_maps);
extern bool __cdecl game_state_header_valid(const game_state_header* header);
extern bool __cdecl game_state_header_valid_for_saving(const game_state_header* header);
extern void __cdecl game_state_initialize();
extern void __cdecl game_state_initialize_for_new_map();
extern void __cdecl game_state_initialize_for_new_non_bsp_zone_set(const s_game_non_bsp_zone_set* new_non_bsp_zone_set);
extern void __cdecl game_state_initialize_for_new_structure_bsp(uns32 activating_structure_bsp_mask);
extern bool __cdecl game_state_is_locked();
extern void __cdecl game_state_load_core(const char* name);
extern void __cdecl game_state_lock(bool locked);
extern void __cdecl game_state_prepare_for_revert();
extern void __cdecl game_state_preserve();
extern bool __cdecl game_state_read_core(const char* core_name, void* buffer, uns32 buffer_length);
extern bool __cdecl game_state_read_from_persistent_storage_blocking(e_controller_index controller_index, void* buffer, uns32 buffer_size);
extern bool __cdecl game_state_read_header_from_persistent_storage_blocking(e_controller_index controller_index, game_state_header* header);
extern void __cdecl game_state_reset_mapping(int32 game_state_proc_flags);
extern void __cdecl game_state_revert(uns32 flags);
extern bool __cdecl game_state_reverted();
extern void __cdecl game_state_save();
extern void __cdecl game_state_save_core(const char* name);
extern void __cdecl game_state_save_to_persistent_storage_blocking();
extern void __cdecl game_state_security_invalidate_signature();
extern bool __cdecl game_state_security_verify_signature_insecure(void* header_address);
extern bool __cdecl game_state_security_verify_signature_internal(void* header_address, bool insecure);
extern bool __cdecl game_state_security_verify_signature_secure(void* header_address);
extern void __cdecl game_state_security_write_signature(bool insecure, void* optional_base_address);
extern void __cdecl game_state_set_header_address(void* header_address);
extern void __cdecl game_state_set_revert_time();
extern void __cdecl game_state_set_test_options(const char* test_type);
extern void __cdecl game_state_shell_dispose();
extern void __cdecl game_state_shell_gobble_first_physical_allocation();
extern void __cdecl game_state_shell_initialize();
extern void __cdecl game_state_try_and_load_from_persistent_storage(e_controller_index controller_index);
extern bool __cdecl game_state_validate_and_prepare_to_load_header(game_state_header* header);
extern const void* __cdecl game_state_with_mirrors_get_buffer_address(int32* buffer_size);
extern bool __cdecl game_state_write_core(const char* core_name, const void* buffer, uns32 buffer_length);
extern void __cdecl game_state_write_to_persistent_storage_blocking(const game_state_header* header, int32 header_size, const void* buffer, int32 buffer_length);
extern void __cdecl initialize_game_state_section(int32 section_index, unsigned int size, uns8** starting_address, uns32* available_memory, e_critical_sections critical_section_id);
extern bool __cdecl player_identifier_exists_in_game_header(const s_player_identifier* player_identifier, const game_state_header* header);

