#include "tdmsProperty.h"

#include <iostream>

namespace tdms
{

    Property::Property(std::string name, uint32_t type, std::string value)
        : lengthOfName(name.size()),
          name(name),
          dataTypeOfProperty(type)
    {
        if (type == tdsTypeString)
        {
            ifStringLength = value.size();
            this->valueStr = value;
            // std::cout << value;
            size = 0;
            size += 4 + name.size() + 4 + 4 + value.size();

            return;
        }
        throw 0; // TODO
    }

    Property::Property(std::string name, uint32_t type, Timestamp value)
        : lengthOfName(name.size()),
          name(name),
          dataTypeOfProperty(type)
    {
        ifNotStringSize = value.Size();
        this->value = new uint8_t[ifNotStringSize];

        // std::cout << "if Not String size is " << ifNotStringSize << std::endl;
        if (!this->value)
        {
            throw -1;
        }

        auto seconds = value.GetSeconds();
        auto frac = value.GetFraction();

        dataToBuffer(&frac, this->value, 1);
        dataToBuffer(&seconds, &this->value[8], 1);

        size = 0;
        size += 4 + name.size() + 4 + ifNotStringSize;
    }


    uint32_t Property::Size()
    {
        return size;
    }

    bool Property::operator==(const Property &other)
    {
        return name == other.name;
    }

    uint32_t *Property::GetNameLength()
    {
        return &lengthOfName;
    }

    const char *Property::GetName()
    {
        return name.data();
    }

    uint32_t *Property::GetDataTypeOfProperty()
    {
        return &dataTypeOfProperty;
    }

    uint32_t *Property::GetLengthStringProperty()
    {
        return &ifStringLength;
    }

    uint32_t Property::GetDataSizeIfNotString()
    {
        return ifNotStringSize;
    }

    std::string *Property::GetValueString()
    {
        if (dataTypeOfProperty == tdsTypeString)
        {
            return &valueStr;
        }
    }

    const uint8_t *Property::GetValue()
    {
        return value;
    }

    uint8_t TDMS_DataBytesOfEachType(uint32_t TdsDataType)
    {
        uint8_t dataSize = 0; // Bytes
        switch (TdsDataType)
        {
        case tdsTypeVoid:
            dataSize = 1;
            break;

        case tdsTypeI8:
            dataSize = sizeof(int8_t);
            break;

        case tdsTypeI16:
            dataSize = sizeof(int16_t);
            break;

        case tdsTypeI32:
            dataSize = sizeof(int32_t);
            break;

        case tdsTypeI64:
            dataSize = sizeof(int64_t);
            break;

        case tdsTypeU8:
            dataSize = sizeof(uint8_t);
            break;

        case tdsTypeU16:
            dataSize = sizeof(uint16_t);
            break;

        case tdsTypeU32:
            dataSize = sizeof(uint32_t);
            break;

        case tdsTypeU64:
            dataSize = sizeof(uint64_t);
            break;

        case tdsTypeSingleFloat:
            dataSize = sizeof(float);
            break;

        case tdsTypeDoubleFloat:
            dataSize = sizeof(double);
            break;

        case tdsTypeString:
            dataSize = sizeof(char); // indeterminate
            break;

        case tdsTypeBoolean:
            dataSize = sizeof(uint8_t);
            break;
            // TODO TIMESTAMP
            // case tdsTypeTimeStamp:
            //     dataSize = sizeof(TDMS_Timestamp_t);
            //     break;
        }

        return dataSize;
    }

} // tdms
