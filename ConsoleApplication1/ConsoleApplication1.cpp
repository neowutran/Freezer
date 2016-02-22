// ConsoleApplication1.cpp : définit le point d'entrée pour l'application console.
//

#include "stdafx.h"

#include <windows.h>
#include <tlhelp32.h>
#include <tchar.h>
#include <thread>
#include <chrono>
#include <comdef.h>

DWORD FindProcessId(const char *processname)
{
	HANDLE hProcessSnap;
	PROCESSENTRY32 pe32;
	DWORD result = NULL;

	// Take a snapshot of all processes in the system.
	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (INVALID_HANDLE_VALUE == hProcessSnap) return(FALSE);

	pe32.dwSize = sizeof(PROCESSENTRY32); // <----- IMPORTANT

										  // Retrieve information about the first process,
										  // and exit if unsuccessful
	if (!Process32First(hProcessSnap, &pe32))
	{
		CloseHandle(hProcessSnap);          // clean the snapshot object
		printf("!!! Failed to gather information on system processes! \n");
		return(NULL);
	}

	do
	{
		_bstr_t b(pe32.szExeFile);
		const char* c = b;

		if (0 == strcmp(processname, c))
		{
			result = pe32.th32ProcessID;
			break;
		}
	} while (Process32Next(hProcessSnap, &pe32));

	CloseHandle(hProcessSnap);

	return result;
}

void suspend_type1(DWORD processId)
{
	HANDLE hThreadSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);

	THREADENTRY32 threadEntry;
	threadEntry.dwSize = sizeof(THREADENTRY32);

	Thread32First(hThreadSnapshot, &threadEntry);

	do
	{
		if (threadEntry.th32OwnerProcessID == processId)
		{
			HANDLE hThread = OpenThread(THREAD_ALL_ACCESS, FALSE,
				threadEntry.th32ThreadID);

			SuspendThread(hThread);
			CloseHandle(hThread);
		}
	} while (Thread32Next(hThreadSnapshot, &threadEntry));

	CloseHandle(hThreadSnapshot);
}


typedef LONG(NTAPI *NtSuspendProcess)(IN HANDLE ProcessHandle);

void suspend_type2(DWORD processId)
{
	HANDLE processHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);
	NtSuspendProcess pfnNtSuspendProcess = (NtSuspendProcess)GetProcAddress(
		GetModuleHandle(L"ntdll"), "NtSuspendProcess");

	pfnNtSuspendProcess(processHandle);
	CloseHandle(processHandle);
}

void suspend_type3(DWORD processId)
{
	DebugActiveProcess(processId);
}


bool xigncode = false;
int main()
{
	//const char* processname = "xm.exe";
	const char* processname2 = "TERA.exe";
	const char* processname4 = "xxd-0.xem";
	//const char* processname3 = "x3.xem";
	//const char* processname5 = "Tera.exe";
	//const char* processname6 = "xnina.xem";
	//const char* processname7 = "xmaf.xem";


	while (!xigncode) {
		/*
		DWORD processid = FindProcessId(processname);
		if (processid != NULL) {
			suspend_type1(processid);
			suspend_type2(processid);
			suspend_type3(processid);
		}
		*/
		DWORD processid = FindProcessId(processname2);
		if (processid != NULL) {
			suspend_type1(processid);
			suspend_type2(processid);
			suspend_type3(processid);
		}
		/*
		processid = FindProcessId(processname3);
		if (processid != NULL) {
			suspend_type1(processid);
			suspend_type2(processid);
			suspend_type3(processid);
		}*/
		processid = FindProcessId(processname4);
		if (processid != NULL) {
			suspend_type1(processid);
			suspend_type2(processid);
			suspend_type3(processid);
		}/*
		processid = FindProcessId(processname5);
		if (processid != NULL) {
			suspend_type1(processid);
			suspend_type2(processid);
			suspend_type3(processid);
		}
		processid = FindProcessId(processname6);
		if (processid != NULL) {
			suspend_type1(processid);
			suspend_type2(processid);
			suspend_type3(processid);
		}
		processid = FindProcessId(processname7);
		if (processid != NULL) {
			suspend_type1(processid);
			suspend_type2(processid);
			suspend_type3(processid);
		}
		*/
		
	}
    return 0;
}

