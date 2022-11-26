#pragma once

#include <stdarg.h>


//extern int ustrcmp(wchar_t const*, wchar_t const*);
//extern unsigned int ustrlen(wchar_t const *);

extern unsigned int ustrnlen(wchar_t const*, long);

//extern wchar_t const * ustrchr(wchar_t const *,wchar_t);
//extern int ustrcoll(wchar_t const *,wchar_t const *);
//extern unsigned int ustrcspn(wchar_t const *,wchar_t const *);

extern wchar_t* ustrnzcat(wchar_t*, wchar_t const*, long);

//extern int ustrncmp(wchar_t const *,wchar_t const *,long);
//extern wchar_t * ustrncpy(wchar_t *,wchar_t const *,long);

extern wchar_t* ustrnzcpy(wchar_t*, wchar_t const*, long);

//extern wchar_t const * ustrpbrk(wchar_t const *,wchar_t const *);
//extern wchar_t const * ustrrchr(wchar_t const *,wchar_t);
//extern unsigned int ustrspn(wchar_t const *,wchar_t const *);
//extern wchar_t const * ustrstr(wchar_t const *,wchar_t const *);
//extern wchar_t * ustrtok(wchar_t *,wchar_t const *);
//extern unsigned int ustrxfrm(wchar_t *,wchar_t const *,long);
//extern wchar_t * ustrnlwr(wchar_t *,long);
//extern wchar_t * ustrnupr(wchar_t *,long);
extern int ustricmp(wchar_t const *,wchar_t const *);
//extern int ustrnicmp(wchar_t const *,wchar_t const *,long);
//extern int uisalpha(wchar_t);
//extern int uisupper(wchar_t);
//extern int uislower(wchar_t);
//extern int uisdigit(wchar_t);
//extern int uisxdigit(wchar_t);
//extern int uisspace(wchar_t);
//extern int uispunct(wchar_t);
//extern int uisalnum(wchar_t);
//extern int uisprint(wchar_t);
//extern int uisgraph(wchar_t);
//extern int uiscntrl(wchar_t);
//extern wchar_t utoupper(wchar_t);
//extern int utolower(wchar_t);
//extern wchar_t ufgetc(struct _iobuf *);
//extern wchar_t ufputc(wchar_t,struct _iobuf *);
//extern wchar_t uungetc(wchar_t,struct _iobuf *);
//extern wchar_t * ufgets(wchar_t *,int,struct _iobuf *);
//extern int ufputs(wchar_t const *,struct _iobuf *);
//extern wchar_t * ugets(wchar_t *);
//extern int uputs(wchar_t const *);
//extern int ufprintf(struct _iobuf *,wchar_t const *,...);
//extern int uprintf(wchar_t const *,...);
//extern int usnprintf(wchar_t *,long,wchar_t const *,...);

extern int usnzprintf(wchar_t *,long,wchar_t const *,...);

//extern int uvfprintf(struct _iobuf *,wchar_t const *,char *);
//extern int uvprintf(wchar_t const *,char *);

extern int uvsnzprintf(wchar_t* string, long size, wchar_t const* format, va_list list);

//extern struct _iobuf * ufdopen(int,wchar_t const *);
//extern struct _iobuf * ufopen(wchar_t const *,wchar_t const *);
//extern int ufclose(struct _iobuf *);
//extern struct _iobuf * ufreopen(wchar_t const *,wchar_t const *,struct _iobuf *);
//extern void uperror(wchar_t const *);
//extern int uremove(wchar_t const *);
//extern wchar_t * utmpnam(wchar_t *);
//extern long ustrtol(wchar_t const *,wchar_t * *,int);
//extern unsigned long ustrtoul(wchar_t const *,wchar_t * *,int);
//extern double ustrtod(wchar_t const *,wchar_t * *);
//extern wchar_t * ustrftime_tm(wchar_t *,long,wchar_t const *,struct tm const *);
//extern wchar_t * ustrftime(wchar_t *,long,wchar_t const *,__int64);
//extern int uatoi(wchar_t const *);

extern void wchar_string_to_ascii_string(wchar_t const* source, char* destination, long source_length, long* destination_length);

//extern struct utf32 ascii_string_to_utf32_characters(char const *,struct s_escape_table const *,char const * *,long *,struct utf32 *,long,long *);
//extern struct utf32 wchar_string_to_utf32_characters(wchar_t const *,struct s_escape_table const *,wchar_t const * *,long *,struct utf32 *,long,long *);
//extern void ascii_string_to_utf32_string(char const *,struct s_escape_table const *,struct utf32 *,long,long *);
//extern void wchar_string_to_utf32_string(wchar_t const *,struct s_escape_table const *,struct utf32 *,long,long *);

extern void ascii_string_to_wchar_string(char const* src, wchar_t* dest, long src_len, long* out_dest_len);

//extern long utf32_character_to_utf16_string(struct utf32,struct utf16 *,long);
//extern struct utf32 utf16_string_to_utf32_character(struct utf16 const *,struct utf16 const * *);
//extern bool utf32_in_list(struct utf32,struct s_utf32_range const *,long);
//extern bool utf32_isspace(struct utf32);
//extern bool utf32_is_id_start(struct utf32);
//extern bool utf32_is_id_continue(struct utf32);
//extern bool utf32_isalpha(struct utf32);
//extern bool utf32_isprint(struct utf32);
//extern bool utf32_ismonochrome(struct utf32);
//extern bool utf32_can_line_break(struct utf32,struct utf32);
//extern unsigned int utf32_strlen(struct utf32 const *);
//extern bool is_private_use_character(wchar_t);
//extern void utf8_string_to_wchar_string(struct utf8 const *,wchar_t *,long,long *);
//extern void wchar_string_to_utf8_string(wchar_t const *,struct utf8 *,long,long *);
//extern void string_to_utf32_string<char,8>(char const *,struct s_escape_table const *,struct utf32 *,struct utf32 (*)(char const *,struct s_escape_table const *,char const * *,long *,struct utf32 *,long,long *),long,long *);
//extern void string_to_utf32_string<wchar_t,8>(wchar_t const *,struct s_escape_table const *,struct utf32 *,struct utf32 (*)(wchar_t const *,struct s_escape_table const *,wchar_t const * *,long *,struct utf32 *,long,long *),long,long *);

template<long k_buffer_size>
struct c_static_wchar_string
{
public:
	c_static_wchar_string() :
		m_string{}
	{
		clear();
	}

	void set(wchar_t const* s)
	{
		ustrnzcpy(m_string, s, k_buffer_size);
	}

	void clear()
	{
		*m_string = 0;
	}

	void append(wchar_t const* s)
	{
		ustrnzcat(m_string, s, k_buffer_size);
	}

	wchar_t const* append_print(wchar_t const* format, ...)
	{
		va_list list;
		va_start(list, format);

		append_vprint(format, list);
			
		va_end(list);
		return m_string;
	}

	wchar_t const* append_vprint(wchar_t const* format, va_list list)
	{
		unsigned int current_length = length();

		//assert(format);
		//assert(current_length >= 0 && current_length < k_buffer_size);

		uvsnzprintf(m_string + current_length, k_buffer_size - current_length, format, list);

		return m_string;
	}

	wchar_t const* get_string() const
	{
		return m_string;
	}

	long length() const
	{
		return ustrnlen(m_string, k_buffer_size);
	}

protected:
	wchar_t m_string[k_buffer_size];
};
