#include "networking/online/online_url.hpp"

#include "memory/module.hpp"
#include "networking/logic/storage/network_http_buffer_downloader.hpp"
#include "text/unicode.hpp"

HOOK_DECLARE(0x004515F0, create_title_url_base);

bool g_title_url_base_override = true;
long g_title_url_base_length = k_title_url_base_override_length;

void create_title_url_base(c_url_string* url)
{
	HOOK_INVOKE(, create_title_url_base, url);

	if (g_title_url_base_override)
		url->m_string.set(k_title_url_base_override);

	g_title_url_base_length = url->m_string.length();
}

