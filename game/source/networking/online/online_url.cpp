#include "networking/online/online_url.hpp"

#include "cseries/language.hpp"
#include "memory/module.hpp"
#include "networking/logic/storage/network_http_buffer_downloader.hpp"
#include "networking/logic/storage/network_http_request_queue.hpp"
#include "networking/online/online_lsp.hpp"
#include "text/unicode.hpp"

#include <string.h>

REFERENCE_DECLARE(0x01860808, int32, g_online_url_hopper_directory_key); // initial value: 1
REFERENCE_DECLARE(0x0186080C, int32, k_default_untracked_lifetime_seconds); // initial value: 600
REFERENCE_DECLARE(0x019AB758, _g_online_url, g_online_url);

HOOK_DECLARE(0x004515F0, create_title_url_base);
HOOK_DECLARE(0x00451710, make_hopper_network_directory);
HOOK_DECLARE(0x00451770, online_url_dispose);
HOOK_DECLARE(0x00451790, online_url_get_title_string);
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
HOOK_DECLARE(0x00452020, online_url_make_user_service_record);
HOOK_DECLARE(0x00452080, online_url_make_vidmaster_popup);
HOOK_DECLARE(0x004520C0, online_url_make_vidmaster_popup_image);
HOOK_DECLARE(0x00452100, online_url_use_hopper_directory);
HOOK_DECLARE(0x00452150, online_url_use_user_override_hopper_directory);

c_url_string::c_url_string(const char* url, e_cachable_type cachable) :
	m_url_string(url),
	m_service_type(_online_lsp_service_type_title_files),
	m_cachable_type(cachable),
	m_untracked_cache_lifetime_seconds(k_default_untracked_lifetime_seconds),
	m_request_type(_network_http_request_queue_type_required)
{
	//DECLFUNC(0x00451460, void, __thiscall, c_url_string*, const char*, e_cachable_type)(this, url, cachable);
}

c_url_string::c_url_string() :
	m_url_string(),
	m_service_type(_online_lsp_service_type_title_files),
	m_cachable_type(_cachable_type_no),
	m_untracked_cache_lifetime_seconds(k_default_untracked_lifetime_seconds),
	m_request_type(_network_http_request_queue_type_required)
{
	//DECLFUNC(0x004514B0, void, __thiscall, c_url_string*)(this);
}

void c_url_string::clear()
{
	m_url_string.clear();
}

void __cdecl create_machine_url_base(c_url_string* url, uns64 machine_id)
{
	INVOKE(0x004515B0, create_machine_url_base, url, machine_id);
}

void __cdecl create_title_url_base(c_url_string* url)
{
	//INVOKE(0x004515F0, create_title_url_base, url);

	char hopper_directory[256]{};
	make_hopper_network_directory(hopper_directory, sizeof(hopper_directory), '/');
	url->get_url_string()->print("/storage/title/%08x/%s/", k_title_id, hopper_directory);
}

void __cdecl create_user_url_base(c_url_string* url, uns64 xuid)
{
	INVOKE(0x00451660, create_user_url_base, url, xuid);

	//byte byte0;
	//byte byte1;
}

c_url_string::e_cachable_type c_url_string::get_cachable() const
{
	return m_cachable_type;
}

e_network_http_request_queue_type c_url_string::get_request_type() const
{
	//return INVOKE_CLASS_MEMBER(0x004516A0, c_url_string, get_request_type);

	return m_request_type;
}

e_online_lsp_service_type c_url_string::get_service_type() const
{
	//return INVOKE_CLASS_MEMBER(0x004516B0, c_url_string, get_service_type);

	return m_service_type;
}

const char* c_url_string::get_string() const
{
	return m_url_string.get_string();
}

int32 c_url_string::get_untracked_cache_lifetime_seconds() const
{
	//return INVOKE_CLASS_MEMBER(0x004516D0, c_url_string, get_untracked_cache_lifetime_seconds);

	return m_untracked_cache_lifetime_seconds;
}

c_static_string<256>* c_url_string::get_url_string()
{
	return &m_url_string;
}

bool c_url_string::is_empty() const
{
	return m_url_string.is_empty();
}

bool c_url_string::is_equal(const c_url_string* url) const
{
	return url->m_service_type == m_service_type
		&& url->m_cachable_type == m_cachable_type
		&& url->m_untracked_cache_lifetime_seconds == m_untracked_cache_lifetime_seconds
		&& url->m_url_string.is_equal(m_url_string.get_string())
		&& url->m_request_type == m_request_type;
}

void __cdecl make_hopper_network_directory(char* hopper_directory, int32 length, char separator)
{
	//INVOKE(0x00451710, make_hopper_network_directory, hopper_directory, length, separator);

	if (g_online_url.current_hopper_directory[0])
	{
		strncpy_s(hopper_directory, length, g_online_url.current_hopper_directory, sizeof(g_online_url.current_hopper_directory));
	}
	else if (g_online_url.current_user_override_hopper_directory[0])
	{
		strncpy_s(hopper_directory, length, g_online_url.current_user_override_hopper_directory, sizeof(g_online_url.current_user_override_hopper_directory));
	}
	else
	{
		strncpy_s(hopper_directory, length, "default_hoppers", strlen_debug("default_hoppers") + 1);
	}
}

void __cdecl online_url_dispose()
{
	//INVOKE(0x00451770, online_url_dispose);

	csmemset(&g_online_url, 0, sizeof(g_online_url));
}

const char* __cdecl online_url_get_title_string()
{
	//INVOKE(0x00451790, online_url_get_title_string);

	return g_online_url.title_strings[_bungie_title_index_donkey].get_string();

	//if (get_is_blue_disk())
	//{
	//	return g_online_url.title_strings[_bungie_title_index_halo3_blue].get_string();
	//}
	//
	//return g_online_url.title_strings[_bungie_title_index_halo3_black].get_string();
}

void __cdecl online_url_initialize()
{
	//INVOKE(0x004517A0, online_url_initialize);

	csmemset(&g_online_url, 0, sizeof(g_online_url));

	g_online_url.title_strings[_bungie_title_index_halo3_legacy].print("%d", _bungie_title_index_halo3_legacy);
	g_online_url.title_strings[_bungie_title_index_halo3].print("%d", _bungie_title_index_halo3);
	g_online_url.title_strings[_bungie_title_index_halo3_blue].print("%d", _bungie_title_index_halo3_blue);
	g_online_url.title_strings[_bungie_title_index_halo3_black].print("%d", _bungie_title_index_halo3_black);
	g_online_url.title_strings[_bungie_title_index_halo3_shared_content].print("%d", _bungie_title_index_halo3_shared_content);
	g_online_url.title_strings[_bungie_title_index_donkey].print("%d", _bungie_title_index_donkey);
}

void __cdecl online_url_make_bnet_consume_begin(c_url_string* url, uns64 xuid, uns32 consumable_id)
{
	//INVOKE(0x00451810, online_url_make_bnet_consume_begin, url, xuid, consumable_id);

	url->get_url_string()->print("/gameapi/UserBeginConsume.ashx?title=%s&userId=%016I64X&consumableId=%08X",
		online_url_get_title_string(),
		xuid,
		consumable_id);
	url->set_cachable(c_url_string::_cachable_type_no);
	url->set_service_type(_online_lsp_service_type_mass_storage);
	url->set_request_type(_network_http_request_queue_type_optional);
}

void __cdecl online_url_make_bnet_consume_complete(c_url_string* url, uns64 xuid, uns32 consumable_id)
{
	//INVOKE(0x00451860, online_url_make_bnet_consume_complete, url, xuid, consumable_id);

	url->get_url_string()->print("/gameapi/UserCompleteConsume.ashx?title=%s&userId=%016I64X&consumableId=%08X",
		online_url_get_title_string(),
		xuid,
		consumable_id);
	url->set_cachable(c_url_string::_cachable_type_no);
	url->set_service_type(_online_lsp_service_type_mass_storage);
	url->set_request_type(_network_http_request_queue_type_optional);
}

//void __cdecl online_url_make_bnet_subscription_get_details(c_url_string* url, uns64 xuid, e_game_region game_region, e_profile_region profile_region, bool extras_portal_debug)
void __cdecl online_url_make_bnet_subscription_get_details(c_url_string* url, uns64 xuid, int32 game_region, int32 profile_region, bool extras_portal_debug)
{
	//INVOKE(0x004518B0, online_url_make_bnet_subscription_get_details, url, xuid, game_region, profile_region, extras_portal_debug);

	url->get_url_string()->print("/gameapi/UserGetBnetSubscription.ashx?title=%s&userId=%016I64X&locale=%s&gameRegion=%d&profileRegion=%d&isDebug=%s",
		online_url_get_title_string(),
		xuid,
		get_current_language_suffix(false),
		game_region,
		profile_region,
		extras_portal_debug ? "true" : "false");
	url->set_cachable(c_url_string::_cachable_type_no);
	url->set_service_type(_online_lsp_service_type_mass_storage);
	url->set_request_type(_network_http_request_queue_type_optional);
}

void __cdecl online_url_make_cache_key(const c_url_string* url, s_network_storage_cache_key* out_cache_key)
{
	INVOKE(0x00451920, online_url_make_cache_key, url, out_cache_key);

	//c_static_string<256> url_string;
	//{
	//	long remove_length;
	//	c_static_string<256> temp;
	//}
}

void __cdecl online_url_make_matchmaking_banhammer_message(c_url_string* url)
{
	//INVOKE(0x00451A00, online_url_make_matchmaking_banhammer_message, url);

	create_title_url_base(url);
	url->get_url_string()->append_print("%s/matchmaking_banhammer_messages.bin", get_current_language_suffix(false));
	url->set_cachable(c_url_string::_cachable_type_on_success);
	url->set_request_type(_network_http_request_queue_type_required);
}

void __cdecl online_url_make_matchmaking_descriptions(c_url_string* url)
{
	//INVOKE(0x00451A40, online_url_make_matchmaking_descriptions, url);

	create_title_url_base(url);
	url->get_url_string()->append_print("%s/matchmaking_hopper_descriptions_%03u.bin", get_current_language_suffix(false), 3);
	url->set_cachable(c_url_string::_cachable_type_on_success_and_failure);
	url->set_request_type(_network_http_request_queue_type_required);
}

void __cdecl online_url_make_matchmaking_game_variant(c_url_string* url, uns16 hopper_id, const char* variant_name)
{
	//INVOKE(0x00451A90, online_url_make_matchmaking_game_variant, url, hopper_id, variant_name);

	create_title_url_base(url);
	url->get_url_string()->append_print("%05u/%s_%03u.bin", hopper_id, variant_name, 10);
	url->set_cachable(c_url_string::_cachable_type_on_success);
	url->set_request_type(_network_http_request_queue_type_required);
}

void __cdecl online_url_make_matchmaking_gameset(c_url_string* url, uns16 hopper_id)
{
	//INVOKE(0x00451AD0, online_url_make_matchmaking_gameset, url, hopper_id);

	create_title_url_base(url);
	url->get_url_string()->append_print("%05u/game_set_%03u.bin", hopper_id, 6);
	url->set_cachable(c_url_string::_cachable_type_on_success);
	url->set_request_type(_network_http_request_queue_type_required);
}

void __cdecl online_url_make_matchmaking_histogram(c_url_string* url, uns16 hopper_id, int32 hopper_skill)
{
	//INVOKE(0x00451B10, online_url_make_matchmaking_histogram, url, hopper_id, hopper_skill);

	create_title_url_base(url);
	url->get_url_string()->append_print("%05u/%s/dynamic_matchmaking_histogram_%02d.jpg", hopper_id, get_current_language_suffix(false), hopper_skill);
	url->set_cachable(c_url_string::_cachable_type_on_success_and_failure);
	url->set_request_type(_network_http_request_queue_type_optional);
}

void __cdecl online_url_make_matchmaking_hopper(c_url_string* url)
{
	//INVOKE(0x00451B60, online_url_make_matchmaking_hopper, url);

	create_title_url_base(url);
	url->get_url_string()->append_print("matchmaking_hopper_%03u.bin", 11);
	url->set_cachable(c_url_string::_cachable_type_on_success);
	url->set_request_type(_network_http_request_queue_type_required);
}

void __cdecl online_url_make_matchmaking_machine(c_url_string* url, uns64 machine_id)
{
	//INVOKE(0x00451BA0, online_url_make_matchmaking_machine, url, machine_id);

	create_machine_url_base(url, machine_id);
	url->get_url_string()->append_print("machine.bin");
	url->set_request_type(_network_http_request_queue_type_required);
}

void __cdecl online_url_make_matchmaking_map_variant(c_url_string* url, uns16 hopper_id, const char* variant_name)
{
	//INVOKE(0x00451BF0, online_url_make_matchmaking_map_variant, url, hopper_id, variant_name);

	create_title_url_base(url);
	url->get_url_string()->append_print("%05u/map_variants/%s_%03u.bin", hopper_id, variant_name, 12);
	url->set_cachable(c_url_string::_cachable_type_on_success);
	url->set_request_type(_network_http_request_queue_type_required);
}

void __cdecl online_url_make_matchmaking_nightmap(c_url_string* url)
{
	//INVOKE(0x00451C30, online_url_make_matchmaking_nightmap, url);

	create_title_url_base(url);
	url->get_url_string()->append_print("dynamic_matchmaking_nightmap.jpg");
	url->set_cachable(c_url_string::_cachable_type_on_success_and_failure);
	url->set_request_type(_network_http_request_queue_type_optional);
}

void __cdecl online_url_make_matchmaking_statistics(c_url_string* url)
{
	//INVOKE(0x00451C70, online_url_make_matchmaking_statistics, url);

	create_title_url_base(url);
	url->get_url_string()->append_print("dynamic_hopper_statistics.bin");
	url->set_cachable(c_url_string::_cachable_type_on_success_and_failure);
	url->set_request_type(_network_http_request_queue_type_optional);
}

void __cdecl online_url_make_matchmaking_tips(c_url_string* url)
{
	//INVOKE(0x00451CB0, online_url_make_matchmaking_tips, url);

	create_title_url_base(url);
	url->get_url_string()->append_print("%s/matchmaking_tips.bin", get_current_language_suffix(false));
	url->set_cachable(c_url_string::_cachable_type_on_success_and_failure);
	url->set_request_type(_network_http_request_queue_type_optional);
}

void __cdecl online_url_make_matchmaking_user(c_url_string* url, uns64 xuid)
{
	//INVOKE(0x00451CF0, online_url_make_matchmaking_user, url, xuid);

	create_user_url_base(url, xuid);
	url->get_url_string()->append_print("user.bin");
	url->set_service_type(_online_lsp_service_type_user_files);
	url->set_request_type(_network_http_request_queue_type_required);
}

void __cdecl online_url_make_matchmaking_user_recent_players(c_url_string* url, uns64 xuid)
{
	//INVOKE(0x00451D50, online_url_make_matchmaking_user_recent_players, url, xuid);

	create_user_url_base(url, xuid);
	url->get_url_string()->append_print("recent_players.bin");
	url->set_service_type(_online_lsp_service_type_user_files);
	url->set_request_type(_network_http_request_queue_type_required);
}

void __cdecl online_url_make_message_of_the_day(c_url_string* url)
{
	//INVOKE(0x00451DB0, online_url_make_message_of_the_day, url);

	create_title_url_base(url);
	url->get_url_string()->append_print("%s/black_motd.bin", get_current_language_suffix(false));
	url->set_cachable(c_url_string::_cachable_type_on_success_and_failure);
	url->set_request_type(_network_http_request_queue_type_optional);
}

void __cdecl online_url_make_message_of_the_day_image(c_url_string* url)
{
	//INVOKE(0x00451DF0, online_url_make_message_of_the_day_image, url);

	create_title_url_base(url);
	url->get_url_string()->append_print("%s/black_motd_image.jpg", get_current_language_suffix(false));
	url->set_cachable(c_url_string::_cachable_type_on_success_and_failure);
	url->set_request_type(_network_http_request_queue_type_optional);
}

void __cdecl online_url_make_message_of_the_day_popup(c_url_string* url)
{
	//INVOKE(0x00451E30, online_url_make_message_of_the_day_popup, url);

	create_title_url_base(url);
	url->get_url_string()->append_print("%s/black_motd_popup.bin", get_current_language_suffix(false));
	url->set_cachable(c_url_string::_cachable_type_on_success_and_failure);
	url->set_request_type(_network_http_request_queue_type_optional);
}

void __cdecl online_url_make_message_of_the_day_popup_image(c_url_string* url)
{
	//INVOKE(0x00451E70, online_url_make_message_of_the_day_popup_image, url);

	create_title_url_base(url);
	url->get_url_string()->append_print("%s/black_motd_popup_image.jpg", get_current_language_suffix(false));
	url->set_cachable(c_url_string::_cachable_type_on_success_and_failure);
	url->set_request_type(_network_http_request_queue_type_optional);
}

void __cdecl online_url_make_network_configuration(c_url_string* url)
{
	//INVOKE(0x00451EB0, online_url_make_network_configuration, url);

	create_title_url_base(url);
	url->get_url_string()->append_print("network_configuration_%03u.bin", 142);
	url->set_cachable(c_url_string::_cachable_type_on_success);
	url->set_request_type(_network_http_request_queue_type_required);
}

void __cdecl online_url_make_network_manifest(c_url_string* url)
{
	//INVOKE(0x00451EF0, online_url_make_network_manifest, url);

	create_title_url_base(url);
	url->get_url_string()->append_print("manifest_%03u.bin", 1);
	url->set_cachable(c_url_string::_cachable_type_no);
	url->set_request_type(_network_http_request_queue_type_required);
}

void __cdecl online_url_make_network_map_signatures(c_url_string* url)
{
	//INVOKE(0x00451F30, online_url_make_network_map_signatures, url);

	create_title_url_base(url);
	url->get_url_string()->append_print("rsa_manifest.bin");
	url->set_request_type(_network_http_request_queue_type_required);
}

void __cdecl online_url_make_update_machine_network_stats(c_url_string* url)
{
	//INVOKE(0x00451F60, online_url_make_update_machine_network_stats, url);

	url->get_url_string()->print("/gameapi/MachineUpdateNetworkStats.ashx");
	url->set_cachable(c_url_string::_cachable_type_no);
	url->set_service_type(_online_lsp_service_type_mass_storage);
	url->set_request_type(_network_http_request_queue_type_optional);
}

void __cdecl online_url_make_update_user_highest_skill(c_url_string* url, uns64 xuid, int32 highest_skill)
{
	//INVOKE(0x00451FA0, online_url_make_update_user_highest_skill, url, xuid, highest_skill);

	url->get_url_string()->print("/gameapi/UserUpdatePlayerStats.ashx?title=%s&userId=%016I64X&highestSkill=%d",
		online_url_get_title_string(), // missing from the original function
		xuid,
		highest_skill);
	url->set_cachable(c_url_string::_cachable_type_no);
	url->set_service_type(_online_lsp_service_type_mass_storage);
	url->set_request_type(_network_http_request_queue_type_optional);
}

void __cdecl online_url_make_upload_saved_screenshot(c_url_string* url)
{
	//INVOKE(0x00451FE0, online_url_make_upload_saved_screenshot, url);

	url->get_url_string()->print("/gameapi/FilesUploadBlind.ashx");
	url->set_cachable(c_url_string::_cachable_type_no);
	url->set_service_type(_online_lsp_service_type_mass_storage);
	url->set_request_type(_network_http_request_queue_type_optional);
}

void __cdecl online_url_make_user_service_record(c_url_string* url, uns64 xuid)
{
	//INVOKE(0x00452020, online_url_make_user_service_record, url, xuid);

	create_user_url_base(url, xuid);
	url->get_url_string()->append_print("user.bin");
	url->set_service_type(_online_lsp_service_type_user_files);
	url->set_request_type(_network_http_request_queue_type_required);
}

void __cdecl online_url_make_vidmaster_popup(c_url_string* url)
{
	//INVOKE(0x00452080, online_url_make_vidmaster_popup, url);

	create_title_url_base(url);
	url->get_url_string()->append_print("%s/black_vidmaster_popup.bin", get_current_language_suffix(false));
	url->set_cachable(c_url_string::_cachable_type_on_success_and_failure);
	url->set_request_type(_network_http_request_queue_type_optional);
}

void __cdecl online_url_make_vidmaster_popup_image(c_url_string* url)
{
	//INVOKE(0x004520C0, online_url_make_vidmaster_popup_image, url);

	create_title_url_base(url);
	url->get_url_string()->append_print("%s/black_vidmaster_popup_image.jpg", get_current_language_suffix(false));
	url->set_cachable(c_url_string::_cachable_type_on_success_and_failure);
	url->set_request_type(_network_http_request_queue_type_optional);
}

void __cdecl online_url_use_hopper_directory(const char* hopper_directory)
{
	//INVOKE(0x00452100, online_url_use_hopper_directory, hopper_directory);

	c_static_string<64> text = hopper_directory;
	if (!text.is_equal(g_online_url.current_hopper_directory))
	{
		text.copy_to(g_online_url.current_hopper_directory, NUMBEROF(g_online_url.current_hopper_directory));
		g_online_url_hopper_directory_key++;
	}
}

void __cdecl online_url_use_user_override_hopper_directory(const char* hopper_directory)
{
	//INVOKE(0x00452150, online_url_use_user_override_hopper_directory, hopper_directory);

	c_static_string<64> text = hopper_directory;
	if (!text.is_equal(g_online_url.current_user_override_hopper_directory))
	{
		text.copy_to(g_online_url.current_user_override_hopper_directory, NUMBEROF(g_online_url.current_user_override_hopper_directory));
		g_online_url_hopper_directory_key++;
	}
}

void c_url_string::set(const c_url_string* url)
{
	//INVOKE_CLASS_MEMBER(0x004521E0, c_url_string, set, url);

	m_url_string = url->m_url_string.get_string();
	m_service_type = url->m_service_type;
	m_cachable_type = url->m_cachable_type;
	m_untracked_cache_lifetime_seconds = url->m_untracked_cache_lifetime_seconds;
	m_request_type = url->m_request_type;
}

void c_url_string::set_cachable(e_cachable_type value)
{
	//INVOKE_CLASS_MEMBER(0x00452230, c_url_string, set_cachable, value);

	m_cachable_type = value;
}

void c_url_string::set_request_type(e_network_http_request_queue_type value)
{
	//INVOKE_CLASS_MEMBER(0x00452240, c_url_string, set_request_type, value);

	m_request_type = value;
}

void c_url_string::set_service_type(e_online_lsp_service_type value)
{
	//INVOKE_CLASS_MEMBER(0x00452250, c_url_string, set_service_type, value);

	m_service_type = value;
}

void c_url_string::set_untracked_cache_lifetime_seconds(int32 value)
{
	m_untracked_cache_lifetime_seconds = value;
}

