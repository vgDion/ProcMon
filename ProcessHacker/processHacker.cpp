#include "processHacker.h"

//class to represent process data

Process::Process(std::wstring&& name,
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
	std::vector<std::wstring>&& dlls) :
	name(std::move(name)),
	PID(std::move(PID)),
	parentPID(std::move(parentPID)),
	parentName(std::move(parentName)),
	exePath(std::move(exePath)),
	description(std::move(description)),
	sid(std::move(sid)),
	ownerName(std::move(ownerName)),
	processType(std::move(processType)),
	compilationMode(std::move(compilationMode)),
	depPolicy(std::move(depPolicy)),
	aslrPolicy(std::move(aslrPolicy)),
	dlls(std::move(dlls))
{};



//helper class to implement RAII for handles

Handle::Handle()
{
	handle = nullptr;
}
Handle::Handle(HANDLE h) : handle(h)
{
}
Handle::~Handle()
{
	if (handle != nullptr)
		CloseHandle(handle);
}
HANDLE Handle::operator()()
{
	return handle;
}
PHANDLE Handle::getPHandle()
{
	return &handle;
}




	//constructor gets system info, sets privileges for current process (ADMINISTRATOR MODE REQUIRED) 
	//and gets all current processes in map
ProcessHacker::ProcessHacker()
{
	GetSystemInfo((LPSYSTEM_INFO)&systemInfo);//set system info

	Handle hToken = nullptr; //get current process token
	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, hToken.getPHandle()))
		throw std::runtime_error("OpenProcessToken failed");

	setPrivelege(hToken, L"SeDebugPrivilege", true);//get debug privilege 
	setPrivelege(hToken, L"SeBackupPrivilege", true);//get backup privilege for getting SACL
	getEveryProcessInfo();
}



//function that gets encapsulates getting process info
Process ProcessHacker::getProcessInfo(PROCESSENTRY32& pe32)
{
	//Forward declaration of process's parameters
	std::wstring name;
	DWORD PID;
	DWORD parentPID;
	std::wstring parentName;
	std::wstring exePath;
	std::wstring description;
	std::wstring sid;
	std::wstring ownerName;
	Process::ProcessType processType;
	Process::CompilationMode compilationMode;
	std::wstring depPolicy;
	std::wstring aslrPolicy;
	std::vector<std::wstring> dlls;

	//name
	name = getProcessName(pe32);

	//PID
	PID = getPID(pe32);


	//next functions need handle with privileges: if it's imbossible to create one, set fields as N/A
	Handle hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, PID); //handle with access right
	
	return Process(std::move(name), std::move(PID), std::move(parentPID), std::move(parentName),
		std::move(exePath), std::move(description), std::move(sid), std::move(ownerName), std::move(processType),
		std::move(compilationMode), std::move(depPolicy), std::move(aslrPolicy), std::move(dlls));
}
//helper function called in constructor to get all processes
void ProcessHacker::getEveryProcessInfo()
{
	PROCESSENTRY32 pe32;
	Handle hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap() == INVALID_HANDLE_VALUE)
		throw std::runtime_error("CreateToolhelp32Snapshot failed");

	pe32.dwSize = sizeof(PROCESSENTRY32);
	if (!Process32First(hProcessSnap(), &pe32))
		throw std::runtime_error("Process32First failed");

	do {
		auto process = std::make_unique<Process>(getProcessInfo(pe32));
		processMap.insert(std::make_pair(process->PID, std::move(process)));
	} while (Process32Next(hProcessSnap(), &pe32));
}

//function that needs valid token with TOKEN_ADJUST_PRIVILEGES, return TRUE if privilege was adjusted, else FALSE
bool ProcessHacker::setPrivelege(Handle& hToken, const LPCTSTR lpszPrivilege, const BOOL bEnablePrivilege)
{
	TOKEN_PRIVILEGES tp;
	LUID luid;

	if (!LookupPrivilegeValue(
		NULL,            // lookup privilege on local system
		lpszPrivilege,   // privilege to lookup 
		&luid))        // receives LUID of privilege
	{
		if (GetLastError() == ERROR_NO_SUCH_PRIVILEGE)
			throw std::invalid_argument("Privelege doesn't exist");
		else
			throw std::runtime_error("LookupPrivilegeValue failed");
	}

	tp.PrivilegeCount = 1; //filling TOKEN_PRIVILEGES struct to pass
	tp.Privileges[0].Luid = luid;
	if (bEnablePrivilege)
		tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	else
		tp.Privileges[0].Attributes = 0;

	if (!AdjustTokenPrivileges(hToken(), FALSE, &tp, sizeof(TOKEN_PRIVILEGES), (PTOKEN_PRIVILEGES)NULL, (PDWORD)NULL))
	{
		throw std::runtime_error("AdjustTokenPrivileges failed");
	}
	if (GetLastError() == ERROR_SUCCESS)
		return true;
	else
		return false;
}


//three functions that get info from PROCESSENTRY32 structure
std::wstring ProcessHacker::getProcessName(PROCESSENTRY32& pe32)
{
	return std::wstring(pe32.szExeFile);
}
DWORD ProcessHacker::getPID(PROCESSENTRY32& pe32)
{
	return pe32.th32ProcessID;
}

std::unordered_map<DWORD, std::shared_ptr<Process>> ProcessHacker::getProcessMap()
{
	return this->processMap;
}