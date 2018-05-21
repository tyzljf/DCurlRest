#ifndef __CTASK_TOOL_H
#define __CTASK_TOOL_H

#include "CThread.h"
#include "Types.h"
#include <vector>
#include <unistd.h>
#include "DCurlRest.h"

typedef struct tag_msg_pair_t
{
    DCurlRest*      curlRest;
    NodeParam*      node;
    tag_msg_pair_t()
    {
        curlRest = NULL;
        node = NULL;
    }
    tag_msg_pair_t(DCurlRest* pCurlRest, NodeParam* pNode)
    {
        curlRest = pCurlRest;
        node = pNode;
    }
}msg_pair_t;

enum ThreadStatus
{
    THREAD_STATUS_IDLE,
    THREAD_STATUS_RUNNING,
    THREAD_STATUS_EXIT
};

class CTaskTool
{
public:
    static CTaskTool& GetInstance();
    ~CTaskTool();
    
    /*********************************************************************
    函数名称:   Init
    函数功能:   初始化线程池，预先分配N个工作线程
    输入:	无
    输出:	无
    返回值:	true：   成功
    特殊说明:
    **********************************************************************/
    bool Init();

public:
    /*********************************************************************
    函数名称:   PopReqMsgQueue
    函数功能:   出队
    输入:	无
    输出:	无
    返回值:	队列开始元素
    特殊说明:
    **********************************************************************/
    msg_pair_t PopReqMsgQueue();
    /*********************************************************************
    函数名称:   PushReqMsgQueue
    函数功能:   入队
    输入:	msg_pair：   队列元素
    输出:	无
    返回值:	无
    特殊说明:
    **********************************************************************/
    void PushReqMsgQueue(msg_pair_t& msg_pair);
    /*********************************************************************
    函数名称:   IncrThreadRunningNum
    函数功能:   记录当前正在工作的线程数
    输入:	无
    输出:	无
    返回值:	无
    特殊说明:
    **********************************************************************/
    void IncrThreadRunningNum();
    /*********************************************************************
    函数名称:   DecThreadRunningNum
    函数功能:   记录当前正在工作的线程数
    输入:	无
    输出:	无
    返回值:	无
    特殊说明:
    **********************************************************************/
    void DecThreadRunningNum();
    /*********************************************************************
    函数名称:   ThreadRunningNum
    函数功能:   返回正在工作的线程数
    输入:	无
    输出:	无
    返回值:	正在工作的线程数
    特殊说明:
    **********************************************************************/
    uint32_t ThreadRunningNum();
    /*********************************************************************
    函数名称:   RecvThreadFunc
    函数功能:   线程处理函数，从队列中取数据，调用协议相关的下载函数
    输入:	arg： 队列元素
    输出:	无
    返回值:	无
    特殊说明:
    **********************************************************************/
    static void* RecvThreadFunc(void *arg);
    /*********************************************************************
    函数名称:   SetReciveThreadStatus
    函数功能:   设置线程状态
    输入:	threadStatus：   线程状态
    输出:	无
    返回值:	无
    特殊说明:
    **********************************************************************/
    void SetReciveThreadStatus(ThreadStatus threadStatus);
    /*********************************************************************
    函数名称:   NeedExit
    函数功能:   线程是否退出
    输入:	无
    输出:	无
    返回值:	true：线程退出
    特殊说明:
    **********************************************************************/
    bool NeedExit();
    /*********************************************************************
    函数名称:   EmptyReqMsgQueue
    函数功能:   队列是否为空
    输入:	无
    输出:	无
    返回值:	true：空
    特殊说明:
    **********************************************************************/
    bool EmptyReqMsgQueue();

private:
    CTaskTool();
    
private:
    static CTaskTool                     m_instance;
    thread_mutex_t                     m_ReqHandlerMutex;
    std::vector<msg_pair_t>       m_ReqMsgQueue;     //消息队列
    thread_mutex_t                     m_ThreadNumMutex; 
    uint32_t                                 m_threadRunningNum; //当前正在工作的线程数 
    bool                                        m_NeedExit;
    volatile int32_t                        m_RecvThreadStatus;
};

#endif