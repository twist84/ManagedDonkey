#include "interface/chud/chud_definitions.hpp"

s_chud_widget_base* s_chud_widget_collection::get_widget_by_index(int32 widget_index, bool* is_text_widget)
{
	return INVOKE_CLASS_MEMBER(0x00AD5F40, s_chud_widget_collection, get_widget_by_index, widget_index, is_text_widget);
}

int32 s_chud_widget_collection::get_widget_count() const
{
	return INVOKE_CLASS_MEMBER(0x00AD5F80, s_chud_widget_collection, get_widget_count);
}

int32 s_chud_widget_collection::get_widget_runtime_index(int32 widget_index)
{
	return INVOKE_CLASS_MEMBER(0x00AD5F90, s_chud_widget_collection, get_widget_runtime_index, widget_index);
}

