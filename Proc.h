#include <iostream>
#include <windows.h>
#include <psapi.h>
#include <tchar.h>
#include <tlhelp32.h>
#include <sddl.h>

struct Proc_info
{
	TCHAR Name[MAX_PATH];
	TCHAR Path[2048];
	DWORD PID;
	DWORD PPID;
	TCHAR Type[11];

};

struct uOwner
{
	TCHAR Name[MAX_PATH];
	TCHAR Domain[MAX_PATH];
	TCHAR sSID[512];
};

class Processes
{
public:
	void GetProcList();
	void GetProcAllInfo(DWORD PID, Proc_info *Proc, uOwner *owner);
private:
	BOOL IsWow64(HANDLE process);
	bool IsX86Process(HANDLE process);
	int GetProcType(DWORD PID, Proc_info *proc);

	int GetPPID(DWORD PID, DWORD *PPID);
	int GetProcOwner(DWORD PID, uOwner *owner);

	int GetProcessName(DWORD PID, Proc_info *proc);
	int GetProcessPath(DWORD PID, Proc_info *proc);
	int GetAllProcPID(DWORD *procs_pid, DWORD size, DWORD *count);

	void error_msg(const char* msg);
};