#pragma once

#include "cseries/cseries.hpp"

enum e_bungie_title_index
{
	// pre-release
	_bungie_title_index_halo3_legacy = 0,

	// release
	_bungie_title_index_halo3,

	// mythic
	_bungie_title_index_halo3_blue,

	// ODST
	_bungie_title_index_halo3_black,

	// ?
	_bungie_title_index_halo3_shared_content,

	// the current project
	_bungie_title_index_donkey,

	k_bungie_title_index_count,
};

class c_url_string
{
public:
	enum e_cachable_type
	{
		_cachable_type_no = 0,
		_cachable_type_on_success,
		_cachable_type_on_success_and_failure,
	};

	c_url_string(const char* url, e_cachable_type cachable);
	c_url_string();

	void clear();

	e_cachable_type get_cachable() const;
	e_network_http_request_queue_type get_request_type() const;
	e_online_lsp_service_type get_service_type() const;
	const char* get_string() const;
	int32 get_untracked_cache_lifetime_seconds() const;
	c_static_string<256>* get_url_string();

	bool is_empty() const;
	bool is_equal(const c_url_string*) const;

	void set(const c_url_string* other);
	void set_cachable(e_cachable_type cachable);
	void set_request_type(e_network_http_request_queue_type value);
	void set_service_type(e_online_lsp_service_type value);
	void set_untracked_cache_lifetime_seconds(int32 value);

private:
	c_static_string<256> m_url_string;
	e_online_lsp_service_type m_service_type;
	e_cachable_type m_cachable_type;
	int32 m_untracked_cache_lifetime_seconds;
	e_network_http_request_queue_type m_request_type;
};
static_assert(sizeof(c_url_string) == 0x110);

struct _g_online_url // supposed to be unnamed
{
	char current_hopper_directory[64];
	char current_user_override_hopper_directory[64];
	c_static_string<4> title_strings[k_bungie_title_index_count];
};
extern _g_online_url& g_online_url;

struct s_network_storage_cache_key
{
	char value[80];
};
static_assert(sizeof(s_network_storage_cache_key) == 0x50);

extern int32& g_online_url_hopper_directory_key;
extern int32& k_default_untracked_lifetime_seconds;

extern void __cdecl create_machine_url_base(c_url_string* url, uns64 machine_id);
extern void __cdecl create_title_url_base(c_url_string* url);
extern void __cdecl create_user_url_base(c_url_string* url, uns64 xuid);
extern void __cdecl make_hopper_network_directory(char* hopper_directory, int32 hopper_directory_size, char some_char);
extern void __cdecl online_url_dispose();
extern const char* __cdecl online_url_get_title_string();
extern void __cdecl online_url_initialize();
extern void __cdecl online_url_make_bnet_consume_begin(c_url_string* url, uns64 xuid, uns32 consumable_id);
extern void __cdecl online_url_make_bnet_consume_complete(c_url_string* url, uns64 xuid, uns32 consumable_id);
extern void __cdecl online_url_make_bnet_subscription_get_details(c_url_string* url, uns64 xuid, int32 game_region, int32 profile_region, bool extras_portal_debug);
extern void __cdecl online_url_make_cache_key(const c_url_string* url, s_network_storage_cache_key* out_cache_key);
extern void __cdecl online_url_make_matchmaking_banhammer_message(c_url_string* url);
extern void __cdecl online_url_make_matchmaking_descriptions(c_url_string* url);
extern void __cdecl online_url_make_matchmaking_game_variant(c_url_string* url, uns16 hopper_id, const char* variant_name);
extern void __cdecl online_url_make_matchmaking_gameset(c_url_string* url, uns16 hopper_id);
extern void __cdecl online_url_make_matchmaking_histogram(c_url_string* url, uns16 hopper_id, int32 hopper_skill);
extern void __cdecl online_url_make_matchmaking_hopper(c_url_string* url);
extern void __cdecl online_url_make_matchmaking_machine(c_url_string* url, uns64 machine_id);
extern void __cdecl online_url_make_matchmaking_map_variant(c_url_string* url, uns16 hopper_id, const char* variant_name);
extern void __cdecl online_url_make_matchmaking_nightmap(c_url_string* url);
extern void __cdecl online_url_make_matchmaking_statistics(c_url_string* url);
extern void __cdecl online_url_make_matchmaking_tips(c_url_string* url);
extern void __cdecl online_url_make_matchmaking_user(c_url_string* url, uns64 xuid);
extern void __cdecl online_url_make_matchmaking_user_recent_players(c_url_string* url, uns64 xuid);
extern void __cdecl online_url_make_message_of_the_day(c_url_string* url);
extern void __cdecl online_url_make_message_of_the_day_image(c_url_string* url);
extern void __cdecl online_url_make_message_of_the_day_popup(c_url_string* url);
extern void __cdecl online_url_make_message_of_the_day_popup_image(c_url_string* url);
extern void __cdecl online_url_make_network_configuration(c_url_string* url);
extern void __cdecl online_url_make_network_manifest(c_url_string* url);
extern void __cdecl online_url_make_network_map_signatures(c_url_string* url);
extern void __cdecl online_url_make_update_machine_network_stats(c_url_string* url);
extern void __cdecl online_url_make_update_user_highest_skill(c_url_string* url, uns64 xuid, int32 highest_skill);
extern void __cdecl online_url_make_upload_saved_screenshot(c_url_string* url);
extern void __cdecl online_url_make_user_service_record(c_url_string* url, uns64 xuid);
extern void __cdecl online_url_make_vidmaster_popup(c_url_string* url);
extern void __cdecl online_url_make_vidmaster_popup_image(c_url_string* url);
extern void __cdecl online_url_use_hopper_directory(const char* hopper_directory);
extern void __cdecl online_url_use_user_override_hopper_directory(const char* hopper_directory);

