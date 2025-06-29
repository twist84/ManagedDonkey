#pragma once

#include "cache/optional_cache.hpp"
#include "cseries/cseries.hpp"
#include "game/game_options.hpp"
#include "scenario/scenario.hpp"

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

	virtual unsigned int filter_size_request(unsigned int size);
	virtual int32 filter_base_offset(int32 a1);
	virtual void handle_allocation(const c_restricted_memory* memory, const char* name, const char* type, int32 member_index, void* base_address, unsigned int allocation_size);
	virtual void handle_release(const c_restricted_memory* memory, int32 member_index, void* base_address, unsigned int allocation_size);
};

class c_gamestate_deterministic_allocation_callbacks :
	public c_restricted_memory_callbacks
{
public:
	c_gamestate_deterministic_allocation_callbacks();

	virtual unsigned int filter_size_request(unsigned int size) override;
	virtual void handle_allocation(const c_restricted_memory* memory, const char* name, const char* type, int32 member_index, void* base_address, unsigned int allocation_size) override;
	virtual void handle_release(const c_restricted_memory* memory, int32 member_index, void* base_address, unsigned int allocation_size) override;
	virtual int32 filter_base_offset(int32 a1, int32 a2);
};

class c_gamestate_nondeterministic_allocation_callbacks :
	public c_restricted_memory_callbacks
{
public:
	c_gamestate_nondeterministic_allocation_callbacks();

	virtual unsigned int filter_size_request(unsigned int size) override;
	virtual int32 filter_base_offset(int32 a1) override;
	virtual void handle_allocation(const c_restricted_memory* memory, const char* name, const char* type, int32 member_index, void* base_address, unsigned int allocation_size) override;
	virtual void handle_release(const c_restricted_memory* memory, int32 member_index, void* base_address, unsigned int allocation_size) override;
};

class c_gamestate_allocation_record_allocation_callbacks :
	public c_restricted_memory_callbacks
{
public:
	c_gamestate_allocation_record_allocation_callbacks();

	virtual void handle_allocation(const c_restricted_memory* memory, const char* name, const char* type, int32 member_index, void* base_address, unsigned int allocation_size) override;
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

struct s_game_state_header
{
	uns32 cookie[2];
	c_static_string<k_tag_long_string_length> scenario_path;
	c_static_string<k_tag_string_length> build_string;
	uns32 map_file_checksum;
	byte __data134[0x4];
	game_options options;
	s_scenario_game_state game_state;
	byte __data24C98[0x38];
};
static_assert(sizeof(s_game_state_header) == 0x24CD0);

struct s_game_state_globals
{
	bool initialized;
	bool allocations_locked;
	bool reverted;
	bool prepared_for_revert;

	int32 runtime_saved_game_storage_count;
	int32 current_saved_game_index;
	c_static_array<s_game_state_header, k_saved_game_storage_max_count> saved_game_header;

	int32 saved_game_time;
	int16 saved_game_failed_revert_count;

	void* base_address;
	void* base_address_backup;

	byte __data499C0[0x4];

	uns32 guard_page_size;
	uns32 checksum;
	int32 revert_time;

	s_game_state_header* header;

	static int32 test_option;
	bool force_immediate_save_on_core_load;

	c_game_state_compressor compressor;
};

extern c_game_state_compressor_callback& g_game_state_compressor_optional_cache_callback;
extern c_gamestate_deterministic_allocation_callbacks g_gamestate_deterministic_allocation_callbacks;
extern c_gamestate_nondeterministic_allocation_callbacks g_gamestate_nondeterministic_allocation_callbacks;
extern c_gamestate_allocation_record_allocation_callbacks g_gamestate_allocation_record_allocation_callbacks;
extern s_game_state_globals& game_state_globals;

struct s_player_identifier;
extern bool __cdecl create_file_from_buffer(const char* file_name, const char* file_contents);
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
extern bool __cdecl game_state_header_matches(const s_game_state_header* header_a, const s_game_state_header* header_b);
extern bool __cdecl game_state_header_matches_current_network_session_settings(const s_game_state_header* header);
extern bool __cdecl game_state_header_prepare_to_load(s_game_state_header* header, bool a2, bool a3, bool* a4);
extern bool __cdecl game_state_header_valid(const s_game_state_header* header);
extern bool __cdecl game_state_header_valid_for_saving(const s_game_state_header* header);
extern void __cdecl game_state_initialize();
extern void __cdecl game_state_initialize_for_new_map();
extern void __cdecl game_state_initialize_for_new_non_bsp_zone_set(const s_game_non_bsp_zone_set* new_non_bsp_zone_set);
extern void __cdecl game_state_initialize_for_new_structure_bsp(uns32 activating_structure_bsp_mask);
extern bool __cdecl game_state_is_locked();
extern void __cdecl game_state_load_core(const char* core_name);
extern void __cdecl game_state_lock(bool a1);
extern void __cdecl game_state_prepare_for_revert();
extern void __cdecl game_state_preserve();
extern bool __cdecl game_state_read_core(const char* core_name, void* buffer, uns32 buffer_length);
extern bool __cdecl game_state_read_from_persistent_storage_blocking(e_controller_index controller_index, void* buffer, uns32 buffer_size);
extern bool __cdecl game_state_read_header_from_persistent_storage_blocking(e_controller_index controller_index, s_game_state_header* header);
extern void __cdecl game_state_reset_mapping(int32 a1);
extern void __cdecl game_state_revert(uns32 flags);
extern bool __cdecl game_state_reverted();
extern void __cdecl game_state_save();
extern void __cdecl game_state_save_core(const char* core_name);
extern void __cdecl game_state_save_to_persistent_storage_blocking();
extern void __cdecl game_state_security_invalidate_signature();
extern bool __cdecl game_state_security_verify_signature_insecure(void* header_address);
extern bool __cdecl game_state_security_verify_signature_internal(void* header_address, bool insecure);
extern bool __cdecl game_state_security_verify_signature_secure(void* header_address);
extern void __cdecl game_state_security_write_signature(bool a1, void* a2);
extern void __cdecl game_state_set_header_address(void* header_address);
extern void __cdecl game_state_set_revert_time();
extern void __cdecl game_state_set_test_options(const char* test_type);
extern void __cdecl game_state_shell_dispose();
extern void __cdecl game_state_shell_gobble_first_physical_allocation();
extern void __cdecl game_state_shell_initialize();
extern void __cdecl game_state_try_and_load_from_persistent_storage(e_controller_index controller_index);
extern bool __cdecl game_state_validate_and_prepare_to_load_header(s_game_state_header* header);
extern const void* __cdecl game_state_with_mirrors_get_buffer_address(int32* buffer_size);
extern bool __cdecl game_state_write_core(const char* core_name, const void* buffer, uns32 buffer_length);
extern void __cdecl game_state_write_to_persistent_storage_blocking(const s_game_state_header* header, int32 header_size, const void* buffer, int32 buffer_length);
extern void __cdecl initialize_game_state_section(int32 section_index, unsigned int size, uns8** starting_address, uns32* available_memory, e_critical_sections critical_section_id);
extern bool __cdecl player_identifier_exists_in_game_header(const s_player_identifier* player_identifier, const s_game_state_header* header);
extern void __cdecl game_state_allocation_record(int32 region_index, const char* name, const char* type, int32 allocation_size);

