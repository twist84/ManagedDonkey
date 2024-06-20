#include "networking/logic/storage/network_storage_manifest.hpp"

#include "cache/security_functions.hpp"
#include "cseries/cseries_events.hpp"
#include "memory/module.hpp"
#include "networking/online/online.hpp"

c_network_storage_manifest* __cdecl c_network_storage_manifest::get()
{
	//return INVOKE(0x004A55F0, c_network_storage_manifest::get);

	void* working_memory = NULL;
	long working_memory_size = 0;
	security_get_working_memory(_secure_memory_region_network_storage_manifest, &working_memory, &working_memory_size);

	if (!working_memory || working_memory_size < sizeof(c_network_storage_manifest))
		generate_event(_event_level_critical, "c_network_storage_manifest: couldn't get secure working memory");

	return (c_network_storage_manifest*)working_memory;
}

void __cdecl network_storage_manifest_dispose()
{
	INVOKE(0x004A59E0, network_storage_manifest_dispose);
}

//.text:004A59F0 ; 
//.text:004A5A50 ; e_network_file_load_status __cdecl network_storage_manifest_get_load_status()

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
	DECLFUNC(0x004A5CF0, void, __thiscall, c_network_storage_manifest*)(this);
}

