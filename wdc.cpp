
#include <windows.h>
#include <stdio.h>

typedef HRESULT(WINAPI* PFN_WdcRunTaskAsInteractiveUser_I)(
    PCWSTR CommandLine,
    PCWSTR CurrentDirectory,
    ULONG Flags
    );

int main(void)
{
    CoInitializeEx(NULL, COINIT_MULTITHREADED);
    HMODULE hWdc = LoadLibraryW(L"wdc.dll");
    if (!hWdc)
    {
        printf("无法加载 wdc.dll, 错误: %lu\n", GetLastError());
        return 1;
    }

    PFN_WdcRunTaskAsInteractiveUser_I pRunAsInteractiveUser =
        (PFN_WdcRunTaskAsInteractiveUser_I)GetProcAddress(hWdc, "WdcRunTaskAsInteractiveUser");

    if (!pRunAsInteractiveUser)
    {
        printf("未找到函数 WdcRunTaskAsInteractiveUser, 错误: %lu\n", GetLastError());
        FreeLibrary(hWdc);
        return 1;
    }

    HRESULT hr = pRunAsInteractiveUser(
        L"C:\\Windows\\System32\\cmd.exe",          // 命令行
        L"C:\\Windows\\System32",// 工作目录
        0                       // Flags，一般为0
    );

    if (SUCCEEDED(hr))
        printf("进程启动成功！\n");
    else
        printf("启动失败, HRESULT=0x%08X\n", hr);

    FreeLibrary(hWdc);
    return 0;
}