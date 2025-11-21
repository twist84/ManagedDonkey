#pragma once

#include "cseries/cseries.hpp"
#include "memory/secure_signature.hpp"
#include "networking/logic/network_bandwidth.hpp"
#include "networking/logic/storage/network_http_buffer_downloader.hpp"
#include "game/players.hpp"

struct s_network_web_event_cheating_local_player
{
	bool caught_cheating;
	byte __pad1[0x7];
	s_player_identifier player_identifier;

	uns64 machine_id;

	wchar_t name[16];

	uns64 game_instance;

	uns32 cheat_flags;
	bool webstats_submitted;
	byte _pad45[0x3];
};
static_assert(sizeof(s_network_web_event_cheating_local_player) == 0x48);

struct s_network_banhammer_remote_cheater_data :
	s_network_web_event_cheating_local_player
{
};
static_assert(sizeof(s_network_banhammer_remote_cheater_data) == sizeof(s_network_web_event_cheating_local_player));

struct s_network_banhammer_cheating_report
{
	uns32 controller_cheat_flags;
	uns32 controller_new_cheat_flags;

	// set from `g_network_banhammer_globals.__unknown160D4`
	bool __unknown8;

	// set from `g_network_banhammer_globals.__unknown160D5`
	bool __unknown9;

	s_rsa_signature map_signature;

	byte __data10A[0x6];
};
static_assert(sizeof(s_network_banhammer_cheating_report) == 0x110);

struct s_network_banhammer_controller_data
{
	// e_network_banhammer_controller_flags
	// 0: repeated play list?
	uns8 flags;
	byte __pad1[0x3];

	byte __data4[0x4];

	uns64 player_xuid;
	wchar_t name[16];
	uns32 old_ban_flags;
	uns32 new_ban_flags;
	uns32 old_cheat_flags;
	uns32 new_cheat_flags;
	uns32 repeated_play_coefficient;

	byte __data44[0xF8C];

	int32 highest_skill;

	byte __dataFD4[0x4];
};
static_assert(sizeof(s_network_banhammer_controller_data) == 0xFD8);

struct s_map_file_manifest_entry
{
	char signature[256];
};
static_assert(sizeof(s_map_file_manifest_entry) == 0x100);

struct s_map_file_manifest
{
	int32 entry_count;
	s_map_file_manifest_entry entries[128];
};
static_assert(sizeof(s_map_file_manifest) == 0x8004);

struct s_files_user_banhammer_messages
{
};

struct __declspec(align(8)) s_network_banhammer_controller_downloaders
{
	c_http_stored_buffer_downloader<4373> user_downloader;
};
static_assert(sizeof(s_network_banhammer_controller_downloaders) == 0x17B0);

struct s_network_banhammer_globals
{
	// e_network_banhammer_flags
	uns8 controller_flags;
	byte __pad1[0x3];

	// e_network_banhammer_cheat_flags
	uns32 controller_cheat_flags;

	e_map_id map_id;

	// e_network_banhammer_ban_flags
	uns32 controller_ban_flags;
	uns32 controller_ban_message_flags;

	// pad?
	byte __data14[0x4];

	c_static_array<s_network_banhammer_controller_data, 4> controllers;
	c_static_array<s_network_banhammer_remote_cheater_data, 64> remote_cheaters;
	uns32 machine_file_download_time;
	uns32 machine_file_refresh_time;
	bool map_file_manifest_valid;
	bool map_file_manifest_failed;
	int32 map_file_manifest_attempt_index;
	s_map_file_manifest current_map_file_manifest;
	c_http_blf_simple_downloader<s_map_file_manifest, 34977> map_file_manifest_downloader;
	bool __unknown160D4;
	bool __unknown160D5;
	s_rsa_signature map_signature;
	c_http_stored_buffer_downloader<2161> machine_downloader;
	c_http_blf_simple_downloader<s_files_user_banhammer_messages, 10401> message_downloader;
	c_static_array<s_network_banhammer_controller_downloaders, 4> controller_downloaders;
	c_http_stored_buffer_downloader<1024> machine_uploader;
	byte __data20984[0x4];
	s_network_quality_statistics network_statistics_upload_buffer;
};
static_assert(sizeof(c_http_stored_buffer_downloader<34977>) == 0x8F3C);
static_assert(sizeof(s_network_banhammer_globals) == 0x20A48);
static_assert(0x00000 == OFFSETOF(s_network_banhammer_globals, controller_flags));
static_assert(0x00004 == OFFSETOF(s_network_banhammer_globals, controller_cheat_flags));
static_assert(0x00008 == OFFSETOF(s_network_banhammer_globals, map_id));
static_assert(0x0000C == OFFSETOF(s_network_banhammer_globals, controller_ban_flags));
static_assert(0x00010 == OFFSETOF(s_network_banhammer_globals, controller_ban_message_flags));
static_assert(0x00014 == OFFSETOF(s_network_banhammer_globals, __data14));
static_assert(0x00018 == OFFSETOF(s_network_banhammer_globals, controllers));
static_assert(0x03F78 == OFFSETOF(s_network_banhammer_globals, remote_cheaters));
static_assert(0x05178 == OFFSETOF(s_network_banhammer_globals, machine_file_download_time));
static_assert(0x0517C == OFFSETOF(s_network_banhammer_globals, machine_file_refresh_time));
static_assert(0x05180 == OFFSETOF(s_network_banhammer_globals, map_file_manifest_valid));
static_assert(0x05181 == OFFSETOF(s_network_banhammer_globals, map_file_manifest_failed));
static_assert(0x05184 == OFFSETOF(s_network_banhammer_globals, map_file_manifest_attempt_index));
static_assert(0x05188 == OFFSETOF(s_network_banhammer_globals, current_map_file_manifest));
static_assert(0x0D18C == OFFSETOF(s_network_banhammer_globals, map_file_manifest_downloader));
static_assert(0x160D4 == OFFSETOF(s_network_banhammer_globals, __unknown160D4));
static_assert(0x160D5 == OFFSETOF(s_network_banhammer_globals, __unknown160D5));
static_assert(0x160D6 == OFFSETOF(s_network_banhammer_globals, map_signature));
static_assert(0x161D8 == OFFSETOF(s_network_banhammer_globals, machine_downloader));
static_assert(0x170E4 == OFFSETOF(s_network_banhammer_globals, message_downloader));
static_assert(0x1A030 == OFFSETOF(s_network_banhammer_globals, controller_downloaders));
static_assert(0x1FEF0 == OFFSETOF(s_network_banhammer_globals, machine_uploader));
static_assert(0x20988 == OFFSETOF(s_network_banhammer_globals, network_statistics_upload_buffer));


extern bool& g_network_banhammer_initialized;
extern s_network_banhammer_globals& g_network_banhammer_globals;

extern bool __cdecl network_banhammer_initialize();
extern void __cdecl network_banhammer_update();
extern void __cdecl network_banhammer_upload_network_statistics();

