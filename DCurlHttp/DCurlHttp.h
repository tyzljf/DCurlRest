#ifndef __DCURL_HTTP_H
#define __DCURL_HTTP_H

#include "DCurlRest.h"

class DCurlHttp: public DCurlRest
{
public:
    DCurlHttp();
    virtual ~DCurlHttp();
    
public:
    /*********************************************************************
    函数名称:   DownloadFile
    函数功能:  下载指定文件片段
    输入:	param  		:  文件片段信息
    输出:	无
    返回值:	DCURL_OK:  成功
    特殊说明:
    **********************************************************************/
    virtual DCURL_RETURN_E DownloadFile(NodeParam* param);
};

#endif