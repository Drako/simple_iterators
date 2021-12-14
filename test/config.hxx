#pragma once

#ifndef CONFIG_HXX
#define CONFIG_HXX

#ifdef __APPLE__

#include <TargetConditionals.h>

# if defined(TARGET_IPHONE_SIMULATOR) && (TARGET_IPHONE_SIMULATOR!=0)
#  define APPLE_SIMULATOR 1
# elif defined(TARGET_OS_IPHONE) && (TARGET_OS_IPHONE!=0)
#  define APPLE_IPHONE 1
# elif defined(TARGET_OS_WATCH) && (TARGET_OS_WATCH!=0)
#  define APPLE_WATCH 1
# elif defined(TARGET_OS_MAC) && (TARGET_OS_MAC!=0)
#  define APPLE_OSX 1
# else
#  error Unknown Apple platform
# endif

#endif // __APPLE__

// might also be broken on other platforms, but currently I only encountered the issue on Mac OS X with GCC 10
#if defined(APPLE_OSX) && !defined(__clang__)
#define SKIP_EXCEPTION_TESTS 1
#endif // APPLE_OSX

#endif // CONFIG_HXX
