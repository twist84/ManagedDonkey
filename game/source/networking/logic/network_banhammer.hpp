#pragma once

#include "cseries/cseries.hpp"
#include "memory/secure_signature.hpp"
#include "networking/logic/network_bandwidth.hpp"
#include "networking/logic/storage/network_http_buffer_downloader.hpp"
#include "game/players.hpp"

#pragma pack(push, 1)
struct s_network_web_event_cheating_local_player
{
	bool caught_cheating;
	byte __pad1[0x7];
	s_player_identifier player_identifier;

	qword machine_id;

	wchar_t name[16];

	qword game_instance;

	dword_flags cheat_flags;
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
	dword_flags controller_cheat_flags;
	dword_flags controller_new_cheat_flags;

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
	byte_flags flags;
	byte __pad1[3];

	byte __data4[0x4];

	qword player_xuid;
	wchar_t name[16];
	dword_flags old_ban_flags;
	dword_flags new_ban_flags;
	dword_flags old_cheat_flags;
	dword_flags new_cheat_flags;
	dword repeated_play_coefficient;

	byte __data44[0xF8C];

	long highest_skill;

	byte __dataFD4[0x4];
};
static_assert(sizeof(s_network_banhammer_controller_data) == 0xFD8);

struct s_map_file_manifest
{
	long signature_counts;
	s_rsa_signature signatures[128];
};
static_assert(sizeof(s_map_file_manifest) == 0x8004);

struct s_network_banhammer_globals
{
	// e_network_banhammer_flags
	byte_flags controller_flags;
	byte __pad1[0x3];

	// e_network_banhammer_cheat_flags
	dword_flags controller_cheat_flags;

	long map_id;

	// e_network_banhammer_ban_flags
	dword_flags controller_ban_flags;
	dword_flags controller_ban_message_flags;

	// pad?
	byte __data14[0x4];

	c_static_array<s_network_banhammer_controller_data, 4> controllers;
	c_static_array<s_network_banhammer_remote_cheater_data, 64> remote_cheaters;

	long __time5178;
	long __time517C;
	bool __unknown5180;
	bool __unknown5181;
	byte __data5182[0x2];

	long network_map_signatures_downloader_attempt_index;
	s_map_file_manifest map_file_manifest;

	c_http_stored_buffer_downloader<34977> network_map_signatures_downloader;
	byte __pad160C5[0x3];

	byte __data160C8[0xC];
	bool __unknown160D4;
	bool __unknown160D5;

	s_rsa_signature map_signature;

	byte __data161D6[0x2];

	c_http_stored_buffer_downloader<2161> matchmaking_machine_downloader;
	byte __pad170E1[0x3];

	c_http_stored_buffer_downloader<10401> matchmaking_banhammer_message_downloader;
	byte __pad1A01D[0x3];

	byte __data1A020[0xC];

	c_http_stored_buffer_downloader<4376> controller_downloaders[4];
	c_http_stored_buffer_downloader<1024> update_machine_network_stats;

	byte __data20984[0x4];
	s_network_quality_statistics quality_statistics;
};
#pragma pack(pop)
static_assert(sizeof(s_network_banhammer_globals) == 0x20A48);
static_assert(0x00000 == OFFSETOF(s_network_banhammer_globals, controller_flags));
static_assert(0x00004 == OFFSETOF(s_network_banhammer_globals, controller_cheat_flags));
static_assert(0x00008 == OFFSETOF(s_network_banhammer_globals, map_id));
static_assert(0x0000C == OFFSETOF(s_network_banhammer_globals, controller_ban_flags));
static_assert(0x00010 == OFFSETOF(s_network_banhammer_globals, controller_ban_message_flags));
static_assert(0x00014 == OFFSETOF(s_network_banhammer_globals, __data14));
static_assert(0x00018 == OFFSETOF(s_network_banhammer_globals, controllers));
static_assert(0x03F78 == OFFSETOF(s_network_banhammer_globals, remote_cheaters));
static_assert(0x05178 == OFFSETOF(s_network_banhammer_globals, __time5178));
static_assert(0x0517C == OFFSETOF(s_network_banhammer_globals, __time517C));
static_assert(0x05180 == OFFSETOF(s_network_banhammer_globals, __unknown5180));
static_assert(0x05181 == OFFSETOF(s_network_banhammer_globals, __unknown5181));
static_assert(0x05182 == OFFSETOF(s_network_banhammer_globals, __data5182));
static_assert(0x05184 == OFFSETOF(s_network_banhammer_globals, network_map_signatures_downloader_attempt_index));
static_assert(0x05188 == OFFSETOF(s_network_banhammer_globals, map_file_manifest));
static_assert(0x0D18C == OFFSETOF(s_network_banhammer_globals, network_map_signatures_downloader));
static_assert(0x160C5 == OFFSETOF(s_network_banhammer_globals, __pad160C5));
static_assert(0x160C8 == OFFSETOF(s_network_banhammer_globals, __data160C8));
static_assert(0x160D4 == OFFSETOF(s_network_banhammer_globals, __unknown160D4));
static_assert(0x160D5 == OFFSETOF(s_network_banhammer_globals, __unknown160D5));
static_assert(0x160D6 == OFFSETOF(s_network_banhammer_globals, map_signature));
static_assert(0x161D6 == OFFSETOF(s_network_banhammer_globals, __data161D6));
static_assert(0x161D8 == OFFSETOF(s_network_banhammer_globals, matchmaking_machine_downloader));
static_assert(0x170E1 == OFFSETOF(s_network_banhammer_globals, __pad170E1));
static_assert(0x170E4 == OFFSETOF(s_network_banhammer_globals, matchmaking_banhammer_message_downloader));
static_assert(0x1A01D == OFFSETOF(s_network_banhammer_globals, __pad1A01D));
static_assert(0x1A020 == OFFSETOF(s_network_banhammer_globals, __data1A020));
static_assert(0x1A02C == OFFSETOF(s_network_banhammer_globals, controller_downloaders));
static_assert(0x1FEEC == OFFSETOF(s_network_banhammer_globals, update_machine_network_stats));
static_assert(0x20984 == OFFSETOF(s_network_banhammer_globals, __data20984));
static_assert(0x20988 == OFFSETOF(s_network_banhammer_globals, quality_statistics));

extern bool& g_network_banhammer_initialized;
extern s_network_banhammer_globals& g_network_banhammer_globals;

extern bool __cdecl network_banhammer_initialize();
extern void __cdecl network_banhammer_update();

