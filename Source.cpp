
#include "Proc.h"

int main()
{
	Processes Processes;

	Processes.GetProcList();
	std::cout << std::endl;

	while (1)
	{
		DWORD PID;
		Proc_info Proc, PProc;
		uOwner Owner, pOwner;

		std::cin >> PID;
		Processes.GetProcAllInfo(PID, &Proc, &Owner);

		Processes.GetProcAllInfo(Proc.PPID, &PProc, &pOwner);


		std::cout << Proc.Name << ":" << "\n\tPID - " << Proc.PID << "\n\tPath - " << Proc.Path << "\n\tPPID - " << PProc.PID  << "\n\tPName - " << PProc.Name << "\n\tUser owner - " << Owner.Name << "\n\tOwner SID - " << Owner.sSID << "\n\tType - " << Proc.Type << std::endl;


	}
	system("pause");
	return 0;
}