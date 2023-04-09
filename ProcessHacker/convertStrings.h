#pragma once
#include <string>

//helper functions for interaction with winapi
std::string convertFromWstring(const std::wstring& wstr);
std::wstring convertToWstring(const std::string& str);