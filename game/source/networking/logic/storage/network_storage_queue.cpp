#include "networking/logic/storage/network_storage_queue.hpp"

#include "cseries/cseries.hpp"

//.text:004E1A30 ; 
//.text:004E1A70 ; 
//.text:004E1AA0 ; 
//.text:004E1AD0 ; 

void __cdecl network_storage_dispose()
{
	INVOKE(0x004E1AF0, network_storage_dispose);
}

void __cdecl network_storage_initialize()
{
	INVOKE(0x004E1C70, network_storage_initialize);
}

//.text:004E1C80 ; 
//.text:004E1CD0 ; 
//.text:004E1DA0 ; 
//.text:004E1E30 ; 
//.text:004E1E60 ; 
//.text:004E1E90 ; 
//.text:004E1EB0 ; void __cdecl network_storage_queue_stop_task(long)

void __cdecl network_storage_queue_update()
{
	INVOKE(0x004E1ED0, network_storage_queue_update);
}

//.text:004E21C0 ; long __cdecl network_storage_queue_upload_item(c_network_storage_item_interface*, s_network_storage_file_header*, long)

