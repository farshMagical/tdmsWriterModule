#ifndef TDMS_TIMESTAMP_H
#define TDMS_TIMESTAMP_H

#include <cstdint>
#include <cstddef>

namespace tdms
{

    class Timestamp
    {
    public:
        Timestamp();
        // Timestamp(uint64_t utcTime); // from UTC time
        // Timestamp(int64_t seconds, uint64_t fraction);
        uint64_t GetFraction();
        int64_t GetSeconds();
        size_t Size();

        // uint64_t &Timestamp::GetFractionPtr();
        // int64_t &Timestamp::GetSecondsPtr();

    private:
        uint64_t fraction; // positive fractions (2-64) of a second
        int64_t seconds;

    private:
        const uint64_t FROM_1904_TO_UTC_SEC = 2082844800;
        const uint64_t FROM_1904_TO_UTC_MSEC = 2082844800000;
        const uint64_t FROM_1904_TO_UTC_USEC = 2082844800000000;
    };
} // tdms

#endif // TDMS_TIMESTAMP_H