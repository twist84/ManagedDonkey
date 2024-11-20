#include "networking/logic/storage/network_storage_manifest.hpp"

#include "cache/security_functions.hpp"
#include "cseries/cseries_events.hpp"
#include "memory/byte_swapping.hpp"
#include "memory/module.hpp"
#include "networking/online/online.hpp"

HOOK_DECLARE_CLASS_MEMBER(0x004A5720, c_network_storage_manifest, get_entry_state);
HOOK_DECLARE(0x004A5A50, network_storage_manifest_get_load_status);

c_network_storage_manifest* __cdecl c_network_storage_manifest::get()
{
	//return INVOKE(0x004A55F0, c_network_storage_manifest::get);

	void* working_memory = NULL;
	long working_memory_size = 0;
	security_get_working_memory(_secure_memory_region_network_storage_manifest, &working_memory, &working_memory_size);

	if (!working_memory || working_memory_size < sizeof(c_network_storage_manifest))
		generate_event(_event_critical, "c_network_storage_manifest: couldn't get secure working memory");

	return (c_network_storage_manifest*)working_memory;
}

//.text:004A5640 ; 
//.text:004A5660 ; public: e_download_status __cdecl c_http_blf_simple_downloader<s_online_file_manifest>::get_data(c_url_string const*, s_online_file_manifest const**, long*)

c_network_storage_manifest::e_entry_state __thiscall c_network_storage_manifest::get_entry_state(s_network_storage_cache_key const* cache_key)
{
	//return INVOKE_CLASS_MEMBER(0x004A5720, c_network_storage_manifest, get_entry_state, cache_key);

	// prevent big endian a manifest from exploding this call
	if (m_manifest && !VALID_COUNT(m_manifest->file_count, NUMBEROF(s_online_file_manifest::files)))
	{
		bswap_dword_inplace(m_manifest->file_count);
		if (!VALID_COUNT(m_manifest->file_count, NUMBEROF(s_online_file_manifest::files)))
		{
			ASSERT2(c_string_builder("c_network_storage_manifest::get_entry_state: invalid manifest (expected file count %d != %d)",
				m_manifest->file_count,
				NUMBEROF(s_online_file_manifest::files)).get_string());
		}
	}

	this;
	c_network_storage_manifest::e_entry_state entry_state;
	HOOK_INVOKE_CLASS_MEMBER(entry_state =, c_network_storage_manifest, get_entry_state, cache_key);
	return entry_state;
}

//.text:004A57E0 ; public: bool __cdecl c_network_storage_manifest::get_hash(long, s_network_storage_cache_key const*, s_network_http_request_hash*)
//.text:004A5910 ; public: long __cdecl c_network_storage_manifest::get_last_downloaded_time(long, s_network_storage_cache_key const*)
//.text:004A5990 ; 
//.text:004A59B0 ; public: void __cdecl c_network_storage_manifest::initialize()

void __cdecl network_storage_manifest_dispose()
{
	INVOKE(0x004A59E0, network_storage_manifest_dispose);
}

//.text:004A59F0 ; 

e_network_file_load_status __cdecl network_storage_manifest_get_load_status()
{
	//return INVOKE(0x004A5A50, network_storage_manifest_get_load_status);

	if (c_network_storage_manifest* manifest = c_network_storage_manifest::get())
	{
		if (manifest->m_manifest)
			return _network_file_load_status_available;

		if (manifest->m_manifest_download_error)
			return _network_file_load_status_unavailable;

		return _network_file_load_status_pending;
	}

	return _network_file_load_status_none;
}

void __cdecl network_storage_manifest_initialize()
{
	INVOKE(0x004A5AC0, network_storage_manifest_initialize);
}

void __cdecl network_storage_manifest_update()
{
	//INVOKE(0x004A5B40, network_storage_manifest_update);

	if (online_is_connected_to_live())
	{
		if (c_network_storage_manifest* manifest = c_network_storage_manifest::get())
			manifest->update();
	}
}

void c_network_storage_manifest::update()
{
	INVOKE_CLASS_MEMBER(0x004A5CF0, c_network_storage_manifest, update);
}

