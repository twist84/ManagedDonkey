#include "networking/tools/network_webstats.hpp"

//.text:004E3F30 ; public: __cdecl s_webstats_globals::s_webstats_globals(void)
//.text:004E3F50 ; 
//.text:004E3F60 ; void __cdecl do_uploading_work()
//.text:004E3FF0 ; 

void __cdecl network_webstats_disable(bool disabled)
{
	INVOKE(0x004E4000, network_webstats_disable, disabled);

	//g_webstats_globals.disabled = disabled;
}

void __cdecl network_webstats_dispose()
{
	INVOKE(0x004E4010, network_webstats_dispose);
}

void __cdecl network_webstats_force_reset()
{
	INVOKE(0x004E4020, network_webstats_force_reset);
}

void __cdecl network_webstats_initialize()
{
	INVOKE(0x004E4040, network_webstats_initialize);
}

//.text:004E40A0 ; void __cdecl network_webstats_set_upstream_quota(int32)
//.text:004E40C0 ; bool __cdecl network_webstats_submit_buffer(e_online_lsp_service_type, const void*, int32, const char*, int32)
//.text:004E4200 ; bool __cdecl network_webstats_submit_file_allowed()
//.text:004E4230 ; e_webstats_submission_results __cdecl network_webstats_submit_file(e_online_lsp_service_type, const wchar_t*, const char*, int32, void(__cdecl*)(bool))
//.text:004E42F0 ; bool __cdecl network_webstats_submit_file_allowed()

void __cdecl network_webstats_update()
{
	INVOKE(0x004E4320, network_webstats_update);
}

//.text:004E4350 ; bool __cdecl start_lsp_upload()
//.text:004E43D0 ; bool __cdecl update_lsp()
//.text:004E4450 ; void __cdecl upload_complete()
//.text:004E4490 ; void __cdecl upload_failed(bool)
//.text:004E45E0 ; bool __cdecl upload_step_is_taking_too_long()
//.text:004E4600 ; void __cdecl upload_success()

