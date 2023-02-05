#include "tdmsParts.h"

using namespace std;

namespace tdms
{

    MetaDataObjectFile::MetaDataObjectFile() {}
    MetaDataObjectFile::MetaDataObjectFile(string name, string description)
    {
        this->name = name;
        path = "/";
        rawDataIndex = tdsTypeDAQmxRawData; // empty
        numberOfProperties = 2;
        properties.insert({"Name", {"Name", tdsTypeString, name}});
        properties.insert({"Description", {"Description", tdsTypeString, description}});

        size = path.size() + 4 + 4 + 4;
        for (auto &p : properties)
        {
            size += p.second.Size();
        }

        isChanged = true;
    }

    std::string *MetaDataObjectFile::GetPath()
    {
        return &path;
    }
    uint32_t *MetaDataObjectFile::GetRawDataIndex()
    {
        return &rawDataIndex;
    }
    uint32_t *MetaDataObjectFile::GetNumberOfProperties()
    {
        return &numberOfProperties;
    }
    std::unordered_map<std::string, Property> &MetaDataObjectFile::GetProperties()
    {
        return properties;
    }
    bool MetaDataObjectFile::ChangeIs()
    {
        return isChanged;
    }

    uint32_t MetaDataObjectFile::Size()
    {
        return size;
    }

    MetaDataObjectGroup::MetaDataObjectGroup() {}
    MetaDataObjectGroup::MetaDataObjectGroup(string name, string description)
    {
        this->name = name;
        path = string("/'") + name + string("'");
        lengthOfPath = path.size();
        rawDataIndex = tdsTypeDAQmxRawData; // empty
        numberOfProperties = 2;
        properties.insert({"Name", {"Name", tdsTypeString, name}});
        properties.insert({"Description", {"Description", tdsTypeString, description}});

        size = 4 + lengthOfPath + 4 + 4;
        for (auto &p : properties)
        {
            size += p.second.Size();
        }

        isChanged = true;
    }

    std::string *MetaDataObjectGroup::GetName()
    {
        return &name;
    }
    uint32_t *MetaDataObjectGroup::GetLengthOfPath()
    {
        return &lengthOfPath;
    }
    std::string *MetaDataObjectGroup::GetPath()
    {
        return &path;
    }
    uint32_t *MetaDataObjectGroup::GetRawDataIndex()
    {
        return &rawDataIndex;
    }
    uint32_t *MetaDataObjectGroup::GetNumberOfProperty()
    {
        return &numberOfProperties;
    }
    std::unordered_map<std::string, Property> &MetaDataObjectGroup::GetProperty()
    {
        return properties;
    }
    bool MetaDataObjectGroup::ChangedIs()
    {
        return isChanged;
    }

    bool MetaDataObjectGroup::operator==(const MetaDataObjectGroup &other)
    {
        return name == other.name;
    }

    uint32_t MetaDataObjectGroup::Size()
    {
        return size;
    }

    MetaDataObjectChannel::MetaDataObjectChannel() {}
    MetaDataObjectChannel::MetaDataObjectChannel(string groupName,
                                                 string name,
                                                 uint32_t tdsType,
                                                 uint32_t numOfRawDataValue)
    {
        this->name = name;

        path = "/'" + groupName + "'/'" + name + "'";
        lengthOfPath = path.size();

        lengthOfIndexInfo = 0x14; // is size of 4 fields below (always is 0x14)

        dataTypeOfRaw = tdsType;
        dimentionOfRawArray = 1; // must be 1
        numberOfRawDataValue = numOfRawDataValue;
        numberOfProperties = 0;

        size = 4 + lengthOfPath + 4 + lengthOfIndexInfo;
        for (auto &p : properties)
        {
            size += p.second.Size();
        }

        isChanged = true;
    }

    bool MetaDataObjectChannel::operator==(const MetaDataObjectChannel &other)
    {
        return name == other.name;
    }

    uint32_t MetaDataObjectChannel::Size()
    {
        return size;
    }

    size_t MetaDataObjectChannel::GetNumOfRawDataValue()
    {
        return numberOfRawDataValue;
    }

    uint32_t *MetaDataObjectChannel::GetLengthOfPath()
    {
        return &lengthOfPath;
    }
    std::string *MetaDataObjectChannel::GetPath()
    {
        return &path;
    }
    uint32_t *MetaDataObjectChannel::GetLengthOfIndexInfo()
    {
        return &lengthOfIndexInfo;
    }
    uint32_t *MetaDataObjectChannel::GetDataTypeOfRaw()
    {
        return &dataTypeOfRaw;
    }
    uint32_t *MetaDataObjectChannel::GetDimentionOfRawArray()
    {
        return &dimentionOfRawArray;
    }
    uint64_t *MetaDataObjectChannel::GetNumberOfRawDataValue()
    {
        return &numberOfRawDataValue;
    }
    uint32_t *MetaDataObjectChannel::GetNumberOfProperties()
    {
        return &numberOfProperties;
    }
    std::unordered_map<std::string, Property> &MetaDataObjectChannel::GetProperties()
    {
        return properties;
    }

    DataObject &MetaDataObjectChannel::GetDataObject()
    {
        return data;
    }

    DataObject::DataObject()
    {
    }

    DataObject::DataObject(uint32_t tdsType, const int8_t *data)
    {
        dataPtr.i8 = data;
    }
    DataObject::DataObject(uint32_t tdsType, const uint8_t *data)
    {
        dataPtr.u8 = data;
    }
    DataObject::DataObject(uint32_t tdsType, const int16_t *data)
    {
        dataPtr.i16 = data;
    }
    DataObject::DataObject(uint32_t tdsType, const uint16_t *data)
    {
        dataPtr.u16 = data;
    }
    DataObject::DataObject(uint32_t tdsType, const int32_t *data)
    {
        dataPtr.i32 = data;
    }
    DataObject::DataObject(uint32_t tdsType, const uint32_t *data)
    {
        dataPtr.u32 = data;
    }
    DataObject::DataObject(uint32_t tdsType, const int64_t *data)
    {
        dataPtr.i64 = data;
    }
    DataObject::DataObject(uint32_t tdsType, const uint64_t *data)
    {
        dataPtr.u64 = data;
    }
    DataObject::DataObject(uint32_t tdsType, const char *data)
    {
        dataPtr.c = data;
    }
    DataObject::DataObject(uint32_t tdsType, const bool *data)
    {
        dataPtr.b = data;
    }
    DataObject::DataObject(uint32_t tdsType, const double *data)
    {
        dataPtr.d = data;
    }
    DataObject::DataObject(uint32_t tdsType, const float *data)
    {
        dataPtr.f = data;
    }
    DataObject::DataObject(uint32_t tdsType, const tdms::Timestamp *data)
    {
        dataPtr.ts = data;
    }

    const int8_t *DataObject::GetDataI8()
    {
        return dataPtr.i8;
    }
    const uint8_t *DataObject::GetDataU8()
    {
        return dataPtr.u8;
    }
    const int16_t *DataObject::GetDataI16()
    {
        return dataPtr.i16;
    }
    const uint16_t *DataObject::GetDataU16()
    {
        return dataPtr.u16;
    }
    const int32_t *DataObject::GetDataI32()
    {
        return dataPtr.i32;
    }
    const uint32_t *DataObject::GetDataU32()
    {
        return dataPtr.u32;
    }
    const int64_t *DataObject::GetDataI64()
    {
        return dataPtr.i64;
    }
    const uint64_t *DataObject::GetDataU64()
    {
        return dataPtr.u64;
    }
    const char *DataObject::GetDataChar()
    {
        return dataPtr.c;
    }
    const bool *DataObject::GetDataBool()
    {
        return dataPtr.b;
    }
    const double *DataObject::GetDataDouble()
    {
        return dataPtr.d;
    }
    const float *DataObject::GetDataFloat()
    {
        return dataPtr.f;
    }
    const tdms::Timestamp *DataObject::GetDataTimestamp()
    {
        return dataPtr.ts;
    }

} // tdms
