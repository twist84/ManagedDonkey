#include "networking/online/online_lsp.hpp"

#include "memory/module.hpp"

REFERENCE_DECLARE_ARRAY(0x0199FD80, c_static_string<4>, g_server_descriptions, 9);
REFERENCE_DECLARE_ARRAY(0x0199FDA8, s_server_connect_info, g_additional_raw_servers, 1);
REFERENCE_DECLARE(0x0199FE88, c_online_lsp_manager, g_online_lsp_manager);

HOOK_DECLARE_CLASS(0x00431440, c_online_lsp_manager, acquire_server);

//enum e_online_lsp_server_acquire_result __cdecl c_online_lsp_manager::acquire_server(enum e_online_lsp_service_type service_type, long, long* connection_token_out, long* ip_address, unsigned short* port, char const* service_description)
long __fastcall c_online_lsp_manager::acquire_server(c_online_lsp_manager* _this, void* unused, long service_type, long* connection_token_out, long* ip_address, unsigned short* port, char const* service_description)
{
    return 0;
}

