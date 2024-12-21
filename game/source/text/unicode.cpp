#include "text/unicode.hpp"

#include "cseries/cseries.hpp"
#include "cseries/cseries_events.hpp"

#include <stdio.h>
#include <string.h>

//bool unicode_warn_on_truncation = false;
//
//void check_source_string_against_copy_buffer_size(wchar_t const* string, long copy_buffer_size)
//{
//	if (unicode_warn_on_truncation)
//	{
//		ASSERT(string != NULL);
//
//		long source_string_size = 0;
//		while (string[source_string_size++]);
//		if (copy_buffer_size < source_string_size)
//			GENERATE_EVENT(_event_warning, "insufficient copy buffer for WCHAR string (%ld<=%ld) '%S'",
//				copy_buffer_size,
//				source_string_size,
//				string);
//	}
//}

//int ustrcmp(wchar_t const*, wchar_t const*)
//unsigned int ustrlen(wchar_t const *)

unsigned int ustrnlen(wchar_t const* string, long count)
{
	ASSERT(string != NULL);

	return wcsnlen(string, count);
}

//wchar_t const * ustrchr(wchar_t const *,wchar_t)
//int ustrcoll(wchar_t const *,wchar_t const *)
//unsigned int ustrcspn(wchar_t const *,wchar_t const *)

wchar_t* ustrnzcat(wchar_t* dest, wchar_t const* src, long count)
{
	wcsncat_s(dest, count - 1, src, count);
	return dest;
}

int ustrncmp(wchar_t const* string1, wchar_t const* string2, long max_count)
{
	ASSERT(string1 != NULL);
	ASSERT(string2 != NULL);

	return wcsncmp(string1, string2, max_count);
}

wchar_t* ustrncpy(wchar_t* dest, wchar_t const* src, long count)
{
	//ASSERT(dest != NULL);
	//ASSERT(src != NULL);
	//
	//check_source_string_against_copy_buffer_size(src, count + 1);

	return INVOKE(0x004ECB90, ustrncpy, dest, src, count);
}

wchar_t* ustrnzcpy(wchar_t* dest, wchar_t const* src, long count)
{
	ASSERT(dest != NULL);
	ASSERT(src != NULL);
	ASSERT(count > 0);

	//check_source_string_against_copy_buffer_size(src, count);

	return ustrncpy(dest, src, count);
}

//wchar_t const * ustrpbrk(wchar_t const *,wchar_t const *)
//wchar_t const * ustrrchr(wchar_t const *,wchar_t)
//unsigned int ustrspn(wchar_t const *,wchar_t const *)
//wchar_t const * ustrstr(wchar_t const *,wchar_t const *)
//wchar_t * ustrtok(wchar_t *,wchar_t const *)
//unsigned int ustrxfrm(wchar_t *,wchar_t const *,long)
//wchar_t * ustrnlwr(wchar_t *,long)
//wchar_t * ustrnupr(wchar_t *,long)

int __cdecl ustricmp(wchar_t const* string1, wchar_t const* string2)
{
	return INVOKE(0x00401370, ustricmp, string1, string2);
}

void __cdecl ascii_string_to_wchar_string(char const* src, wchar_t* dest, long src_len, long* out_dest_len)
{
	return INVOKE(0x004EC600, ascii_string_to_wchar_string, src, dest, src_len, out_dest_len);
}

//wchar_t* __cdecl unicode_byte_swap_wchar_string(wchar_t* string, long maximum_count, e_byte_order byte_order)
wchar_t* __cdecl unicode_byte_swap_wchar_string(wchar_t* string, long maximum_count, long byte_order)
{
	return INVOKE(0x004EC970, unicode_byte_swap_wchar_string, string, maximum_count, byte_order);
}

int __cdecl ustrnicmp(wchar_t const* string1, wchar_t const* string2, long max_count)
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
//wchar_t ufgetc(struct _iobuf *)
//wchar_t ufputc(wchar_t,struct _iobuf *)
//wchar_t uungetc(wchar_t,struct _iobuf *)
//wchar_t * ufgets(wchar_t *,int,struct _iobuf *)
//int ufputs(wchar_t const *,struct _iobuf *)
//wchar_t * ugets(wchar_t *)
//int uputs(wchar_t const *)
//int ufprintf(struct _iobuf *,wchar_t const *,...)
//int uprintf(wchar_t const *,...)
//int usnprintf(wchar_t *,long,wchar_t const *,...)

int usnzprintf(wchar_t* string, long size, wchar_t const* format, ...)
{
	va_list list;
	va_start(list, format);

	int result = uvsnzprintf(string, size, format, list);

	va_end(list);
	return result;
}

//int uvfprintf(struct _iobuf *,wchar_t const *,char *)
//int uvprintf(wchar_t const *,char *)

int uvsnzprintf(wchar_t* string, long size, wchar_t const* format, va_list list)
{
	ASSERT(string && format);
	ASSERT(size > 0);

	int result = _vsnwprintf_s(string, size, size - 1, format, list);
	string[size - 1] = 0;

	return result;
}

//struct _iobuf * ufdopen(int,wchar_t const *)
//struct _iobuf * ufopen(wchar_t const *,wchar_t const *)
//int ufclose(struct _iobuf *)
//struct _iobuf * ufreopen(wchar_t const *,wchar_t const *,struct _iobuf *)
//void uperror(wchar_t const *)
//int uremove(wchar_t const *)
//wchar_t * utmpnam(wchar_t *)
//long ustrtol(wchar_t const *,wchar_t * *,int)
//unsigned long ustrtoul(wchar_t const *,wchar_t * *,int)
//double ustrtod(wchar_t const *,wchar_t * *)
//wchar_t * ustrftime_tm(wchar_t *,long,wchar_t const *,struct tm const *)
//wchar_t * ustrftime(wchar_t *,long,wchar_t const *,__int64)
//int uatoi(wchar_t const *)

void wchar_string_to_ascii_string(wchar_t const* source, char* destination, long source_length, long* destination_length)
{
	INVOKE(0x004EDD60, wchar_string_to_ascii_string, source, destination, source_length, destination_length);
}

//struct utf32 ascii_string_to_utf32_characters(char const *,struct s_escape_table const *,char const * *,long *,struct utf32 *,long,long *)
//struct utf32 wchar_string_to_utf32_characters(wchar_t const *,struct s_escape_table const *,wchar_t const * *,long *,struct utf32 *,long,long *)
//void ascii_string_to_utf32_string(char const *,struct s_escape_table const *,struct utf32 *,long,long *)
//void wchar_string_to_utf32_string(wchar_t const *,struct s_escape_table const *,struct utf32 *,long,long *)
//void ascii_string_to_wchar_string(char const *,wchar_t *,long,long *)
//long utf32_character_to_utf16_string(struct utf32,struct utf16 *,long)
//struct utf32 utf16_string_to_utf32_character(struct utf16 const *,struct utf16 const * *)
//bool utf32_in_list(struct utf32,struct s_utf32_range const *,long)
//bool utf32_isspace(struct utf32)
//bool utf32_is_id_start(struct utf32)
//bool utf32_is_id_continue(struct utf32)
//bool utf32_isalpha(struct utf32)
//bool utf32_isprint(struct utf32)
//bool utf32_ismonochrome(struct utf32)
//bool utf32_can_line_break(struct utf32,struct utf32)
//unsigned int utf32_strlen(struct utf32 const *)
//bool is_private_use_character(wchar_t)
//void utf8_string_to_wchar_string(struct utf8 const *,wchar_t *,long,long *)
//void wchar_string_to_utf8_string(wchar_t const *,struct utf8 *,long,long *)
//void string_to_utf32_string<char,8>(char const *,struct s_escape_table const *,struct utf32 *,struct utf32 (*)(char const *,struct s_escape_table const *,char const * *,long *,struct utf32 *,long,long *),long,long *)
//void string_to_utf32_string<wchar_t,8>(wchar_t const *,struct s_escape_table const *,struct utf32 *,struct utf32 (*)(wchar_t const *,struct s_escape_table const *,wchar_t const * *,long *,struct utf32 *,long,long *),long,long *)