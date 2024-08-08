#define _CRT_SECURE_NO_WARNINGS // strncpy

#include "config/version.hpp"
#include "cseries/cseries.hpp"
#include "cseries/cseries_events.hpp"
#include "editor/editor_stubs.hpp"
#include "interface/damaged_media.hpp"
#include "main/main.hpp"
#include "memory/byte_swapping.hpp"
#include "multithreading/synchronized_value.hpp"
#include "multithreading/threads.hpp"
#include "shell/shell.hpp"
#include "tag_files/string_ids.hpp"

#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <windows.h>

REFERENCE_DECLARE(0x0189CD4C, bool, g_catch_exceptions);

REFERENCE_DECLARE(0x0189CD54, real_argb_color const* const, global_real_argb_white);
REFERENCE_DECLARE(0x0189CD58, real_argb_color const* const, global_real_argb_grey);
REFERENCE_DECLARE(0x0189CD5C, real_argb_color const* const, global_real_argb_black);
REFERENCE_DECLARE(0x0189CD60, real_argb_color const* const, global_real_argb_red);
REFERENCE_DECLARE(0x0189CD64, real_argb_color const* const, global_real_argb_green);
REFERENCE_DECLARE(0x0189CD68, real_argb_color const* const, global_real_argb_blue);
REFERENCE_DECLARE(0x0189CD6C, real_argb_color const* const, global_real_argb_yellow);
REFERENCE_DECLARE(0x0189CD70, real_argb_color const* const, global_real_argb_cyan);
REFERENCE_DECLARE(0x0189CD74, real_argb_color const* const, global_real_argb_magenta);
REFERENCE_DECLARE(0x0189CD78, real_argb_color const* const, global_real_argb_pink);
REFERENCE_DECLARE(0x0189CD7C, real_argb_color const* const, global_real_argb_lightblue);
REFERENCE_DECLARE(0x0189CD80, real_argb_color const* const, global_real_argb_orange);
REFERENCE_DECLARE(0x0189CD84, real_argb_color const* const, global_real_argb_purple);
REFERENCE_DECLARE(0x0189CD88, real_argb_color const* const, global_real_argb_aqua);
REFERENCE_DECLARE(0x0189CD8C, real_argb_color const* const, global_real_argb_darkgreen);
REFERENCE_DECLARE(0x0189CD90, real_argb_color const* const, global_real_argb_salmon);
REFERENCE_DECLARE(0x0189CD94, real_argb_color const* const, global_real_argb_violet);
REFERENCE_DECLARE(0x0189CD98, real_rgb_color const* const, global_real_rgb_white);
REFERENCE_DECLARE(0x0189CD9C, real_rgb_color const* const, global_real_rgb_grey);
REFERENCE_DECLARE(0x0189CDA0, real_rgb_color const* const, global_real_rgb_black);
REFERENCE_DECLARE(0x0189CDA4, real_rgb_color const* const, global_real_rgb_red);
REFERENCE_DECLARE(0x0189CDA8, real_rgb_color const* const, global_real_rgb_green);
REFERENCE_DECLARE(0x0189CDAC, real_rgb_color const* const, global_real_rgb_blue);
REFERENCE_DECLARE(0x0189CDB0, real_rgb_color const* const, global_real_rgb_yellow);
REFERENCE_DECLARE(0x0189CDB4, real_rgb_color const* const, global_real_rgb_cyan);
REFERENCE_DECLARE(0x0189CDB8, real_rgb_color const* const, global_real_rgb_magenta);
REFERENCE_DECLARE(0x0189CDBC, real_rgb_color const* const, global_real_rgb_pink);
REFERENCE_DECLARE(0x0189CDC0, real_rgb_color const* const, global_real_rgb_lightblue);
REFERENCE_DECLARE(0x0189CDC4, real_rgb_color const* const, global_real_rgb_orange);
REFERENCE_DECLARE(0x0189CDC8, real_rgb_color const* const, global_real_rgb_purple);
REFERENCE_DECLARE(0x0189CDCC, real_rgb_color const* const, global_real_rgb_aqua);
REFERENCE_DECLARE(0x0189CDD0, real_rgb_color const* const, global_real_rgb_darkgreen);
REFERENCE_DECLARE(0x0189CDD4, real_rgb_color const* const, global_real_rgb_salmon);
REFERENCE_DECLARE(0x0189CDD8, real_rgb_color const* const, global_real_rgb_violet);

REFERENCE_DECLARE(0x0189CDDC, c_system_allocation*, g_system_allocation);
REFERENCE_DECLARE(0x0189CDE0, c_normal_allocation*, g_normal_allocation);
REFERENCE_DECLARE(0x0189CDE4, c_no_allocation*, g_no_allocation);

//bool set_catch_exceptions = []() -> bool
//{
//	g_catch_exceptions = false;
//
//	return true;
//}();

static c_interlocked_long g_entry_gate;

void display_assert(char const* statement, char const* file, long line, bool assertion_failed)
{
	for (long i = g_entry_gate.set_if_equal(0, 1); i == 1; g_entry_gate.set_if_equal(0, 1))
		switch_to_thread();

	c_static_string<1156> crash_info;

	if (assertion_failed && !is_debugger_present())
	{
		generate_event(_event_level_critical, "");
		stack_walk(1);
		editor_save_progress();
	}

	generate_event(_event_level_critical, "");

	if (is_debugger_present())
	{
		generate_event(_event_level_critical, "%s(%d): %s: %s",
			file,
			line,
			assertion_failed ? "ASSERT" : "WARNING",
			statement ? statement : "");
		crash_info.print("halt:\r\n%s(%d): %s: %s\r\n",
			file,
			line,
			assertion_failed ? "ASSERT" : "WARNING",
			statement ? statement : "");
	}
	else
	{
		generate_event(_event_level_critical, "%s",
			version_get_full_string());
		crash_info.print("version:\r\n%s\r\n",
			version_get_full_string());

		generate_event(_event_level_critical, "%s at %s,#%d",
			assertion_failed ? "### ASSERTION FAILED: " : "### RUNTIME WARNING: ",
			file,
			line);
		crash_info.append_print("halt:\r\n%s at %s,#%d\r\n",
			assertion_failed ? "### ASSERTION FAILED: " : "### RUNTIME WARNING: ",
			file,
			line);

		if (statement)
		{
			generate_event(_event_level_critical, "  %s", statement);
			crash_info.append_print("halt information:\r\n  %s\r\n", statement);
		}
	}

	main_write_stack_to_crash_info_status_file(crash_info.get_string(), nullptr);

	if (assertion_failed)
	{
		//call_fatal_error_callbacks();

		if (k_tracked_build)
			RaiseException('stk', 0, 0, NULL);

		main_halt_and_catch_fire();
	}

	g_entry_gate.set(0);
}

bool handle_assert_as_exception(char const* statement, char const* file, long line, bool assertion_failed)
{
	if (is_debugger_present() && !g_catch_exceptions || !assertion_failed || is_main_thread())
		return false;

	s_thread_assert_arguments arguments
	{
		.statement = statement,
		.file = file,
		.line = line,
		.assertion_failed = assertion_failed,
	};

	post_thread_assert_arguments(&arguments);
	RaiseException('stk', 0, 0, NULL);

	return true;
}

int(__cdecl* csmemcmp)(void const* _Buf1, void const* _Buf2, size_t _Size) = memcmp;
void* (__cdecl* csmemcpy)(void* _Dst, void const* _Src, size_t _Size) = memcpy;
void* (__cdecl* csmemset)(void* _Dst, int _Val, size_t _Size) = memset;

#define MAXIMUM_STRING_SIZE 0x100000

long csstricmp(char const* s1, char const* s2)
{
	return _stricmp(s1, s2);
}

long csstrcmp(char const* s1, char const* s2)
{
	return strcmp(s1, s2);
}

long csstrnicmp(char const* s1, char const* s2, dword max_count)
{
	return _strnicmp(s1, s2, max_count);
}

char* __cdecl csstristr(char const* s1, char const* s2)
{
	return INVOKE(0x00401520, csstristr, s1, s2);
}

char* csstrnzcpy(char* s1, char const* s2, dword size)
{
	ASSERT(s1 && s2);
	ASSERT(size > 0 && size <= MAXIMUM_STRING_SIZE);

	char* result = strncpy(s1, s2, size);
	s1[size - 1] = 0;

	return result;
}

char* csstrnzcat(char* s1, char const* s2, dword size)
{
	ASSERT(s1 && s2);
	ASSERT(size > 0 && size <= MAXIMUM_STRING_SIZE);

	dword len = csstrnlen(s1, size);
	return csstrnzcpy(s1 + len, s2, size - len);
}

dword csstrnlen(char const* s, dword size)
{
	ASSERT(s);
	ASSERT(size > 0 && size <= MAXIMUM_STRING_SIZE);

	return strnlen(s, size);
}

char* csstrnupr(char* s, dword size)
{
	ASSERT(s);
	ASSERT(size >= 0 && size <= MAXIMUM_STRING_SIZE);

	for (dword i = 0; i < size; i++)
		s[i] = toupper(s[i]);

	return s;
}

char* csstrnlwr(char* s, dword size)
{
	ASSERT(s);
	ASSERT(size >= 0 && size <= MAXIMUM_STRING_SIZE);

	for (dword i = 0; i < size; i++)
		s[i] = tolower(s[i]);

	return s;
}

char const* csstrstr(char const* look_inside, char const* look_for)
{
	return strstr(look_inside, look_for);
}

//char* __cdecl csstrtok(char*, char const*, e_csstrtok_delimiter_mode, csstrtok_data*)
char* __cdecl csstrtok(char* s, char const* delimiters, long delimiter_mode, char** data)
{
	return INVOKE(0x00401A20, csstrtok, s, delimiters, delimiter_mode, data);
}

long cvsnzprintf(char* buffer, dword size, char const* format, va_list list)
{
	ASSERT(buffer);
	ASSERT(format);
	ASSERT(size > 0);

	long result = vsnprintf(buffer, size - 1, format, list);
	buffer[size - 1] = 0;

	size_t buf_size = strlen(buffer);
	csmemset(buffer + buf_size, 0, size - buf_size);

	return result;
}

char* csnzprintf(char* buffer, dword size, char const* format, ...)
{
	va_list list;
	va_start(list, format);

	cvsnzprintf(buffer, size, format, list);

	va_end(list);

	return buffer;
}

char* csnzappendf(char* buffer, dword size, char const* format, ...)
{
	dword current_length = strlen(buffer);
	ASSERT(current_length >= 0 && current_length < size);

	va_list list;
	va_start(list, format);

	cvsnzprintf(&buffer[current_length], size - current_length, format, list);

	va_end(list);

	return buffer;
}

//.text:00670E70 ; char const* __cdecl find_string_end(char const*, char const*)
//.text:00670E90 ; char const* __cdecl find_string_end_not_in_delimiter(char const *, char const *)
//.text:00670EB0 ; 
//.text:00670EC0 ; 
//.text:00670ED0 ; 
//.text:00670EE0 ; 
//.text:00670EF0 ; 
//.text:00670F20 ; void __cdecl string_copy_until_delimiter(c_wrapped_array<char>, char const*, char const*)

bool string_is_not_empty(char const* s)
{
	return s && *s;
}

//.text:00670F70 ; long __cdecl string_list_find(char const*, long, char const* const* const)
//.text:00670FB0 ; long __cdecl string_list_find_explicit(c_wrapped_array<char const> const&, c_wrapped_array<char const*> const&)
//.text:00671010 ; void __cdecl string_replace_character(char *, char, char)

void string_terminate_at_first_delimiter(char* s, char const* delimiter)
{
	s[strcspn(s, delimiter)] = 0;
}

long ascii_tolower(long C)
{
	return ascii_isupper((char)C) ? C + 32 : C;
}

bool ascii_isupper(char C)
{
	return C >= 'A' && C <= 'Z';
}

void ascii_strnlwr(char* string, long count)
{
	ASSERT(string != NULL || count == 0);
	ASSERT(count >= 0 && count < MAXIMUM_STRING_SIZE);

	for (long i = 0; i < count && string[i]; i++)
	{
		if (ascii_isupper(string[i]))
			string[i] += ' ';
	}
}

int __fastcall ascii_stristr(char const* look_inside, char const* look_for)
{
	return INVOKE(0x00401160, ascii_stristr, look_inside, look_for);
}

long __cdecl ascii_strnicmp(char const* s1, char const* s2, unsigned int size)
{
	return INVOKE(0x004011E0, ascii_strnicmp, s1, s2, size);
}

long __cdecl ascii_stricmp(char const* s1, char const* s2)
{
	return INVOKE(0x00401270, ascii_stricmp, s1, s2) > 0;
}

char* tag_to_string(tag _tag, char* buffer)
{
	*(tag*)buffer = bswap_dword(_tag);
	buffer[4] = 0;

	return buffer;
}

char const* c_string_id::get_string()
{
	return string_id_get_string_const(m_value);
}

char const* c_string_id::get_string() const
{
	return string_id_get_string_const(m_value);
}

bool c_old_string_id::is_string(char const* string) const
{
	return m_value != NONE && m_value == string_id_retrieve(string);
}

__int64 make_int64(__int64 a, __int64 b)
{
	return ((a << 0) | (b << 32));
}

void* offset_pointer(void* pointer, long offset)
{
	return pointer_from_address(address_from_pointer(pointer) + offset);
}

void const* offset_pointer(void const* pointer, long offset)
{
	return pointer_from_address(address_from_pointer(pointer) + offset);
}

unsigned int address_from_pointer(void const* pointer)
{
	return reinterpret_cast<unsigned int>(pointer);
}

void* pointer_from_address(unsigned int address)
{
	return reinterpret_cast<void*>(address);
}

unsigned int align_address(unsigned int address, long alignment_bits)
{
	return (address + (1 << alignment_bits) - 1) & ~((1 << alignment_bits) - 1);
}

void* align_pointer(void* pointer, long alignment_bits)
{
	return pointer_from_address(align_address(address_from_pointer(pointer), alignment_bits));
}

long pointer_distance(void const* pointer_a, void const* pointer_b)
{
	return static_cast<long>((char*)pointer_b - (char*)pointer_a);
}

long pointer_difference(void const* pointer_a, void const* pointer_b)
{
	return static_cast<long>((char*)pointer_b - (char*)pointer_a);
}

void __cdecl cseries_dispose()
{
	INVOKE(0x004EBC40, cseries_dispose);

	//destroy_thread_management();
	//destroy_synchronization_objects();
}

void __cdecl cseries_initialize()
{
	INVOKE(0x004EBC50, cseries_initialize);

	//initialize_performance_counters();
	//initialize_thread_management();
	//initialize_synchronization_objects();
	//debug_memory_manager_initialize();
	//std::set_new_handler((void (__cdecl *)())exit_with_code_one);
}

c_string_builder::c_string_builder() :
	c_static_string<1024>()
{
}

c_string_builder::c_string_builder(char const* format, ...) :
	c_static_string<1024>()
{
	va_list list;
	va_start(list, format);

	print_va(format, list);

	va_end(list);
}

c_string_builder::~c_string_builder()
{

}

