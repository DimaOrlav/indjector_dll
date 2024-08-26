//#include <iostream>
//#include <windows.h>
//#include <tlhelp32.h>
//
//int main()
//{
//    const WCHAR* processName = L"mspaint.exe";
//    const char* dllLocation = "C:\\Users\\Administrator\\source\repos\\proect_dll_cheack\\Release\\proect_dll_cheack.dll";
//    size_t dllLocationSize = strlen(dllLocation);
//
//    HANDLE processSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
//
//    if (processSnap == INVALID_HANDLE_VALUE) {
//        std::cout << "[-] Could not retrieve process snapshot. Error code: " << GetLastError();
//        return -1;
//    }
//    std::cout << "[+] Process snapshot retrieved successfully." << std::endl;
//
//    PROCESSENTRY32 processEntry;
//    processEntry.dwSize = sizeof(PROCESSENTRY32);
//
//    if (!Process32First(processSnap, &processEntry)) {
//        std::cout << "[-] Could not iterate through process snapshot. Error code: " << GetLastError();
//        return -1;
//    }
//
//    do {
//        if (!wcscmp(processEntry.szExeFile, processName)) {
//            std::wcout << "[+] Target process found: " << processEntry.szExeFile << " (" << processEntry.th32ProcessID << ")" << std::endl;
//            break;
//        }
//    } while (Process32Next(processSnap, &processEntry));
//
//    CloseHandle(processSnap);
//
//    if (wcscmp(processEntry.szExeFile, processName)) {
//        std::cout << "[-] Target process not found.";
//        return -1;
//    }
//
//    HANDLE targetProcess = OpenProcess(PROCESS_ALL_ACCESS, TRUE, processEntry.th32ProcessID);
//    if (targetProcess == NULL) {
//        std::cout << "[-] Could not get a process handle. Error code: " << GetLastError();
//        return -1;
//    }
//    std::cout << "[+] Process handle retrieved successfully." << std::endl;
//
//    LPVOID dllPath = VirtualAllocEx(targetProcess, NULL, dllLocationSize, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
//    if (dllPath == NULL) {
//        std::cout << "[-] Could not allocate memory. Error code: " << GetLastError();
//        return -1;
//    }
//    std::cout << "[+] Menory allocated successfully." << std::endl;
//
//    if (!WriteProcessMemory(targetProcess, dllPath, dllLocation, dllLocationSize, NULL)) {
//        std::cout << "[-] Could not inject dll. Error code: " << GetLastError();
//        return -1;
//    }
//
//    std::cout << "[+] DLL injected successfully." << std::endl;
//
//    LPTHREAD_START_ROUTINE loadLibraryAddress = (LPTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandle(L"kernel32.dll"), "LoadLibraryA");
//    if (!loadLibraryAddress) {
//        std::cout << "[-] Could not get the address of LoadLibrary function.";
//    }
//    HANDLE remoteThread = CreateRemoteThread(targetProcess, NULL, 0, loadLibraryAddress, dllPath, 0, NULL);
//    if (remoteThread == NULL) {
//        std::cout << "[-] Could create new thread. Error code: " << GetLastError();
//        return -1;
//    }
//
//    /*
//    * We could implement DLL injection through NtCreateThreadEx or RtlCreateUserThread.
//    */
//
//    std::cout << "[+] New thread created successfully";
//
//}