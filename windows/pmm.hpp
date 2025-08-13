#include "Windows.h"
#include <cstdint>

class ProcessMemoryManager
{
private:
	/* data */
public:
	ProcessMemoryManager(/* args */)
	{
	}
	~ProcessMemoryManager()
	{
	}

	bool Read(DWORD processId, int addr, uint8_t &value)
	{
		bool success = false;
		HANDLE hProcess = OpenProcess(PROCESS_VM_READ, FALSE, processId);
		if (hProcess == NULL)
		{
			return false;
		}
		else
		{
			SIZE_T bytesRead = 0;
			success = ReadProcessMemory(hProcess, (LPCVOID)addr, &value, sizeof(value), &bytesRead);
		}
		CloseHandle(hProcess);
		return success;
	}

	bool Read(DWORD processId, int addr, uint16_t &value)
	{
		bool success = false;
		HANDLE hProcess = OpenProcess(PROCESS_VM_READ, FALSE, processId);
		if (hProcess == NULL)
		{
			return false;
		}
		else
		{
			SIZE_T bytesRead = 0;
			success = ReadProcessMemory(hProcess, (LPCVOID)addr, &value, sizeof(value), &bytesRead);
		}
		CloseHandle(hProcess);
		return success;
	}

	bool Read(DWORD processId, int addr, uint32_t &value)
	{
		bool success = false;
		HANDLE hProcess = OpenProcess(PROCESS_VM_READ, FALSE, processId);
		if (hProcess == NULL)
		{
			return false;
		}
		else
		{
			SIZE_T bytesRead = 0;
			success = ReadProcessMemory(hProcess, (LPCVOID)addr, &value, sizeof(value), &bytesRead);
		}
		CloseHandle(hProcess);
		return success;
	}

	bool Read(DWORD processId, int addr, uint64_t &value)
	{
		bool success = false;
		HANDLE hProcess = OpenProcess(PROCESS_VM_READ, FALSE, processId);
		if (hProcess == NULL)
		{
			return false;
		}
		else
		{
			SIZE_T bytesRead = 0;
			success = ReadProcessMemory(hProcess, (LPCVOID)addr, &value, sizeof(value), &bytesRead);
		}
		CloseHandle(hProcess);
		return success;
	}
};
