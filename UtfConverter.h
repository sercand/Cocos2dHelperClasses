#ifndef __UTFCONVERTER_H__
#define __UTFCONVERTER_H__
#include <string>
#include <cocos2d.h>
namespace cocos2d { namespace extension{
	std::wstring FromUtf8(const std::string& utf8string);
	std::string ToUtf8(const std::wstring& widestring);
}}

#endif //__UTFCONVERTER_H__