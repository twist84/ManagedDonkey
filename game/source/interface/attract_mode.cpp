#include "interface/attract_mode.hpp"

#include "cseries/cseries.hpp"

//.text:00A99120 ; bool __cdecl attract_mode_get_built_in_movie_path(e_movie_type, c_static_string<256>*)
//.text:00A99220 ; 
//.text:00A99250 ; bool __cdecl attract_mode_get_movie_path(char const*, c_static_string<256>*)
//.text:00A99270 ; void __cdecl attract_mode_reset_timer()
//.text:00A99280 ; 
//.text:00A99290 ; bool __cdecl attract_mode_should_music_be_playing()

bool __cdecl attract_mode_should_start()
{
	return INVOKE(0x00A992A0, attract_mode_should_start);
}

void __cdecl attract_mode_start()
{
	INVOKE(0x00A993A0, attract_mode_start);
}

//.text:00A99460 ; 

