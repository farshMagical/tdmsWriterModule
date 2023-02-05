#include "tdmsTimestamp.h"

#include <chrono>
#include <iostream>

using namespace std;

namespace tdms
{

    Timestamp::Timestamp()
    // convert from UTC into S+F
    {
        auto duration = chrono::system_clock::now().time_since_epoch();
        seconds = chrono::duration_cast<chrono::seconds>(duration).count() + FROM_1904_TO_UTC_SEC;
        auto us = chrono::duration_cast<chrono::microseconds>(duration).count() - seconds * 1000000;
        fraction = (uint64_t(18446744073700) * uint64_t(us)); // need to get fract, where 1000000 us - 2^64
        // fraction = static_cast<uint64_t>(18446744073700000000 * us / 1000000); // need to get fract, where 1000000 us - 2^64
    }

    size_t Timestamp::Size(){
        return sizeof(uint64_t)+sizeof(int64_t);
    }

    uint64_t Timestamp::GetFraction()
    {
        return fraction;
    }
    int64_t Timestamp::GetSeconds()
    {
        return seconds;
    }

} // tdms
