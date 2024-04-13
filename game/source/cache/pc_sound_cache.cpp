#include "cache/pc_sound_cache.hpp"

#include "cseries/cseries.hpp"

void __cdecl sound_cache_close()
{
	INVOKE(0x0065FEC0, sound_cache_close);
}

void __cdecl sound_cache_delete()
{
	INVOKE(0x0065FEF0, sound_cache_delete);
}

//.text:0065FF00 ; 

void __cdecl sound_cache_flush_all_sound_guaranteed()
{
	INVOKE(0x0065FF90, sound_cache_flush_all_sound_guaranteed);
}

void __cdecl sound_cache_idle()
{
	INVOKE(0x00660050, sound_cache_idle);
}

//.text:00660060 ; 

void __cdecl sound_cache_new()
{
	INVOKE(0x00660090, sound_cache_new);
}

void __cdecl sound_cache_open()
{
	INVOKE(0x006600C0, sound_cache_open);
}

void __cdecl sound_cache_prepare_for_tag_reload()
{
	INVOKE(0x006600D0, sound_cache_prepare_for_tag_reload);
}

//.text:006600E0 ; bool __cdecl sound_cache_sound_add_reference(s_sound_permutation_chunk*)
//.text:00660120 ; 
//.text:00660150 ; void __cdecl sound_cache_sound_finished(s_sound_permutation_chunk*)

