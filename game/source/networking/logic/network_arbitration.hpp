#pragma once

enum e_network_arbitration_status
{
	_network_arbitration_status_none = 0,
	_network_arbitration_status_registration_in_progress,
	_network_arbitration_status_registered,
	_network_arbitration_status_registration_failed,

	k_network_arbitration_status_count
};

struct s_arbitration_registrant
{
	uns64 machine_id;
	int32 trustworthiness;
	uns64 xuid;
};
COMPILE_ASSERT(sizeof(s_arbitration_registrant) == 0x18);

struct s_arbitration_registration_result
{
	int32 registrant_count;
	s_arbitration_registrant registrants[16];
};
COMPILE_ASSERT(sizeof(s_arbitration_registration_result) == 0x188);

struct s_network_arbitration_globals
{
	bool initialized;
	int32 arbitration_managed_session_index;
	e_network_arbitration_status arbitration_status;
	s_arbitration_registration_result registration_result;
};
COMPILE_ASSERT(sizeof(s_network_arbitration_globals) == 0x198);

//extern const char*(&g_network_arbitration_status_string)[k_network_arbitration_status_count];
extern const char* g_network_arbitration_status_string[k_network_arbitration_status_count];
extern s_network_arbitration_globals& network_arbitration_globals;

extern void __cdecl network_arbitration_destory();
extern bool __cdecl network_arbitration_get_results(int32 managed_session_index, s_arbitration_registration_result* registration_result);
extern e_network_arbitration_status __cdecl network_arbitration_get_status();
extern bool __cdecl network_arbitration_initialize();
extern bool __cdecl network_arbitration_initiate(int32 managed_session_index, uns64 nonce);
extern void __cdecl network_arbitration_update();
extern void __cdecl network_arbitration_update_registration();

