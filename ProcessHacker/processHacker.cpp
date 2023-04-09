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

// deleter for unique_ptr, constructed from malloc - allocated pointers
void FreeDeleter::operator()(void* x) { free(x); }

//deleter for unique_ptr <LPVOID> (security descriptors)

void LocalFreeDeleter::operator()(LPVOID* x) { LocalFree(x); }


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

std::unordered_map<DWORD, std::shared_ptr<Process>> ProcessHacker::getProcessMap()
{
	return this->processMap;
}

//functions for integrity level interaction (overloaded for processes and files)
//integrity level is being extracted from SACL
decltype(SECURITY_MANDATORY_MEDIUM_RID) ProcessHacker::getIntegrityLevel(const std::wstring& filename)
{
	return getIntegrityLevelImpl(filename);
}

decltype(SECURITY_MANDATORY_MEDIUM_RID) ProcessHacker::getIntegrityLevel(const DWORD PID)
{
	auto exePath = processMap.at(PID)->exePath;
	if (exePath == L"N/A")
		throw std::invalid_argument("Process is not available");

	return getIntegrityLevelImpl(exePath);
}

void ProcessHacker::setIntegrityLevel(const decltype(SECURITY_MANDATORY_MEDIUM_RID) integrityLevel, const std::wstring& filename)
{
	return setIntegrityLevelImpl(integrityLevel, filename);
}

void ProcessHacker::setIntegrityLevel(const decltype(SECURITY_MANDATORY_MEDIUM_RID) integrityLevel, const DWORD PID)
{
	auto exePath = processMap.at(PID)->exePath;
	if (exePath == L"N/A")
		throw std::invalid_argument("Process is not available");

	return setIntegrityLevelImpl(integrityLevel, processMap.at(PID)->exePath);
}

//functions for process priveleges interaction
//values stored in vector are pairs: PRIVELEGE and ALLOWED/DISALLOWED
//process privileges are being extracted from DACL
std::vector<std::pair<std::wstring, std::wstring>> ProcessHacker::getProcessPrivileges(const DWORD PID)
{
	Handle hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, PID); //handle with access right
	if (!isHandleAvailable(hProcess))
		throw std::invalid_argument("Process is not available");

	HANDLE hTokenTemp;
	auto success = OpenProcessToken(hProcess(), TOKEN_QUERY, &hTokenTemp);//token handle with access right
	Handle hToken = hTokenTemp;
	if (!success)
		throw std::runtime_error("OpenProcessToken failed");


	DWORD tpSize;
	if (!GetTokenInformation(hToken(), TokenPrivileges, NULL, 0, &tpSize))//get required structure size
		if (GetLastError() != ERROR_INSUFFICIENT_BUFFER)
			throw std::runtime_error("GetTokenInformation failed");

	PTOKEN_PRIVILEGES pTpTemp = (PTOKEN_PRIVILEGES)malloc(tpSize);
	std::unique_ptr<TOKEN_PRIVILEGES, FreeDeleter> pTp(pTpTemp); //allocate Size bytes and make unique_ptr for RAII

	if (!GetTokenInformation(hToken(), TokenPrivileges, pTp.get(), tpSize, &tpSize))
		throw std::runtime_error("GetTokenInformation failed");

	std::vector<std::pair<std::wstring, std::wstring>> processPrivileges;
	processPrivileges.reserve(pTp->PrivilegeCount);

	//extract privilege name from ACE Luid
	for (DWORD i = 0; i < pTp->PrivilegeCount; i++)
	{
		auto privilege = pTp->Privileges[i];

		DWORD nameSize = 0;
		//get privilege name size
		if (!LookupPrivilegeName(NULL, &privilege.Luid, NULL, &nameSize))
			if (GetLastError() != ERROR_INSUFFICIENT_BUFFER)
				throw std::runtime_error("LookupPrivilegeName failed");
		//construct wstring with needed size
		std::wstring privilegeName(nameSize, '\0');

		if (!LookupPrivilegeName(NULL, &privilege.Luid, &privilegeName[0], &nameSize))
			throw std::runtime_error("LookupPrivilegeName failed");

		std::wstring privilegeValue = pTp->Privileges[i].Attributes == SE_PRIVILEGE_ENABLED ? L"Enabled" : L"Disabled";
		processPrivileges.push_back(std::make_pair(privilegeName, privilegeValue));
	}
	return processPrivileges;
}
void ProcessHacker::setProcessPrivilege(const std::wstring& privilegeName, const bool enablePrivelege, const DWORD PID)
{
	Handle hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, PID); //handle with access right
	if (!isHandleAvailable(hProcess))
		throw std::invalid_argument("Process is not available");

	HANDLE hTokenTemp;
	auto success = OpenProcessToken(hProcess(), TOKEN_ADJUST_PRIVILEGES, &hTokenTemp);//token handle with access right
	Handle hToken = hTokenTemp;
	if (!success)
		throw std::runtime_error("OpenProcessToken failed");

	setPrivelege(hToken, &privilegeName[0], enablePrivelege);//use helper function
}

//functions for file owner interaction
//file owner is being extracted from SID
std::wstring ProcessHacker::getFileOwner(const std::wstring& filename)
{
	PSECURITY_DESCRIPTOR pSDtemp = nullptr;
	PSID pSidOwner = nullptr;
	//get security SID security descriptor
	auto error = GetNamedSecurityInfo(&filename[0], SE_FILE_OBJECT, OWNER_SECURITY_INFORMATION,
		&pSidOwner, 0, 0, 0, &pSDtemp);
	//make unique_ptr for RAII
	auto kek = pSDtemp;
	std::unique_ptr<PSECURITY_DESCRIPTOR, LocalFreeDeleter> pSD(&pSDtemp);

	if (error != ERROR_SUCCESS)
	{
		if (error == ERROR_FILE_NOT_FOUND)
			throw std::invalid_argument("File with such name doesn't exist");
		else
			throw std::runtime_error(std::string("GetNamedSecurityInfo failed with error " + std::to_string(error)));
	}

	return getSidOwnerName(pSidOwner);//use helper function
}

//functions for file DACL interaction
std::vector<std::tuple<std::wstring, std::wstring, std::wstring>> ProcessHacker::getFileAccessRights(const std::wstring& filename)
{
	PSECURITY_DESCRIPTOR pSDtemp = nullptr;
	PACL pDacl = nullptr;
	//get DACL security descriptor
	auto error = GetNamedSecurityInfo(&filename[0], SE_FILE_OBJECT, DACL_SECURITY_INFORMATION,
		0, 0, &pDacl, 0, &pSDtemp);

	std::unique_ptr<PSECURITY_DESCRIPTOR, LocalFreeDeleter> pSD(&pSDtemp);//make unique ptr for RAII

	if (error != ERROR_SUCCESS)
	{
		if (error == ERROR_FILE_NOT_FOUND)
			throw std::invalid_argument("File with such name doesn't exist");
		else
			throw std::runtime_error(std::string("GetNamedSecurityInfo failed with error " + std::to_string(error)));
	}

	std::vector<std::tuple<std::wstring, std::wstring, std::wstring>> accessRights;
	//{subject of accessRight, accessRight, accessMode}

	if (pDacl != nullptr)
	{
		//extract all ACEs from DACL
		for (int i = 0; i < pDacl->AceCount; ++i)
		{
			ACCESS_ALLOWED_ACE* pAce = 0;
			if (!GetAce(pDacl, i, (LPVOID*)&pAce))
				throw std::runtime_error("GetAce failed");

			PSID pSid = &pAce->SidStart;
			//construct three string to represent info
			std::wstring accessRightSubject = getSidOwnerName(pSid);
			std::wstring accessMode = pAce->Header.AceType == ACCESS_ALLOWED_ACE_TYPE ? L"Allowed" : L"Disallowed";
			std::wstring accessRight = L"";
			auto mask = pAce->Mask;
			//get all access right from mask
			if ((mask & STANDARD_RIGHTS_ALL) == STANDARD_RIGHTS_ALL)
				accessRight += L"STANDARD_RIGHTS_ALL ";
			else
			{
				if ((mask & FILE_GENERIC_EXECUTE) == FILE_GENERIC_EXECUTE)
					accessRight += L"FILE_GENERIC_EXECUTE ";
				if ((mask & FILE_GENERIC_READ) == FILE_GENERIC_READ)
					accessRight += L"FILE_GENERIC_READ ";
				if ((mask & FILE_GENERIC_WRITE) == FILE_GENERIC_WRITE)
					accessRight += L"FILE_GENERIC_WRITE ";

				if (accessRight == L"")//there are lots of other rights, may be implemented later
					accessRight = L"Unknown access right";
			}
			accessRights.push_back(std::make_tuple(accessRightSubject, accessRight, accessMode));
		}
	}

	return accessRights;
}
//access right needs to be VALID mask, currently function sets right for current user only
void ProcessHacker::setFileAccessRight(const decltype(STANDARD_RIGHTS_ALL) accessRight, const bool accessMode, const std::wstring& filename)
{
	PSECURITY_DESCRIPTOR pSDtemp = nullptr;
	PACL pOldDacl = nullptr, pNewDacl = nullptr;
	//get old DACL security descriptor
	auto error = GetNamedSecurityInfo(&filename[0], SE_FILE_OBJECT, DACL_SECURITY_INFORMATION,
		0, 0, &pOldDacl, 0, &pSDtemp);
	//make unique_ptr for RAII
	std::unique_ptr<PSECURITY_DESCRIPTOR, LocalFreeDeleter> pSD(&pSDtemp);

	if (error != ERROR_SUCCESS)
	{
		if (error == ERROR_FILE_NOT_FOUND)
			throw std::invalid_argument("File with such name doesn't exist");
		else
			throw std::runtime_error(std::string("GetNamedSecurityInfo failed with error " + std::to_string(error)));
	}

	//get current user SID
	Handle hToken = GetCurrentProcessToken();
	DWORD tuSize = 0;
	//get size needed
	if (!GetTokenInformation(hToken(), TokenUser, NULL, 0, &tuSize))
		if (GetLastError() != ERROR_INSUFFICIENT_BUFFER)
			throw std::runtime_error("GetTokenInformation failed");
	//allocate size bytes
	PTOKEN_USER pTuTemp = (PTOKEN_USER)malloc(tuSize);
	std::unique_ptr<TOKEN_USER, FreeDeleter> pTu(pTuTemp);//make unique_ptr for RAII
	//set TOKEN_USER structure
	if (!GetTokenInformation(hToken(), TokenUser, pTuTemp, tuSize, &tuSize))
		throw std::runtime_error("GetTokenInformation failed");

	//EXPLICIT_ACCESS structure represents ACE
	EXPLICIT_ACCESS ea;
	ea.grfAccessPermissions = accessRight; //access right mask
	ea.grfAccessMode = accessMode == true ? SET_ACCESS : DENY_ACCESS; //Allow or Disallow
	ea.grfInheritance = NO_INHERITANCE;//let NO_INHERITANCE be default value for less bugs
	ea.Trustee.pMultipleTrustee = NULL; //must be 0
	ea.Trustee.MultipleTrusteeOperation = NO_MULTIPLE_TRUSTEE;//must be 0
	ea.Trustee.TrusteeForm = TRUSTEE_IS_SID;//use SID as trustee id
	ea.Trustee.TrusteeType = TRUSTEE_IS_USER;//use current user SID
	ea.Trustee.ptstrName = (LPWCH)pTu->User.Sid;//pointer to SID

	// Create a new DACL that merges the new ACE into the existing DACL.
	error = SetEntriesInAcl(1, &ea, pOldDacl, &pNewDacl);
	if (ERROR_SUCCESS != error)
		throw std::runtime_error("SetEntriesInAcl failed, error code " + std::to_string(error));

	// Attach the new DACL as the object's DACL.
	error = SetNamedSecurityInfo((LPWSTR)&filename[0], SE_FILE_OBJECT, DACL_SECURITY_INFORMATION,
		NULL, NULL, pNewDacl, NULL);
	if (ERROR_SUCCESS != error)
		throw std::runtime_error("SetNamedSecurityInfo failed, error code " + std::to_string(error));
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

//next functions need VALID handle with access rights to work right, these two functions are created to check handle
bool ProcessHacker::isHandleAvailable(Handle& hProcess)
{
	if (hProcess() == nullptr) //handle is invalid
	{
		auto error = GetLastError();
		if (error == ERROR_ACCESS_DENIED || error == ERROR_INVALID_PARAMETER)
			//ERROR_ACCESS_DENIED = system process, ERROR_INVALID_PARAMETER = idle process (PID = 0) or non-existent process
			return false;
		else
			throw std::runtime_error("Handle was opened with unexpected error");
	}
	else if (isZombieProcess(hProcess))
	{
		return false;
	}

	return true;
}

bool ProcessHacker::isZombieProcess(Handle& hProcess)
{
	//assime hProcess() != nullptr
	DWORD exitCode;
	if (!GetExitCodeProcess(hProcess(), &exitCode))
		throw std::runtime_error("GetExitCodeProcess failed");
	if (exitCode == STILL_ACTIVE)
		return false;
	else return true;
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

std::wstring ProcessHacker::getProcessName(Handle& hProcess)
{
	std::wstring name;

	wchar_t nameTemp[1024];
	DWORD nameTempSize = sizeof(nameTemp) / sizeof(wchar_t);
	if (!GetModuleBaseName(hProcess(), 0, nameTemp, nameTempSize))
	{
		auto kek = GetLastError();
		throw std::runtime_error("GetModuleBaseName failed");
		name = nameTemp;
	}
	return name;
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

Process::ProcessType ProcessHacker::getProcessType(Handle& hProcess)
{
	Process::ProcessType processType;

	if (systemInfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_INTEL) //x86 enviroment
		processType = Process::ProcessType::x86;
	else //either x86 process running in x64 enviroment or x64 process
		processType = isWow64(hProcess) ? Process::ProcessType::x86 : Process::ProcessType::x64;

	return processType;
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

//function to get name by PID, needs handle with access right
std::wstring ProcessHacker::getProcessName(const DWORD PID)
{
	std::wstring name;
	Handle hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, PID); //handle with access right

	if (!isHandleAvailable(hProcess))
	{
		name = L"N/A";
	}
	else
	{
		wchar_t nameTemp[1024];
		DWORD nameTempSize = sizeof(nameTemp) / sizeof(wchar_t);
		if (!GetModuleBaseName(hProcess(), 0, nameTemp, nameTempSize))
		{
			throw std::runtime_error("GetModuleBaseName failed");
			name = nameTemp;
		}
	}
	return name;
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

Process::ProcessType ProcessHacker::getSystemType()
{
	if (systemInfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_INTEL) //x86 enviroment
		return Process::ProcessType::x86;
	else
		return Process::ProcessType::x64; //x64 enviroment
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

std::wstring ProcessHacker::getSidOwnerName(PSID pSid)
{
	DWORD nameSize = 0, domainSize = 0;
	SID_NAME_USE snu;

	if (!LookupAccountSid(NULL, pSid, NULL, &nameSize, NULL, &domainSize, &snu))
		if (GetLastError() != ERROR_INSUFFICIENT_BUFFER)
			throw std::runtime_error("LookupAccountSid failed");

	std::wstring name(nameSize, '\0');
	std::wstring domain(domainSize - 1, '\0');

	if (!LookupAccountSid(NULL, pSid, &name[0], &nameSize, &domain[0], &domainSize, &snu))
		throw std::runtime_error("LookupAccountSid failed");

	return domain + L"/" + name;
}