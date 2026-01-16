#pragma once

//#include "cseries/async_xoverlapped.hpp"

struct s_arbitration_registration_result;

enum e_online_arbitration_registration_status
{
	_online_arbitration_registration_none = 0,
	_online_arbitration_registration_in_progress,
	_online_arbitration_registration_complete_success,
	_online_arbitration_registration_complete_failed,

	k_online_arbitration_registration_count,
};

//class c_arbitration_overlapped_task :
//	public c_overlapped_task
//{
//public:
//	byte m_registration_result[0x1000];
//	void* m_session_handle;
//	uns64 m_session_nonce;
//};
//COMPILE_ASSERT(sizeof(c_arbitration_overlapped_task) == 0x1020);
//
//struct s_online_arbitration_globals
//{
//	bool initialized;
//	c_arbitration_overlapped_task* arbitration_overlapped_task;
//};
//COMPILE_ASSERT(sizeof(s_online_arbitration_globals) == 0x8);
//
//extern s_online_arbitration_globals g_online_arbitration_globals;

extern void online_arbitration_dispose();
extern void online_arbitration_initialize();
extern bool online_arbitration_initiate_registration(void* session_handle, uns64 session_nonce);
extern bool online_arbitration_registration_get_result(s_arbitration_registration_result* arbitration_result_out);
extern e_online_arbitration_registration_status online_arbitration_registration_get_status();

