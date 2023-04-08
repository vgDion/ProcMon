#include "myjson.h"

MyJsonOStream::MyJsonOStream(const std::string& filename)
{
	this->open(filename);
	if (!this->good())
		throw std::invalid_argument("Can't open file with such name");
}
MyJsonOStream::~MyJsonOStream()
{
	this->close();
}


//operators for writing process data in JSON format
MyJsonOStream& operator<< (MyJsonOStream& jsonOStream, const std::wstring& wstr)//wstring needs to be converted to multibyte
{
	auto str = convertFromWstring(wstr);

	jsonOStream << '"';
	for (std::size_t i = 0; str[i] != '\0'; ++i)
	{
		jsonOStream << str[i];
		if (str[i] == '\\') //duplicate backslash '\' symbol for JSON format
			jsonOStream << '\\';
	}
	jsonOStream << '"';

	return jsonOStream;
}
MyJsonOStream& operator<< (MyJsonOStream& jsonOStream, const std::string& str)
{
	jsonOStream << '"';

	for (std::size_t i = 0; str[i] != '\0'; ++i)
	{
		jsonOStream << str[i];
		if (str[i] == '\\') //duplicate backslash '\' symbol for JSON format
			jsonOStream << '\\';
	}
	jsonOStream << '"';

	return jsonOStream;
}
MyJsonOStream& operator<< (MyJsonOStream& jsonOStream, const Process::ProcessType& processType)
{
	std::string processTypeStr = "";
	switch (processType)
	{
	case Process::ProcessType::x86:
		processTypeStr = "x86";
		break;
	case Process::ProcessType::x64:
		processTypeStr = "x64";
		break;
	case Process::ProcessType::NA:
		processTypeStr = "N/A";
		break;
	}
	jsonOStream << processTypeStr;
	return jsonOStream;
}
MyJsonOStream& operator<< (MyJsonOStream& jsonOStream, const Process::CompilationMode& compilationMode)
{
	std::string compilationModeStr = "";
	switch (compilationMode)
	{
	case Process::CompilationMode::Native:
		compilationModeStr = "Native";
		break;
	case Process::CompilationMode::CLR:
		compilationModeStr = "CLR";
		break;
	case Process::CompilationMode::NA:
		compilationModeStr = "N/A";
		break;
	}
	jsonOStream << compilationModeStr;
	return jsonOStream;
}

MyJsonOStream& operator<< (MyJsonOStream& jsonOStream, Process& process)
{
	jsonOStream << '{' << '\n';
	jsonOStream << Key("name") << process.name << ',' << '\n';
	jsonOStream << Key("PID") << process.PID << ',' << '\n';
	jsonOStream << Key("parentPID") << process.parentPID << ',' << '\n';
	jsonOStream << Key("parentName") << process.parentName << ',' << '\n';
	jsonOStream << Key("exePath") << process.exePath << ',' << '\n';
	jsonOStream << Key("description") << process.description << ',' << '\n';
	jsonOStream << Key("sid") << process.sid << ',' << '\n';
	jsonOStream << Key("ownerName") << process.ownerName << ',' << '\n';
	jsonOStream << Key("processType") << process.processType << ',' << '\n';
	jsonOStream << Key("compilationMode") << process.compilationMode << ',' << '\n';
	jsonOStream << Key("depPolicy") << process.depPolicy << ',' << '\n';
	jsonOStream << Key("aslrPolicy") << process.aslrPolicy << ',' << '\n';
	jsonOStream << Key("dlls") << process.dlls << '\n';
	jsonOStream << '}' << '\n';
	return jsonOStream;
}


Key::Key(std::string& key) : key(key)
{
}
Key::Key(std::string&& key) : key(std::move(key))
{
}
MyJsonOStream& operator<< (MyJsonOStream& jsonOStream, const Key& key)
{
	jsonOStream << '"' << key.key << '"' << ':';
	return jsonOStream;
}
