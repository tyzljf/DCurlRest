#include <unistd.h>
#include "CThread.h"
#include "DCurlRest.h"
#include "CTaskTool.h"

thread_mutex_t DCurlRest::m_mutex;

DCurlRest::DCurlRest()
{
    m_fileDir = "./";
    CThread::InitLock(&m_mutex);
}

DCurlRest::~DCurlRest()
{
    CThread::DestoryLock(&m_mutex);
}

void DCurlRest::SetFileDir(const std::string& dir)
{
    m_fileDir = dir;
}

DCURL_RETURN_E DCurlRest::GetFileLength(const std::string& url,  long* fileLength)
{
    double length = 0;
    
    CURL* curl = curl_easy_init();
    if (NULL == curl)
    {
        std::cout << "init curl handle failed." << std::endl;
        return DCURL_INIT_CURL_HANDLE_FAILED;
    }

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_HEADER, 1L);
    curl_easy_setopt(curl, CURLOPT_NOBODY, 1L);
    //curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);  调试信息
    
    CURLcode ret = curl_easy_perform(curl);
    if (CURLE_OK != ret)
    {
        std::cout << "curl_easy_perform error is " << curl_easy_strerror(ret) << std::endl;
        curl_easy_cleanup(curl);
        return DCURL_PERFORM_CURL_FAILED;
    }
    
    curl_easy_getinfo(curl, CURLINFO_CONTENT_LENGTH_DOWNLOAD, &length);
    *fileLength = length;
    curl_easy_cleanup(curl);
    
    return DCURL_OK; 
}


DCURL_RETURN_E DCurlRest::Download(const std::string& url, const std::string& localFile, uint32_t timeout)
{
    long fileLength = 0;
    DCURL_RETURN_E dRet = GetFileLength(url, &fileLength);
    if (DCURL_OK != dRet)
    {
        std::cout << "get the length of the remote file " << url << " failed, ret:" << dRet << std::endl;
        return dRet;
    }
    std::cout << "the length of the remote file " << url << " is " << fileLength << std::endl;
    
    std::string filename = m_fileDir + "/" + localFile;
    FILE* fp = fopen(filename.c_str(), "wb");
    if (NULL == fp)
    {
        std::cout << "fopen " << filename << " failed." << std::endl;
        return DCURL_INTERNAL_FAILED;
    }
    
    uint8_t threadNum = WORK_THREAD_NUM;     //从配置文件中读取
    uint64_t blockSize = fileLength / threadNum;
    
    for (int i = 0; i <= threadNum; i++)
    {
        NodeParam *nodeParam = new NodeParam();
        if (NULL == nodeParam)
        {
            std::cout << "new NodeParam failed." << std::endl;
            fclose(fp);
            return DCURL_INTERNAL_FAILED;
        }
        
        if (i < threadNum)  //计算每个线程需要下载的文件偏移[start,   end]
        {
            nodeParam->startPos = i * blockSize;
            nodeParam->endPos = (i + 1) * blockSize - 1;
        }
        else
        {
            if (fileLength % threadNum != 0)
            {
                nodeParam->startPos = i * blockSize;
                nodeParam->endPos = fileLength - 1;
            }
            else        //每个线程下载的文件偏移量相同
            {
                break;
            }
        }
        
        nodeParam->fp = fp;
        nodeParam->timeout = timeout;
        nodeParam->url = url;
        
        msg_pair_t msg_pair;
        msg_pair.node = nodeParam;
        msg_pair.curlRest = this;

        CTaskTool::GetInstance().PushReqMsgQueue(msg_pair);     //放入线程池队列中
        std::cout << "i = " << i << ", startPos:" << msg_pair.node->startPos 
                << ", endPos:" << msg_pair.node->endPos << std::endl;
    }
    
    sleep(5);  //睡眠几秒钟，让线程池中的线程能从队列中获取到数据
    std::cout << "The number of the current thread running is  " << CTaskTool::GetInstance().ThreadRunningNum() << std::endl;
    while (CTaskTool::GetInstance().ThreadRunningNum() > 0)
    {
        sleep(2);
    }
    
    fclose(fp);
    
    std::cout << "download end ...." << std::endl;
    return DCURL_OK;
}

size_t DCurlRest::writeData(void *buffer, size_t size, size_t nmemb, void *data)
{
    size_t written = 0;
    NodeParam* param = (NodeParam *)data;

    CAuthThreadLock threadLock(&m_mutex);
    if (param->startPos + size * nmemb <= param->endPos)
    {
        fseek(param->fp, param->startPos, SEEK_SET);
        written = fwrite(buffer, size, nmemb, param->fp);
        param->startPos += size * nmemb;
    }
    else
    {
        fseek(param->fp, param->startPos, SEEK_SET);
        written = fwrite(buffer, 1, param->endPos - param->startPos + 1, param->fp);
        written = size * nmemb;
    }
    
    return written;  
}

DCURL_RETURN_E DCurlRest::DownloadFile(NodeParam* param)
{
    return DCURL_OK;
}
