//
//  string.hpp
//  utf_string
//
//  Created by DemonShi on 10/18/12.
//  Copyright (c) 2012 DemonShi. All rights reserved.
//

#pragma once

#include <string>
#include <vector>
//#include <cstdint>

namespace utf {
    
	typedef char char8_t;
	typedef unsigned short char16_t;
	typedef unsigned int char32_t;

    class string {
	public:
        string();
		string(const string& str);
        string(const std::string& str);
		string(const std::wstring& str);
		string(const char* str);
		string(const wchar_t* str);
        
        size_t size() const;
		size_t length() const;
        
        void clear();

        string& operator <<(const string& str);

		operator std::string() const;
		operator std::wstring() const;
        
        const char8_t* to_utf8() const;
        const char16_t* to_utf16() const;
        const char32_t* to_utf32() const;
        
		string& from_utf8(const char8_t* str);
		string& from_utf16(const char16_t* str);
		string& from_utf32(const char32_t* str);
        
        
    private:
		void init();

		std::vector<char8_t> m_buffer8;
		mutable std::vector<char16_t> m_buffer16;
		mutable std::vector<char32_t> m_buffer32;

		// number of printable characters, not including zero terminator
        size_t m_length;
        
		friend std::ostream& operator<<(std::ostream& os, const string& str);
		friend std::istream& operator>>(std::istream& is, string& str);
		friend std::wostream& operator<<(std::wostream& os, const string& str);
		friend std::wistream& operator>>(std::wistream& is, string& str);
    };
    
}
