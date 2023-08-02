#include "game/game_engine_event_definitions.hpp"

void s_sound_response_extra_sounds_definition::update_reference_names()
{
	UPDATE_REFERENCE_NAME(japanese_sound);
	UPDATE_REFERENCE_NAME(german_sound);
	UPDATE_REFERENCE_NAME(french_sound);
	UPDATE_REFERENCE_NAME(spanish_sound);
	UPDATE_REFERENCE_NAME(mexican_sound);
	UPDATE_REFERENCE_NAME(italian_sound);
	UPDATE_REFERENCE_NAME(korean_sound);
	UPDATE_REFERENCE_NAME(chinese_sound_traditional);
	UPDATE_REFERENCE_NAME(chinese_sound_simplified);
	UPDATE_REFERENCE_NAME(portuguese_sound);
	UPDATE_REFERENCE_NAME(russian_sound);
}

void s_multiplayer_event_sound_response_definition::update_reference_names()
{
	UPDATE_REFERENCE_NAME(english_sound);

	UPDATE_STRUCT_REFERENCE_NAMES(extra_sounds);
}

void s_multiplayer_event_response_definition::update_reference_names()
{
	UPDATE_REFERENCE_NAME(sound);

	UPDATE_STRUCT_REFERENCE_NAMES(extra_sounds);

	UPDATE_BLOCK_REFERENCE_NAMES(sound_permutations);
}

