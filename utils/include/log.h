#pragma once

#ifndef ENABLE_LOGGING
#define ENABLE_LOGGING 0
#endif

#define dbgs                                                                   \
    if (!ENABLE_LOGGING)                                                       \
    {                                                                          \
    }                                                                          \
    else                                                                       \
        std::cout
