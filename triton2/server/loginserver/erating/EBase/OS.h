#ifndef __OS_H__
#define __OS_H__

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#ifdef WIN32
#define INVALID_HANDLE INVALID_SOCKET
#   define HANDLE_ERROR   SOCKET_ERROR
#else
#   define INVALID_HANDLE (-1)
#   define HANDLE_ERROR   (-1)
#   define SOCKET_ERROR   (-1)
#endif

#ifdef WIN32
#   include <WinSock2.h>
#   include <direct.h>

#   define int8_t   char
#   define int16_t  short
#   define int32_t  int
#   define int64_t  long long
#   define uint8_t  unsigned char
#   define uint16_t unsigned short
#   define uint32_t unsigned int
#   define uint64_t unsigned long long
#   pragma comment(lib,"Ws2_32.lib")
#   define SYS_HANDLE SOCKET
#   define SYS_NO_BLOCKED 1
#include <Windows.h>
#else
#   include <sys/socket.h>
#   include <netinet/in.h>
#   include <arpa/inet.h>
#   include <dirent.h>
#   include <inttypes.h>
#   include <unistd.h>
#   include <pthread.h>
#   include <fcntl.h>
#   define SYS_HANDLE int
#   define SOCKET int
#   define SYS_NO_BLOCKED 1
#endif

#ifndef INFINITE
#define INFINITE 0xFFFFFFFF
#endif

namespace OS
{
#ifdef WIN32
#   define snprintf     _snprintf
#   define usleep(X)    Sleep((X) / 1000)
#   define sleep(X)     Sleep((X) * 1000)
#   define mkdir        _mkdir
#   define getpid       _getpid
#   define unlink       _unlink
#else
#endif
}

#endif

