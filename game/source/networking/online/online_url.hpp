#pragma once

#include "cseries/cseries.hpp"

enum e_online_lsp_service_type;
enum e_network_http_request_queue_type;

struct c_url_string
{
	enum e_cachable_type;

	c_static_string<256> m_string;
	e_online_lsp_service_type m_service_type;
	e_cachable_type m_cachable;
	long m_untracked_cache_lifetime_seconds;
	e_network_http_request_queue_type m_request_type;
};
static_assert(sizeof(c_url_string) == 0x110);

extern bool g_title_url_base_override;
char const k_title_url_base_override[] = "/storage/";
long const k_title_url_base_override_length = sizeof(k_title_url_base_override) - 1;

extern long g_title_url_base_length;

extern void create_title_url_base(c_url_string* url);

