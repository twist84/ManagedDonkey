#pragma once

#include "cseries/cseries.hpp"

#pragma pack(push, 1)
struct s_motd_data
{
	static long const k_chunk_type = 'motd';
	static long const k_version_major = 1;
	//static long const k_version_minor = 0;

	long message_length;
	char message[511];
	char __data[2721 - sizeof(message_length) - 511];
};
static_assert(sizeof(s_motd_data) == 2721);
#pragma pack(pop)

struct s_motd_popup_data
{
	static long const k_chunk_type = 'mtdp';
	static long const k_version_major = 4;
	//static long const k_version_minor = 0;

	long title_index_identifier;
	dword button_key_wait_time;

	long title_size;
	wchar_t title[48];

	long header_size;
	wchar_t header[48];

	long button_key_size;
	wchar_t button_key[48];

	long button_key_wait_size;
	wchar_t button_key_wait[48];

	long message_size;
	wchar_t message[1024];
};
static_assert(sizeof(s_motd_popup_data) == 0x99C);

struct c_url_string
{
	c_static_string<256> m_string;
	long m_service_type;
	dword __unknown104;
	dword m_untracked_cache_lifetime_seconds;
	dword m_cachable;
};
static_assert(sizeof(c_url_string) == 0x110);

extern bool g_title_url_base_override;
const char k_title_url_base_override[] = "/storage/";
const long k_title_url_base_override_length = sizeof(k_title_url_base_override) - 1;

extern long g_title_url_base_length;

extern void create_title_url_base(c_url_string* url);