#include <Windows.h>
#include <TlHelp32.h>
#include <iostream>
#include <io.h>
#include <shlwapi.h>

DWORD GetProcessByName(const WCHAR* process_name)
{
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    PROCESSENTRY32 process;
    DWORD proc_id = 0;
    if (Process32FirstW(snapshot, &process))
    {
        while (Process32NextW(snapshot, &process))
        {
            if (_wcsicmp(process.szExeFile, process_name) == 0)
            {
                proc_id = process.th32ProcessID;
                break;
            }
        }
    }
    CloseHandle(snapshot);
    return proc_id;
}

bool FileExist(WCHAR* name)
{
    return _waccess(name, 0) != -1;
}

bool Inject(DWORD pID, WCHAR* path)
{
    HANDLE proc_handle;
    LPVOID RemoteString;
    LPCVOID LoadLibAddy;

    if (pID == 0)
        
        return false;

    proc_handle = OpenProcess(PROCESS_ALL_ACCESS, false, pID);
    if (proc_handle == 0)
        
        return false;
   
    

    RemoteString = VirtualAllocEx(proc_handle, NULL, wcslen(path) + 1, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    WriteProcessMemory(proc_handle, RemoteString, path, wcslen(path) + 1, NULL);

    LoadLibAddy = GetProcAddress(GetModuleHandleW(L"kernel32.dll"), "LoadLibraryA");
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
   

    WCHAR process_name[256];
    WCHAR dll_name[256] = {L"dwmapi.dll"};
    WCHAR path[256];
    DWORD pID = 7268;
    
    /*pID = GetProcessByName(L"Telegram.exe");
    if (pID == 0)
    {
        pID = GetProcessByName(L"Discord.exe");
        if (pID == 0)
        {
            MessageBox(NULL, L"lox", NULL, MB_ICONERROR);
        }
        
    }*/
     
    
  
    printf("%s found (pid = %X)!\n", process_name, pID);
    while (FileExist(path) == false)
    {
        //printf("Enter DLL name: ");
        //scanf_s("%s", dll_name);
        
        GetFullPathNameW(dll_name, sizeof(path), path, NULL);
        if (FileExist(path))
        {
            printf("DLL found!\n");
            
            break;
        }
        else
        {

            printf("DLL not found!\n");
            MessageBox(NULL, L"ошибка", NULL, MB_ICONERROR);
            break;
        }
    }
    printf("Preparing DLL for injection...\n");
    if (Inject(pID, path))
    {
        printf("DLL successfully injected!\n");
        MessageBox(NULL, L"инжект успешен", NULL, MB_ICONERROR);
        
    }
    else
    {
        printf("CRITICAL ERROR! \nDestroying window...\n");
        MessageBox(NULL, L"ошибка111", NULL, MB_ICONERROR);
        
    }
   
}
