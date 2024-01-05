#pragma once

#include "cseries/cseries.hpp"

enum
{
	k_title_id = 0x4D530877
};

enum e_online_lsp_service_type;
enum e_network_http_request_queue_type;

struct c_url_string
{
	enum e_cachable_type
	{
		_cachable_type_unknown0 = 0,
		_cachable_type_unknown1,
		_cachable_type_unknown2,
	};

	c_url_string(char const* url, e_cachable_type cachable);
	c_url_string();

	char const* get_string() const;
	e_cachable_type get_cachable() const;
	e_network_http_request_queue_type get_request_type() const;
	e_online_lsp_service_type get_service_type() const;
	long get_untracked_cache_lifetime_seconds() const;

	void set(c_url_string const* other);
	void set_cachable(e_cachable_type cachable);
	void set_request_type(e_network_http_request_queue_type queue_type);
	void set_service_type(e_online_lsp_service_type service_type);

	void operator=(c_url_string const* other) { set(other); }

	c_static_string<256> m_string;
	e_online_lsp_service_type m_service_type;
	e_cachable_type m_cachable;
	long m_untracked_cache_lifetime_seconds;
	e_network_http_request_queue_type m_request_type;
};
static_assert(sizeof(c_url_string) == 0x110);

struct _g_online_url // supposed to be unnamed
{
	char hopper_directory[64];
	char user_override_hopper_directory[64];

	// titles
	c_static_string<4> __unknown80; // unknown
	c_static_string<4> __unknown84; // Halo 3
	c_static_string<4> __unknown88; // Halo 3: Mythic
	c_static_string<4> __unknown8C; // Halo 3: ODST and Halo Online
	c_static_string<4> __unknown90; // unknown
};
extern _g_online_url& g_online_url;

struct s_network_storage_cache_key;

extern void __cdecl create_machine_url_base(c_url_string* url, qword machine_id);
extern void __cdecl create_title_url_base(c_url_string* url);
extern void __cdecl create_user_url_base(c_url_string* url, qword user_id);
extern void __cdecl make_hopper_network_directory(char* hopper_directory, long hopper_directory_size, char some_char);
extern void __cdecl online_url_dispose();
extern char const* __cdecl online_url_get_title();
extern void __cdecl online_url_initialize();
extern void __cdecl online_url_make_bnet_consume_begin(c_url_string* url, qword user_id, dword consumable_id);
extern void __cdecl online_url_make_bnet_consume_complete(c_url_string* url, qword user_id, dword consumable_id);
extern void __cdecl online_url_make_bnet_subscription_get_details(c_url_string* url, qword user_id, long game_region, long profile_region, bool extras_portal_debug);
extern void __cdecl online_url_make_cache_key(c_url_string const* url, s_network_storage_cache_key* cache_key);
extern void __cdecl online_url_make_matchmaking_banhammer_message(c_url_string* url);
extern void __cdecl online_url_make_matchmaking_descriptions(c_url_string* url);
extern void __cdecl online_url_make_matchmaking_game_variant(c_url_string* url, word hopper_id, char const* variant_name);
extern void __cdecl online_url_make_matchmaking_gameset(c_url_string* url, word hopper_id);
extern void __cdecl online_url_make_matchmaking_histogram(c_url_string* url, word hopper_id, long hopper_skill);
extern void __cdecl online_url_make_matchmaking_hopper(c_url_string* url);
extern void __cdecl online_url_make_matchmaking_machine(c_url_string* url, qword machine_id);
extern void __cdecl online_url_make_matchmaking_map_variant(c_url_string* url, word hopper_id, char const* variant_name);
extern void __cdecl online_url_make_matchmaking_nightmap(c_url_string* url);
extern void __cdecl online_url_make_matchmaking_statistics(c_url_string* url);
extern void __cdecl online_url_make_matchmaking_tips(c_url_string* url);
extern void __cdecl online_url_make_matchmaking_user(c_url_string* url, qword user_id);
extern void __cdecl online_url_make_matchmaking_user_recent_players(c_url_string* url, qword user_id);
extern void __cdecl online_url_make_message_of_the_day(c_url_string* url);
extern void __cdecl online_url_make_message_of_the_day_image(c_url_string* url);
extern void __cdecl online_url_make_message_of_the_day_popup(c_url_string* url);
extern void __cdecl online_url_make_message_of_the_day_popup_image(c_url_string* url);
extern void __cdecl online_url_make_network_configuration(c_url_string* url);
extern void __cdecl online_url_make_network_manifest(c_url_string* url);
extern void __cdecl online_url_make_network_map_signatures(c_url_string* url);
extern void __cdecl online_url_make_update_machine_network_stats(c_url_string* url);
extern void __cdecl online_url_make_update_user_highest_skill(c_url_string* url, qword user_id, long highest_skill);
extern void __cdecl online_url_make_upload_saved_screenshot(c_url_string* url);
extern void __cdecl online_url_make_user_file(c_url_string* url, qword user_id);
extern void __cdecl online_url_make_vidmaster_popup(c_url_string* url);
extern void __cdecl online_url_make_vidmaster_popup_image(c_url_string* url);
extern void __cdecl online_url_use_hopper_directory(char const* hopper_directory);
extern void __cdecl online_url_use_user_override_hopper_directory(char const* hopper_directory);

