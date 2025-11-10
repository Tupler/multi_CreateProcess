#include <windows.h>
#include <bits.h>
#include <stdio.h>

#pragma comment(lib, "bits.lib")

int main()
{
    HRESULT hr;
    IBackgroundCopyManager* pManager = NULL;
    IBackgroundCopyJob* pJob = NULL;
    IBackgroundCopyJob2* pJob2 = NULL;
    GUID jobId;

    // 初始化 COM
    hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
    if (FAILED(hr))
    {
        printf("CoInitializeEx failed: 0x%08lx\n", hr);
        return 1;
    }

    // 获取 BITS 管理器
    hr = CoCreateInstance(__uuidof(BackgroundCopyManager),
        NULL,
        CLSCTX_LOCAL_SERVER,
        __uuidof(IBackgroundCopyManager),
        (void**)&pManager);
    if (FAILED(hr))
    {
        printf("CoCreateInstance failed: 0x%08lx\n", hr);
        CoUninitialize();
        return 1;
    }

    // 创建任务
    hr = pManager->CreateJob(L"TestJob",
        BG_JOB_TYPE_DOWNLOAD,
        &jobId,
        &pJob);
    if (FAILED(hr))
    {
        printf("CreateJob failed: 0x%08lx\n", hr);
        pManager->Release();
        CoUninitialize();
        return 1;
    }

    // 获取 IBackgroundCopyJob2 接口
    hr = pJob->QueryInterface(__uuidof(IBackgroundCopyJob2), (void**)&pJob2);
    if (FAILED(hr))
    {
        printf("QueryInterface failed: 0x%08lx\n", hr);
        pJob->Release();
        pManager->Release();
        CoUninitialize();
        return 1;
    }

    // 设置通知命令行
    hr = pJob2->SetNotifyCmdLine(
        L"file://C:/Windows/System32/calc.exe",  // 程序路径
        L""                         // 参数
    );
    if (FAILED(hr))
    {
        printf("SetNotifyCmdLine failed: 0x%08lx\n", hr);
    }
    else
    {
        printf("NotifyCmdLine set successfully.\n");
    }

    // 设置通知类型：完成时触发
    pJob->SetNotifyFlags(BG_NOTIFY_JOB_TRANSFERRED);

    // 提交任务
    hr = pJob->Resume();
    if (FAILED(hr))
    {
        printf("Resume failed: 0x%08lx\n", hr);
    }

    printf("Job configured. It will run the command line when done.\n");

    // 清理
    pJob2->Release();
    pJob->Release();
    pManager->Release();
    CoUninitialize();

    return 0;
}
