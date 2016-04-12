#pragma once
#include "platform.h"

#if PLATFORM == PLATFORM_WINDOWS
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

typedef int socklen_t;
#else
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>

#define INVALID_SOCKET static_cast<uint64_t>(-1)
typedef int SOCKET;
#endif