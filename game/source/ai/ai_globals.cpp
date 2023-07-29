#include "ai/ai_globals.hpp"

void s_ai_globals_definition::update_reference_names()
{
	global_dialogue_tag.get_name();

	UPDATE_REFERENCE_NAMES(global_styles);
	UPDATE_REFERENCE_NAMES(spawn_formations);
	UPDATE_REFERENCE_NAMES(squad_templates);
}

void s_ai_globals_styles_definition::update_reference_names()
{
	style.get_name();
}

void s_ai_globals_formation_definition::update_reference_names()
{
	formation.get_name();
}

void s_ai_globals_squad_template_definition::update_reference_names()
{
	squad_template.get_name();
}

