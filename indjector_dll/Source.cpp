#include <Windows.h>
#include <TlHelp32.h>
#include <iostream>
#include <io.h>
#include <shlwapi.h>
#define _alloc(size) HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, size + 64)
#define _free(mem) if (mem) HeapFree(GetProcessHeap(), 0, mem)
DWORD GetProcessByName(const char* process_name)
{
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    PROCESSENTRY32 process;
    DWORD proc_id = 0;
    if (Process32First(snapshot, &process))
    {
        while (Process32Next(snapshot, &process))
        {
            if (_stricmp(process.szExeFile, process_name) == 0)
            {
                proc_id = process.th32ProcessID;
                break;
            }
        }
    }
    CloseHandle(snapshot);
    return proc_id;
}

bool FileExist(char* name)
{
    return _access(name, 0) != -1;
}

bool Inject(DWORD pID, char* path)
{
    HANDLE proc_handle;
    LPVOID RemoteString;
    LPCVOID LoadLibAddy;

    if (pID == 0)
        return false;

    proc_handle = OpenProcess(PROCESS_ALL_ACCESS, false, pID);
    if (proc_handle == 0)
        return false;

    

    RemoteString = VirtualAllocEx(proc_handle, NULL, strlen(path) + 1, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    WriteProcessMemory(proc_handle, RemoteString, path, strlen(path) + 1, NULL);

    LoadLibAddy = GetProcAddress(GetModuleHandle("kernel32.dll"), "LoadLibraryA");
    HANDLE az = CreateRemoteThread(proc_handle, NULL, NULL, (LPTHREAD_START_ROUTINE)LoadLibAddy, RemoteString, NULL, NULL);
    if (az == NULL) {
        std::cout << "[-] Could create new thread. Error code: " << GetLastError();
        return -1;
    }

    CloseHandle(proc_handle);
    return true;
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
   

    char process_name[32];
    char dll_name[32] = {"exloder.dll"};
    char path[256];
    DWORD pID;
    
    pID = GetProcessByName("Telegram.exe");
    if (pID == 0)
    {
        pID = GetProcessByName("Discord.exe");
        if (pID == 0)
        {
            exit(1);
        }
        
    }
     
    
  
    printf("%s found (pid = %X)!\n", process_name, pID);
    while (FileExist(path) == false)
    {
        //printf("Enter DLL name: ");
        //scanf_s("%s", dll_name);
        
        GetFullPathName(dll_name, sizeof(path), path, NULL);
        if (FileExist(path))
        {
            printf("DLL found!\n");
            break;
        }
        else
        {

            printf("DLL not found!\n");
            break;
        }
    }
    printf("Preparing DLL for injection...\n");
    if (Inject(pID, path))
    {
        printf("DLL successfully injected!\n");
        MessageBox(NULL, "инжект успешен", NULL, MB_ICONERROR);
        exit(1);
    }
    else
    {
        printf("CRITICAL ERROR! \nDestroying window...\n");
        exit(1);
        
    }
}
