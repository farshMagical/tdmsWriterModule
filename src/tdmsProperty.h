#ifndef TDMS_PROPERTY_H
#define TDMS_PROPERTY_H

#include <unordered_map>
#include <vector>
#include <iostream>
#include <string>
// #include <cstdint>

#include "tdmsBuffer.h"
#include "tdmsTimestamp.h"

#define tdsTypeVoid 0x00000000
#define tdsTypeI8 0x00000001
#define tdsTypeI16 0x00000002
#define tdsTypeI32 0x00000003
#define tdsTypeI64 0x00000004
#define tdsTypeU8 0x00000005
#define tdsTypeU16 0x00000006
#define tdsTypeU32 0x00000007
#define tdsTypeU64 0x00000008
#define tdsTypeSingleFloat 0x00000009
#define tdsTypeDoubleFloat 0x0000000A
#define tdsTypeExtendedFloat 0x0000000B
#define tdsTypeSingleFloatWithUnit 0x00000019
#define tdsTypeDoubleFloatWithUnit 0x0000001A
#define tdsTypeExtendedFloatWithUnit 0x0000001B
#define tdsTypeString 0x00000020
#define tdsTypeBoolean 0x00000021
#define tdsTypeTimeStamp 0x00000044
#define tdsTypeFixedPoint 0x0000004F
#define tdsTypeComplexSingleFloat 0x0008000C
#define tdsTypeComplexDoubleFloat 0x0010000D
#define tdsTypeDAQmxRawData 0xFFFFFFFF

namespace tdms
{

    uint8_t TDMS_DataBytesOfEachType(uint32_t TdsDataType);

    class Property
    {
    public:

        template <typename T>
        Property(std::string name, uint32_t type, T value);
        Property(std::string name, uint32_t type, std::string value);
        Property(std::string name, uint32_t type, Timestamp value);

        bool operator==(const Property &other);

        uint32_t *GetNameLength();
        const char *GetName();
        uint32_t *GetDataTypeOfProperty();
        uint32_t *GetLengthStringProperty();
        uint32_t GetDataSizeIfNotString();
        const uint8_t *GetValue();
        std::string *GetValueString();

        uint32_t Size();

    private:
        uint32_t lengthOfName; // name.size()
        std::string name;
        uint32_t dataTypeOfProperty;
        uint32_t ifStringLength; // value.size()
        uint32_t ifNotStringSize;
        uint8_t *value;
        std::string valueStr;
        size_t size;
    };

    template <typename T>
    Property::Property(std::string name, uint32_t type, T value)
        : lengthOfName(name.size()),
          name(name),
          dataTypeOfProperty(type)
    {
        ifStringLength = 0;

        this->value = new uint8_t[sizeof(T)];
        ifNotStringSize = sizeof(T);

        // std::cout << "if Not String size is " << ifNotStringSize << std::endl;

        if (!this->value)
        {
            throw -1;
        }
        dataToBuffer(&value, this->value, 1);
        size = 0;
        size += 4 + name.size() + 4 + sizeof(T);
    }

}

#endif // TDMS_PROPERTY_H