//
//  string.cpp
//  utf_string
//
//  Created by DemonShi on 10/18/12.
//  Copyright (c) 2012 DemonShi. All rights reserved.
//

#include "string.hpp"

#include <string.h>
#include <vector>

#include "utf8cpp/utf8.h"

#if __SIZEOF_WCHAR_T__ == 4 || __WCHAR_MAX__ > 0x10000
#define WCHAR_SIZE 4
#else
#define WCHAR_SIZE 2
#endif

namespace utf {
    
	string::string()
    {
		init();
	}

	string::string(const string &str)
	{
		init();
		m_buffer8 = str.m_buffer8;
		m_length = str.m_length;
	}

	string::string(const std::string& str)
	{
		init();
		from_utf8(str.c_str());
	}

	string::string(const std::wstring& str)
	{
		init();
#	if WCHAR_SIZE == 2
		from_utf16(str.c_str());
#	elif WCHAR_SIZE == 4
		from_utf32(reinterpret_cast<const char32_t*>(str.c_str()));
#	endif
	}

	string::string(const char* str)
	{
		init();
		from_utf8(str);
	}

	string::string(const wchar_t* str)
	{
		init();
#	if WCHAR_SIZE == 2
		from_utf16(str);
#	elif WCHAR_SIZE == 4
		from_utf32(reinterpret_cast<const char32_t*>(str));
#	endif
	}

	size_t string::size() const
	{
		return m_buffer8.size();
	}

	size_t string::length() const
	{
		return m_length;
	}
    
    void string::clear()
    {
		if (!m_buffer8.empty())
        {
			m_buffer8.clear();
			m_buffer16.clear();
			m_buffer32.clear();

            m_length = 0;
        }
    }
    
	string::operator std::string() const
	{
		return std::string(reinterpret_cast<const char*>(to_utf8()));
	}

	string::operator std::wstring() const
	{
#	if WCHAR_SIZE == 2
		return std::wstring(reinterpret_cast<const wchar_t*>(to_utf16()));
#	elif WCHAR_SIZE == 4
		return std::wstring(reinterpret_cast<const wchar_t*>(to_utf32()));
#	endif
		return NULL;
	}

	const char8_t* string::to_utf8() const
    {
		return &m_buffer8[0];
    }
    
    const char16_t* string::to_utf16() const
    {
		if (!m_buffer16.empty())
        {
			m_buffer16.reserve(m_length);
            
			utf8::unchecked::utf8to16(m_buffer8.begin(), m_buffer8.end()-1, std::back_inserter(m_buffer16));
			m_buffer16.push_back(0);
        }
        
		return &m_buffer16[0];
    }
    
    const char32_t* string::to_utf32() const
    {
		if (m_buffer32.empty())
        {
			m_buffer32.reserve(m_length);
            
			utf8::unchecked::utf8to32(m_buffer8.begin(), m_buffer8.end()-1, std::back_inserter(m_buffer32));

			m_buffer32.push_back(0);
        }
        
		return &m_buffer32[0];
    }
    
	string& string::from_utf8(const char8_t* str)
    {
        clear();
        
		if (!str || str[0] == 0)
        {
			return *this;
        }
        
		size_t elements = 0;
		const char8_t* end = str;
		while (*(end++) != 0) { ++elements; }

		if (!elements) {
			return *this;
		}

		m_buffer8.resize(elements);
		memcpy(&m_buffer8[0], str, elements * sizeof(m_buffer8[0]));
		m_buffer8.push_back(0);

		m_length = utf8::unchecked::distance(m_buffer8.begin(), m_buffer8.end());
		return *this;
    }
    
	string& string::from_utf16(const char16_t* str)
    {
        clear();
        
		if (!str || str[0] == 0)
        {
			return *this;
        }
        
		size_t elements = 0;
		const char16_t* end = str;
		while (*(end++) != 0) { ++elements; }

		if (!elements) {
			return *this;
		}

		m_buffer16.resize(elements);
		memcpy(&m_buffer16[0], str, elements * sizeof(m_buffer16[0]));
		m_buffer16.push_back(0);

		m_length = utf8::unchecked::distance(m_buffer16.begin(), m_buffer16.end());

		m_buffer8.reserve(m_length);
		utf8::unchecked::utf16to8(m_buffer16.begin(), m_buffer16.end(), std::back_inserter(m_buffer8));
		m_buffer8.push_back(0);
		return *this;
    }
    
	string& string::from_utf32(const char32_t* str)
    {
        clear();
        
		if (!str || str[0] == 0)
        {
			return *this;
        }

		size_t elements = 0;
		const char32_t* end = str;
		while (*(end++) != 0) { ++elements; }

		if (!elements) {
			return *this;
		}

		m_buffer32.resize(elements);
		memcpy(&m_buffer32[0], str, elements * sizeof(m_buffer32[0]));
		m_buffer32.push_back(0);

		m_length = utf8::unchecked::distance(m_buffer32.begin(), m_buffer32.end());

		m_buffer8.reserve(m_length);
		utf8::unchecked::utf32to8(m_buffer32.begin(), m_buffer32.end(), std::back_inserter(m_buffer8));
		m_buffer8.push_back(0);
		return *this;
    }

	void string::init()
	{
		m_length = 0;
	}
    
	std::ostream& operator<<(std::ostream& os, const string& str)
	{
		os << std::string(str);
		return os;
	}

	std::istream& operator>>(std::istream& is, string& str)
	{
		std::string s;
		is >> s;
		str = string(s.c_str());
		return is;
	}

	std::wostream& operator<<(std::wostream& os, const string& str)
	{
		os << std::wstring(str);
		return os;
	}

	std::wistream& operator>>(std::wistream& is, string& str)
	{
		std::wstring s;
		is >> s;
		str = string(s.c_str());
		return is;
	}
}
