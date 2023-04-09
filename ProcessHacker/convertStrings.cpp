#include <Windows.h>
#include "convertStrings.h"

std::string convertFromWstring(const std::wstring& wstr)
{
	auto wstrLen = wstr.length();
	int numChars = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], wstrLen, nullptr, 0, nullptr, nullptr);
	std::string str(numChars, '\0');

	WideCharToMultiByte(CP_UTF8, 0, &wstr[0], wstrLen, &str[0], numChars, nullptr, nullptr);

	return str;
}
std::wstring convertToWstring(const std::string& str)
{
	auto strLen = str.length();
	int numChars = MultiByteToWideChar(CP_UTF8, 0, &str[0], strLen, nullptr, 0);
	std::wstring wstr(numChars, '\0');

	MultiByteToWideChar(CP_UTF8, 0, &str[0], strLen, &wstr[0], numChars);
	return wstr;
}