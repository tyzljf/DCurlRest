#include "CTaskTool.h"
#include "CThread.h"

CTaskTool CTaskTool::m_instance;

CTaskTool::CTaskTool()
{
    m_NeedExit = false;
    m_threadRunningNum = 0;
    CThread::InitLock(&m_ReqHandlerMutex);
    CThread::InitLock(&m_ThreadNumMutex);
}

CTaskTool::~CTaskTool()
{
    CThread::DestoryLock(&m_ReqHandlerMutex);
    CThread::DestoryLock(&m_ThreadNumMutex);
}

CTaskTool& CTaskTool::GetInstance()
{
    return m_instance;
}

bool CTaskTool::Init()
{
    int threadNum = WORK_THREAD_NUM; // 从配置文件中读取
    thread_t id;
    
    for (int i = 0; i < threadNum; i++)
    {
        int ret = CThread::CreateThread(&id, RecvThreadFunc, this);
        if (0 != ret)
        {
            std::cout << "create thread failed, ret:" << ret << std::endl;
            return false;
        }
    }
    
    return true;
}

void* CTaskTool::RecvThreadFunc(void *arg)
{
    CTaskTool* pInstance = (CTaskTool*)arg;
    DCURL_RETURN_E  dRet = DCURL_OK;
    
    pInstance->SetReciveThreadStatus(THREAD_STATUS_RUNNING);
    while(!pInstance->NeedExit())
    {
        if (pInstance->EmptyReqMsgQueue())
        {
            usleep(20000L);
            continue;
        }
        
        msg_pair_t msg_pair = pInstance->PopReqMsgQueue();
        pInstance->IncrThreadRunningNum();

        std::cout << "pop message queue, startPos:" << msg_pair.node->startPos << ", endPos:" << msg_pair.node->endPos << std::endl;
        DCurlRest *curlRest = msg_pair.curlRest;
        NodeParam *nodeParam = msg_pair.node;
        
        //开始下载
        dRet = curlRest->DownloadFile(nodeParam);
        if (DCURL_OK != dRet)
        {
            std::cout << "download file failed, ret:" << std::endl;
        }
        
        delete nodeParam;
        pInstance->DecThreadRunningNum();
    }
    pInstance->SetReciveThreadStatus(THREAD_STATUS_EXIT);
}

void CTaskTool::IncrThreadRunningNum()
{
    CAuthThreadLock tLock(&m_ThreadNumMutex);
    m_threadRunningNum++;
}

void CTaskTool::DecThreadRunningNum()
{
    CAuthThreadLock tLock(&m_ThreadNumMutex);
    m_threadRunningNum--;
}

uint32_t CTaskTool::ThreadRunningNum()
{
    uint32_t tmpThreadNum = 0;
    {
        CAuthThreadLock tLock(&m_ThreadNumMutex);
        tmpThreadNum = m_threadRunningNum;
    }
    
    return tmpThreadNum;
}

msg_pair_t CTaskTool::PopReqMsgQueue()
{
    CAuthThreadLock tLock(&m_ReqHandlerMutex);
    for(std::vector<msg_pair_t>::iterator it = m_ReqMsgQueue.begin(); it != m_ReqMsgQueue.end(); it++)
    {
        msg_pair_t msg_pair = *it;
        m_ReqMsgQueue.erase(it);
        return msg_pair;
    }
}

void CTaskTool::PushReqMsgQueue(msg_pair_t& msg_pair)
{
    CAuthThreadLock tLock(&m_ReqHandlerMutex);
    m_ReqMsgQueue.push_back(msg_pair);
}

bool CTaskTool::EmptyReqMsgQueue()
{
    CAuthThreadLock tLock(&m_ReqHandlerMutex);
    if(0 == m_ReqMsgQueue.size())
    {
        return true;
    }
    
    return false;
}

bool CTaskTool::NeedExit()
{
    return m_NeedExit;
}

void CTaskTool::SetReciveThreadStatus(ThreadStatus threadStatus)
{
    m_RecvThreadStatus = threadStatus;
}