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

