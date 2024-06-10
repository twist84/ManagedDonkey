#pragma once

#include "cseries/cseries.hpp"
#include "memory/secure_signature.hpp"
#include "networking/logic/storage/network_http_buffer_downloader.hpp"
#include "networking/online/online_url.hpp"

struct s_online_file
{
	s_network_storage_cache_key cache_key;
	s_network_http_request_hash hash;
};
static_assert(sizeof(s_online_file) == 0x64);

struct s_online_file_manifest
{
	long file_count;
	s_online_file files[128];
};
static_assert(sizeof(s_online_file_manifest) == 0x3204);

struct s_network_storage_manifest_untracked_entry
{
	long state;
	s_online_file online_file;
	long __time68;
};
static_assert(sizeof(s_network_storage_manifest_untracked_entry) == 0x6C);

struct c_network_storage_manifest
{
public:
	static c_network_storage_manifest* __cdecl get();
	void update();

protected:
	long __time0;
	s_online_file_manifest* m_online_file_manifest;
	bool m_unavailable;
	byte __data9[0x3];
	long m_last_downloaded_time[128];

	struct //c_http_blf_simple_downloader<s_online_file_manifest> m_online_file_manifest_downloader;
	{
		c_http_stored_buffer_downloader<15009> downloader;
		long __unknown_index_or_count;
		char const* m_chunk_buffer;
		long m_chunk_buffer_size;
	} m_online_file_manifest_downloader;

	s_network_storage_manifest_untracked_entry m_untracked_entries[32];
};
static_assert(sizeof(c_network_storage_manifest) == 0x50D4);

extern void __cdecl network_storage_manifest_dispose();
extern void __cdecl network_storage_manifest_initialize();
extern void __cdecl network_storage_manifest_update();

