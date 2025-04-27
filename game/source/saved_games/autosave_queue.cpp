#include "saved_games/autosave_queue.hpp"

#include "cseries/cseries.hpp"

void __cdecl autosave_queue_dispose()
{
	INVOKE(0x0067BDA0, autosave_queue_dispose);
}

void __cdecl autosave_queue_dispose_from_old_map()
{
	INVOKE(0x0067BDB0, autosave_queue_dispose_from_old_map);
}

//.text:0067BDC0 ; 
//.text:0067BEA0 ; 
//.text:0067BEB0 ; 

void __cdecl autosave_queue_initialize()
{
	INVOKE(0x0067BEC0, autosave_queue_initialize);
}

void __cdecl autosave_queue_initialize_for_new_map()
{
	INVOKE(0x0067BED0, autosave_queue_initialize_for_new_map);
}

//.text:0067BEE0 ; int32 __cdecl autosave_queue_read_file(s_file_reference const*, void*, int32, c_synchronized_long*, c_synchronized_long*)

