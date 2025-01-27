#include "process.h"

DWORD GetProcessId(const wchar_t* processName) {

	int procId = 0;

	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (hSnap != INVALID_HANDLE_VALUE) 
	{
		PROCESSENTRY32 procEntry;
		procEntry.dwSize = sizeof(procEntry);

		if (Process32First(hSnap, &procEntry))
		{
			do 
			{
				if (!_wcsicmp(processName, procEntry.szExeFile))
				{
					procId = procEntry.th32ProcessID;
					break;
				}

			}
			while (Process32Next(hSnap, &procEntry));
		}



	}
	CloseHandle(hSnap);

	return procId;
}

uintptr_t GetModuleBaseAddress(DWORD processId, const wchar_t* moduleName) {

	uintptr_t moduleBase = 0;

	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, processId);

	if (hSnap != INVALID_HANDLE_VALUE)
	{
		MODULEENTRY32 modEntry;
		modEntry.dwSize = sizeof(modEntry);

		if (Module32First(hSnap, &modEntry))
		{
			do
			{
				if (!_wcsicmp(moduleName, modEntry.szModule))
				{
					moduleBase = (uintptr_t)modEntry.modBaseAddr;
					break;
				}

			
			} while (Module32Next(hSnap, &modEntry));
		}
	}
	CloseHandle(hSnap);
	
	return moduleBase;

}

uintptr_t FinDMAAddy(HANDLE hProc, uintptr_t localPtr, std::vector<unsigned int>offsets) {

	uintptr_t addr = localPtr;

	for (unsigned int i = 0; i < offsets.size(); ++i) 
	{
		ReadProcessMemory(hProc, (BYTE*) addr, &addr, sizeof(addr), 0);
		addr += offsets[i];
	}
	
	return addr;
}

