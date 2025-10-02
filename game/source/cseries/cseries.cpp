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
#include "tag_files/string_ids.hpp"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <ctime>
#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>

REFERENCE_DECLARE(0x0189CD4C, bool, g_catch_exceptions) = true;

//REFERENCE_DECLARE(0x0189CD54, const real_argb_color* const, global_real_argb_white);
//REFERENCE_DECLARE(0x0189CD58, const real_argb_color* const, global_real_argb_grey);
//REFERENCE_DECLARE(0x0189CD5C, const real_argb_color* const, global_real_argb_black);
//REFERENCE_DECLARE(0x0189CD60, const real_argb_color* const, global_real_argb_red);
//REFERENCE_DECLARE(0x0189CD64, const real_argb_color* const, global_real_argb_green);
//REFERENCE_DECLARE(0x0189CD68, const real_argb_color* const, global_real_argb_blue);
//REFERENCE_DECLARE(0x0189CD6C, const real_argb_color* const, global_real_argb_yellow);
//REFERENCE_DECLARE(0x0189CD70, const real_argb_color* const, global_real_argb_cyan);
//REFERENCE_DECLARE(0x0189CD74, const real_argb_color* const, global_real_argb_magenta);
//REFERENCE_DECLARE(0x0189CD78, const real_argb_color* const, global_real_argb_pink);
//REFERENCE_DECLARE(0x0189CD7C, const real_argb_color* const, global_real_argb_lightblue);
//REFERENCE_DECLARE(0x0189CD80, const real_argb_color* const, global_real_argb_orange);
//REFERENCE_DECLARE(0x0189CD84, const real_argb_color* const, global_real_argb_purple);
//REFERENCE_DECLARE(0x0189CD88, const real_argb_color* const, global_real_argb_aqua);
//REFERENCE_DECLARE(0x0189CD8C, const real_argb_color* const, global_real_argb_darkgreen);
//REFERENCE_DECLARE(0x0189CD90, const real_argb_color* const, global_real_argb_salmon);
//REFERENCE_DECLARE(0x0189CD94, const real_argb_color* const, global_real_argb_violet);
//REFERENCE_DECLARE(0x0189CD98, const real_rgb_color* const, global_real_rgb_white);
//REFERENCE_DECLARE(0x0189CD9C, const real_rgb_color* const, global_real_rgb_grey);
//REFERENCE_DECLARE(0x0189CDA0, const real_rgb_color* const, global_real_rgb_black);
//REFERENCE_DECLARE(0x0189CDA4, const real_rgb_color* const, global_real_rgb_red);
//REFERENCE_DECLARE(0x0189CDA8, const real_rgb_color* const, global_real_rgb_green);
//REFERENCE_DECLARE(0x0189CDAC, const real_rgb_color* const, global_real_rgb_blue);
//REFERENCE_DECLARE(0x0189CDB0, const real_rgb_color* const, global_real_rgb_yellow);
//REFERENCE_DECLARE(0x0189CDB4, const real_rgb_color* const, global_real_rgb_cyan);
//REFERENCE_DECLARE(0x0189CDB8, const real_rgb_color* const, global_real_rgb_magenta);
//REFERENCE_DECLARE(0x0189CDBC, const real_rgb_color* const, global_real_rgb_pink);
//REFERENCE_DECLARE(0x0189CDC0, const real_rgb_color* const, global_real_rgb_lightblue);
//REFERENCE_DECLARE(0x0189CDC4, const real_rgb_color* const, global_real_rgb_orange);
//REFERENCE_DECLARE(0x0189CDC8, const real_rgb_color* const, global_real_rgb_purple);
//REFERENCE_DECLARE(0x0189CDCC, const real_rgb_color* const, global_real_rgb_aqua);
//REFERENCE_DECLARE(0x0189CDD0, const real_rgb_color* const, global_real_rgb_darkgreen);
//REFERENCE_DECLARE(0x0189CDD4, const real_rgb_color* const, global_real_rgb_salmon);
//REFERENCE_DECLARE(0x0189CDD8, const real_rgb_color* const, global_real_rgb_violet);

REFERENCE_DECLARE(0x0189CDDC, c_system_allocation*, g_system_allocation);// = new c_system_allocation();
REFERENCE_DECLARE(0x0189CDE0, c_normal_allocation*, g_normal_allocation);// = new c_normal_allocation();
REFERENCE_DECLARE(0x0189CDE4, c_no_allocation*, g_no_allocation);// = new c_no_allocation();

static c_interlocked_long g_entry_gate;

void display_assert(const char* statement, const char* file, int32 line, bool fatal)
{
	for (int32 i = g_entry_gate.set_if_equal(0, 1); i == 1; g_entry_gate.set_if_equal(0, 1))
		switch_to_thread();

	c_static_string<1156> crash_info;

	if (fatal && !is_debugger_present())
	{
		event(_event_critical, "");
		stack_walk(1);
		editor_save_progress();
	}

	event(_event_critical, "");

	if (is_debugger_present())
	{
		event(_event_critical, "%s(%d): %s: %s",
			file,
			line,
			fatal ? "ASSERT" : "WARNING",
			statement ? statement : "");
		crash_info.print("halt:\r\n%s(%d): %s: %s\r\n",
			file,
			line,
			fatal ? "ASSERT" : "WARNING",
			statement ? statement : "");
	}
	else
	{
		event(_event_critical, "%s",
			version_get_full_string());
		crash_info.print("version:\r\n%s\r\n",
			version_get_full_string());

		event(_event_critical, "%s at %s,#%d",
			fatal ? "### ASSERTION FAILED: " : "### RUNTIME WARNING: ",
			file,
			line);
		crash_info.append_print("halt:\r\n%s at %s,#%d\r\n",
			fatal ? "### ASSERTION FAILED: " : "### RUNTIME WARNING: ",
			file,
			line);

		if (statement)
		{
			event(_event_critical, "  %s", statement);
			crash_info.append_print("halt information:\r\n  %s\r\n", statement);
		}
	}

	main_write_stack_to_crash_info_status_file(crash_info.get_string(), nullptr);

	if (fatal)
	{
		//call_fatal_error_callbacks();

		if (k_tracked_build)
		{
			RaiseException('stk', 0, 0, NULL);
		}

		main_halt_and_catch_fire();
	}

	g_entry_gate.set(0);
}

bool handle_assert_as_exception(const char* statement, const char* file, int32 line, bool fatal)
{
	if (is_debugger_present() && !g_catch_exceptions || !fatal || is_main_thread())
		return false;

	s_thread_assert_arguments arguments
	{
		.statement = statement,
		.file = file,
		.line = line,
		.fatal = fatal,
	};

	post_thread_assert_arguments(&arguments);
	RaiseException('stk', 0, 0, NULL);

	return true;
}

int(__cdecl* csmemcmp)(const void* _Buf1, const void* _Buf2, size_t _Size) = memcmp;
void* (__cdecl* csmemcpy)(void* _Dst, const void* _Src, size_t _Size) = memcpy;
void* (__cdecl* csmemset)(void* _Dst, int _Val, size_t _Size) = memset;

#define MAXIMUM_STRING_SIZE 0x100000

size_t strlen_debug(const char* str)
{
	return strlen(str);
}

int strncmp_debug(const char* s1, const char* s2, size_t size)
{
	ASSERT(s1 && s2);
	ASSERT(size >= 0 && size <= MAXIMUM_STRING_SIZE);

	return strncmp(s1, s2, size);
}

int32 csstricmp(const char* s1, const char* s2)
{
	return _stricmp(s1, s2);
}

int32 csstrcmp(const char* s1, const char* s2)
{
	return strcmp(s1, s2);
}

int32 csstrnicmp(const char* s1, const char* s2, uns32 max_count)
{
	return _strnicmp(s1, s2, max_count);
}

char* __cdecl csstristr(const char* s1, const char* s2)
{
	return INVOKE(0x00401520, csstristr, s1, s2);
}

char* csstrnzcpy(char* s1, const char* s2, uns32 size)
{
	ASSERT(s1 && s2);
	ASSERT(size > 0 && size <= MAXIMUM_STRING_SIZE);

	char* result = strncpy(s1, s2, size);
	s1[size - 1] = 0;

	return result;
}

char* csstrnzcat(char* s1, const char* s2, uns32 size)
{
	ASSERT(s1 && s2);
	ASSERT(size > 0 && size <= MAXIMUM_STRING_SIZE);

	uns32 len = csstrnlen(s1, size);
	return csstrnzcpy(s1 + len, s2, size - len);
}

uns32 csstrnlen(const char* s, uns32 size)
{
	ASSERT(s);
	ASSERT(size > 0 && size <= MAXIMUM_STRING_SIZE);

	return strnlen(s, size);
}

char* csstrnupr(char* s, uns32 size)
{
	ASSERT(s);
	ASSERT(size >= 0 && size <= MAXIMUM_STRING_SIZE);

	for (uns32 i = 0; i < size; i++)
		s[i] = toupper(s[i]);

	return s;
}

char* csstrnlwr(char* s, uns32 size)
{
	ASSERT(s);
	ASSERT(size >= 0 && size <= MAXIMUM_STRING_SIZE);

	for (uns32 i = 0; i < size; i++)
		s[i] = tolower(s[i]);

	return s;
}

const char* csstrstr(const char* look_inside, const char* look_for)
{
	return strstr(look_inside, look_for);
}

//char* __cdecl csstrtok(char*, const char*, e_csstrtok_delimiter_mode, csstrtok_data*)
char* __cdecl csstrtok(char* s, const char* delimiters, int32 delimiter_mode, char** data)
{
	return INVOKE(0x00401A20, csstrtok, s, delimiters, delimiter_mode, data);
}

int32 cvsnzprintf(char* buffer, uns32 size, const char* format, va_list list)
{
	ASSERT(buffer);
	ASSERT(format);
	ASSERT(size > 0);

	int32 result = vsnprintf(buffer, size - 1, format, list);
	buffer[size - 1] = 0;

	size_t buf_size = strlen(buffer);
	csmemset(buffer + buf_size, 0, size - buf_size);

	return result;
}

char* csnzprintf(char* buffer, uns32 size, const char* format, ...)
{
	va_list list;
	va_start(list, format);

	cvsnzprintf(buffer, size, format, list);

	va_end(list);

	return buffer;
}

char* csnzappendf(char* buffer, uns32 size, const char* format, ...)
{
	uns32 current_length = strlen(buffer);
	ASSERT(current_length >= 0 && current_length < size);

	va_list list;
	va_start(list, format);

	cvsnzprintf(&buffer[current_length], size - current_length, format, list);

	va_end(list);

	return buffer;
}

const char* __cdecl find_string_end(const char* string, const char* delimiters)
{
	return INVOKE(0x00670E70, find_string_end, string, delimiters);
}

const char* __cdecl find_string_end_not_in_delimiter(const char* string, const char* delimiters)
{
	return INVOKE(0x00670E90, find_string_end_not_in_delimiter, string, delimiters);
}

//.text:00670EB0 ; 
//.text:00670EC0 ; 
//.text:00670ED0 ; 
//.text:00670EE0 ; 

void string_copy_bounded(c_wrapped_array<char> out_dest_string, c_wrapped_array<char const> const in_source_string)
{
	//INVOKE(0x00670EF0, string_copy_bounded, out_dest_string, in_source_string);

	ASSERT(out_dest_string.count() > 0);

	int32 copy_length = out_dest_string.m_count - 1;
	if (copy_length > in_source_string.m_count)
	{
		copy_length = in_source_string.m_count;
	}

	_memccpy(out_dest_string.m_elements, in_source_string.m_elements, 0, copy_length);

	out_dest_string.m_elements[copy_length] = 0;
}

//.text:00670F20 ; void __cdecl string_copy_until_delimiter(c_wrapped_array<char>, const char*, const char*)

bool string_is_not_empty(const char* s)
{
	return s && *s;
}

int16 __cdecl string_list_find(const char* string, int16 list_count, const char* const* string_list)
{
	return INVOKE(0x00670F70, string_list_find, string, list_count, string_list);

	//int16 list_index = 0;
	//if (list_count > 0)
	//{
	//	while (ascii_stricmp(string, string_list[list_index]))
	//	{
	//		if (++list_index >= list_count)
	//		{
	//			list_index = NONE;
	//			break;
	//		}
	//	}
	//}
	//else
	//{
	//	list_index = NONE;
	//}
	//return list_index;
}

//.text:00670FB0 ; int32 __cdecl string_list_find_explicit(const c_wrapped_array<char const>&, const c_wrapped_array<const char*>&)
//.text:00671010 ; void __cdecl string_replace_character(char *, char, char)

void string_terminate_at_first_delimiter(char* s, const char* delimiter)
{
	s[strcspn(s, delimiter)] = 0;
}

int32 ascii_tolower(int32 C)
{
	return ascii_isupper((char)C) ? C + 32 : C;
}

bool ascii_isupper(char C)
{
	return C >= 'A' && C <= 'Z';
}

void ascii_strnlwr(char* string, int32 count)
{
	ASSERT(string != NULL || count == 0);
	ASSERT(count >= 0 && count < MAXIMUM_STRING_SIZE);

	for (int32 i = 0; i < count && string[i]; i++)
	{
		if (ascii_isupper(string[i]))
			string[i] += ' ';
	}
}

int __fastcall ascii_stristr(const char* look_inside, const char* look_for)
{
	return INVOKE(0x00401160, ascii_stristr, look_inside, look_for);
}

int32 __cdecl ascii_strnicmp(const char* s1, const char* s2, unsigned int size)
{
	return INVOKE(0x004011E0, ascii_strnicmp, s1, s2, size);
}

int32 __cdecl ascii_stricmp(const char* s1, const char* s2)
{
	return INVOKE(0x00401270, ascii_stricmp, s1, s2);
}

char* tag_to_string(tag _tag, char* buffer)
{
	*(tag*)buffer = bswap_uns32(_tag);
	buffer[4] = 0;

	return buffer;
}

const char* c_string_id::get_string()
{
	return string_id_get_string_const(m_id);
}

const char* c_string_id::get_string() const
{
	return string_id_get_string_const(m_id);
}

bool c_old_string_id::is_string(const char* string) const
{
	return m_id != NONE && m_id == string_id_retrieve(string);
}

int64 make_int64(int32 low, int32 high)
{
	return low | ((int64)high << 32);
}

void* offset_pointer(void* pointer, int32 offset)
{
	return (byte*)pointer + offset;
}

const void* offset_pointer(const void* pointer, int32 offset)
{
	return (const byte*)pointer + offset;
}

unsigned int align_address(unsigned int address, int32 alignment_bits)
{
	return(address + (unsigned int)(1L << alignment_bits) - 1) & ~(unsigned int)((1L << alignment_bits) - 1);
}

void* align_pointer(void* pointer, int32 alignment_bits)
{
	return (void*)align_address((unsigned int)pointer, alignment_bits);
}

int32 pointer_distance(const void* pointer_a, const void* pointer_b)
{
	return static_cast<int32>((byte*)pointer_b - (byte*)pointer_a);
}

int32 pointer_difference(const void* pointer_a, const void* pointer_b)
{
	return static_cast<int32>((byte*)pointer_b - (byte*)pointer_a);
}

void __cdecl cseries_dispose()
{
	//INVOKE(0x004EBC40, cseries_dispose);

	events_dispose();
	destroy_thread_management();
	destroy_synchronization_objects();
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

uns64 get_current_time_in_seconds(uns64 seconds)
{
	return (uns64)_time64(0) - seconds;
}

c_string_builder::c_string_builder() :
	c_static_string<1024>()
{
}

c_string_builder::c_string_builder(const char* format, ...) :
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

real_argb_color const private_real_argb_colors[17]
{
	{ 1.0f,  1.0f,  1.0f,  1.0f },
	{ 1.0f,  0.5f,  0.5f,  0.5f },
	{ 1.0f,  0.0f,  0.0f,  0.0f },
	{ 1.0f,  1.0f,  0.0f,  0.0f },
	{ 1.0f,  0.0f,  1.0f,  0.0f },
	{ 1.0f,  0.0f,  0.0f,  1.0f },
	{ 1.0f,  0.0f,  1.0f,  1.0f },
	{ 1.0f,  1.0f,  1.0f,  0.0f },
	{ 1.0f,  1.0f,  0.0f,  1.0f },
	{ 1.0f,  1.0f, 0.41f,  0.7f },
	{ 1.0f, 0.39f, 0.58f, 0.93f },
	{ 1.0f,  1.0f,  0.5f,  0.0f },
	{ 1.0f, 0.44f, 0.05f, 0.43f },
	{ 1.0f,  0.5f,  1.0f, 0.83f },
	{ 1.0f,  0.0f, 0.39f,  0.0f },
	{ 1.0f,  1.0f, 0.63f, 0.48f },
	{ 1.0f, 0.81f, 0.13f, 0.56f }
};

const real_argb_color* const global_real_argb_color_table = private_real_argb_colors;

const real_argb_color* const global_real_argb_white = &private_real_argb_colors[0];
const real_argb_color* const global_real_argb_grey = &private_real_argb_colors[1];
const real_argb_color* const global_real_argb_black = &private_real_argb_colors[2];
const real_argb_color* const global_real_argb_red = &private_real_argb_colors[3];
const real_argb_color* const global_real_argb_green = &private_real_argb_colors[4];
const real_argb_color* const global_real_argb_blue = &private_real_argb_colors[5];
const real_argb_color* const global_real_argb_yellow = &private_real_argb_colors[7];
const real_argb_color* const global_real_argb_cyan = &private_real_argb_colors[6];
const real_argb_color* const global_real_argb_magenta = &private_real_argb_colors[8];
const real_argb_color* const global_real_argb_pink = &private_real_argb_colors[9];
const real_argb_color* const global_real_argb_lightblue = &private_real_argb_colors[10];
const real_argb_color* const global_real_argb_orange = &private_real_argb_colors[11];
const real_argb_color* const global_real_argb_purple = &private_real_argb_colors[12];
const real_argb_color* const global_real_argb_aqua = &private_real_argb_colors[13];
const real_argb_color* const global_real_argb_darkgreen = &private_real_argb_colors[14];
const real_argb_color* const global_real_argb_salmon = &private_real_argb_colors[15];
const real_argb_color* const global_real_argb_violet = &private_real_argb_colors[16];

const real_rgb_color* const global_real_rgb_white = &private_real_argb_colors[0].rgb;
const real_rgb_color* const global_real_rgb_grey = &private_real_argb_colors[1].rgb;
const real_rgb_color* const global_real_rgb_black = &private_real_argb_colors[2].rgb;
const real_rgb_color* const global_real_rgb_red = &private_real_argb_colors[3].rgb;
const real_rgb_color* const global_real_rgb_green = &private_real_argb_colors[4].rgb;
const real_rgb_color* const global_real_rgb_blue = &private_real_argb_colors[5].rgb;
const real_rgb_color* const global_real_rgb_yellow = &private_real_argb_colors[7].rgb;
const real_rgb_color* const global_real_rgb_cyan = &private_real_argb_colors[6].rgb;
const real_rgb_color* const global_real_rgb_magenta = &private_real_argb_colors[8].rgb;
const real_rgb_color* const global_real_rgb_pink = &private_real_argb_colors[9].rgb;
const real_rgb_color* const global_real_rgb_lightblue = &private_real_argb_colors[10].rgb;
const real_rgb_color* const global_real_rgb_orange = &private_real_argb_colors[11].rgb;
const real_rgb_color* const global_real_rgb_purple = &private_real_argb_colors[12].rgb;
const real_rgb_color* const global_real_rgb_aqua = &private_real_argb_colors[13].rgb;
const real_rgb_color* const global_real_rgb_darkgreen = &private_real_argb_colors[14].rgb;
const real_rgb_color* const global_real_rgb_salmon = &private_real_argb_colors[15].rgb;
const real_rgb_color* const global_real_rgb_violet = &private_real_argb_colors[16].rgb;

