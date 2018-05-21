#include "DCurlHttp.h"
#include "CThread.h"

DCurlHttp::DCurlHttp()
{
}

DCurlHttp::~DCurlHttp()
{
}

DCURL_RETURN_E DCurlHttp::DownloadFile(NodeParam* param)
{
    if (NULL == param)
    {
        return DCURL_PARAM_INVALID;
    }

    CURL* curl = curl_easy_init();
    if (NULL == curl)
    {
        std::cout << "init curl handle failed." << std::endl;
        return DCURL_INIT_CURL_HANDLE_FAILED;
    }
    
    //设置参数
    char range[64] = {0};
    snprintf(range, sizeof(range), "%lld-%lld", param->startPos, param->endPos);
    
    curl_easy_setopt(curl, CURLOPT_URL, param->url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeData);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)param);
    curl_easy_setopt(curl, CURLOPT_RANGE, range);
    //curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
    curl_easy_setopt (curl, CURLOPT_NOSIGNAL, 1L);
    
/*    
    if (param->timeout > 0)
    {
        curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, param->timeout);  
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, param->timeout);
    }
*/
    //TODO:设置HTTP头、ca证书、cookie等
    
    CURLcode ret = curl_easy_perform(curl);
    if (CURLE_OK != ret)
    {
        std::cout << "curl_easy_perform error is " << curl_easy_strerror(ret) << std::endl;
        if (curl)
        {
            curl_easy_cleanup(curl);    
        }
        return DCURL_PERFORM_CURL_FAILED;
    }
    
    if (curl)
    {
        curl_easy_cleanup(curl);
    }

    return DCURL_OK;
}
