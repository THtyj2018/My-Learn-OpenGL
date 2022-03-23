#pragma once

#include <string>
#include <string_view>

using namespace std::string_literals;
using namespace std::string_view_literals;

using AnsiString = std::string;
using WideString = std::wstring;
using UTF8String = std::u8string;
using UTF16String = std::u16string;
using UTF32String = std::u32string;

using AnsiStringView = std::string_view;
using WideStringView = std::wstring_view;
using UTF8StringView = std::u8string_view;
using UTF16StringView = std::u16string_view;
using UTF32StringView = std::u32string_view;