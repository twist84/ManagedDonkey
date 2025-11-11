#include "ai/ai_globals.hpp"

void s_ai_globals_definition::update_reference_names()
{
	UPDATE_REFERENCE_NAME(global_dialogue);

	UPDATE_BLOCK_REFERENCE_NAMES(styles);
	UPDATE_BLOCK_REFERENCE_NAMES(spawn_formations);
	UPDATE_BLOCK_REFERENCE_NAMES(squad_templates);
}

void s_ai_globals_styles_definition::update_reference_names()
{
	UPDATE_REFERENCE_NAME(style);
}

void s_ai_globals_formation_definition::update_reference_names()
{
	UPDATE_REFERENCE_NAME(formation);
}

void s_ai_globals_squad_template_definition::update_reference_names()
{
	UPDATE_REFERENCE_NAME(squad_template);
}

