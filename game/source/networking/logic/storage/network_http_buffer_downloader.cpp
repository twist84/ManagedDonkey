#include "networking/logic/storage/network_http_buffer_downloader.hpp"

#include "memory/module.hpp"

HOOK_DECLARE_CLASS_MEMBER(0x00463A90, c_http_buffer_downloader, get_data);
HOOK_DECLARE_CLASS_MEMBER(0x00463B30, c_http_buffer_downloader, get_download_status);
HOOK_DECLARE_CLASS(0x00463B80, c_http_buffer_downloader, get_download_status_from_internal_status);
HOOK_DECLARE_CLASS_MEMBER(0x004649A0, c_http_buffer_downloader, update);

e_download_status __thiscall c_http_buffer_downloader::get_data(char const** buffer, long* buffer_size)
{
	update();

	if (buffer)
		*buffer = 0;

	e_internal_status internal_status = m_internal_status;
	e_download_status download_status = get_download_status();

	if (m_buffer && !(internal_status || download_status))
	{
		internal_status = _internal_status_succeeded;
		download_status = _http_download_status_succeeded;
	}

	if (internal_status == _internal_status_succeeded || internal_status == _internal_status_cache_submitting)
	{
		if (buffer)
			*buffer = m_buffer;

		if (buffer_size)
			*buffer_size = m_buffer_size;
	}

	return download_status;
}

e_download_status __thiscall c_http_buffer_downloader::get_download_status()
{
	return get_download_status_from_internal_status(m_internal_status);
}

e_download_status __cdecl c_http_buffer_downloader::get_download_status_from_internal_status(e_internal_status internal_status)
{
	switch (internal_status)
	{
	case _internal_status_none:
		return _http_download_status_none;
	case _internal_status_cache_retrieving:
	case _internal_status_unknown2:
	case _internal_status_unknown3:
		return _http_download_status_pending;
	case _internal_status_cache_submitting:
	case _internal_status_succeeded:
		return _http_download_status_succeeded;
	case _internal_status_failed:
		return _http_download_status_failed;
	case _internal_status_failed_file_not_found:
		return _http_download_status_failed_file_not_found;
	}

	return _http_download_status_none;
}

void __thiscall c_http_buffer_downloader::update()
{
	HOOK_INVOKE_CLASS(, c_http_buffer_downloader, update, void(__thiscall*)(c_http_buffer_downloader*), this);
}

