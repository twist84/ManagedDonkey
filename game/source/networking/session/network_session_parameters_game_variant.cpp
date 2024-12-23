#include "networking/session/network_session_parameters_game_variant.hpp"

#include "cseries/cseries_events.hpp"

c_game_variant const* c_network_session_parameter_game_variant::get() const
{
	if (!get_allowed())
	{
		WARNING_EVENT("networking:session_parameters:chunked:game_variant: [%s] can't get variant, unavailable", get_session_description());
		return nullptr;
	}

	return &m_data.game_variant;
}

bool c_network_session_parameter_game_variant::request_change(c_game_variant const* game_variant)
{
	if (set_allowed())
		return set(game_variant);

	if (!change_request_allowed())
	{
		WARNING_EVENT("networking:session_parameters:chunked:game_variant: [%s] failed to request change, access denied [%s]", get_session_description(), get_change_request_denied_reason());
		return false;
	}

	if (game_variant)
	{
		m_change_request_data.valid = true;
		m_change_request_data.game_variant.copy_from_and_validate(game_variant);
	}
	else
	{
		m_change_request_data.valid = false;
		csmemset(&m_change_request_data.game_variant, 0, sizeof(c_game_variant));
	}

	set_desires_change_request();
	return true;
}

bool c_network_session_parameter_game_variant::set(c_game_variant const* game_variant)
{
	return INVOKE_CLASS_MEMBER(0x00456220, c_network_session_parameter_game_variant, set, game_variant);
}

