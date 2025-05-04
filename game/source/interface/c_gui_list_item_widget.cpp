#include "interface/c_gui_list_item_widget.hpp"

//.text:00B18260 ; public: __cdecl c_gui_list_item_widget::c_gui_list_item_widget()
//.text:00B18350 ; 
//.text:00B183B0 ; public: virtual void* __cdecl c_gui_list_item_widget::`vector deleting destructor'(unsigned int)
//.text:00B183E0 ; public: virtual bool __cdecl c_gui_list_item_widget::can_receive_focus()
//.text:00B18420 ; public: virtual c_gui_bitmap_widget* __cdecl c_gui_list_item_widget::create_bitmap_widget(s_runtime_bitmap_widget_definition const*)
//.text:00B18430 ; public: virtual c_gui_button_key_widget* __cdecl c_gui_list_item_widget::create_button_key_widget(s_button_key_definition const*)
//.text:00B18440 ; public: virtual c_gui_group_widget* __cdecl c_gui_list_item_widget::create_group_widget(s_group_widget_definition const*)
//.text:00B18450 ; public: virtual c_gui_list_item_widget* __cdecl c_gui_list_item_widget::create_list_item_widget(s_list_item_widget_block const*)
//.text:00B18460 ; public: virtual c_gui_list_widget* __cdecl c_gui_list_item_widget::create_list_widget(s_list_widget_block const*)
//.text:00B18470 ; public: virtual c_gui_model_widget* __cdecl c_gui_list_item_widget::create_model_widget(s_model_widget_block const*)
//.text:00B18480 ; public: virtual c_gui_text_widget* __cdecl c_gui_list_item_widget::create_text_widget(s_runtime_text_widget_definition const*)
//.text:00B18490 ; public: virtual s_runtime_core_widget_definition* __cdecl c_gui_list_item_widget::get_core_definition()
//.text:00B184A0 ; public: virtual int32 __cdecl c_gui_list_item_widget::get_element_handle()
//.text:00B184C0 ; public: int32 __cdecl c_gui_list_item_widget::get_item_label()

int32 c_gui_list_item_widget::get_list_item_index()
{
	return INVOKE_CLASS_MEMBER((uns32)__vftable->__funcs[37], c_gui_list_item_widget, get_list_item_index);
	//return m_list_item_index;
}

//.text:00B184D0 ; 
//.text:00B184E0 ; public: c_gui_list_item_widget* __cdecl c_gui_list_item_widget::get_sibling_list_item_widget(bool, int)
//.text:00B18520 ; 

