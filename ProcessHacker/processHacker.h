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

class MyJsonOStream;

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

//deleter for unique_ptr, constructed from malloc-allocated pointers
struct FreeDeleter
{
	void operator()(void* x);
};
//deleter for unique_ptr <LPVOID> (security descriptors)
struct LocalFreeDeleter
{
	void operator()(LPVOID* x);
};


class ProcessHacker
{
public:
	//constructor gets system info, sets privileges for current process (ADMINISTRATOR MODE REQUIRED) 
	//and gets all current processes in map
	ProcessHacker();

	//functions for integrity level interaction (overloaded for processes and files)
	//integrity level is being extracted from SACL
	decltype(SECURITY_MANDATORY_MEDIUM_RID) getIntegrityLevel(const std::wstring& filename);
	decltype(SECURITY_MANDATORY_MEDIUM_RID) getIntegrityLevel(const DWORD PID);

	void setIntegrityLevel(const decltype(SECURITY_MANDATORY_MEDIUM_RID) integrityLevel, const std::wstring& filename);
	void setIntegrityLevel(const decltype(SECURITY_MANDATORY_MEDIUM_RID) integrityLevel, const DWORD PID);

	//functions for process priveleges interaction
	//values stored in vector are pairs: PRIVELEGE and ALLOWED/DISALLOWED
	//process privileges are being extracted from DACL
	std::vector<std::pair<std::wstring, std::wstring>> getProcessPrivileges(const DWORD PID);
	void setProcessPrivilege(const std::wstring& privilegeName, const bool enablePrivelege, const DWORD PID);

	//functions for file owner interaction
	//file owner is being extracted from SID
	std::wstring getFileOwner(const std::wstring& filename);

	//functions for file DACL interaction
	std::vector<std::tuple<std::wstring, std::wstring, std::wstring>> getFileAccessRights(const std::wstring& filename);
	//access right needs to be VALID mask, currently function sets right for current user only
	void setFileAccessRight(const decltype(STANDARD_RIGHTS_ALL) accessRight, const bool accessMode, const std::wstring& filename);
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

	//next functions need VALID handle with access rights to work right, these two functions are created to check handle
	bool isHandleAvailable(Handle& hProcess);
	bool isZombieProcess(Handle& hProcess);

	//functions that need VALID handle with access rights
	bool isWow64(Handle& hProcess);
	std::wstring getProcessName(Handle& hProcess);
	std::wstring getExePath(Handle& hProcess);
	Process::ProcessType getProcessType(Handle& hProcess);
	std::pair<std::wstring, std::wstring> getSidAndOwnerName(Handle& hProcess);
	std::wstring getDepPolicy(Handle& hProcess);
	std::wstring getAslrPolicy(Handle& hProcess);
	std::vector<std::wstring> getProcessDlls(Handle& hProcess);

	//function to get name by PID, needs handle with access right
	std::wstring getProcessName(const DWORD PID);

	//functions that dont need handle with access rights
	Process::CompilationMode getCompilationMode(const std::wstring& filename);
	Process::ProcessType getSystemType();
	std::wstring getProcessDescription(const std::wstring& filename);
	std::wstring getSidOwnerName(PSID pSid);
};