#include <Windows.h>
#include <TlHelp32.h>
#include <iostream>
#include <string>

DWORD FindProcessId(const std::wstring& processName) {
    PROCESSENTRY32W entry = { sizeof(PROCESSENTRY32W) };
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    DWORD pid = 0;

    if (Process32FirstW(snapshot, &entry)) {
        do {
            if (processName == entry.szExeFile) {
                pid = entry.th32ProcessID;
                break;
            }
        } while (Process32NextW(snapshot, &entry));
    }

    CloseHandle(snapshot);
    return pid;
}

bool InjectDLL(DWORD pid, const std::wstring& dllPath) {
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
    if (!hProcess) return false;

    void* remoteMem = VirtualAllocEx(hProcess, nullptr, dllPath.size() * sizeof(wchar_t), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    if (!remoteMem) return false;

    WriteProcessMemory(hProcess, remoteMem, dllPath.c_str(), dllPath.size() * sizeof(wchar_t), nullptr);

    HMODULE hKernel32 = GetModuleHandleW(L"kernel32.dll");
    FARPROC loadLib = GetProcAddress(hKernel32, "LoadLibraryW");

    HANDLE thread = CreateRemoteThread(hProcess, nullptr, 0,
        (LPTHREAD_START_ROUTINE)loadLib, remoteMem, 0, nullptr);

    WaitForSingleObject(thread, INFINITE);
    VirtualFreeEx(hProcess, remoteMem, 0, MEM_RELEASE);
    CloseHandle(thread);
    CloseHandle(hProcess);
    return true;
}

int wmain(int argc, wchar_t* argv[]) {
    if (argc != 3) {
        std::wcout << L"Usage: Injector.exe <ProcessName.exe> <FullPathToDLL>\n";
        return 1;
    }

    std::wstring processName = argv[1];
    std::wstring dllPath = argv[2];

    DWORD pid = FindProcessId(processName);
    if (pid == 0) {
        std::wcout << L"Process not found.\n";
        return 1;
    }

    if (InjectDLL(pid, dllPath)) {
        std::wcout << L"Injected successfully.\n";
    } else {
        std::wcout << L"Injection failed.\n";
    }

    return 0;
}
