#include "networking/online/online_guide_windows.hpp"

#include "cseries/async_xoverlapped_memory.hpp"
#include "cseries/console.hpp"
#include "interface/user_interface_text.hpp"
#include "memory/module.hpp"

#include <string.h>

HOOK_DECLARE(0x004E1A20, virtual_keyboard_task_success);
c_hook virtual_keyboard_task_get_instance_hook(0x004E1840, { .pointer = c_virtual_keyboard_task::get_instance });

c_virtual_keyboard_task* c_virtual_keyboard_task::constructor(
	const char* file,
	long line,
	long controller_index,
	const wchar_t* default_text,
	const wchar_t* title_text,
	const wchar_t* description_text,
	dword maximum_input_characters
)
{
	return DECLFUNC(0x004E16A0, c_virtual_keyboard_task*, __thiscall,
		c_virtual_keyboard_task const*,
		char const*,
		long,
		long,
		wchar_t const*,
		wchar_t const*,
		wchar_t const*,
		dword,
		long,
		bool
	)(
		this,
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
}

void c_overlapped_task::set_file(char const* file)
{
	m_file = file;
}

void c_overlapped_task::set_line(long line)
{
	m_line = line;
}

void c_virtual_keyboard_task::set_controller_index(long controller_index)
{
	m_controller_index = controller_index;
}

void c_virtual_keyboard_task::set_default_text(wchar_t const* default_text)
{
	if (default_text)
		wcscpy_s(m_default_text, default_text);
	else
		m_default_text[0] = 0;
}

void c_virtual_keyboard_task::set_title_text(wchar_t const* title_text)
{
	if (title_text)
		wcscpy_s(m_title_text, title_text);
	else
		m_title_text[0] = 0;
}

void c_virtual_keyboard_task::set_description_text(wchar_t const* description_text)
{
	if (description_text)
		wcscpy_s(m_description_text, description_text);
	else
		m_description_text[0] = 0;
}

void c_virtual_keyboard_task::set_maximum_input_characters(dword maximum_input_characters)
{
	m_maximum_input_characters = maximum_input_characters;
}

void c_virtual_keyboard_task::set_character_flags(dword_flags character_flags)
{
	m_character_flags = character_flags;
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
	if (!m_instance)
	{
		m_instance = (c_virtual_keyboard_task*)overlapped_malloc_tracked(sizeof(c_virtual_keyboard_task), __FILE__, __LINE__);
		m_instance = m_instance->constructor(
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
			m_instance->set_controller_index(controller_index);
			m_instance->set_default_text(default_text);
			m_instance->set_title_text(title_text);
			m_instance->set_description_text(description_text);
			m_instance->set_maximum_input_characters(maximum_input_characters);
			m_instance->set_character_flags(character_flags);
		}
		else
			c_console::write_line("ui: someone tried to get a duplicate instance of the virtual keyboard!");
	}

	return m_instance;
}

c_virtual_keyboard_task* c_virtual_keyboard_task::m_instance = nullptr;

void __fastcall virtual_keyboard_task_success(c_virtual_keyboard_task* task, dword a1)
{
	virtual_keyboard_task_success_hook.apply(true);
	reinterpret_cast<void(__thiscall*)(c_virtual_keyboard_task*, dword)>(virtual_keyboard_task_success_hook.get_original())(task, a1);
	virtual_keyboard_task_success_hook.apply(false);

	if (!wcsncmp(task->m_keyboard_results, L".fortune", 9))
		wcsncpy_s(task->m_keyboard_results, L"My modem is on file", 256);
	wchar_string_sanitize_for_game(task->m_keyboard_results, 256);
}
