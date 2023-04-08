#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <unordered_map>

#include "processHacker.h"
#include "convertStrings.h"

class MyJsonOStream: public std::ofstream
{
public:
	MyJsonOStream(const std::string& filename);
	virtual ~MyJsonOStream();


	//operators for writing process data in JSON format
	friend MyJsonOStream& operator<< (MyJsonOStream& jsonOStream, const std::wstring& wstr);//wstring needs to be converted to multibyte
	friend MyJsonOStream& operator<< (MyJsonOStream& jsonOStream, const std::string& string);
	friend MyJsonOStream& operator<< (MyJsonOStream& jsonOStream,  Process& process);
	friend MyJsonOStream& operator<< (MyJsonOStream& jsonOStream, const Process::ProcessType& processType);
	friend MyJsonOStream& operator<< (MyJsonOStream& jsonOStream, const Process::CompilationMode& compilationMode);
	//template operator for vectors (included in header only)
	template <typename T>
	friend MyJsonOStream& operator<< (MyJsonOStream& jsonOStream, std::shared_ptr<T> ptr)
	{
		jsonOStream << *ptr;
		return jsonOStream;
	}
	template <typename T>
	friend MyJsonOStream& operator<< (MyJsonOStream& jsonOStream, const std::vector<T>& vector)
	{
		jsonOStream << '[';
		if (!vector.empty())
		{
			for (auto it = vector.cbegin(); it != vector.cend(); ++it)
			{
				jsonOStream << *it; //print value stored in vector
				if (it != vector.cend() - 1) //print "," for each element in vector but the last one
					jsonOStream << ',';
			}
		}
		jsonOStream << ']';
		return jsonOStream;
	}
	//template operator for unordered maps (ignore key) (included in header only)
	template <typename Key, typename T>
	friend MyJsonOStream& operator<< (MyJsonOStream& jsonOStream, const std::unordered_map<Key, T>& uMap)
	{
		auto mapSize = uMap.size();
		std::size_t mapIndex = 0;

		jsonOStream << '[';
		if (!uMap.empty())
		{
			for (auto it = uMap.cbegin(); it != uMap.cend(); ++it)
			{
				jsonOStream << (*it).second; //print value stored in vector
				if (mapIndex != mapSize - 1) //print "," for each element in vector but the last one
					jsonOStream << ',';
				++mapIndex;
			}
		}
		jsonOStream << ']';
		return jsonOStream;
	}
};

class Key
{
public:
	Key(std::string& key);
	Key(std::string&& key);
	friend MyJsonOStream& operator<< (MyJsonOStream& jsonOStream, const Key& key);
private:
	std::string key;
};
