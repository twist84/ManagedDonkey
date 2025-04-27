#pragma once

#include "cseries/cseries.hpp"
#include "game/players.hpp"

enum e_online_arbitration_registration_status
{
	_network_arbitration_status_none = 0,
	_network_arbitration_status_registration_in_progress,
	_network_arbitration_status_registered,
	_network_arbitration_status_registration_failed,

	k_network_arbitration_status_count
};

struct s_arbitration_registration_result
{
	long registrant_count;

	struct
	{
		uint64 machine;
		uint32 trust;
		uint64 xuid;
	} registrants[16];
};
static_assert(sizeof(s_arbitration_registration_result) == 0x188);

struct s_network_arbitration_globals
{
	bool initialized;
	long managed_session_index;
	c_enum<e_online_arbitration_registration_status, long, _network_arbitration_status_none, k_network_arbitration_status_count> arbitration_status;
	s_arbitration_registration_result registration_result;
};
static_assert(sizeof(s_network_arbitration_globals) == 0x198);

extern s_network_arbitration_globals& network_arbitration_globals;

extern void __cdecl network_arbitration_destory();
extern bool __cdecl network_arbitration_initialize();
extern void __cdecl network_arbitration_update();

