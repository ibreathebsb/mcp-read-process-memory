#include "Windows.h"
#include <cstdint>
#include <string>
#include <vector>

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

	bool Read(DWORD processId, int addr, std::wstring &value)
	{
		HANDLE hProcess = OpenProcess(PROCESS_VM_READ, FALSE, processId);
		if (hProcess == NULL)
		{
			return false;
		}

		value.clear();
		const size_t chunkSize = 512;
		std::vector<wchar_t> buffer(chunkSize);
		SIZE_T bytesRead = 0;
		bool nullTerminatorFound = false;

		while (!nullTerminatorFound)
		{
			if (!ReadProcessMemory(hProcess, (LPCVOID)addr, buffer.data(), buffer.size() * sizeof(wchar_t), &bytesRead))
			{
				CloseHandle(hProcess);
				return false;
			}

			if (bytesRead == 0)
			{
				break;
			}

			size_t charsRead = bytesRead / sizeof(wchar_t);
			for (size_t i = 0; i < charsRead; ++i)
			{
				if (buffer[i] == L'\0')
				{
					nullTerminatorFound = true;
					value.append(buffer.data(), i);
					break;
				}
			}

			if (!nullTerminatorFound)
			{
				value.append(buffer.data(), charsRead);
				addr += bytesRead;
			}
		}

		CloseHandle(hProcess);
		return true;
	}

	bool Read(DWORD processId, int addr, std::vector<uint8_t> &value)
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
			success = ReadProcessMemory(hProcess, (LPCVOID)addr, value.data(), value.size(), &bytesRead);
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
