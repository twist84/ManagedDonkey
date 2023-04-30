#include "networking/logic/storage/network_http_buffer_downloader.hpp"

#include "cseries/cseries_console.hpp"
#include "memory/module.hpp"

HOOK_DECLARE_CLASS(0x00463A90, c_http_buffer_downloader, get_data);
HOOK_DECLARE_CLASS(0x00463B30, c_http_buffer_downloader, get_download_status);
HOOK_DECLARE_CLASS(0x00463B80, c_http_buffer_downloader, get_download_status_from_internal_status);
HOOK_DECLARE_CLASS(0x004649A0, c_http_buffer_downloader, update);

e_download_status __fastcall c_http_buffer_downloader::get_data(c_http_buffer_downloader* _this, void* unused, char const** buffer, long* buffer_size)
{
    update(_this);

    if (buffer)
        *buffer = 0;

    e_internal_status internal_status = _this->m_internal_status;
    e_download_status download_status = get_download_status(_this);

    if (_this->m_buffer && !(internal_status || download_status))
    {
        internal_status = _internal_status_succeeded;
        download_status = _http_download_status_succeeded;
    }

    if (internal_status == _internal_status_succeeded || internal_status == _internal_status_cache_submitting)
    {
        if (buffer)
            *buffer = _this->m_buffer;

        if (buffer_size)
            *buffer_size = _this->m_buffer_size;
    }

    return download_status;
}

e_download_status __fastcall c_http_buffer_downloader::get_download_status(c_http_buffer_downloader* _this)
{
    //c_console::write_line(_this->m_url.m_string.get_string() + (g_title_url_base_length == k_title_url_base_override_length ? 0 : g_title_url_base_length));

    return get_download_status_from_internal_status(_this->m_internal_status);
}

e_download_status __cdecl c_http_buffer_downloader::get_download_status_from_internal_status(e_internal_status internal_status)
{
    e_download_status result = _http_download_status_none;
    switch (internal_status)
    {
    case _internal_status_none:
        result = _http_download_status_none;
        break;

    case _internal_status_cache_retrieving:
    case _internal_status_unknown2:
    case _internal_status_unknown3:
        result = _http_download_status_pending;
        break;

    case _internal_status_cache_submitting:
    case _internal_status_succeeded:
        result = _http_download_status_succeeded;
        break;

    case _internal_status_failed:
        result = _http_download_status_failed;
        break;

    case _internal_status_failed_file_not_found:
        result = _http_download_status_failed_file_not_found;
        break;
    }

    return result;
}

void __fastcall c_http_buffer_downloader::update(c_http_buffer_downloader* _this)
{
    HOOK_INVOKE_CLASS(, c_http_buffer_downloader, update, void(__thiscall*)(c_http_buffer_downloader*), _this);
}

