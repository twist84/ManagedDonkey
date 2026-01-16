#pragma once

#include "memory/secure_signature.hpp"
#include "networking/logic/storage/network_http_buffer_downloader.hpp"
#include "networking/online/online_url.hpp"

struct s_online_file
{
	s_network_storage_cache_key cache_key;
	s_network_http_request_hash hash;
};
COMPILE_ASSERT(sizeof(s_online_file) == 0x64);

struct s_online_file_manifest
{
	int32 file_count;
	s_online_file files[128];
};
COMPILE_ASSERT(sizeof(s_online_file_manifest) == 0x3204);

struct s_network_storage_manifest_untracked_entry
{
	int32 state;
	s_online_file online_file;
	int32 __time68;
};
COMPILE_ASSERT(sizeof(s_network_storage_manifest_untracked_entry) == 0x6C);

class c_network_storage_manifest
{
public:
	enum e_entry_state
	{
		// $TODO map me
	};

	static c_network_storage_manifest* __cdecl get();
	e_entry_state __thiscall get_entry_state(const s_network_storage_cache_key* cache_key);
	void update();

//protected:
	int32 m_next_refresh_time_milliseconds;
	s_online_file_manifest* m_manifest;
	bool m_manifest_download_error;
	int32 m_manifest_entry_last_download_time[128];
	c_http_blf_simple_downloader<s_online_file_manifest, 15009> m_online_file_manifest_downloader;
	s_network_storage_manifest_untracked_entry m_untracked_entries[32];
};
COMPILE_ASSERT(sizeof(c_network_storage_manifest) == 0x50D4);

extern void __cdecl network_storage_manifest_dispose();
extern void __cdecl network_storage_manifest_expire_now();
extern e_network_file_load_status __cdecl network_storage_manifest_get_load_status();
extern void __cdecl network_storage_manifest_initialize();
extern void __cdecl network_storage_manifest_update();

