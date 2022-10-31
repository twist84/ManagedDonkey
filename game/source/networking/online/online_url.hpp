#pragma once

#include "cseries/cseries.hpp"

struct c_url_string
{
	c_static_string<256> m_string;
	long m_service_type;
	dword __unknown104;
	dword m_untracked_cache_lifetime_seconds;
	dword m_cachable;
};
static_assert(sizeof(c_url_string) == 0x110);

extern bool g_title_url_base_override;
const char k_title_url_base_override[] = "/storage/";
const long k_title_url_base_override_length = sizeof(k_title_url_base_override) - 1;

extern long g_title_url_base_length;

extern void create_title_url_base(c_url_string* url);