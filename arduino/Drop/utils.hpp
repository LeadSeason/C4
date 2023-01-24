#pragma once
#include <Arduino.h>
#include <stdio.h>

namespace utils
{
    template <typename... Args>
    inline void Print(const char *str, Args... args)
    {
        char buffer[1 << 10];
        sprintf(buffer, str, args...);

        Serial.print(buffer);
    }

    template <>
    inline void Print(const char *str)
    {
        Serial.print(str);
    }

    template<typename T>
    inline T Max(T a, T b)
    {
        return (a > b) ? a : b;
    }

    template<typename T>
    inline T Min(T a, T b)
    {
        return (a > b) ? b : a;
    }
}