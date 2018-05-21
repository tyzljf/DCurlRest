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
    ��������:   Init
    ��������:   ��ʼ���̳߳أ�Ԥ�ȷ���N�������߳�
    ����:	��
    ���:	��
    ����ֵ:	true��   �ɹ�
    ����˵��:
    **********************************************************************/
    bool Init();

public:
    /*********************************************************************
    ��������:   PopReqMsgQueue
    ��������:   ����
    ����:	��
    ���:	��
    ����ֵ:	���п�ʼԪ��
    ����˵��:
    **********************************************************************/
    msg_pair_t PopReqMsgQueue();
    /*********************************************************************
    ��������:   PushReqMsgQueue
    ��������:   ���
    ����:	msg_pair��   ����Ԫ��
    ���:	��
    ����ֵ:	��
    ����˵��:
    **********************************************************************/
    void PushReqMsgQueue(msg_pair_t& msg_pair);
    /*********************************************************************
    ��������:   IncrThreadRunningNum
    ��������:   ��¼��ǰ���ڹ������߳���
    ����:	��
    ���:	��
    ����ֵ:	��
    ����˵��:
    **********************************************************************/
    void IncrThreadRunningNum();
    /*********************************************************************
    ��������:   DecThreadRunningNum
    ��������:   ��¼��ǰ���ڹ������߳���
    ����:	��
    ���:	��
    ����ֵ:	��
    ����˵��:
    **********************************************************************/
    void DecThreadRunningNum();
    /*********************************************************************
    ��������:   ThreadRunningNum
    ��������:   �������ڹ������߳���
    ����:	��
    ���:	��
    ����ֵ:	���ڹ������߳���
    ����˵��:
    **********************************************************************/
    uint32_t ThreadRunningNum();
    /*********************************************************************
    ��������:   RecvThreadFunc
    ��������:   �̴߳��������Ӷ�����ȡ���ݣ�����Э����ص����غ���
    ����:	arg�� ����Ԫ��
    ���:	��
    ����ֵ:	��
    ����˵��:
    **********************************************************************/
    static void* RecvThreadFunc(void *arg);
    /*********************************************************************
    ��������:   SetReciveThreadStatus
    ��������:   �����߳�״̬
    ����:	threadStatus��   �߳�״̬
    ���:	��
    ����ֵ:	��
    ����˵��:
    **********************************************************************/
    void SetReciveThreadStatus(ThreadStatus threadStatus);
    /*********************************************************************
    ��������:   NeedExit
    ��������:   �߳��Ƿ��˳�
    ����:	��
    ���:	��
    ����ֵ:	true���߳��˳�
    ����˵��:
    **********************************************************************/
    bool NeedExit();
    /*********************************************************************
    ��������:   EmptyReqMsgQueue
    ��������:   �����Ƿ�Ϊ��
    ����:	��
    ���:	��
    ����ֵ:	true����
    ����˵��:
    **********************************************************************/
    bool EmptyReqMsgQueue();

private:
    CTaskTool();
    
private:
    static CTaskTool                     m_instance;
    thread_mutex_t                     m_ReqHandlerMutex;
    std::vector<msg_pair_t>       m_ReqMsgQueue;     //��Ϣ����
    thread_mutex_t                     m_ThreadNumMutex; 
    uint32_t                                 m_threadRunningNum; //��ǰ���ڹ������߳��� 
    bool                                        m_NeedExit;
    volatile int32_t                        m_RecvThreadStatus;
};

#endif