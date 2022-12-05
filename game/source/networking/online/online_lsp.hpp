#pragma once

struct c_online_lsp_manager
{
	static long __fastcall acquire_server(void* _this, void* unused, long service_type, long* connection_token_out, long* ip_address, unsigned short* port, char const* service_description);
};

