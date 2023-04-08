#include "processHackerCommandLine.h"

//constructor creates ProcessHacker, writes process data in JSON file and displays initialisation message
ProcessHackerCommandLine::ProcessHackerCommandLine() : pProcessHacker(std::make_unique<ProcessHacker>())
{
	auto processMap = pProcessHacker->getProcessMap();
	MyJsonOStream jsonStream("ProcessHacker.json");
	jsonStream << processMap;
	std::cout << "Process Hacker initialised successfully.\nProcess information was put in ProcessHacker.json\n";
}

//the only public command for user interaction
void ProcessHackerCommandLine::getCommand()
{
	std::cout << "\nEnter command:\n@ ";
	std::string commandLineInput;
	std::getline(std::cin, commandLineInput);

	//parse the input using stringstream into command and arguments
	std::string command;
	std::vector<std::string> arguments;

	std::stringstream commandStream(commandLineInput);
	commandStream >> command;

	std::string argument;
	while (commandStream >> argument)//get all arguments
	{
		arguments.push_back(std::move(argument));
	}

	handleCommand(command, arguments);
}

//function for calling the right function. handles invalid arguments exceptions and displays them
void ProcessHackerCommandLine::handleCommand(const std::string& command, const std::vector<std::string>& arguments)
{
	try {
		if (command == "help")
		{
			handleHelp(arguments);
		}
		else if (command == "getIntegrityLevel")
		{
			handleGetIntegrityLevel(arguments);
		}
		else if (command == "setIntegrityLevel")
		{
			handleSetIntegrityLevel(arguments);
		}
		else if (command == "getProcessPrivileges")
		{
			handleGetProcessPrivileges(arguments);
		}
		else if (command == "setProcessPrivilege")
		{
			handleSetProcessPrivilege(arguments);
		}
		else if (command == "getFileOwner")
		{
			handleGetFileOwner(arguments);
		}
		else if (command == "getFileAccessRights")
		{
			handleGetFileAccessRights(arguments);
		}
		else if (command == "setFileAccessRight")
		{
			handleSetFileAccessRight(arguments);
		}
		else std::cout << "Unknown function";
	}
	catch (std::out_of_range)
	{
		std::cout << "Not enough arguments.\n";
	}
	catch (std::invalid_argument iae)
	{
		std::cout << "Invalid argument entered.\n" << iae.what() << "\n";
	}
}

//displays list of available functions or detailed function arguments info
void ProcessHackerCommandLine::handleHelp(const std::vector<std::string>& arguments)
{
	if (arguments.empty())
	{
		std::cout << "Available commands: \ngetIntegrityLevel <filename>\nsetIntegrityLevel <integrityLevel> <filename>\n\
getProcessPrivileges <PID>\nsetProcessPrivilege <privilege name> <privilege value> <PID>\ngetFileAccessRights <filename>\n\
setFileAccessRight <access right> <access right value> <filename>\n";
	}
	else
	{
		auto helpArgument = arguments.at(0);
		if (helpArgument == "getIntegrityLevel")
		{
			std::cout << "<filename> - full path to existing file\n";
		}
		else if (helpArgument == "setIntegrityLevel")
		{
			std::cout << "<integrityLevel> - string with possible values: \"untrusted\", \"low\", \"medium\", \"high\", \"system\"\n\
<filename> - full path to existing file\n";
		}
		else if (helpArgument == "getProcessPrivileges")
		{
			std::cout << "<PID> - integer value, PIDs for every process are available in JSON file\n";
		}
		else if (helpArgument == "setProcessPrivilege")
		{
			std::cout << "<privilege name> - string with possible values:\n\nSeAssignPrimaryTokenPrivilege\nSeAuditPrivilege\n\
SeBackupPrivilege\nSeChangeNotifyPrivilege\nSeCreateGlobalPrivilege\nSeCreatePagefilePrivilege\nSeCreatePermanentPrivilege\n\
SeCreateSymbolicLinkPrivilege\nSeCreateTokenPrivilege\nSeDebugPrivilege\nSeDelegateSessionUserImpersonatePrivilege\n\
SeEnableDelegationPrivilege\nSeImpersonatePrivilege\nSeIncreaseBasePriorityPrivilege\nSeIncreaseQuotaPrivilege\nSeIncreaseWorkingSetPrivilege\n\
SeLoadDriverPrivilege\nSeLockMemoryPrivilege\nSeMachineAccountPrivilege\nSeManageVolumePrivilege\nSeProfileSingleProcessPrivilege\n\
SeRelabelPrivilege\nSeRemoteShutdownPrivilege\nSeRestorePrivilege\nSeSecurityPrivilege\nSeShutdownPrivilege\nSeSyncAgentPrivilege\n\
SeSystemEnvironmentPrivilege\nSeSystemProfilePrivilege\nSeSystemtimePrivilege\nSeTakeOwnershipPrivilege\nSeTcbPrivilege\nSeTimeZonePrivilege\n\
SeTrustedCredManAccessPrivilege\nSeUndockPrivilege\nSeUnsolicitedInputPrivilege\n\n";
			std::cout << "<privilege value> - boolean value (1 = Enable or 0 = Disable)\n\
<PID> - integer value, PIDs for every process are available in JSON file\n";
		}
		else if (helpArgument == "getFileAccessRights")
		{
			std::cout << "<filename> - full path to existing file\n";
		}
		else if (helpArgument == "setFileAccessRight")
		{
			std::cout << "<access right> - string with possible values:\STANDARD_RIGHTS_ALL\nFILE_GENERIC_EXECUTE\nFILE_GENERIC_READ\n\
FILE_GENERIC_WRITE\n<access right value> - boolean value (1 = Allowed or 0 = Disallowed)\n<filename> - full path to existing file\n\
function sets right for CURRENT USER\n";
		}
		else std::cout << "Unknown function\n";
	}
}
//next functions are converting arguments in required by ProcessHacker format, call ProcessHacker methods and display result
void ProcessHackerCommandLine::handleGetIntegrityLevel(const std::vector<std::string>& arguments)
{
	//convert string to wstring as winapi demands
	auto result = pProcessHacker->getIntegrityLevel(convertToWstring(arguments.at(0)));

	if (result == SECURITY_MANDATORY_UNTRUSTED_RID)
		std::cout << "SECURITY_MANDATORY_UNTRUSTED_RID" << '\n';
	else if (result == SECURITY_MANDATORY_LOW_RID)
		std::cout << "SECURITY_MANDATORY_LOW_RID" << '\n';
	else if (result == SECURITY_MANDATORY_MEDIUM_RID)
		std::cout << "SECURITY_MANDATORY_MEDIUM_RID" << '\n';
	else if (result == SECURITY_MANDATORY_HIGH_RID)
		std::cout << "SECURITY_MANDATORY_HIGH_RID" << '\n';
	else if (result == SECURITY_MANDATORY_SYSTEM_RID)
		std::cout << "SECURITY_MANDATORY_SYSTEM_RID" << '\n';
}
void ProcessHackerCommandLine::handleSetIntegrityLevel(const std::vector<std::string>& arguments)
{
	decltype(SECURITY_MANDATORY_MEDIUM_RID) requestedLevel;
	auto requestedLevelStr = arguments.at(0);
	if (requestedLevelStr == "untrusted")
		requestedLevel = SECURITY_MANDATORY_UNTRUSTED_RID;
	else if (requestedLevelStr == "low")
		requestedLevel = SECURITY_MANDATORY_LOW_RID;
	else if (requestedLevelStr == "medium")
		requestedLevel = SECURITY_MANDATORY_MEDIUM_RID;
	else if (requestedLevelStr == "high")
		requestedLevel = SECURITY_MANDATORY_HIGH_RID;
	else if (requestedLevelStr == "system")
		requestedLevel = SECURITY_MANDATORY_SYSTEM_RID;
	else throw std::invalid_argument("Unknown integrity level");

	auto wfilename = convertToWstring(arguments.at(1));
	pProcessHacker->setIntegrityLevel(requestedLevel, wfilename);
}
void ProcessHackerCommandLine::handleGetProcessPrivileges(const std::vector<std::string>& arguments)
{
	auto privileges = pProcessHacker->getProcessPrivileges(std::stoi(arguments.at(0)));
	for (auto& privilege : privileges)
	{
		std::cout << convertFromWstring(privilege.first) << "\t" << convertFromWstring(privilege.second) << "\n";
	}
}
void ProcessHackerCommandLine::handleSetProcessPrivilege(const std::vector <std::string>& arguments)
{
	pProcessHacker->setProcessPrivilege(convertToWstring(arguments.at(0)), std::stoi(arguments.at(1)), std::stoi(arguments.at(2)));
}
void ProcessHackerCommandLine::handleGetFileOwner(const std::vector<std::string>& arguments)
{
	auto ownerName = pProcessHacker->getFileOwner(convertToWstring(arguments.at(0)));
	std::cout << convertFromWstring(ownerName) << "\n";
}
void ProcessHackerCommandLine::handleGetFileAccessRights(const std::vector<std::string>& arguments)
{
	auto accessRights = pProcessHacker->getFileAccessRights(convertToWstring(arguments.at(0)));
	for (auto& accessRight : accessRights)
	{
		std::cout << convertFromWstring(std::get<0>(accessRight)) << "\t" << convertFromWstring(std::get<1>(accessRight)) << "\t"\
			<< convertFromWstring(std::get<2>(accessRight)) << "\n";
	}
}
void ProcessHackerCommandLine::handleSetFileAccessRight(const std::vector<std::string>& arguments)
{
	auto accessRightStr = arguments.at(0);
	decltype (GENERIC_ALL) accessRight;
	if (accessRightStr == "STANDARD_RIGHTS_ALL")
		accessRight = STANDARD_RIGHTS_ALL;
	else if (accessRightStr == "FILE_GENERIC_EXECUTE")
		accessRight = FILE_GENERIC_EXECUTE;
	else if (accessRightStr == "FILE_GENERIC_READ")
		accessRight = FILE_GENERIC_READ;
	else if (accessRightStr == "FILE_GENERIC_WRITE")
		accessRight = FILE_GENERIC_WRITE;
	else throw std::invalid_argument("Unknown access right");

	pProcessHacker->setFileAccessRight(accessRight, std::stoi(arguments.at(1)), convertToWstring(arguments.at(2)));
}