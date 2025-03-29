#include "game/game_engine_event_definitions.hpp"

void s_multiplayer_event_sound_response_definition::update_reference_names()
{
	UPDATE_ARRAY_REFERENCE_NAMES(sounds);
}

void s_multiplayer_event_response_definition::update_reference_names()
{
	UPDATE_STRUCT_REFERENCE_NAMES(default_sound);

	UPDATE_BLOCK_REFERENCE_NAMES(sound_permutations);
}

