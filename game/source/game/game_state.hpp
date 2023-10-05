#pragma once

#include "cseries/cseries.hpp"
#include "game/game_options.hpp"
#include "scenario/scenario.hpp"

struct c_game_state_compressor
{
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
	dword cookie[2];
	c_static_string<k_tag_long_string_length> scenario_path;
	c_static_string<k_tag_string_length> build_string;
	dword map_file_checksum;
	byte __data134[0x4];
	game_options options;
	s_scenario_game_state game_state;
	byte __data24C98[0x38];
};
static_assert(sizeof(s_game_state_header) == 0x24CD0);

struct s_game_state_globals
{
	bool initialized;
	bool locked;
	bool reverted;
	bool prepared_for_revert;

	long game_state_count;
	long game_state_index;
	c_static_array<s_game_state_header, 2> game_states;

	dword game_state_save_time;
	short game_state_save_count;

	void* base_address;
	void* game_state_backup_address;

	byte __data499C0[4];

	dword cookie;
	dword checksum;

	dword __unknown499CC;

	s_game_state_header* header;

	bool __unknown499D4;

	c_game_state_compressor compressor;
};

extern s_game_state_globals& game_state_globals;

extern void __cdecl game_state_prepare_for_revert();

