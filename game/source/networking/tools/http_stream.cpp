#include "networking/tools/http_stream.hpp"

#include <assert.h>
#include <string>

long c_http_stream::get_position()
{
	return m_position;
}

void c_http_stream::set_url(char const* url)
{
	assert(url);
	assert(strlen(url) < k_url_size);

	m_url.set(url);
}

char const* c_http_stream::get_url() const
{
	return m_url.get_string();
}

