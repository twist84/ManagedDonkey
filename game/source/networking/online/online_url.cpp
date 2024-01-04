#include "networking/online/online_url.hpp"

#include "cseries/language.hpp"
#include "memory/module.hpp"
#include "networking/logic/storage/network_http_buffer_downloader.hpp"
#include "networking/logic/storage/network_http_request_queue.hpp"
#include "networking/online/online_lsp.hpp"
#include "text/unicode.hpp"

#include <string.h>

REFERENCE_DECLARE(0x019AB758, _g_online_url, g_online_url);

HOOK_DECLARE(0x004515F0, create_title_url_base);
HOOK_DECLARE(0x00451710, make_hopper_network_directory);
HOOK_DECLARE(0x00451770, online_url_dispose);
HOOK_DECLARE(0x00451790, online_url_get_title);
HOOK_DECLARE(0x004517A0, online_url_initialize);
HOOK_DECLARE(0x00451810, online_url_make_bnet_consume_begin);
HOOK_DECLARE(0x00451860, online_url_make_bnet_consume_complete);
HOOK_DECLARE(0x004518B0, online_url_make_bnet_subscription_get_details);
//HOOK_DECLARE(0x00451920, online_url_make_cache_key);
HOOK_DECLARE(0x00451A00, online_url_make_matchmaking_banhammer_message);
HOOK_DECLARE(0x00451A40, online_url_make_matchmaking_descriptions);
HOOK_DECLARE(0x00451A90, online_url_make_matchmaking_game_variant);
HOOK_DECLARE(0x00451AD0, online_url_make_matchmaking_gameset);
HOOK_DECLARE(0x00451B10, online_url_make_matchmaking_histogram);
HOOK_DECLARE(0x00451B60, online_url_make_matchmaking_hopper);
HOOK_DECLARE(0x00451BA0, online_url_make_matchmaking_machine);
HOOK_DECLARE(0x00451BF0, online_url_make_matchmaking_map_variant);
HOOK_DECLARE(0x00451C30, online_url_make_matchmaking_nightmap);
HOOK_DECLARE(0x00451C70, online_url_make_matchmaking_statistics);
HOOK_DECLARE(0x00451CB0, online_url_make_matchmaking_tips);
HOOK_DECLARE(0x00451CF0, online_url_make_matchmaking_user);
HOOK_DECLARE(0x00451D50, online_url_make_matchmaking_user_recent_players);
HOOK_DECLARE(0x00451DB0, online_url_make_message_of_the_day);
HOOK_DECLARE(0x00451DF0, online_url_make_message_of_the_day_image);
HOOK_DECLARE(0x00451E30, online_url_make_message_of_the_day_popup);
HOOK_DECLARE(0x00451E70, online_url_make_message_of_the_day_popup_image);
HOOK_DECLARE(0x00451EB0, online_url_make_network_configuration);
HOOK_DECLARE(0x00451EF0, online_url_make_network_manifest);
HOOK_DECLARE(0x00451F30, online_url_make_network_map_signatures);
HOOK_DECLARE(0x00451F60, online_url_make_update_machine_network_stats);
HOOK_DECLARE(0x00451FA0, online_url_make_update_user_highest_skill); // bugfix for missing title string
HOOK_DECLARE(0x00451FE0, online_url_make_upload_saved_screenshot);
HOOK_DECLARE(0x00452020, online_url_make_user_file);
HOOK_DECLARE(0x00452080, online_url_make_vidmaster_popup);
HOOK_DECLARE(0x004520C0, online_url_make_vidmaster_popup_image);
//HOOK_DECLARE(0x00452100, online_url_use_hopper_directory);
//HOOK_DECLARE(0x00452150, online_url_use_user_override_hopper_directory);

//.text:00451460 ; public: __cdecl c_url_string::c_url_string(char const* url, e_cachable_type cachable)
//.text:004514B0 ; public: __cdecl c_url_string::c_url_string()

void __cdecl create_machine_url_base(c_url_string* url, qword machine_id)
{
	INVOKE(0x004515B0, create_machine_url_base, url, machine_id);
}

void __cdecl create_title_url_base(c_url_string* url)
{
	//INVOKE(0x004515F0, create_title_url_base, url);

	char hopper_directory[256]{};
	make_hopper_network_directory(hopper_directory, sizeof(hopper_directory), '/');
	url->m_string.print("/storage/title/%08x/%s/", k_title_id, hopper_directory);
}

void __cdecl create_user_url_base(c_url_string* url, qword user_id)
{
	INVOKE(0x00451660, create_user_url_base, url, user_id);
}

//.text:004516A0 ; c_url_string::get_request_type
//.text:004516B0 ; c_url_string::get_service_type
//.text:004516D0 ; c_url_string::get_untracked_cache_lifetime_seconds

void __cdecl make_hopper_network_directory(char* hopper_directory, long hopper_directory_size, char some_char /* separator? */)
{
	//INVOKE(0x00451710, make_hopper_network_directory, hopper_directory, hopper_directory_size, some_char);

	if (g_online_url.hopper_directory[0])
	{
		strncpy_s(hopper_directory, hopper_directory_size, g_online_url.hopper_directory, sizeof(g_online_url.hopper_directory));
	}
	else if (g_online_url.user_override_hopper_directory[0])
	{
		strncpy_s(hopper_directory, hopper_directory_size, g_online_url.user_override_hopper_directory, sizeof(g_online_url.user_override_hopper_directory));
	}
	else
	{
		strncpy_s(hopper_directory, hopper_directory_size, "default_hoppers", strlen("default_hoppers") + 1);
	}
}

void __cdecl online_url_dispose()
{
	//INVOKE(0x00451770, online_url_dispose);

	csmemset(&g_online_url, 0, sizeof(g_online_url));
}

char const* __cdecl online_url_get_title()// pc only?
{
	//INVOKE(0x00451790, online_url_get_title);

	return g_online_url.__unknown8C.get_string();
}

void __cdecl online_url_initialize()
{
	//INVOKE(0x004517A0, online_url_initialize);

	csmemset(&g_online_url, 0, sizeof(g_online_url));

	g_online_url.__unknown80.print("%d", 0); // unknown
	g_online_url.__unknown84.print("%d", 1); // Halo 3
	g_online_url.__unknown88.print("%d", 2); // Halo 3: Mythic
	g_online_url.__unknown8C.print("%d", 3); // Halo 3: ODST and Halo Online
	g_online_url.__unknown90.print("%d", 4); // unknown
}

void __cdecl online_url_make_bnet_consume_begin(c_url_string* url, qword user_id, dword consumable_id)
{
	//INVOKE(0x00451810, online_url_make_bnet_consume_begin, url, user_id, consumable_id);

	url->m_string.print("/gameapi/UserBeginConsume.ashx?title=%s&userId=%016I64X&consumableId=%08X",
		online_url_get_title(),
		user_id,
		consumable_id);
	url->m_cachable = c_url_string::_cachable_type_unknown0;
	url->m_service_type = _online_lsp_service_type_mass_storage;
	url->m_request_type = _network_http_request_queue_type_unknown1;
}

void __cdecl online_url_make_bnet_consume_complete(c_url_string* url, qword user_id, dword consumable_id)
{
	//INVOKE(0x00451860, online_url_make_bnet_consume_complete, url, user_id, consumable_id);

	url->m_string.print("/gameapi/UserCompleteConsume.ashx?title=%s&userId=%016I64X&consumableId=%08X",
		online_url_get_title(),
		user_id,
		consumable_id);
	url->m_cachable = c_url_string::_cachable_type_unknown0;
	url->m_service_type = _online_lsp_service_type_mass_storage;
	url->m_request_type = _network_http_request_queue_type_unknown1;
}

//void __cdecl online_url_make_bnet_subscription_get_details(c_url_string* url, qword user_id, e_game_region game_region, e_profile_region profile_region, bool extras_portal_debug)
void __cdecl online_url_make_bnet_subscription_get_details(c_url_string* url, qword user_id, long game_region, long profile_region, bool extras_portal_debug)
{
	//INVOKE(0x004518B0, online_url_make_bnet_subscription_get_details, url, user_id, game_region, profile_region, extras_portal_debug);

	url->m_string.print("/gameapi/UserGetBnetSubscription.ashx?title=%s&userId=%016I64X&locale=%s&gameRegion=%d&profileRegion=%d&isDebug=%s",
		online_url_get_title(),
		user_id,
		get_current_language_suffix(false),
		game_region,
		profile_region,
		extras_portal_debug ? "trye" : "false");
	url->m_cachable = c_url_string::_cachable_type_unknown0;
	url->m_service_type = _online_lsp_service_type_mass_storage;
	url->m_request_type = _network_http_request_queue_type_unknown1;
}

void __cdecl online_url_make_cache_key(c_url_string const* url, s_network_storage_cache_key* cache_key)
{
	INVOKE(0x00451920, online_url_make_cache_key, url, cache_key);
}

void __cdecl online_url_make_matchmaking_banhammer_message(c_url_string* url)
{
	//INVOKE(0x00451A00, online_url_make_matchmaking_banhammer_message, url);

	create_title_url_base(url);
	url->m_string.append_print("%s/matchmaking_banhammer_messages.bin", get_current_language_suffix(false));
	url->m_cachable = c_url_string::_cachable_type_unknown1;
	url->m_request_type = _network_http_request_queue_type_unknown0;
}

void __cdecl online_url_make_matchmaking_descriptions(c_url_string* url)
{
	//INVOKE(0x00451A40, online_url_make_matchmaking_descriptions, url);

	create_title_url_base(url);
	url->m_string.append_print("%s/matchmaking_hopper_descriptions_%03u.bin", get_current_language_suffix(false), 3);
	url->m_cachable = c_url_string::_cachable_type_unknown2;
	url->m_request_type = _network_http_request_queue_type_unknown0;
}

void __cdecl online_url_make_matchmaking_game_variant(c_url_string* url, word hopper_id, char const* variant_name)
{
	//INVOKE(0x00451A90, online_url_make_matchmaking_game_variant, url, hopper_id, variant_name);

	create_title_url_base(url);
	url->m_string.append_print("%05u/%s_%03u.bin", hopper_id, variant_name, 10);
	url->m_cachable = c_url_string::_cachable_type_unknown1;
	url->m_request_type = _network_http_request_queue_type_unknown0;
}

void __cdecl online_url_make_matchmaking_gameset(c_url_string* url, word hopper_id)
{
	//INVOKE(0x00451AD0, online_url_make_matchmaking_gameset, url, hopper_id);

	create_title_url_base(url);
	url->m_string.append_print("%05u/game_set_%03u.bin", hopper_id, 6);
	url->m_cachable = c_url_string::_cachable_type_unknown1;
	url->m_request_type = _network_http_request_queue_type_unknown0;
}

void __cdecl online_url_make_matchmaking_histogram(c_url_string* url, word hopper_id, long hopper_skill)
{
	//INVOKE(0x00451B10, online_url_make_matchmaking_histogram, url, hopper_id, hopper_skill);

	create_title_url_base(url);
	url->m_string.append_print("%05u/%s/dynamic_matchmaking_histogram_%02d.jpg", hopper_id, get_current_language_suffix(false), hopper_skill);
	url->m_cachable = c_url_string::_cachable_type_unknown2;
	url->m_request_type = _network_http_request_queue_type_unknown1;
}

void __cdecl online_url_make_matchmaking_hopper(c_url_string* url)
{
	//INVOKE(0x00451B60, online_url_make_matchmaking_hopper, url);

	create_title_url_base(url);
	url->m_string.append_print("matchmaking_hopper_%03u.bin", 11);
	url->m_cachable = c_url_string::_cachable_type_unknown1;
	url->m_request_type = _network_http_request_queue_type_unknown0;
}

void __cdecl online_url_make_matchmaking_machine(c_url_string* url, qword machine_id)
{
	//INVOKE(0x00451BA0, online_url_make_matchmaking_machine, url, machine_id);

	create_machine_url_base(url, machine_id);
	url->m_string.append_print("machine.bin");
	url->m_request_type = _network_http_request_queue_type_unknown0;
}

void __cdecl online_url_make_matchmaking_map_variant(c_url_string* url, word hopper_id, char const* variant_name)
{
	//INVOKE(0x00451BF0, online_url_make_matchmaking_map_variant, url, hopper_id, variant_name);

	create_title_url_base(url);
	url->m_string.append_print("%05u/map_variants/%s_%03u.bin", hopper_id, variant_name, 12);
	url->m_cachable = c_url_string::_cachable_type_unknown1;
	url->m_request_type = _network_http_request_queue_type_unknown0;
}

void __cdecl online_url_make_matchmaking_nightmap(c_url_string* url)
{
	//INVOKE(0x00451C30, online_url_make_matchmaking_nightmap, url);

	create_title_url_base(url);
	url->m_string.append_print("dynamic_matchmaking_nightmap.jpg");
	url->m_cachable = c_url_string::_cachable_type_unknown2;
	url->m_request_type = _network_http_request_queue_type_unknown1;
}

void __cdecl online_url_make_matchmaking_statistics(c_url_string* url)
{
	//INVOKE(0x00451C70, online_url_make_matchmaking_statistics, url);

	create_title_url_base(url);
	url->m_string.append_print("dynamic_hopper_statistics.bin");
	url->m_cachable = c_url_string::_cachable_type_unknown2;
	url->m_request_type = _network_http_request_queue_type_unknown1;
}

void __cdecl online_url_make_matchmaking_tips(c_url_string* url)
{
	//INVOKE(0x00451CB0, online_url_make_matchmaking_tips, url);

	create_title_url_base(url);
	url->m_string.append_print("%s/matchmaking_tips.bin", get_current_language_suffix(false));
	url->m_cachable = c_url_string::_cachable_type_unknown2;
	url->m_request_type = _network_http_request_queue_type_unknown1;
}

void __cdecl online_url_make_matchmaking_user(c_url_string* url, qword user_id)
{
	//INVOKE(0x00451CF0, online_url_make_matchmaking_user, url, user_id);

	create_user_url_base(url, user_id);
	url->m_string.append_print("user.bin");
	url->m_service_type = _online_lsp_service_type_user_files;
	url->m_request_type = _network_http_request_queue_type_unknown0;
}

void __cdecl online_url_make_matchmaking_user_recent_players(c_url_string* url, qword user_id)
{
	//INVOKE(0x00451D50, online_url_make_matchmaking_user_recent_players, url, user_id);

	create_user_url_base(url, user_id);
	url->m_string.append_print("recent_players.bin");
	url->m_service_type = _online_lsp_service_type_user_files;
	url->m_request_type = _network_http_request_queue_type_unknown0;
}

void __cdecl online_url_make_message_of_the_day(c_url_string* url)
{
	//INVOKE(0x00451DB0, online_url_make_message_of_the_day, url);

	create_title_url_base(url);
	url->m_string.append_print("%s/black_motd.bin", get_current_language_suffix(false));
	url->m_cachable = c_url_string::_cachable_type_unknown2;
	url->m_request_type = _network_http_request_queue_type_unknown1;
}

void __cdecl online_url_make_message_of_the_day_image(c_url_string* url)
{
	//INVOKE(0x00451DF0, online_url_make_message_of_the_day_image, url);

	create_title_url_base(url);
	url->m_string.append_print("%s/black_motd_image.jpg", get_current_language_suffix(false));
	url->m_cachable = c_url_string::_cachable_type_unknown2;
	url->m_request_type = _network_http_request_queue_type_unknown1;
}

void __cdecl online_url_make_message_of_the_day_popup(c_url_string* url)
{
	//INVOKE(0x00451E30, online_url_make_message_of_the_day_popup, url);

	create_title_url_base(url);
	url->m_string.append_print("%s/black_motd_popup.bin", get_current_language_suffix(false));
	url->m_cachable = c_url_string::_cachable_type_unknown2;
	url->m_request_type = _network_http_request_queue_type_unknown1;
}

void __cdecl online_url_make_message_of_the_day_popup_image(c_url_string* url)
{
	//INVOKE(0x00451E70, online_url_make_message_of_the_day_popup_image, url);

	create_title_url_base(url);
	url->m_string.append_print("%s/black_motd_popup_image.jpg", get_current_language_suffix(false));
	url->m_cachable = c_url_string::_cachable_type_unknown2;
	url->m_request_type = _network_http_request_queue_type_unknown1;
}

void __cdecl online_url_make_network_configuration(c_url_string* url)
{
	//INVOKE(0x00451EB0, online_url_make_network_configuration, url);

	create_title_url_base(url);
	url->m_string.append_print("network_configuration_%03u.bin", 142);
	url->m_cachable = c_url_string::_cachable_type_unknown1;
	url->m_request_type = _network_http_request_queue_type_unknown0;
}

void __cdecl online_url_make_network_manifest(c_url_string* url)
{
	//INVOKE(0x00451EF0, online_url_make_network_manifest, url);

	create_title_url_base(url);
	url->m_string.append_print("manifest_%03u.bin", 1);
	url->m_cachable = c_url_string::_cachable_type_unknown0;
	url->m_request_type = _network_http_request_queue_type_unknown0;
}

void __cdecl online_url_make_network_map_signatures(c_url_string* url)
{
	//INVOKE(0x00451F30, online_url_make_network_map_signatures, url);

	create_title_url_base(url);
	url->m_string.append_print("rsa_manifest.bin");
	url->m_request_type = _network_http_request_queue_type_unknown0;
}

void __cdecl online_url_make_update_machine_network_stats(c_url_string* url)
{
	//INVOKE(0x00451F60, online_url_make_update_machine_network_stats, url);

	url->m_string.print("/gameapi/MachineUpdateNetworkStats.ashx");
	url->m_cachable = c_url_string::_cachable_type_unknown0;
	url->m_service_type = _online_lsp_service_type_mass_storage;
	url->m_request_type = _network_http_request_queue_type_unknown1;
}

void __cdecl online_url_make_update_user_highest_skill(c_url_string* url, qword user_id, long highest_skill)
{
	//INVOKE(0x00451FA0, online_url_make_update_user_highest_skill, url, user_id, highest_skill);

	url->m_string.print("/gameapi/UserUpdatePlayerStats.ashx?title=%s&userId=%016I64X&highestSkill=%d",
		online_url_get_title(), // missing from the original function
		user_id,
		highest_skill);
	url->m_cachable = c_url_string::_cachable_type_unknown0;
	url->m_service_type = _online_lsp_service_type_mass_storage;
	url->m_request_type = _network_http_request_queue_type_unknown1;
}

void __cdecl online_url_make_upload_saved_screenshot(c_url_string* url)
{
	//INVOKE(0x00451FE0, online_url_make_upload_saved_screenshot, url);

	url->m_string.print("/gameapi/FilesUploadBlind.ashx");
	url->m_cachable = c_url_string::_cachable_type_unknown0;
	url->m_service_type = _online_lsp_service_type_mass_storage;
	url->m_request_type = _network_http_request_queue_type_unknown1;
}

void __cdecl online_url_make_user_file(c_url_string* url, qword user_id)
{
	//INVOKE(0x00452020, online_url_make_user_file, url, user_id);

	create_user_url_base(url, user_id);
	url->m_string.append_print("user.bin");
	url->m_service_type = _online_lsp_service_type_user_files;
	url->m_request_type = _network_http_request_queue_type_unknown0;
}

void __cdecl online_url_make_vidmaster_popup(c_url_string* url)
{
	//INVOKE(0x00452080, online_url_make_vidmaster_popup, url);

	create_title_url_base(url);
	url->m_string.append_print("%s/black_vidmaster_popup.bin", get_current_language_suffix(false));
	url->m_cachable = c_url_string::_cachable_type_unknown2;
	url->m_request_type = _network_http_request_queue_type_unknown1;
}

void __cdecl online_url_make_vidmaster_popup_image(c_url_string* url)
{
	//INVOKE(0x004520C0, online_url_make_vidmaster_popup_image, url);

	create_title_url_base(url);
	url->m_string.append_print("%s/black_vidmaster_popup_image.jpg", get_current_language_suffix(false));
	url->m_cachable = c_url_string::_cachable_type_unknown2;
	url->m_request_type = _network_http_request_queue_type_unknown1;
}

void __cdecl online_url_use_hopper_directory(char const* hopper_directory)
{
	INVOKE(0x00452100, online_url_use_hopper_directory, hopper_directory);

	//c_static_string<64> _hopper_directory = hopper_directory;
	//if (!_hopper_directory.is_empty())
	//{
	//	_hopper_directory.copy_to(g_online_url.hopper_directory, sizeof(g_online_url.hopper_directory));
	//	dword_1860808++;
	//}
}

void __cdecl online_url_use_user_override_hopper_directory(char const* hopper_directory)
{
	INVOKE(0x00452150, online_url_use_user_override_hopper_directory, hopper_directory);

	//c_static_string<64> _hopper_directory = hopper_directory;
	//if (!_hopper_directory.is_empty())
	//{
	//	_hopper_directory.copy_to(g_online_url.user_override_hopper_directory, sizeof(g_online_url.user_override_hopper_directory));
	//	dword_1860808++;
	//}
}

//.text:004521E0 ; public: void __cdecl c_url_string::set(c_url_string const* other)
//.text:00452230 ; public: void __cdecl c_url_string::set_cachable(e_cachable_type cachable)
//.text:00452240 ; public: void __cdecl c_url_string::set_request_type(e_network_http_request_queue_type queue_type)
//.text:00452250 ; public: void __cdecl c_url_string::set_service_type(e_online_lsp_service_type service_type)

