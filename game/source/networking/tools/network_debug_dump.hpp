#pragma once

#include "cseries/cseries.hpp"
#include "multithreading/synchronized_value.hpp"
#include "networking/tools/http_client.hpp"
#include "networking/tools/http_stream.hpp"
#include "tag_files/files.hpp"

struct s_netdebug_upload_task
{
	bool active;
	c_static_string<256> __string4;

	void(__cdecl* update_proc)(long, long);
	void(__cdecl* completion_proc)(bool, void*);
	void* completion_data;

	s_file_reference file;
	dword checksum;
};
static_assert(sizeof(s_netdebug_upload_task) == 0x224);

struct s_netdebug_globals
{
	c_static_string<64> title;
	c_static_string<64> build;
	c_static_string<160> system;
	c_static_string<128> sessionid;
	c_static_string<256> system_version;
	c_static_string<256> xtl_version;

	bool prefer_internet;

	c_http_post_stream http_post_stream;
	c_http_client http_client;

	short task_fails;

	s_netdebug_upload_task current_task;
	c_static_array<s_netdebug_upload_task, 10> task_queue;

	bool initialized;
	byte __pad3969[7];
};
static_assert(sizeof(s_netdebug_globals) == 0x3970);

extern s_netdebug_globals& g_netdebug_globals;
extern c_synchronized_long& g_net_debug_thread_has_exited;

extern char const* const k_sessionid_filename_pack_token;
extern char const* const k_session_description_title_string;
extern char const* const k_session_description_build_string;
extern char const* const k_session_description_system_string;
extern char const* const k_netdebug_session_description_file_location;
extern char const* const k_debug_server_url;

extern void __cdecl add_http_xbox_upload_info_headers(c_http_post_stream* stream, struct s_netdebug_upload_task const* task);
extern void __cdecl build_sessionid_time_segment();
extern void __cdecl compute_task_file_upload_signatures(s_netdebug_upload_task* task);
extern void __cdecl create_sessionid();
extern void __cdecl fill_packed_sessionid_filename(char const* filename, class c_static_string<256>* sessionid_filename);
extern void __cdecl get_system_ip_addresses(c_static_string<16>* insecure_ip, c_static_string<128>* secure_ip);
extern void __cdecl netdebug_dispose();
extern char const* __cdecl netdebug_get_build();
extern void __cdecl netdebug_get_next_task();
extern char const* __cdecl netdebug_get_sessionid();
extern char const* __cdecl netdebug_get_system();
extern char const* __cdecl netdebug_get_title();
extern bool __cdecl netdebug_initialize(char const* title_string, char const* build_identifier_string, char const* system_identifier_string);
extern void __cdecl netdebug_prefer_internet(bool prefer_internet);
extern bool __cdecl netdebug_process_file_upload(s_netdebug_upload_task* task);
extern void __cdecl netdebug_process_next_task();
extern bool __cdecl netdebug_queue_task(s_netdebug_upload_task const* task);
extern void __cdecl netdebug_set_sessionid(char const* sessionid);
extern dword __cdecl netdebug_thread_function(void* thread_parameter);
extern void __cdecl netdebug_upload_file(char const* a1, char const* path, void(__cdecl* update_proc)(long, long), void(__cdecl* completion_proc)(bool, void*), void* completion_data);
extern void __cdecl remove_current_task(bool a1);
extern void __cdecl netdebug_set_system_version();
extern void __cdecl netdebug_set_xtl_version();
extern bool __cdecl upload_synchronous(c_http_client* client, c_http_stream* stream, long seconds, s_netdebug_upload_task const* task);
extern void __cdecl create_session_description();

