#pragma once

#include "cache/optional_cache.hpp"
#include "cseries/cseries.hpp"
#include "game/game_options.hpp"
#include "scenario/scenario.hpp"

enum
{
	k_game_state_maximum_compressed_size = 0x4F0000,
	k_saved_game_storage_max_count = 2
};

struct c_game_state_compressor_callback :
	public c_optional_cache_user_callback
{
};
static_assert(sizeof(c_game_state_compressor_callback) == sizeof(c_optional_cache_user_callback));

struct c_restricted_memory;
struct c_restricted_memory_callbacks
{
public:
	c_restricted_memory_callbacks();

	virtual unsigned int filter_size_request(unsigned int size);
	virtual long filter_base_offset(long a1);
	virtual void handle_allocation(c_restricted_memory const* memory, char const* name, char const* type, long member_index, void* base_address, unsigned int allocation_size);
	virtual void handle_release(c_restricted_memory const* memory, long member_index, void* base_address, unsigned int allocation_size);
};

struct c_gamestate_deterministic_allocation_callbacks :
	public c_restricted_memory_callbacks
{
public:
	c_gamestate_deterministic_allocation_callbacks();

	virtual unsigned int filter_size_request(unsigned int size) override;
	virtual void handle_allocation(c_restricted_memory const* memory, char const* name, char const* type, long member_index, void* base_address, unsigned int allocation_size) override;
	virtual void handle_release(c_restricted_memory const* memory, long member_index, void* base_address, unsigned int allocation_size) override;
	virtual long filter_base_offset(long a1, long a2);
};

struct c_gamestate_nondeterministic_allocation_callbacks :
	public c_restricted_memory_callbacks
{
public:
	c_gamestate_nondeterministic_allocation_callbacks();

	virtual unsigned int filter_size_request(unsigned int size) override;
	virtual long filter_base_offset(long a1) override;
	virtual void handle_allocation(c_restricted_memory const* memory, char const* name, char const* type, long member_index, void* base_address, unsigned int allocation_size) override;
	virtual void handle_release(c_restricted_memory const* memory, long member_index, void* base_address, unsigned int allocation_size) override;
};

struct c_gamestate_allocation_record_allocation_callbacks :
	public c_restricted_memory_callbacks
{
public:
	c_gamestate_allocation_record_allocation_callbacks();

	virtual void handle_allocation(c_restricted_memory const* memory, char const* name, char const* type, long member_index, void* base_address, unsigned int allocation_size) override;
};

struct c_game_state_compressor
{
public:
	void initialize();

protected:
	bool m_locked;
	bool m_lock_pending;
	char* m_scratch_buffer;
	long m_scratch_buffer_size;
	char* m_compressed_game_state_buffer;
	long m_compressed_game_state_buffer_size;
	long m_compressed_game_state_size;
	long m_status;
};
static_assert(sizeof(c_game_state_compressor) == 0x1C);

struct s_game_state_header
{
	uint32 cookie[2];
	c_static_string<k_tag_long_string_length> scenario_path;
	c_static_string<k_tag_string_length> build_string;
	uint32 map_file_checksum;
	uint8 __data134[0x4];
	game_options options;
	s_scenario_game_state game_state;
	uint8 __data24C98[0x38];
};
static_assert(sizeof(s_game_state_header) == 0x24CD0);

struct s_game_state_globals
{
	bool initialized;
	bool allocations_locked;
	bool reverted;
	bool prepared_for_revert;

	long runtime_saved_game_storage_count;
	long current_saved_game_index;
	c_static_array<s_game_state_header, k_saved_game_storage_max_count> saved_game_header;

	long saved_game_time;
	short saved_game_failed_revert_count;

	void* base_address;
	void* base_address_backup;

	uint8 __data499C0[4];

	uint32 guard_page_size;
	uint32 checksum;
	long revert_time;

	s_game_state_header* header;

	static long test_option;
	bool force_immediate_save_on_core_load;

	c_game_state_compressor compressor;
};

extern c_game_state_compressor_callback& g_game_state_compressor_optional_cache_callback;
extern c_gamestate_deterministic_allocation_callbacks g_gamestate_deterministic_allocation_callbacks;
extern c_gamestate_nondeterministic_allocation_callbacks g_gamestate_nondeterministic_allocation_callbacks;
extern c_gamestate_allocation_record_allocation_callbacks g_gamestate_allocation_record_allocation_callbacks;
extern s_game_state_globals& game_state_globals;

enum e_controller_index;
enum e_critical_sections;
struct s_player_identifier;
extern bool __cdecl create_file_from_buffer(char const* file_name, char const* file_contents);
extern void __cdecl game_state_buffer_handle_read();
extern bool __cdecl game_state_can_save_to_persistent_storage();
extern bool __cdecl game_state_compressor_lock_pending();
extern void __cdecl game_state_compressor_lock_update();
extern bool __cdecl game_state_debug_server_file_uploading_enabled(c_static_string<256>* reason);
extern void __cdecl game_state_dispose();
extern void __cdecl game_state_dispose_from_old_map();
extern void __cdecl game_state_dispose_from_old_non_bsp_zone_set(s_game_non_bsp_zone_set const* old_non_bsp_zone_set);
extern void __cdecl game_state_dispose_from_old_structure_bsp(uint32 deactivating_structure_bsp_mask);
extern void const* __cdecl game_state_get_buffer_address(long* buffer_size);
extern c_game_state_compressor* __cdecl game_state_get_compressor();
extern bool __cdecl game_state_get_game_options_from_core(char const* core_name, game_options* options);
extern bool __cdecl game_state_get_scenario_path_from_core(char const* core_name, char* buffer, long buffer_length);
extern char const* __cdecl game_state_get_test_option_string();
extern bool __cdecl game_state_header_matches(s_game_state_header const* header_a, s_game_state_header const* header_b);
extern bool __cdecl game_state_header_matches_current_network_session_settings(s_game_state_header const* header);
extern bool __cdecl game_state_header_prepare_to_load(s_game_state_header* header, bool a2, bool a3, bool* a4);
extern bool __cdecl game_state_header_valid(s_game_state_header const* header);
extern bool __cdecl game_state_header_valid_for_saving(s_game_state_header const* header);
extern void __cdecl game_state_initialize();
extern void __cdecl game_state_initialize_for_new_map();
extern void __cdecl game_state_initialize_for_new_non_bsp_zone_set(s_game_non_bsp_zone_set const* new_non_bsp_zone_set);
extern void __cdecl game_state_initialize_for_new_structure_bsp(uint32 activating_structure_bsp_mask);
extern bool __cdecl game_state_is_locked();
extern void __cdecl game_state_load_core(char const* core_name);
extern void __cdecl game_state_lock(bool a1);
extern void __cdecl game_state_prepare_for_revert();
extern void __cdecl game_state_preserve();
extern bool __cdecl game_state_read_core(char const* core_name, void* buffer, uint32 buffer_length);
extern bool __cdecl game_state_read_from_persistent_storage_blocking(e_controller_index controller_index, void* buffer, uint32 buffer_size);
extern bool __cdecl game_state_read_header_from_persistent_storage_blocking(e_controller_index controller_index, s_game_state_header* header);
extern void __cdecl game_state_reset_mapping(long a1);
extern void __cdecl game_state_revert(uint32 flags);
extern bool __cdecl game_state_reverted();
extern void __cdecl game_state_save();
extern void __cdecl game_state_save_core(char const* core_name);
extern void __cdecl game_state_save_to_persistent_storage_blocking();
extern void __cdecl game_state_security_invalidate_signature();
extern bool __cdecl game_state_security_verify_signature_insecure(void* header_address);
extern bool __cdecl game_state_security_verify_signature_internal(void* header_address, bool insecure);
extern bool __cdecl game_state_security_verify_signature_secure(void* header_address);
extern void __cdecl game_state_security_write_signature(bool a1, void* a2);
extern void __cdecl game_state_set_header_address(void* header_address);
extern void __cdecl game_state_set_revert_time();
extern void __cdecl game_state_set_test_options(char const* test_type);
extern void __cdecl game_state_shell_dispose();
extern void __cdecl game_state_shell_gobble_first_physical_allocation();
extern void __cdecl game_state_shell_initialize();
extern void __cdecl game_state_try_and_load_from_persistent_storage(e_controller_index controller_index);
extern bool __cdecl game_state_validate_and_prepare_to_load_header(s_game_state_header* header);
extern void const* __cdecl game_state_with_mirrors_get_buffer_address(long* buffer_size);
extern bool __cdecl game_state_write_core(char const* core_name, void const* buffer, uint32 buffer_length);
extern void __cdecl game_state_write_to_persistent_storage_blocking(s_game_state_header const* header, long header_size, void const* buffer, long buffer_length);
extern void __cdecl initialize_game_state_section(long section_index, unsigned int size, uint8** starting_address, uint32* available_memory, e_critical_sections critical_section_id);
extern bool __cdecl player_identifier_exists_in_game_header(s_player_identifier const* player_identifier, s_game_state_header const* header);
extern void __cdecl game_state_allocation_record(long region_index, char const* name, char const* type, long allocation_size);

