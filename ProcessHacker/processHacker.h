#pragma once
#include <iostream>
#include <string>
#include <cstdint>
#include <vector>
#include <stdexcept>
#include <fstream>
#include <unordered_map>
#include <tuple>
#include <fstream>
#include <algorithm>

#include <Windows.h>
#include <Psapi.h>
#include <TlHelp32.h>
#include <sddl.h>
#include <accctrl.h>
#include <AclAPI.h>


//class to represent process data
struct Process
{
	enum class ProcessType { x86, x64, NA };
	enum class CompilationMode { Native, CLR, NA };

	Process(std::wstring&& name,
		DWORD&& PID,
		DWORD&& parentPID,
		std::wstring&& parentName,
		std::wstring&& exePath,
		std::wstring&& description,
		std::wstring&& sid,
		std::wstring&& ownerName,
		Process::ProcessType&& processType,
		Process::CompilationMode&& compilationMode,
		std::wstring&& depPolicy,
		std::wstring&& aslrPolicy,
		std::vector<std::wstring>&& dlls);

	const std::wstring name;
	const DWORD PID;
	const DWORD parentPID;
	const std::wstring parentName;
	const std::wstring exePath;
	const std::wstring description;
	const std::wstring sid;
	const std::wstring ownerName;
	const Process::ProcessType processType;
	const Process::CompilationMode compilationMode;
	const std::wstring depPolicy;
	const std::wstring aslrPolicy;
	const std::vector<std::wstring> dlls;
};

class ProcessHacker
{
public:
	//constructor gets system info, sets privileges for current process (ADMINISTRATOR MODE REQUIRED) 
	//and gets all current processes in map
	ProcessHacker();

	std::unordered_map<DWORD, std::shared_ptr<Process>> getProcessMap();

private:
	//member that is set in constructor (needed for getting system architecture(x86, x64))
	SYSTEM_INFO systemInfo;

	//key of a process is his PID
	std::unordered_map<DWORD, std::shared_ptr<Process>> processMap;

	//function that gets encapsulates getting process info
	Process getProcessInfo(PROCESSENTRY32& pe32);
	//helper function called in constructor to get all processes in map
	void getEveryProcessInfo();

	//function that needs valid token with TOKEN_ADJUST_PRIVILEGES, return TRUE if privilege was adjusted, else FALSE
	bool setPrivelege(Handle& hToken, const LPCTSTR lpszPrivilege, const BOOL bEnablePrivilege);

	//functions for integrity level interaction
	decltype(SECURITY_MANDATORY_MEDIUM_RID) getIntegrityLevelImpl(const std::wstring& filename);
	void setIntegrityLevelImpl(const decltype(SECURITY_MANDATORY_MEDIUM_RID) integrityLevel, const std::wstring& filename);

	//three functions that get info from PROCESSENTRY32 structure
	std::wstring getProcessName(PROCESSENTRY32& pe32);
	DWORD getPID(PROCESSENTRY32& pe32);
	DWORD getParentPID(PROCESSENTRY32& pe32);

	//functions that need VALID handle with access rights
	bool isWow64(Handle& hProcess);
	std::wstring getProcessName(Handle& hProcess);
	std::wstring getExePath(Handle& hProcess);
	Process::ProcessType getProcessType(Handle& hProcess);
	std::pair<std::wstring, std::wstring> getSidAndOwnerName(Handle& hProcess);
	std::wstring getDepPolicy(Handle& hProcess);
	std::wstring getAslrPolicy(Handle& hProcess);
	std::vector<std::wstring> getProcessDlls(Handle& hProcess);
};

//helper class to implement RAII for handles
class Handle
{
public:
	Handle();
	Handle(HANDLE h);
	~Handle();
	HANDLE operator()();
	PHANDLE getPHandle();
private:
	HANDLE handle;
};
