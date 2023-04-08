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

	//parent PID
	parentPID = getParentPID(pe32);

	//parent name
	parentName = getProcessName(parentPID);

	//next functions need handle with privileges: if it's imbossible to create one, set fields as N/A
	Handle hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, PID); //handle with access right

	if (!isHandleAvailable(hProcess))
	{
		//data is not available, set all fields as "N/A"
		exePath = L"N/A";
		description = L"N/A";
		sid = L"N/A";
		ownerName = L"N/A";
		processType = Process::ProcessType::NA;
		compilationMode = Process::CompilationMode::NA;
		depPolicy = L"N/A";
		aslrPolicy = L"N/A";
		dlls = { L"N/A" };
	}
	else
	{
		//exe path
		exePath = getExePath(hProcess);

		//description
		description = getProcessDescription(exePath);

		//SID
		auto sidNamePair = getSidAndOwnerName(hProcess);
		sid = sidNamePair.first;

		//owner name
		ownerName = sidNamePair.second;

		//process type
		processType = getProcessType(hProcess);

		//compilation mode
		compilationMode = getCompilationMode(exePath);

		//DEP
		depPolicy = getDepPolicy(hProcess);

		//ASLR
		aslrPolicy = getAslrPolicy(hProcess);

		//dll
		dlls = getProcessDlls(hProcess);
	}
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


//functions for integrity level interaction (implementation)
decltype(SECURITY_MANDATORY_MEDIUM_RID) ProcessHacker::getIntegrityLevelImpl(const std::wstring& filename)
{
	DWORD integrityLevel;
	PSECURITY_DESCRIPTOR pSDtemp = nullptr;
	PACL sacl = nullptr;
	//get SACL security descriptor
	auto error = GetNamedSecurityInfo(&filename[0], SE_FILE_OBJECT, LABEL_SECURITY_INFORMATION,
		0, 0, 0, &sacl, &pSDtemp);
	//make unique_ptr for RAII
	std::unique_ptr<PSECURITY_DESCRIPTOR, LocalFreeDeleter> pSD(&pSDtemp);

	if (error != ERROR_SUCCESS)
	{
		if (error == ERROR_FILE_NOT_FOUND)
			throw std::invalid_argument("File with such name doesn't exist");
		else
			throw std::runtime_error(std::string("GetNamedSecurityInfo failed with error " + std::to_string(error)));
	}

	//first ACE contains SID that includes integrity level
	if (sacl != nullptr && sacl->AceCount > 0)
	{
		SYSTEM_MANDATORY_LABEL_ACE* ace = 0;
		if (!GetAce(sacl, 0, (LPVOID*)&ace))
			throw std::runtime_error("GetAce failed");

		SID* sid = reinterpret_cast<SID*>(&ace->SidStart);
		integrityLevel = sid->SubAuthority[0];
	}
	else
		integrityLevel = SECURITY_MANDATORY_MEDIUM_RID; //SACL doesn't exist, integrity level is default

	return integrityLevel;
}
void ProcessHacker::setIntegrityLevelImpl(const decltype(SECURITY_MANDATORY_MEDIUM_RID) integrityLevel, const std::wstring& filename)
{

	//SDDL-style security descriptor initialization
	LPCWSTR INTEGRITY_SDDL_SACL_W;
	if (integrityLevel == SECURITY_MANDATORY_LOW_RID)
		INTEGRITY_SDDL_SACL_W = L"S:(ML;;NR;;;LW)";
	else if (integrityLevel == SECURITY_MANDATORY_MEDIUM_RID)
		INTEGRITY_SDDL_SACL_W = L"S:(ML;;NR;;;ME)";
	else if (integrityLevel == SECURITY_MANDATORY_HIGH_RID)
		INTEGRITY_SDDL_SACL_W = L"S:(ML;;NR;;;HI)";
	else if (integrityLevel == SECURITY_MANDATORY_UNTRUSTED_RID ||
		integrityLevel == SECURITY_MANDATORY_SYSTEM_RID)
		throw std::invalid_argument("Can't set Untrusted or System integrity level");
	else
		throw std::invalid_argument("Invalid integrity level");

	DWORD error;
	PSECURITY_DESCRIPTOR pSDtemp = nullptr;

	PACL pSacl = nullptr;
	BOOL fSaclPresent = FALSE;
	BOOL fSaclDefaulted = FALSE;
	//convert SDDL security descriptor to structure
	auto returnCode = ConvertStringSecurityDescriptorToSecurityDescriptor(
		INTEGRITY_SDDL_SACL_W, SDDL_REVISION_1, &pSDtemp, NULL);

	std::unique_ptr<PSECURITY_DESCRIPTOR, LocalFreeDeleter> pSD(&pSDtemp);//make unique_ptr for RAII

	if (returnCode == 0)
		throw std::runtime_error("ConvertStringSecurityDescriptorToSecurityDescriptor failed");

	if (!GetSecurityDescriptorSacl(*pSD, &fSaclPresent, &pSacl, &fSaclDefaulted))
		throw std::runtime_error("GetSecurityDescriptorSacl failed");

	error = SetNamedSecurityInfo((LPWSTR)&filename[0],
		SE_FILE_OBJECT, LABEL_SECURITY_INFORMATION,
		NULL, NULL, NULL, pSacl);

	if (error != ERROR_SUCCESS)
	{
		if (error == ERROR_FILE_NOT_FOUND)
			throw std::invalid_argument("File with such name doesn't exist");
		else
			throw std::runtime_error(std::string("SetNamedSecurityInfo failed with error " + std::to_string(error)));
	}

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
DWORD ProcessHacker::getParentPID(PROCESSENTRY32& pe32)
{
	return pe32.th32ParentProcessID;
}



std::unordered_map<DWORD, std::shared_ptr<Process>> ProcessHacker::getProcessMap()
{
	return this->processMap;
}

std::wstring ProcessHacker::getExePath(Handle& hProcess)
{
	std::wstring exePath;

	wchar_t exePathTemp[1024];
	DWORD exePathSize = sizeof(exePathTemp) / sizeof(wchar_t);
	if (!QueryFullProcessImageName(hProcess(), 0, exePathTemp, &exePathSize))
	{
		auto kek = GetLastError();
		throw std::runtime_error("QueryFullProcessImageName failed");
	}
	exePath = exePathTemp;

	return exePath;
}

std::wstring ProcessHacker::getProcessDescription(const std::wstring& filename)
{
	std::wstring description = L"";
	int versionInfoSize = GetFileVersionInfoSize(&filename[0], NULL);
	if (!versionInfoSize)
	{
		if (GetLastError() == ERROR_RESOURCE_TYPE_NOT_FOUND) //there is no description
			return description;
		else
			throw std::runtime_error("GetFileVersionInfoSize failed");
	}

	auto versionInfoTemp = new BYTE[versionInfoSize];//allocate size bytes
	std::unique_ptr<BYTE[]> versionInfo(versionInfoTemp);//make unique_ptr for RAII
	if (!GetFileVersionInfo(&filename[0], NULL, versionInfoSize, versionInfo.get()))
	{
		if (GetLastError() == ERROR_RESOURCE_TYPE_NOT_FOUND) //there is no description
			return description;
		else
			throw std::runtime_error("GetFileVersionInfo failed");
	}

	struct LANGANDCODEPAGE {
		WORD wLanguage;
		WORD wCodePage;
	} *translationArray;

	UINT translationArrayByteLength = 0;
	if (!VerQueryValue(versionInfo.get(), L"\\VarFileInfo\\Translation", (LPVOID*)&translationArray, &translationArrayByteLength)) {
		throw std::runtime_error("GetFileVersionInfo failed");
	}

	// You may check GetSystemDefaultUILanguage() == translationArray[i].wLanguage 
	// if only the system language required
	for (unsigned int i = 0; i < (translationArrayByteLength / sizeof(LANGANDCODEPAGE)); i++) {
		wchar_t fileDescriptionKey[256];
		wsprintf(
			fileDescriptionKey,
			L"\\StringFileInfo\\%04x%04x\\FileDescription",
			translationArray[i].wLanguage,
			translationArray[i].wCodePage
		);

		wchar_t* fileDescription = NULL;
		UINT fileDescriptionSize;
		if (VerQueryValue(versionInfo.get(), fileDescriptionKey, (LPVOID*)&fileDescription, &fileDescriptionSize)) {
			description = fileDescription;
			break; //description on one language is enough
		}

	}
	return description;
}

std::pair<std::wstring, std::wstring> ProcessHacker::getSidAndOwnerName(Handle& hProcess)
{
	Handle hToken = nullptr; //get current process token
	if (!OpenProcessToken(hProcess(), TOKEN_QUERY, hToken.getPHandle()))
		throw std::runtime_error("OpenProcessToken failed");

	PTOKEN_USER pTuTemp = NULL;
	DWORD dwLength = 0;

	// Call GetTokenInformation to get the buffer size in dwLength.
	if (!GetTokenInformation(hToken(), TokenUser, (LPVOID)pTuTemp, 0, &dwLength))
	{
		if (GetLastError() != ERROR_INSUFFICIENT_BUFFER)
			throw std::runtime_error("GetTokenInformation failed");
	}

	pTuTemp = (PTOKEN_USER)malloc(dwLength * sizeof(dwLength));
	std::unique_ptr<TOKEN_USER> pTu(pTuTemp);


	if (!GetTokenInformation(
		hToken(),         // handle to the access token
		TokenUser,    // get information about the token's groups 
		(LPVOID)pTuTemp,   // pointer to PTOKEN_USER buffer
		dwLength,       // size of buffer
		&dwLength       // receives required buffer size
	))
		throw std::runtime_error("GetTokenInformation failed");

	auto pSid = pTu->User.Sid;
	wchar_t* temp;
	ConvertSidToStringSid(pSid, &temp);
	std::wstring stringSid(temp);
	LocalFree(temp);

	SID_NAME_USE SidType;
	wchar_t lpName[1024];
	wchar_t lpDomain[1024];
	DWORD dwSize = 1024;
	if (!LookupAccountSid(NULL, pSid, lpName, &dwSize, lpDomain, &dwSize, &SidType))
		throw std::runtime_error("LookupAccountSid failed");
	auto ownerName = std::wstring(lpDomain) + L"/" + std::wstring(lpName);

	return { stringSid, ownerName };
}

Process::ProcessType ProcessHacker::getProcessType(Handle& hProcess)
{
	Process::ProcessType processType;

	if (systemInfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_INTEL) //x86 enviroment
		processType = Process::ProcessType::x86;
	else //either x86 process running in x64 enviroment or x64 process
		processType = isWow64(hProcess) ? Process::ProcessType::x86 : Process::ProcessType::x64;

	return processType;
}

bool ProcessHacker::isWow64(Handle& hProcess)
{
	BOOL bWow64Process = FALSE;

	if (!IsWow64Process(hProcess(), &bWow64Process))
	{
		throw std::runtime_error("isWow64Process failed");
	}

	return bWow64Process;
}

Process::CompilationMode ProcessHacker::getCompilationMode(const std::wstring& filename)
{
	// Verify this is a executable/dll
	std::uint8_t data[4096];

	std::basic_ifstream<std::uint8_t> fStream(filename, std::ios::binary);
	fStream.read(&data[0], 4096);

	if ((data[1] << 8 | data[0]) != 0x5a4d)
		throw std::runtime_error("getCompilationMode failed: invalid header");

	// This will get the address for the WinNT header
	std::uint32_t iWinNTHdr = data[63] << 24 | data[62] << 16 | data[61] << 8 | data[60];

	// Verify this is an NT address
	if ((data[iWinNTHdr + 3] << 24 | data[iWinNTHdr + 2] << 16 | data[iWinNTHdr + 1] << 8 | data[iWinNTHdr]) != 0x00004550)
		throw std::runtime_error("getCompilationMode failed: invalid header");

	std::uint32_t iLightningAddr = iWinNTHdr + 24 + 208;
	std::uint32_t iSum = 0;
	std::uint32_t iTop = iLightningAddr + 8;

	for (auto i = iLightningAddr; i < iTop; ++i)
		iSum |= data[i];

	if (iSum == 0)
		return Process::CompilationMode::Native;
	else
		return Process::CompilationMode::CLR;
}

std::wstring ProcessHacker::getDepPolicy(Handle& hProcess)
{
	PROCESS_MITIGATION_DEP_POLICY depPolicy;
	if (!GetProcessMitigationPolicy(hProcess(), ProcessDEPPolicy, (LPVOID)&depPolicy, sizeof(depPolicy)))
	{
		throw std::runtime_error("GetProcessMitigationPolicy failed");
	}
	std::wstring depPolicyStr = L"";

	depPolicyStr += depPolicy.Enable ? L"Enabled " : L"Disabled ";
	depPolicyStr += depPolicy.Permanent ? L"Permanent " : L"Not permanent ";
	return depPolicyStr;
}

std::wstring ProcessHacker::getAslrPolicy(Handle& hProcess)
{
	PROCESS_MITIGATION_ASLR_POLICY aslrPolicy;
	if (!GetProcessMitigationPolicy(hProcess(), ProcessASLRPolicy, (LPVOID)&aslrPolicy, sizeof(aslrPolicy)))
	{
		throw std::runtime_error("GetProcessMitigationPolicy failed");
	}
	std::wstring aslrPolicyStr = L"";
	aslrPolicyStr += aslrPolicy.EnableBottomUpRandomization ? L"BottomUpRandomization " : L"";
	aslrPolicyStr += aslrPolicy.EnableForceRelocateImages ? L"ForceRelocateImages " : L"";
	aslrPolicyStr += aslrPolicy.EnableHighEntropy ? L"HighEntropy " : L"";
	aslrPolicyStr += aslrPolicy.DisallowStrippedImages ? L"DisallowStrippedImages " : L"";
	return aslrPolicyStr;
}

std::vector<std::wstring> ProcessHacker::getProcessDlls(Handle& hProcess)
{
	HMODULE hMods[1024];
	DWORD cbNeeded;
	std::vector<std::wstring> dlls;

	if (EnumProcessModules(hProcess(), hMods, sizeof(hMods), &cbNeeded))
	{
		auto modulesCount = cbNeeded / sizeof(HMODULE);
		dlls.reserve(modulesCount);
		for (DWORD i = 1; i < modulesCount; i++) //first module is .exe, so we skip it
		{
			wchar_t szModName[MAX_PATH];

			// Get the full path to the module's file.
			if (GetModuleFileNameEx(hProcess(), hMods[i], szModName,
				sizeof(szModName) / sizeof(TCHAR)))
			{
				// Print the module name and handle value.
				dlls.push_back(szModName);
			}
		}
	}
	return dlls;
}