#include "interface/chud/chud_messaging.hpp"

#include "cache/cache_files.hpp"
#include "cseries/cseries.hpp"
#include "game/player_control.hpp"
#include "game/players.hpp"
#include "items/item_definitions.hpp"
#include "items/items.hpp"
#include "memory/module.hpp"

HOOK_DECLARE_CALL(0x00A970BB, chud_post_action_response); // c_chud_messaging_manager::update

//.text:00A95850 ; void __cdecl chud_messaging_picked_up_powerup(long, long)
//.text:00A95890 ; void __cdecl chud_messaging_picked_up_weapon(long, long)
//.text:00A958D0 ; void __cdecl chud_messaging_post(long, long, e_chud_message_context)

void __cdecl chud_messaging_post(long user_index, wchar_t const* message_text, e_chud_message_context chud_message_context)
{
	INVOKE(0x00A95920, chud_messaging_post, user_index, message_text, chud_message_context);
}

//.text:00A95950 ; void __cdecl chud_messaging_post_progression_toast(long, long, real)
//.text:00A95990 ; 
//.text:00A959C0 ; 
//.text:00A95A10 ; 
//.text:00A95A40 ; 
//.text:00A95A90 ; 
//.text:00A95AC0 ; void __cdecl chud_messaging_post_primary(long, wchar_t const*, long, e_chud_message_context)
//.text:00A95B40 ; 

void __cdecl chud_messaging_special_autosave(bool end)
{
	INVOKE(0x00A95B60, chud_messaging_special_autosave, end);
}

void __cdecl chud_messaging_special_load(bool end)
{
	INVOKE(0x00A95BF0, chud_messaging_special_load, end);
}

void __cdecl chud_messaging_special_saving(bool end)
{
	INVOKE(0x00A95C80, chud_messaging_special_saving, end);
}

void __cdecl chud_post_action_response(long user_index)
{
	//INVOKE(0x00A95D10, chud_post_action_response, user_index);

	s_player_action_context const* action_context = player_control_get_action_context(user_index);
	if (action_context->interaction.type == 1 || action_context->interaction.type == 9)
	{
		if (!player_interaction_exists(user_index, _object_mask_item, &action_context->interaction))
			return;

		item_datum* item = (item_datum*)object_get_and_verify_type(action_context->interaction.object_index, _object_mask_item);
		struct item_definition* item_definition = (struct item_definition*)tag_get(ITEM_TAG, item->definition_index);

		string_id message = _string_id_empty_string;
		string_id message_dual = _string_id_empty_string;

		if (TEST_BIT(action_context->interaction.seat_index, 0))
			message = item_definition->item.pickup_message.get_value();
		else if (TEST_BIT(action_context->interaction.seat_index, 1))
			message = item_definition->item.swap_message.get_value();

		if (TEST_BIT(action_context->interaction.seat_index, 2))
			message_dual = item_definition->item.pickup_message_dual.get_value();
		else if (TEST_BIT(action_context->interaction.seat_index, 3))
			message_dual = item_definition->item.swap_message_dual.get_value();

		if (message != _string_id_empty_string)
			sub_A964E0(user_index, message, NONE, 0);

		if (message_dual != _string_id_empty_string)
			sub_A964E0(user_index, message_dual, NONE, 1);
	}

	// call the original function
	INVOKE(0x00A95D10, chud_post_action_response, user_index);
}

bool __cdecl sub_A964E0(long user_index, long message, long a3, long placement)
{
	return INVOKE(0x00A964E0, sub_A964E0, user_index, message, a3, placement);
}

