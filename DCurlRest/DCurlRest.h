#ifndef __DCURL_REST_H
#define __DCURL_REST_H

#include <iostream>
#include "Types.h"

class DCurlRest
{
public:
    DCurlRest();
    virtual ~DCurlRest();

public:
    /*********************************************************************
    函数名称:   Download
    函数功能:   下载指定文件
    输入:	url ：           URL路径
                localFile：    本地保存文件名
                timeout：    超时时间      
    输出:	无
    返回值:	DCURL_OK:  成功
    特殊说明:
    **********************************************************************/
    DCURL_RETURN_E Download(const std::string& url, const std::string& localFile, uint32_t timeout = 30);
    /*********************************************************************
    函数名称:   SetFileDir
    函数功能:   设置本地目录路径，默认值为当前目录
    输入:	dir ：           本地目录路径
    输出:	无
    返回值:	无
    特殊说明:
    **********************************************************************/
    void SetFileDir(const std::string& dir);
    /*********************************************************************
    函数名称:   DownloadFile
    函数功能:  下载指定文件片段
    输入:	param:  文件片段信息
    输出:	无
    返回值:	DCURL_OK:  成功
    特殊说明:
    **********************************************************************/
    virtual DCURL_RETURN_E DownloadFile(NodeParam* param);
protected: 
    /*********************************************************************
    函数名称:   GetFileLength
    函数功能:   返回下载文件的长度
    输入:	url：  URL路径
    输出:	fileLength： 文件长度
    返回值:	DCURL_OK:  成功
    特殊说明:
    **********************************************************************/
    DCURL_RETURN_E GetFileLength(const std::string& url, long* fileLength);
    /*********************************************************************
    函数名称:   writeData
    函数功能:   保存从服务器上接收到的文件长度
    输入:	buffer：  接收到的地址
                size：     块大小
                nmemb：块数量
                data：     用户数据，通常包含文件流
    输出:	无
    返回值:	写入到文件流中的字节数
    特殊说明:
    **********************************************************************/
    static size_t writeData(void *buffer, size_t size, size_t nmemb, void *data);
    
public:    
    static thread_mutex_t      m_mutex;  //同步写文件操作
    
private:
    std::string         m_fileDir;  //文件存放目录
};

#endif