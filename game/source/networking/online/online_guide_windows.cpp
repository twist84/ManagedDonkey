#include "networking/online/online_guide_windows.hpp"

#include "cseries/async_xoverlapped_memory.hpp"
#include "cseries/console.hpp"
#include "interface/user_interface_text.hpp"
#include "memory/module.hpp"

#include <string.h>

HOOK_DECLARE_CLASS(0x004E16A0, c_virtual_keyboard_task, constructor);
HOOK_DECLARE_CLASS(0x004E1840, c_virtual_keyboard_task, get_instance);
HOOK_DECLARE_CLASS(0x004E1A20, c_virtual_keyboard_task, success);

c_virtual_keyboard_task* __fastcall c_virtual_keyboard_task::constructor(
	c_virtual_keyboard_task* _this,
	const char* file,
	long line,
	long controller_index,
	const wchar_t* default_text,
	const wchar_t* title_text,
	const wchar_t* description_text,
	dword maximum_input_characters
)
{
	FUNCTION_BEGIN(true);

	c_virtual_keyboard_task* result = nullptr;
	HOOK_INVOKE_CLASS(result =, c_virtual_keyboard_task, constructor, 
		c_virtual_keyboard_task * (__thiscall*)(
			c_virtual_keyboard_task*,
			char const*,
			long,
			long,
			wchar_t const*,
			wchar_t const*,
			wchar_t const*,
			dword,
			long,
			bool
		),
		_this,
		file,
		line,
		controller_index,
		default_text,
		title_text,
		description_text,
		maximum_input_characters,
		0,
		false
	);
	return result;
}

void c_overlapped_task::set_file(char const* file)
{
	FUNCTION_BEGIN(true);

	m_file = file;
}

void c_overlapped_task::set_line(long line)
{
	FUNCTION_BEGIN(true);

	m_line = line;
}

void __fastcall c_virtual_keyboard_task::set_controller_index(c_virtual_keyboard_task* _this, long controller_index)
{
	FUNCTION_BEGIN(true);

	_this->m_controller_index = controller_index;
}

void __fastcall c_virtual_keyboard_task::set_default_text(c_virtual_keyboard_task* _this, wchar_t const* default_text)
{
	FUNCTION_BEGIN(true);

	if (default_text)
		wcscpy_s(_this->m_default_text, default_text);
	else
		_this->m_default_text[0] = 0;
}

void __fastcall c_virtual_keyboard_task::set_title_text(c_virtual_keyboard_task* _this, wchar_t const* title_text)
{
	FUNCTION_BEGIN(true);

	if (title_text)
		wcscpy_s(_this->m_title_text, title_text);
	else
		_this->m_title_text[0] = 0;
}

void __fastcall c_virtual_keyboard_task::set_description_text(c_virtual_keyboard_task* _this, wchar_t const* description_text)
{
	FUNCTION_BEGIN(true);

	if (description_text)
		wcscpy_s(_this->m_description_text, description_text);
	else
		_this->m_description_text[0] = 0;
}

void __fastcall c_virtual_keyboard_task::set_maximum_input_characters(c_virtual_keyboard_task* _this, dword maximum_input_characters)
{
	FUNCTION_BEGIN(true);

	_this->m_maximum_input_characters = maximum_input_characters;
}

void __fastcall c_virtual_keyboard_task::set_character_flags(c_virtual_keyboard_task* _this, dword_flags character_flags)
{
	FUNCTION_BEGIN(true);

	_this->m_character_flags = character_flags;
}

c_virtual_keyboard_task* __cdecl c_virtual_keyboard_task::get_instance(
	char const* file,
	long line,
	long controller_index,
	wchar_t const* default_text,
	wchar_t const* title_text,
	wchar_t const* description_text,
	dword maximum_input_characters,
	dword_flags character_flags
)
{
	FUNCTION_BEGIN(true);

	//c_virtual_keyboard_task* result = nullptr;
	//HOOK_INVOKE_CLASS(result =, c_virtual_keyboard_task, get_instance, decltype(get_instance)*,
	//	file,
	//	line,
	//	controller_index,
	//	default_text,
	//	title_text,
	//	description_text,
	//	maximum_input_characters,
	//	character_flags);
	//return result;

	if (!m_instance)
	{
		m_instance = (c_virtual_keyboard_task*)overlapped_malloc_tracked(sizeof(c_virtual_keyboard_task), __FILE__, __LINE__);
		m_instance = m_instance->constructor(
			m_instance,
			file,
			line,
			controller_index,
			default_text,
			title_text,
			description_text,
			maximum_input_characters
		);
	}
	else
	{
		if (!m_instance->busy())
		{
			m_instance->set_file(file);
			m_instance->set_line(line);
			m_instance->set_controller_index(m_instance, controller_index);
			m_instance->set_default_text(m_instance, default_text);
			m_instance->set_title_text(m_instance, title_text);
			m_instance->set_description_text(m_instance, description_text);
			m_instance->set_maximum_input_characters(m_instance, maximum_input_characters);
			m_instance->set_character_flags(m_instance, character_flags);
		}
		else
			c_console::write_line("ui: someone tried to get a duplicate instance of the virtual keyboard!");
	}

	return m_instance;
}

c_virtual_keyboard_task* c_virtual_keyboard_task::m_instance = nullptr;

void __fastcall c_virtual_keyboard_task::success(c_virtual_keyboard_task* _this, dword a1)
{
	FUNCTION_BEGIN(true);

	HOOK_INVOKE_CLASS(, c_virtual_keyboard_task, success, void(__thiscall*)(c_virtual_keyboard_task*, dword), _this, a1);

	if (!wcsncmp(_this->m_keyboard_results, L".fortune", 9))
		wcsncpy_s(_this->m_keyboard_results, L"My modem is on file", 256);
	wchar_string_sanitize_for_game(_this->m_keyboard_results, 256);
}
