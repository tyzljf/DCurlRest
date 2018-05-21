#ifndef __TYPES_H
#define __TYPES_H

#include <pthread.h>
#include <stdio.h>
#include <curl/curl.h>
#include <iostream>

typedef unsigned int    uint32_t;
typedef unsigned long   uint64_t;
typedef unsigned char   uint8_t;
//typedef long long int     int64_t;

typedef pthread_mutex_t     thread_mutex_t;
typedef pthread_t           thread_t;

#define DP_FAILED           1
#define DP_SUCCESS          0

#define WORK_THREAD_NUM     10          //工作线程数

typedef enum
{
    DCURL_OK = 0,
    DCURL_GET_FILELENGTH_FAILED,
    DCURL_INIT_CURL_FAILED,
    DCURL_PARAM_INVALID,
    DCURL_INIT_CURL_HANDLE_FAILED,
    DCURL_PERFORM_CURL_FAILED,
    DCURL_INTERNAL_FAILED
}DCURL_RETURN_E;

typedef struct 
{
    FILE*       fp;
    long long int    startPos;
    long long int    endPos;
    uint32_t    timeout;
    CURL*       curl;
    thread_t    id;
    std::string url;
    void*       data;
}NodeParam;

#endif