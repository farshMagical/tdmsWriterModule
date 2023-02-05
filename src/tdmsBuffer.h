#ifndef TDMS_BUFFER_H
#define TDMS_BUFFER_H

#include <cstdint>
#include <cstddef>

#define TDMS_SYSTEM_ENDIANNESS 0

namespace tdms
{

    template <typename T>
    size_t unitToBuffer(T *unit, uint8_t *buf)
    {
        typedef union
        {
            T TValue;
            uint8_t Uint8Value[sizeof(T)];
        } TtoU8_t;

        TtoU8_t tmpBuf = {
            .TValue = *unit,
        };

        for (uint8_t i = 0; i < sizeof(T); i++)
        {
#if (TDMS_SYSTEM_ENDIANNESS == 0)
            buf[i] = tmpBuf.Uint8Value[i];
#else
            buf[i] = tmpBuf.Uint8Value[sizeof(T) - i];
#endif
        }
        return sizeof(T);
    }

    template <typename T>
    size_t dataToBuffer(T *data, uint8_t *buf, size_t dataSize)
    {
        size_t tmpSize = 0;
        for (size_t i = 0; i < dataSize; i++)
        {
            tmpSize += unitToBuffer(&data[i], &buf[tmpSize]);
        }

        return tmpSize;
    }

} // tdms

#endif // TDMS_BUFFER_H