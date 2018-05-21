#include "DCurlHttp.h"
#include "DCurlRest.h"
#include "CTaskTool.h"

int main()
{
    //初始化任务池
    bool bRet = CTaskTool::GetInstance().Init();
    if (false == bRet)
    {
        std::cout << "init task pool failed, ret:" << bRet << std::endl;
        return 1;
    }
    
    DCurlRest *curlRest = new DCurlHttp();
    if(NULL == curlRest)
    {
        std::cout << "new DCurlHttp failed." << std::endl;
        return 1;
    }
    
    //多线程环境下，需调用curl_global_init
    int ret = curl_global_init(CURL_GLOBAL_ALL);
    if (0 != ret)
    {
        std::cout << "curl_global_init failed." << std::endl;
        return 1;
    }
    
    //下载
    std::string url("http://ardownload.adobe.com/pub/adobe/reader/win/11.x/11.0.01/en_US/AdbeRdr11001_en_US.exe");
    curlRest->Download(url, "AdbeRdr11001_en_US.exe");
    
    return 0;
}
