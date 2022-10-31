#include "networking/logic/storage/network_http_buffer_downloader.hpp"

#include "cseries/console.hpp"
#include "memory/module.hpp"

HOOK_DECLARE_CLASS(0x00463A90, c_http_buffer_downloader, get_data);
HOOK_DECLARE_CLASS(0x00463B30, c_http_buffer_downloader, get_download_status);

long __fastcall c_http_buffer_downloader::get_data(c_http_buffer_downloader* _this, void* unused, char const** buffer, long* buffer_size)
{
    if (buffer)
        *buffer = 0;

    long internal_status = _this->m_internal_status;
    long download_status = get_download_status(_this);

    if (internal_status == 5 || internal_status == 4)
    {
        if (buffer)
            *buffer = _this->m_buffer;

        if (buffer_size)
            *buffer_size = _this->m_buffer_size;
    }

    return download_status;
}

long __fastcall c_http_buffer_downloader::get_download_status(c_http_buffer_downloader* _this)
{
    long result = 0;
    switch (_this->m_internal_status)
    {
    case 0:
        // user file download failed
        result = 0;
        break;

    case 1:
    case 2:
    case 3:
        result = 1;
        break;

    case 4:
    case 5:
        // user file downloaded
        result = 2; // _http_download_status_succeeded
        break;

    case 6:
        result = 3;
        break;

    case 7:
        // user file not found
        result = 4;
        break;
    }

    c_console::write_line(_this->m_url.m_string.get_string() + (g_title_url_base_length == k_title_url_base_override_length ? 0 : g_title_url_base_length));

    return result;
}

