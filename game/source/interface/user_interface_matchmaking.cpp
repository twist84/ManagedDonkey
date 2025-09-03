#include "interface/user_interface_matchmaking.hpp"

#include "memory/module.hpp"
#include "networking/logic/network_life_cycle.hpp"
#include "networking/session/network_session_parameters_matchmaking.hpp"

REFERENCE_DECLARE(0x0191D298, int32, g_network_matchmaking_fake_progress_stage) = NONE;
REFERENCE_DECLARE(0x052604C8, s_life_cycle_matchmaking_progress, g_test_progress);

HOOK_DECLARE(0x00A98920, user_interface_matchmaking_get_matchmaking_progress);

//.text:00A98330 ; void __cdecl user_interface_matchmaking_abort()
//.text:00A98340 ; bool __cdecl user_interface_matchmaking_get_category_description(e_hopper_category_id, wchar_t* const)
//.text:00A984A0 ; int32 __cdecl user_interface_matchmaking_get_category_ids(e_hopper_category_id* const)
//.text:00A98510 ; int32 __cdecl user_interface_matchmaking_get_category_image_index(e_hopper_category_id)
//.text:00A985F0 ; bool __cdecl user_interface_matchmaking_get_category_name(e_hopper_category_id, wchar_t* const)
//.text:00A98730 ; uns16 __cdecl user_interface_matchmaking_get_hopper()
//.text:00A98740 ; bool __cdecl user_interface_matchmaking_get_hopper_description(uns16, wchar_t* const)
//.text:00A98780 ; int32 __cdecl user_interface_matchmaking_get_hopper_ids(e_hopper_category_id, uns16* const)
//.text:00A98810 ; bool __cdecl user_interface_matchmaking_get_hopper_information(uns16, s_hopper_information*)

void __cdecl user_interface_matchmaking_get_matchmaking_progress(s_life_cycle_matchmaking_progress* progress_out)
{
	//INVOKE(0x00A98920, user_interface_matchmaking_get_matchmaking_progress, progress_out);

	network_life_cycle_get_matchmaking_progress(progress_out);

	if (g_network_matchmaking_fake_progress_stage == NONE)
	{
		csmemcpy(&g_test_progress, progress_out, sizeof(g_test_progress));
	}
	else
	{
		csmemcpy(progress_out, &g_test_progress, sizeof(s_life_cycle_matchmaking_progress));
	}
}

//.text:00A98960 ; const s_replicated_life_cycle_matchmaking_progress_search_criteria* __cdecl user_interface_matchmaking_get_search_criteria(const s_life_cycle_matchmaking_progress*)

e_network_file_load_status __cdecl user_interface_matchmaking_hopper_catalog_load_status()
{
	return INVOKE(0x00A98990, user_interface_matchmaking_hopper_catalog_load_status);
}

bool __cdecl user_interface_matchmaking_set_hopper(uns16 hopper_identifier)
{
	return INVOKE(0x00A989F0, user_interface_matchmaking_set_hopper, hopper_identifier);

	//return network_squad_session_set_hopper(hopper_identifier);
}

//.text:00A98A00 ; e_matchmaking_search_preference __cdecl user_interface_matchmaking_squad_get_search_preference()

bool __cdecl user_interface_matchmaking_squad_set_search_preference(e_matchmaking_search_preference value)
{
	return INVOKE(0x00A98A10, user_interface_matchmaking_squad_set_search_preference, value);

	//return network_squad_session_set_matchmaking_search_preference(value);
}

//.text:00A98A20 ; void __cdecl user_interface_matchmaking_test_get_fake_progress()
//.text:00A98A30 ; void __cdecl user_interface_matchmaking_update_hopper_statistics_for_screen(c_http_blf_simple_downloader<s_dynamic_matchmaking_hopper_statistics>*, uns16*, s_matchmaking_single_hopper_statistics*)

