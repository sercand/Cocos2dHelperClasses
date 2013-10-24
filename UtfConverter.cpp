#include <string.h>
#include "UtfConverter.h"

namespace cocos2d{ namespace extension {
	std::wstring FromUtf8(const std::string& utf8string)
	{
		// Create and display a C style string, and then use it 
		// to create different kinds of strings.
		const char *orig = utf8string.c_str();

		// newsize describes the length of the 
		// wchar_t string called wcstring in terms of the number 
		// of wide characters, not the number of bytes.
		size_t newsize = strlen(orig) + 1;

		// The following creates a buffer large enough to contain 
		// the exact number of characters in the original string
		// in the new format. If you want to add more characters
		// to the end of the string, increase the value of newsize
		// to increase the size of the buffer.
		wchar_t * wcstring = new wchar_t[newsize];

		// Convert char* string to a wchar_t* string.
		size_t convertedChars = 0;
		mbstowcs_s(&convertedChars, wcstring, newsize, orig, _TRUNCATE);
		return wcstring;
	}

	std::string ToUtf8(const std::wstring& widestring)
	{
		// Create a string of wide characters, display it, and then
		// use this string to create other types of strings.
		const wchar_t *orig = widestring.c_str();

		// Convert the wchar_t string to a char* string. Record 
		//.the length of the original string and add 1 to it to
		//.account for the terminating null character.
		size_t origsize = wcslen(orig) + 1;
		size_t convertedChars = 0;

		// Use a multibyte string to append the type of string
		// to the new string before displaying the result.
		char strConcat[] = " (char *)";
		size_t strConcatsize = (strlen( strConcat ) + 1)*2;

		// Allocate two bytes in the multibyte output string for every wide
		// character in the input string (including a wide character
		// null). Because a multibyte character can be one or two bytes,
		// you should allot two bytes for each character. Having extra
		// space for the new string is not an error, but having
		// insufficient space is a potential security problem.
		const size_t newsize = origsize*2;
		// The new string will contain a converted copy of the original
		// string plus the type of string appended to it.
		char *nstring = new char[newsize+strConcatsize];

		// Put a copy of the converted string into nstring
		wcstombs_s(&convertedChars, nstring, newsize, orig, _TRUNCATE);
		// append the type of string to the new string.
		
		//mbscat_s((unsigned char*)nstring, newsize+strConcatsize, (unsigned char*)strConcat);
		// Display the result.
		return std::string(nstring);
	}
}}
