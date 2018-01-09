#if defined(_MSC_VER)
  #if (_MSC_VER < 1400)
    #error ("Your version of Microsoft Visual C/C++ is unsupported! Please upgrade to Microsoft Visual C/C++ 2005 or newer.")
  #endif
#endif

#if defined(_MSC_VER)
  // Don't moan about non-standard extensions.
  #pragma warning(disable: 4200 4201)

  // Disable local function reference checking because this is a unity build.
  // We don't care if our functions are dead-code eliminated because that's
  // the point!
  #pragma warning(disable: 4505)

  // Don't care about buffer overflows.
  #define _CRT_SECURE_NO_WARNINGS
#endif

#if defined(_WIN32) || defined(_WIN64)
  // Only care about Windows 7 and later.
  #define WINVER 0x0601
#endif

#include "demo.c"
