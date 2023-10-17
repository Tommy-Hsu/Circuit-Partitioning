#ifndef DEBUG_H
#define DEBUG_H

#if DEBUG_FLAG
    #define DEBUG_COUT(...) std::cout << __VA_ARGS__
#else
    #define DEBUG_COUT(...)
#endif

#endif // DEBUG_H
