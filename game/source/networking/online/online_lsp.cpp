#include "networking/online/online_lsp.hpp"

#include "memory/module.hpp"

HOOK_DECLARE_CLASS(0x00431440, c_online_lsp_manager, acquire_server);

//enum e_online_lsp_server_acquire_result __cdecl c_online_lsp_manager::acquire_server(enum e_online_lsp_service_type service_type, long, long* connection_token_out, long* ip_address, unsigned short* port, char const* service_description)
long __fastcall c_online_lsp_manager::acquire_server(void* _this, void* unused, long service_type, long* connection_token_out, long* ip_address, unsigned short* port, char const* service_description)
{
    return 0;
}

