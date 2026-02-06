#pragma once

#include "cseries/cseries.hpp"

template<int32 k_maximum_count>
class c_json_static_string :
	public c_static_string<k_maximum_count>
{
public:
	c_json_static_string() :
		c_static_string<k_maximum_count>(),
		m_depth(0),
		m_needs_comma(false)
	{
	}

	// Start a JSON object
	void begin_object()
	{
		write_comma_if_needed();
		write_indent();
		this->append("{\r\n");
		m_depth++;
		m_needs_comma = false;
	}

	// End a JSON object
	void end_object()
	{
		this->append("\r\n");
		m_depth--;
		write_indent();
		this->append("}");
		m_needs_comma = true;
	}

	// Start a JSON array
	void begin_array()
	{
		write_comma_if_needed();
		write_indent();
		this->append("[\r\n");
		m_depth++;
		m_needs_comma = false;
	}

	// End a JSON array
	void end_array()
	{
		this->append("\r\n");
		m_depth--;
		write_indent();
		this->append("]");
		m_needs_comma = true;
	}

	// Add a string property
	void add_string(const char* key, const char* value)
	{
		ASSERT(key != nullptr);
		ASSERT(value != nullptr);

		c_static_string<k_maximum_count> escaped;
		escape_string(value, escaped);

		write_comma_if_needed();
		write_indent();
		this->append_print("\"%s\": \"%s\"", key, escaped.get_string());
	}

	// Add a string value (for arrays)
	void add_string_value(const char* value)
	{
		ASSERT(value != nullptr);

		c_static_string<k_maximum_count> escaped;
		escape_string(value, escaped);

		write_comma_if_needed();
		write_indent();
		this->append_print("\"%s\"", escaped.get_string());
	}

	// Add an integer property
	void add_integer(const char* key, int32 value)
	{
		ASSERT(key != nullptr);

		write_comma_if_needed();
		write_indent();
		this->append_print("\"%s\": %d", key, value);
	}

	// Add an integer value (for arrays)
	void add_integer_value(int32 value)
	{
		write_comma_if_needed();
		write_indent();
		this->append_print("%d", value);
	}

	// Add a 64-bit integer property
	void add_integer64(const char* key, int64 value)
	{
		ASSERT(key != nullptr);

		write_comma_if_needed();
		write_indent();
		this->append_print("\"%s\": %lld", key, value);
	}

	// Add an unsigned integer property
	void add_unsigned(const char* key, uns32 value)
	{
		ASSERT(key != nullptr);

		write_comma_if_needed();
		write_indent();
		this->append_print("\"%s\": %u", key, value);
	}

	// Add a float property
	void add_float(const char* key, float value)
	{
		ASSERT(key != nullptr);

		write_comma_if_needed();
		write_indent();
		this->append_print("\"%s\": %.6f", key, value);
	}

	// Add a double property
	void add_double(const char* key, double value)
	{
		ASSERT(key != nullptr);

		write_comma_if_needed();
		write_indent();
		this->append_print("\"%s\": %.12f", key, value);
	}

	// Add a boolean property
	void add_boolean(const char* key, bool value)
	{
		ASSERT(key != nullptr);

		write_comma_if_needed();
		write_indent();
		this->append_print("\"%s\": %s", key, value ? "true" : "false");
	}

	// Add a boolean value (for arrays)
	void add_boolean_value(bool value)
	{
		write_comma_if_needed();
		write_indent();
		this->append(value ? "true" : "false");
	}

	// Add a null property
	void add_null(const char* key)
	{
		ASSERT(key != nullptr);

		write_comma_if_needed();
		write_indent();
		this->append_print("\"%s\": null", key);
	}

	// Add a null value (for arrays)
	void add_null_value()
	{
		write_comma_if_needed();
		write_indent();
		this->append("null");
	}

	// Start a nested object with a key
	void begin_object(const char* key)
	{
		ASSERT(key != nullptr);

		write_comma_if_needed();
		write_indent();
		this->append_print("\"%s\": {\r\n", key);
		m_depth++;
		m_needs_comma = false;
	}

	// Start a nested array with a key
	void begin_array(const char* key)
	{
		ASSERT(key != nullptr);

		write_comma_if_needed();
		write_indent();
		this->append_print("\"%s\": [\r\n", key);
		m_depth++;
		m_needs_comma = false;
	}

	// Add raw JSON (be careful with this!)
	void add_raw(const char* key, const char* raw_json)
	{
		ASSERT(key != nullptr);
		ASSERT(raw_json != nullptr);

		write_comma_if_needed();
		write_indent();
		this->append_print("\"%s\": %s", key, raw_json);
	}

	// Add raw JSON value for arrays
	void add_raw_value(const char* raw_json)
	{
		ASSERT(raw_json != nullptr);

		write_comma_if_needed();
		write_indent();
		this->append(raw_json);
	}

	// Reset to start building a new JSON
	void reset()
	{
		this->clear();
		m_depth = 0;
		m_needs_comma = false;
	}

	// Build a complete JSON object (convenience method)
	void build_root_object()
	{
		reset();
		begin_object();
	}

	// Finalize the root object
	void finalize_root_object()
	{
		end_object();
	}

	// Build a complete JSON array (convenience method)
	void build_root_array()
	{
		reset();
		begin_array();
	}

	// Finalize the root array
	void finalize_root_array()
	{
		end_array();
	}

private:
	void write_indent()
	{
		for (int32 i = 0; i < m_depth; i++)
		{
			this->append("  ");
		}
	}

	void write_comma_if_needed()
	{
		if (m_needs_comma)
		{
			this->append(",\r\n");
		}
		m_needs_comma = true;
	}

	void escape_string(const char* str, c_static_string<k_maximum_count>& escaped)
	{
		escaped.clear();

		for (const char* c = str; *c; c++)
		{
			switch (*c)
			{
			case '"':  escaped.append("\\\""); break;
			case '\\': escaped.append("\\\\"); break;
			case '\b': escaped.append("\\b"); break;
			case '\f': escaped.append("\\f"); break;
			case '\n': escaped.append("\\n"); break;
			case '\r': escaped.append("\\r"); break;
			case '\t': escaped.append("\\t"); break;
			default:
			{
				if (*c < 0x20)
				{
					escaped.append_print("\\u%04x", (int)*c);
				}
				else
				{
					char temp[2] = { *c, 0 };
					escaped.append(temp);
				}
			}
			break;
			}
		}
	}

private:
	int32 m_depth;
	bool m_needs_comma;
};

template<int32 k_maximum_count>
class c_html_static_string :
	public c_static_string<k_maximum_count>
{
public:
	c_html_static_string() :
		c_static_string<k_maximum_count>(),
		m_depth(0),
		m_in_opening_tag(false)
	{
	}

	// ========================================================================
	// Document Structure
	// ========================================================================

	void doctype()
	{
		this->set("<!DOCTYPE html>\r\n");
	}

	void begin_html()
	{
		doctype();
		begin_tag("html");
	}

	void end_html()
	{
		end_tag("html");
	}

	// ========================================================================
	// Generic Tag Methods
	// ========================================================================

	void begin_tag(const char* tag_name)
	{
		ASSERT(tag_name != nullptr);

		close_opening_tag_if_needed();
		write_indent();
		this->append_print("<%s", tag_name);
		m_current_tag.set(tag_name);
		m_in_opening_tag = true;
		this->append(">\r\n");
		m_depth++;
		m_in_opening_tag = false;
	}

	void end_tag(const char* tag_name)
	{
		ASSERT(tag_name != nullptr);

		close_opening_tag_if_needed();
		m_depth--;
		write_indent();
		this->append_print("</%s>\r\n", tag_name);
	}

	void self_closing_tag(const char* tag_name)
	{
		ASSERT(tag_name != nullptr);

		close_opening_tag_if_needed();
		write_indent();
		this->append_print("<%s />\r\n", tag_name);
	}

	// ========================================================================
	// Attributes (must be called immediately after begin_tag before any content)
	// ========================================================================

	c_html_static_string& attr(const char* name, const char* value)
	{
		ASSERT(name != nullptr);
		ASSERT(value != nullptr);

		c_static_string<k_maximum_count> escaped;
		escape_attribute(value, escaped);

		// Remove the closing > and newline from the tag
		int32 len = this->length();
		if (len >= 3 && this->get_string()[len - 3] == '>' &&
			this->get_string()[len - 2] == '\r' && this->get_string()[len - 1] == '\n')
		{
			this->set_length(len - 3);
		}

		this->append_print(" %s=\"%s\">\r\n", name, escaped.get_string());
		return *this;
	}

	c_html_static_string& attr(const char* name, int32 value)
	{
		ASSERT(name != nullptr);

		int32 len = this->length();
		if (len >= 3 && this->get_string()[len - 3] == '>' &&
			this->get_string()[len - 2] == '\r' && this->get_string()[len - 1] == '\n')
		{
			this->set_length(len - 3);
		}

		this->append_print(" %s=\"%d\">\r\n", name, value);
		return *this;
	}

	c_html_static_string& id(const char* value)
	{
		return attr("id", value);
	}

	c_html_static_string& css_class(const char* value)
	{
		return attr("class", value);
	}

	c_html_static_string& style(const char* value)
	{
		return attr("style", value);
	}

	c_html_static_string& href(const char* value)
	{
		return attr("href", value);
	}

	c_html_static_string& src(const char* value)
	{
		return attr("src", value);
	}

	c_html_static_string& type(const char* value)
	{
		return attr("type", value);
	}

	c_html_static_string& name(const char* value)
	{
		return attr("name", value);
	}

	c_html_static_string& value(const char* value)
	{
		return attr("value", value);
	}

	c_html_static_string& placeholder(const char* value)
	{
		return attr("placeholder", value);
	}

	// ========================================================================
	// Content Methods
	// ========================================================================

	void text(const char* content)
	{
		ASSERT(content != nullptr);

		close_opening_tag_if_needed();
		c_static_string<k_maximum_count> escaped;
		escape_html(content, escaped);

		write_indent();
		this->append(escaped.get_string());
		this->append("\r\n");
	}

	void raw(const char* content)
	{
		ASSERT(content != nullptr);

		close_opening_tag_if_needed();
		write_indent();
		this->append(content);
		this->append("\r\n");
	}

	// ========================================================================
	// Common Elements
	// ========================================================================

	void head_begin() { begin_tag("head"); }
	void head_end() { end_tag("head"); }

	void body_begin() { begin_tag("body"); }
	void body_end() { end_tag("body"); }

	void title(const char* text_content)
	{
		begin_tag("title");
		text(text_content);
		end_tag("title");
	}

	void meta_charset(const char* charset)
	{
		close_opening_tag_if_needed();
		write_indent();
		this->append_print("<meta charset=\"%s\" />\r\n", charset);
	}

	void meta_viewport()
	{
		close_opening_tag_if_needed();
		write_indent();
		this->append("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\" />\r\n");
	}

	void link_stylesheet(const char* href_url)
	{
		close_opening_tag_if_needed();
		write_indent();
		this->append_print("<link rel=\"stylesheet\" href=\"%s\" />\r\n", href_url);
	}

	void script_src(const char* src_url)
	{
		close_opening_tag_if_needed();
		write_indent();
		this->append_print("<script src=\"%s\"></script>\r\n", src_url);
	}

	void style_begin() { begin_tag("style"); }
	void style_end() { end_tag("style"); }

	void script_begin() { begin_tag("script"); }
	void script_end() { end_tag("script"); }

	// Headings
	void h1(const char* text_content) { begin_tag("h1"); text(text_content); end_tag("h1"); }
	void h2(const char* text_content) { begin_tag("h2"); text(text_content); end_tag("h2"); }
	void h3(const char* text_content) { begin_tag("h3"); text(text_content); end_tag("h3"); }
	void h4(const char* text_content) { begin_tag("h4"); text(text_content); end_tag("h4"); }
	void h5(const char* text_content) { begin_tag("h5"); text(text_content); end_tag("h5"); }
	void h6(const char* text_content) { begin_tag("h6"); text(text_content); end_tag("h6"); }

	void h1_begin() { begin_tag("h1"); }
	void h1_end() { end_tag("h1"); }
	void h2_begin() { begin_tag("h2"); }
	void h2_end() { end_tag("h2"); }
	void h3_begin() { begin_tag("h3"); }
	void h3_end() { end_tag("h3"); }

	// Paragraphs and text
	void p(const char* text_content) { begin_tag("p"); text(text_content); end_tag("p"); }
	void p_begin() { begin_tag("p"); }
	void p_end() { end_tag("p"); }

	void span_begin() { begin_tag("span"); }
	void span_end() { end_tag("span"); }

	void strong(const char* text_content) { begin_tag("strong"); text(text_content); end_tag("strong"); }
	void em(const char* text_content) { begin_tag("em"); text(text_content); end_tag("em"); }
	void code(const char* text_content) { begin_tag("code"); text(text_content); end_tag("code"); }

	// Links
	void a_begin() { begin_tag("a"); }
	void a_end() { end_tag("a"); }

	void a(const char* href_url, const char* text_content)
	{
		begin_tag("a");
		attr("href", href_url);
		text(text_content);
		end_tag("a");
	}

	// Images
	void img(const char* src_url, const char* alt_text)
	{
		close_opening_tag_if_needed();
		write_indent();

		c_static_string<k_maximum_count> escaped_alt;
		escape_attribute(alt_text, escaped_alt);

		this->append_print("<img src=\"%s\" alt=\"%s\" />\r\n", src_url, escaped_alt.get_string());
	}

	// Lists
	void ul_begin() { begin_tag("ul"); }
	void ul_end() { end_tag("ul"); }
	void ol_begin() { begin_tag("ol"); }
	void ol_end() { end_tag("ol"); }
	void li_begin() { begin_tag("li"); }
	void li_end() { end_tag("li"); }

	void li(const char* text_content)
	{
		begin_tag("li");
		text(text_content);
		end_tag("li");
	}

	// Divisions
	void div_begin() { begin_tag("div"); }
	void div_end() { end_tag("div"); }

	// Tables
	void table_begin() { begin_tag("table"); }
	void table_end() { end_tag("table"); }
	void thead_begin() { begin_tag("thead"); }
	void thead_end() { end_tag("thead"); }
	void tbody_begin() { begin_tag("tbody"); }
	void tbody_end() { end_tag("tbody"); }
	void tr_begin() { begin_tag("tr"); }
	void tr_end() { end_tag("tr"); }
	void th_begin() { begin_tag("th"); }
	void th_end() { end_tag("th"); }
	void td_begin() { begin_tag("td"); }
	void td_end() { end_tag("td"); }

	void th(const char* text_content) { begin_tag("th"); text(text_content); end_tag("th"); }
	void td(const char* text_content) { begin_tag("td"); text(text_content); end_tag("td"); }

	// Forms
	void form_begin() { begin_tag("form"); }
	void form_end() { end_tag("form"); }

	void input(const char* type_value, const char* name_value)
	{
		close_opening_tag_if_needed();
		write_indent();
		this->append_print("<input type=\"%s\" name=\"%s\" />\r\n", type_value, name_value);
	}

	void button_begin() { begin_tag("button"); }
	void button_end() { end_tag("button"); }

	void button(const char* text_content)
	{
		begin_tag("button");
		text(text_content);
		end_tag("button");
	}

	void textarea_begin() { begin_tag("textarea"); }
	void textarea_end() { end_tag("textarea"); }

	void label_begin() { begin_tag("label"); }
	void label_end() { end_tag("label"); }

	void label(const char* for_id, const char* text_content)
	{
		begin_tag("label");
		attr("for", for_id);
		text(text_content);
		end_tag("label");
	}

	// Semantic HTML5
	void header_begin() { begin_tag("header"); }
	void header_end() { end_tag("header"); }
	void footer_begin() { begin_tag("footer"); }
	void footer_end() { end_tag("footer"); }
	void nav_begin() { begin_tag("nav"); }
	void nav_end() { end_tag("nav"); }
	void main_begin() { begin_tag("main"); }
	void main_end() { end_tag("main"); }
	void section_begin() { begin_tag("section"); }
	void section_end() { end_tag("section"); }
	void article_begin() { begin_tag("article"); }
	void article_end() { end_tag("article"); }
	void aside_begin() { begin_tag("aside"); }
	void aside_end() { end_tag("aside"); }

	// Line breaks
	void br()
	{
		close_opening_tag_if_needed();
		write_indent();
		this->append("<br />\r\n");
	}

	void hr()
	{
		close_opening_tag_if_needed();
		write_indent();
		this->append("<hr />\r\n");
	}

	// Comments
	void comment(const char* text_content)
	{
		close_opening_tag_if_needed();
		write_indent();
		this->append_print("<!-- %s -->\r\n", text_content);
	}

	// Reset
	void reset()
	{
		this->clear();
		m_depth = 0;
		m_in_opening_tag = false;
		m_current_tag.clear();
	}

private:
	void write_indent()
	{
		for (int32 i = 0; i < m_depth; i++)
		{
			this->append("  ");
		}
	}

	void escape_html(const char* str, c_static_string<k_maximum_count>& escaped)
	{
		escaped.clear();

		for (const char* c = str; *c; c++)
		{
			switch (*c)
			{
			case '<':  escaped.append("&lt;"); break;
			case '>':  escaped.append("&gt;"); break;
			case '&':  escaped.append("&amp;"); break;
			case '"':  escaped.append("&quot;"); break;
			case '\'': escaped.append("&#39;"); break;
			default:
				char temp[2] = { *c, 0 };
				escaped.append(temp);
				break;
			}
		}
	}

	void escape_attribute(const char* str, c_static_string<k_maximum_count>& escaped)
	{
		escape_html(str, escaped);
	}

	void close_opening_tag_if_needed()
	{
		if (m_in_opening_tag)
		{
			this->append(">");
			m_in_opening_tag = false;
		}
	}

private:
	int32 m_depth;
	bool m_in_opening_tag;
	c_static_string<64> m_current_tag;
};

