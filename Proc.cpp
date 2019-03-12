#define _CRT_SECURE_NO_WARNINGS

#include "Proc.h"



void Processes::error_msg(const char* msg)
{
	std::cout << "Error: " << msg << std::endl;
}


BOOL Processes::IsWow64(HANDLE process)
{
	BOOL bIsWow64 = FALSE;

	typedef BOOL(WINAPI *LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);
	LPFN_ISWOW64PROCESS fnIsWow64Process;
	fnIsWow64Process = (LPFN_ISWOW64PROCESS)GetProcAddress(GetModuleHandle(TEXT("kernel32")), "IsWow64Process");

	if (NULL != fnIsWow64Process)
	{
		if (!fnIsWow64Process(process, &bIsWow64))
		{
			//handle error
		}
	}
	return bIsWow64;
}

bool Processes::IsX86Process(HANDLE process)
{
	SYSTEM_INFO systemInfo = { 0 };
	GetNativeSystemInfo(&systemInfo);

	// x86 environment
	if (systemInfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_INTEL)
		return true;

	// Check if the process is an x86 process that is running on x64 environment.
	// IsWow64 returns true if the process is an x86 process
	return IsWow64(process);
}

int Processes::GetProcType(DWORD PID, Proc_info *proc)
{
	proc->PID = PID;
	HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION |
		PROCESS_VM_READ,
		FALSE, proc->PID);
	bool isX86;

	if (NULL != hProcess)
	{
		isX86 = IsX86Process(hProcess);
		if (isX86 == true)
		{
			std::strcpy(proc->Type, "x86");
		}
		else std::strcpy(proc->Type, "x64");
	}
	else
	{
		return -1;
	}


	CloseHandle(hProcess);

	return 0;
}

int Processes::GetProcOwner(DWORD PID, uOwner *owner) //возвращать структуру owner
{

	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS,
		FALSE, PID);

	HANDLE hToken;
	OpenProcessToken(hProcess, TOKEN_ALL_ACCESS, &hToken);
	BYTE buf[512];
	DWORD cb = sizeof(buf);

	if (GetTokenInformation(hToken, TokenUser, buf, cb, &cb) == 0)
	{
		error_msg("LookupAccountSid error!");
		return -1;
	}

	TCHAR szName[MAX_PATH];
	DWORD dwName = MAX_PATH;
	TCHAR szDomain[MAX_PATH];
	DWORD dwDomain = MAX_PATH;
	SID_NAME_USE snu;
	if (LookupAccountSid(NULL, ((TOKEN_USER *)(buf))->User.Sid, szName, &dwName, szDomain, &dwDomain, &snu) == 0)
	{
		return -1;
	}

	
	PSID pSid = ((TOKEN_USER *)(buf))->User.Sid;

	std::strcpy(owner->Name, szName);
	std::strcpy(owner->Domain, szDomain);

	if (pSid != NULL)

	{
		
		LPTSTR sStringSid = NULL;
		if (ConvertSidToStringSid(pSid, &sStringSid))
		{
			
			std::strcpy(owner->sSID, sStringSid);
			LocalFree(sStringSid);
		}
		else
			wprintf(L"ConvertSidToSTringSid failed with error %d\n",
				GetLastError());

		CloseHandle(hToken);
		CloseHandle(hProcess);
		return 0;
	}
	else 
	{
		error_msg("pSID is NULL!");
		return -1;
	}
}

int Processes::GetPPID(DWORD pid, DWORD* PPID)
{
	HANDLE hSnapshot;
	PROCESSENTRY32 pe32;
	DWORD ppid = -1;

	hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	__try {
		if (hSnapshot == INVALID_HANDLE_VALUE) __leave;

		ZeroMemory(&pe32, sizeof(pe32));
		pe32.dwSize = sizeof(pe32);
		if (!Process32First(hSnapshot, &pe32)) __leave;

		do {
			if (pe32.th32ProcessID == pid) {
				ppid = pe32.th32ParentProcessID;
				break;
			}
		} while (Process32Next(hSnapshot, &pe32));

	}
	__finally {
		if (hSnapshot != INVALID_HANDLE_VALUE) CloseHandle(hSnapshot);
	}
	if (ppid == -1)
		return -1;
	*PPID = ppid;
	return 0;
}

int Processes::GetProcessPath(DWORD PID, Proc_info *proc)
{
	proc->PID = PID;

	// Get a handle to the process.

	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS,
		FALSE, proc->PID);

	// Get the process name.

	if (NULL != hProcess)
	{
		HMODULE hMod;
		DWORD cbNeeded;

		if (EnumProcessModules(hProcess, &hMod, sizeof(hMod),
			&cbNeeded))
		{
			if (GetModuleFileNameEx(hProcess, hMod, proc->Path,
				sizeof(proc->Path) / sizeof(TCHAR)) == 0)
			{
				std::cout << "ER:" << GetLastError();
			}
		}
		else
		{
			return -1;
		}
	}
	else
	{
		return -1;
	}

	// Release the handle to the process.

	CloseHandle(hProcess);

	return 0;
}

int Processes::GetProcessName(DWORD PID, Proc_info *proc)
{
	proc->PID = PID;

	// Get a handle to the process.

	HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION |
		PROCESS_VM_READ,
		FALSE, proc->PID);

	// Get the process name.

	if (NULL != hProcess)
	{
		HMODULE hMod;
		DWORD cbNeeded;

		if (EnumProcessModules(hProcess, &hMod, sizeof(hMod),
			&cbNeeded))
		{
			GetModuleBaseName(hProcess, hMod, proc->Name,
				sizeof(proc->Name) / sizeof(TCHAR));
		}
		else
		{
			return -1;
		}
	}
	else
	{
		return -1;
	}

	// Release the handle to the process.

	CloseHandle(hProcess);

	return 0;
} //EnumProcessModules Трабл 32 битки error 299

int Processes::GetAllProcPID(DWORD *procs_pid, DWORD size, DWORD *count)
{
	if (!EnumProcesses(procs_pid, size, count))
	{
		return -1;
	}
	*count /= sizeof(DWORD);
	return 0;
}

void Processes::GetProcList()
{
	DWORD ProcessesPID[2048];
	DWORD CountProc;

	if (GetAllProcPID(ProcessesPID, sizeof(ProcessesPID), &CountProc) != 0)
	{
		error_msg("Can't get processes' PIDs");
	}
	for (DWORD i = 0; i < CountProc; i++)
	{
		Proc_info Proc;
		if (GetProcessName(ProcessesPID[i] , &Proc) != 0)
		{
			std::strcpy(Proc.Name, "<unknown>");
		}
		_tprintf(TEXT("%s  (PID: %u)\n"), Proc.Name, Proc.PID);

	}
	return;

}

void Processes::GetProcAllInfo(DWORD PID, Proc_info *Proc, uOwner *owner)
{
	Proc->PID = PID;

	if (GetProcessName(Proc->PID, Proc) != 0)
	{
		std::strcpy(Proc->Name, "<unknown>");
	}

	if (GetProcessPath(Proc->PID, Proc) != 0)
	{
		std::strcpy(Proc->Path, "<unknown>");
	}


	if (GetPPID(Proc->PID, &(Proc->PPID)) != 0)
	{
		Proc->PPID = 0;
	}

	if (GetProcOwner(Proc->PID, owner) != 0)
	{
		std::strcpy(owner->Name, "<unknown>");
		std::strcpy(owner->sSID, "<unknown>");
	}

	if (GetProcType(Proc->PID, Proc) != 0)
	{
		std::strcpy(Proc->Path, "<unknown>");
	}
}