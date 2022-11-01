#pragma once

#include "cseries/cseries.hpp"
#include "multithreading/synchronized_value.hpp"
#include "networking/tools/http_client.hpp"
#include "networking/tools/http_stream.hpp"
#include "tag_files/files.hpp"

struct s_netdebug_upload_task
{
	bool active;

	c_static_string<260> __string4;

	void(__cdecl* __func108)(bool, int);
	dword __func108_data;

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

	bool refresh_system_name;

	c_http_post_stream http_post_stream;
	c_http_client http_client;

	short __unknown21D8;

	s_netdebug_upload_task current_task;
	s_netdebug_upload_task task_queue[10];

	bool initialized;
	byte __pad3969[7];
};
static_assert(sizeof(s_netdebug_globals) == 0x3970);

extern s_netdebug_globals& g_netdebug_globals;
extern c_synchronized_long& g_net_debug_thread_has_exited;

