#pragma once

#include "cseries/cseries.hpp"
#include "multithreading/synchronized_value.hpp"
#include "networking/tools/http_client.hpp"
#include "networking/tools/http_stream.hpp"
#include "tag_files/files.hpp"

typedef void(__cdecl* netdebug_update_routine)(int32, int32);
typedef void(__cdecl* netdebug_completion_routine)(bool, void*);
typedef void* netdebug_completion_routine_argument;

struct s_netdebug_upload_task
{
	bool active;
	c_static_string<256> custom_directory;
	netdebug_update_routine update_routine;
	netdebug_completion_routine completion_routine;
	netdebug_completion_routine_argument args;
	s_file_reference file_to_upload;
	uns32 checksum;
};
COMPILE_ASSERT(sizeof(s_netdebug_upload_task) == 0x224);

struct s_netdebug_globals
{
	// $TODO replace with raw string
	c_static_string<64> title;
	c_static_string<64> build;
	c_static_string<160> system;
	c_static_string<128> sessionid;
	c_static_string<256> system_version;
	c_static_string<256> xtl_version;
	bool prefer_internet;
	c_http_post_stream upload_input_stream;
	c_http_client upload_client;
	int16 current_task_fail_count;
	s_netdebug_upload_task current_task;
	s_netdebug_upload_task task_queue[10];
	bool initialized;
};
COMPILE_ASSERT(sizeof(s_netdebug_globals) == 0x3970);

extern s_netdebug_globals& g_netdebug_globals;
extern c_synchronized_long& g_net_debug_thread_has_exited;

extern const char* const k_sessionid_filename_pack_token;
extern const char* const k_session_description_title_string;
extern const char* const k_session_description_build_string;
extern const char* const k_session_description_system_string;
extern const char* const k_netdebug_session_description_file_location;
extern const char* const k_debug_server_url;

extern void __cdecl add_http_xbox_upload_info_headers(c_http_post_stream* stream, const s_netdebug_upload_task* task);
extern void __cdecl build_sessionid_time_segment();
extern void __cdecl compute_task_file_upload_signatures(s_netdebug_upload_task* task);
extern void __cdecl create_sessionid();
extern void __cdecl fill_packed_sessionid_filename(const char* filename, class c_static_string<256>* sessionid_filename);
extern void __cdecl get_system_ip_addresses(c_static_string<16>* insecure_ip, c_static_string<128>* secure_ip);
extern void __cdecl netdebug_dispose();
extern const char* __cdecl netdebug_get_build();
extern void __cdecl netdebug_get_next_task();
extern const char* __cdecl netdebug_get_sessionid();
extern const char* __cdecl netdebug_get_system();
extern const char* __cdecl netdebug_get_title();
extern bool __cdecl netdebug_initialize(const char* title_string, const char* build_identifier_string, const char* system_identifier_string);
extern void __cdecl netdebug_prefer_internet(bool prefer_internet);
extern bool __cdecl netdebug_process_file_upload(s_netdebug_upload_task* task);
extern void __cdecl netdebug_process_next_task();
extern bool __cdecl netdebug_queue_task(const s_netdebug_upload_task* task);
extern void __cdecl netdebug_set_sessionid(const char* sessionid);
extern uns32 __cdecl netdebug_thread_function(void* thread_parameter);
extern void __cdecl netdebug_upload_file(const char* custom_directory, const char* path, netdebug_update_routine update_routine, netdebug_completion_routine completion_routine, netdebug_completion_routine_argument args);
extern void __cdecl remove_current_task(bool succeeded);
extern void __cdecl netdebug_set_system_version();
extern void __cdecl netdebug_set_xtl_version();
extern bool __cdecl upload_synchronous(c_http_client* client, c_http_stream* stream, int32 seconds, const s_netdebug_upload_task* task);
extern void __cdecl create_session_description();

