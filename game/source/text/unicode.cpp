#include "text/unicode.hpp"

#include "cseries/cseries.hpp"
#include "cseries/cseries_events.hpp"

#include <stdio.h>
#include <string.h>

//bool unicode_warn_on_truncation = false;
//
//void check_source_string_against_copy_buffer_size(const wchar_t* string, int32 copy_buffer_size)
//{
//	if (unicode_warn_on_truncation)
//	{
//		ASSERT(string != nullptr);
//
//		int32 source_string_size = 0;
//		while (string[source_string_size++]);
//		if (copy_buffer_size < source_string_size)
//			event(_event_warning, "insufficient copy buffer for WCHAR string (%ld<=%ld) '%S'",
//				copy_buffer_size,
//				source_string_size,
//				string);
//	}
//}

//int ustrcmp(const wchar_t*, const wchar_t*)
//unsigned int ustrlen(const wchar_t*)

unsigned int ustrnlen(const wchar_t* string, int32 count)
{
	ASSERT(string != nullptr);

	return wcsnlen(string, count);
}

//const wchar_t* ustrchr(const wchar_t*,wchar_t)
//int ustrcoll(const wchar_t*,const wchar_t*)
//unsigned int ustrcspn(const wchar_t*,const wchar_t*)

wchar_t* ustrnzcat(wchar_t* dest, const wchar_t* src, int32 count)
{
	wcsncat_s(dest, count - 1, src, count);
	return dest;
}

int ustrncmp(const wchar_t* string1, const wchar_t* string2, int32 max_count)
{
	ASSERT(string1 != nullptr);
	ASSERT(string2 != nullptr);

	return wcsncmp(string1, string2, max_count);
}

wchar_t* ustrncpy(wchar_t* dest, const wchar_t* src, int32 count)
{
	//ASSERT(dest != nullptr);
	//ASSERT(src != nullptr);
	//
	//check_source_string_against_copy_buffer_size(src, count + 1);

	return INVOKE(0x004ECB90, ustrncpy, dest, src, count);
}

wchar_t* ustrnzcpy(wchar_t* dest, const wchar_t* src, int32 count)
{
	ASSERT(dest != nullptr);
	ASSERT(src != nullptr);
	ASSERT(count > 0);

	//check_source_string_against_copy_buffer_size(src, count);

	return ustrncpy(dest, src, count);
}

//const wchar_t* ustrpbrk(const wchar_t*,const wchar_t*)
//const wchar_t* ustrrchr(const wchar_t*,wchar_t)
//unsigned int ustrspn(const wchar_t*,const wchar_t*)
//const wchar_t* ustrstr(const wchar_t*,const wchar_t*)
//wchar_t* ustrtok(wchar_t*,const wchar_t*)
//unsigned int ustrxfrm(wchar_t*,const wchar_t*,int32)
//wchar_t* ustrnlwr(wchar_t*,int32)

wchar_t* ustrnupr(wchar_t* string, int32 count)
{
	return INVOKE(0x004ECC30, ustrnupr, string, count);
}

int __cdecl ustricmp(const wchar_t* string1, const wchar_t* string2)
{
	return INVOKE(0x00401370, ustricmp, string1, string2);
}

void __cdecl ascii_string_to_wchar_string(const char* src, wchar_t* dest, int32 src_len, int32* out_dest_len)
{
	return INVOKE(0x004EC600, ascii_string_to_wchar_string, src, dest, src_len, out_dest_len);
}

//wchar_t* __cdecl unicode_byte_swap_wchar_string(wchar_t* string, int32 maximum_count, e_byte_order byte_order)
wchar_t* __cdecl unicode_byte_swap_wchar_string(wchar_t* string, int32 maximum_count, int32 byte_order)
{
	return INVOKE(0x004EC970, unicode_byte_swap_wchar_string, string, maximum_count, byte_order);
}

int __cdecl ustrnicmp(const wchar_t* string1, const wchar_t* string2, int32 max_count)
{
	return INVOKE(0x004ECBC0, ustrnicmp, string1, string2, max_count);
}

//int uisalpha(wchar_t)
//int uisupper(wchar_t)
//int uislower(wchar_t)
//int uisdigit(wchar_t)
//int uisxdigit(wchar_t)
//int uisspace(wchar_t)
//int uispunct(wchar_t)
//int uisalnum(wchar_t)
//int uisprint(wchar_t)
//int uisgraph(wchar_t)
//int uiscntrl(wchar_t)
//wchar_t utoupper(wchar_t)
//int utolower(wchar_t)
//wchar_t ufgetc(_iobuf*)
//wchar_t ufputc(wchar_t,_iobuf*)
//wchar_t uungetc(wchar_t,_iobuf*)
//wchar_t* ufgets(wchar_t*,int,_iobuf*)
//int ufputs(const wchar_t*,_iobuf*)
//wchar_t* ugets(wchar_t*)
//int uputs(const wchar_t*)
//int ufprintf(_iobuf*,const wchar_t*,...)
//int uprintf(const wchar_t*,...)
//int usnprintf(wchar_t*,int32,const wchar_t*,...)

int usnzprintf(wchar_t* string, int32 size, const wchar_t* format, ...)
{
	va_list list;
	va_start(list, format);

	int result = uvsnzprintf(string, size, format, list);

	va_end(list);
	return result;
}

//int uvfprintf(_iobuf*,const wchar_t*,char*)
//int uvprintf(const wchar_t*,char*)

int uvsnzprintf(wchar_t* string, int32 size, const wchar_t* format, va_list list)
{
	ASSERT(string && format);
	ASSERT(size > 0);

	int result = _vsnwprintf_s(string, size, size - 1, format, list);
	string[size - 1] = 0;

	return result;
}

const wchar_t* uvsnzappend(wchar_t* string, int32 size, const wchar_t* format, va_list list)
{
	int32 current_length = ustrnlen(string, size);

	//ASSERT(format);
	//ASSERT(current_length >= 0 && current_length < k_buffer_size);

	uvsnzprintf(string + current_length, size - current_length, format, list);

	return string;
}

const wchar_t* usnzappend(wchar_t* string, int32 size, const wchar_t* format, ...)
{
	va_list list;
	va_start(list, format);

	uvsnzappend(string, size, format, list);

	va_end(list);
	return string;
}

//_iobuf* ufdopen(int,const wchar_t*)
//_iobuf* ufopen(const wchar_t*,const wchar_t*)
//int ufclose(_iobuf*)
//_iobuf* ufreopen(const wchar_t*,const wchar_t*,_iobuf*)
//void uperror(const wchar_t*)
//int uremove(const wchar_t*)
//wchar_t* utmpnam(wchar_t*)
//int32 ustrtol(const wchar_t*,wchar_t**,int)
//uns32 ustrtoul(const wchar_t*,wchar_t**,int)
//real64 ustrtod(const wchar_t*,wchar_t**)
//wchar_t* ustrftime_tm(wchar_t*,int32,const wchar_t*,const tm*)
//wchar_t* ustrftime(wchar_t*,int32,const wchar_t*,int64)
//int uatoi(const wchar_t*)

void wchar_string_to_ascii_string(const wchar_t* source, char* destination, int32 source_length, int32* destination_length)
{
	INVOKE(0x004EDD60, wchar_string_to_ascii_string, source, destination, source_length, destination_length);
}

//utf32 ascii_string_to_utf32_characters(const char*,const s_escape_table*,const char**,int32*,utf32*,int32,int32*)
//utf32 wchar_string_to_utf32_characters(const wchar_t*,const s_escape_table*,const wchar_t**,int32*,utf32*,int32,int32*)
//void ascii_string_to_utf32_string(const char*,const s_escape_table*,utf32*,int32,int32*)
//void wchar_string_to_utf32_string(const wchar_t*,const s_escape_table*,utf32*,int32,int32*)
//void ascii_string_to_wchar_string(const char*,wchar_t*,int32,int32*)
//int32 utf32_character_to_utf16_string(utf32,utf16*,int32)
//utf32 utf16_string_to_utf32_character(const utf16*,const utf16**)
//bool utf32_in_list(utf32,const s_utf32_range*,int32)
//bool utf32_isspace(utf32)
//bool utf32_is_id_start(utf32)
//bool utf32_is_id_continue(utf32)
//bool utf32_isalpha(utf32)
//bool utf32_isprint(utf32)
//bool utf32_ismonochrome(utf32)
//bool utf32_can_line_break(utf32,utf32)
//unsigned int utf32_strlen(const utf32*)
//bool is_private_use_character(wchar_t)
//void utf8_string_to_wchar_string(const utf8*,wchar_t*,int32,int32*)
//void wchar_string_to_utf8_string(const wchar_t*,utf8*,int32,int32*)
//void string_to_utf32_string<char,8>(const char*,const s_escape_table*,utf32*,utf32 (*)(const char*,const s_escape_table*,const char**,int32*,utf32*,int32,int32*),int32,int32*)
//void string_to_utf32_string<wchar_t,8>(const wchar_t*,const s_escape_table*,utf32*,utf32 (*)(const wchar_t*,const s_escape_table*,const wchar_t**,int32*,utf32*,int32,int32*),int32,int32*)