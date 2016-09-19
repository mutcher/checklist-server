#ifndef PLATFORM_H_9192016
#define PLATFORM_H_9192016
#pragma once

#define PLATFORM_WINDOWS 1
#define PLATFORM_APPLE 2
#define PLATFORM_UNIX 3

#if defined(_WIN32)
#define PLATFORM PLATFORM_WINDOWS
#elif defined(__APPLE__)
#define PLATFORM PLATFORM_APPLE
#else
#define PLATFORM PLATFORM_UNIX
#endif //if defined(_WIN32)
#endif //PLATFORM_H_9192016
