#pragma once
#include <iostream>
#include <sstream>
#include "processHacker.h"
#include "myjson.h"
#include "convertStrings.h"

class ProcessHackerCommandLine
{
public:
	//constructor creates ProcessHacker
	ProcessHackerCommandLine();
	//the only public command for user interaction
	void getCommand();
private:
	std::unique_ptr<ProcessHacker> pProcessHacker;//ProcessHacker is command receiver

	//command implementation
	void handleCommand(const std::string& command, const std::vector<std::string>& arguments);
	void handleHelp(const std::vector<std::string>& arguments);
	void handleGetIntegrityLevel(const std::vector<std::string>& arguments);
	void handleSetIntegrityLevel(const std::vector<std::string>& arguments);
	void handleGetProcessPrivileges(const std::vector<std::string>& arguments);
	void handleSetProcessPrivilege(const std::vector <std::string>& arguments);
	void handleGetFileOwner(const std::vector<std::string>& arguments);
	void handleGetFileAccessRights(const std::vector<std::string>& arguments);
	void handleSetFileAccessRight(const std::vector<std::string>& arguments);
};