#pragma once

#include "cseries/cseries.hpp"
#include "game/players.hpp"
#include "networking/logic/storage/network_http_buffer_downloader.hpp"
#include "text/unicode.hpp"

enum e_campaign_progress
{
	_campaign_progress_complete_easy = 0,
	_campaign_progress_complete_normal,
	_campaign_progress_complete_hard,
	_campaign_progress_complete_impossible,
	_campaign_progress_none,
	_campaign_progress_partial_easy,
	_campaign_progress_partial_normal,
	_campaign_progress_partial_hard,
	_campaign_progress_partial_impossible,

	k_campaign_progress_complete_count,

	k_campaign_progress_complete_bits = 4
};

enum e_experience_rank
{
	_experience_rank_none = 0,
	_experience_rank_recruit,
	_experience_rank_apprentice,
	_experience_rank_private,
	_experience_rank_corporal,
	_experience_rank_sergeant,
	_experience_rank_gunnery_sergeant,
	_experience_rank_lieutenant,
	_experience_rank_captain,
	_experience_rank_major,
	_experience_rank_commander,
	_experience_rank_colonel,
	_experience_rank_brigadier,
	_experience_rank_general,

	k_experience_rank_count,

	k_experience_rank_bits = 4
};
enum e_experience_grade
{
	_experience_grade_one = 0,
	_experience_grade_two,
	_experience_grade_three,
	_experience_grade_four,

	k_experience_grade_count,

	k_experience_grade_bits = 2
};

class c_online_service_record
{
public:
	const char* m_buffer;
	int32 m_buffer_length;
};
COMPILE_ASSERT(sizeof(c_online_service_record) == 0x8);

class c_online_service_record_manager
{
public:
	int32 m_last_download_status;
	c_http_stored_buffer_downloader<4373> m_downloader;
	byte __align17B0[0x8];
	uns64 m_current_service_record_xuid;
	int64 m_current_service_record_refresh_milliseconds;
	c_online_service_record m_current_service_record;
};
COMPILE_ASSERT(sizeof(c_online_service_record_manager) == 0x17D0);

struct s_service_record_identity
{
	c_static_wchar_string<16> name;
	s_player_appearance appearance;
	bool extras_portal_debug;
	uns8 vidmaster;
	e_campaign_progress campaign_progress;
	int32 highest_skill;
	int32 experience;
	int32 experience_base;
	e_experience_rank experience_rank;
	e_experience_grade experience_grade;
	int32 games_completed;
};
COMPILE_ASSERT(sizeof(s_service_record_identity) == 0x6A0);

extern c_online_service_record_manager& g_online_service_record_manager;

extern void __cdecl online_service_record_manager_dispose();
extern void __cdecl online_service_record_manager_initialize();
extern void __cdecl online_service_record_manager_update();

