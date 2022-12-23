#include "dialogs/show_direct_connect.hpp"

#include "cseries/cseries.hpp"
#include "interface/user_interface_networking.hpp"
#include "memory/module.hpp"
#include "networking/session/network_managed_session.hpp"
#include "networking/tools/network_debug_dump.hpp"
#include "networking/transport/transport_security.hpp"
#include "shell/shell.hpp"
#include "text/unicode.hpp"
#include "xbox/xnet.hpp"
#include "xbox/xbox.hpp"

void __cdecl direct_connect(transport_address address, s_transport_session_description description)
{
	XNetAddEntry(&address, &description.address, &description.id);
	sub_69D600();
	user_interface_join_remote_session(false, _network_session_class_system_link, &description.id, &description.address, &description.key);
}

void show_direct_connect_dialog()
{
	bool dialog_succeeded = false;

	c_static_wchar_string<16> insecure_ip;
	c_static_wchar_string<16> port;
	c_static_wchar_string<128> secure_ip;
	c_static_wchar_string<128> session_id;
	{
		c_static_string<16> _insecure_ip;
		c_static_string<128> _secure_ip;
		get_system_ip_addresses(&_insecure_ip, &_secure_ip);

		char const* _session_id = managed_session_get_id_string(1);

		insecure_ip.print(L"%hs", _insecure_ip.get_string());
		secure_ip.print(L"%hs", _secure_ip.get_string());

		session_id.print(L"%hs", _session_id);

		REFERENCE_DECLARE(0x01860454, word, game_port);
		port.print(L"%hd", game_port);
	}

	transport_address address{};
	s_transport_session_description description{};
	{
		wchar_t result_ip_text[128]{};
		wchar_t result_port_text[128]{};
		wchar_t result_id_text[128]{};
		wchar_t result_address_text[128]{};
		XShowConnectUI(insecure_ip.get_string(), port.get_string(), session_id.get_string(), secure_ip.get_string(), result_ip_text, result_port_text, result_id_text, result_address_text, get_donkey_module(), &dialog_succeeded);

		c_static_wchar_string<32> ip_port_str;
		ip_port_str.print(L"%s:%s", result_ip_text, result_port_text);

		transport_address_from_string(ip_port_str.get_string(), address);
		transport_secure_identifier_from_string(result_id_text, description.id);
		transport_secure_address_from_string(result_address_text, description.address);
	}

	if (dialog_succeeded)
		direct_connect(address, description);
}

