#ifndef TDMS_PARTS_H
#define TDMS_PARTS_H

#include <cstdint>
#include <string>
#include <vector>

#include <tdmsProperty.h>
#include <tdmsTimestamp.h>

namespace tdms
{

    class MetaDataObjectFile
    {
    public:
        MetaDataObjectFile();
        MetaDataObjectFile(std::string name, std::string description);
        uint32_t Size();

        template <typename T>
        void SetProperty(std::string name, uint32_t tdsType, T value)
        {
            isChanged = true;
            if (properties.count(name) == 0)
            {
                numberOfProperties += 1;
                properties.insert({name, {name, tdsType, value}});
                size += properties.at(name).Size();
                return;
            }
            size -= properties.at(name).Size();
            properties.at(name) = Property({name, tdsType, value});
            size += properties.at(name).Size();
        }

        std::string *GetPath();
        uint32_t *GetRawDataIndex();
        uint32_t *GetNumberOfProperties();
        std::unordered_map<std::string, Property> &GetProperties();
        bool ChangeIs();

    private:
        std::string name; // just for me and program logic
        uint32_t lengthOfPath;
        std::string path;
        uint32_t rawDataIndex;
        uint32_t numberOfProperties;
        std::unordered_map<std::string, Property> properties;
        uint32_t size;
        bool isChanged = false;
    };

    class MetaDataObjectGroup
    {
    public:
        MetaDataObjectGroup();
        MetaDataObjectGroup(std::string name, std::string description);
        bool operator==(const MetaDataObjectGroup &other);
        uint32_t Size();

        template <typename T>
        void SetProperty(std::string name, uint32_t tdsType, T value)
        {
            isChanged = true;
            if (properties.count(name) == 0)
            {
                numberOfProperties += 1;
                properties.insert({name, {name, tdsType, value}});
                size += properties.at(name).Size();
                return;
            }
            size -= properties.at(name).Size();
            properties.at(name) = Property({name, tdsType, value});
            size += properties.at(name).Size();
        }

        std::string *GetName();
        uint32_t *GetLengthOfPath();
        std::string *GetPath();
        uint32_t *GetRawDataIndex();
        uint32_t *GetNumberOfProperty();
        std::unordered_map<std::string, Property> &GetProperty();
        bool ChangedIs();

    private:
        std::string name; // just for me and program logic
        uint32_t lengthOfPath;
        std::string path;
        uint32_t rawDataIndex;
        uint32_t numberOfProperties;
        std::unordered_map<std::string, Property> properties;
        uint32_t size;
        bool isChanged = false;
    };

    // Only Channels
    class DataObject
    {
    public:
        DataObject();
        DataObject(uint32_t tdsType, const int8_t *data);
        DataObject(uint32_t tdsType, const uint8_t *data);
        DataObject(uint32_t tdsType, const int16_t *data);
        DataObject(uint32_t tdsType, const uint16_t *data);
        DataObject(uint32_t tdsType, const int32_t *data);
        DataObject(uint32_t tdsType, const uint32_t *data);
        DataObject(uint32_t tdsType, const int64_t *data);
        DataObject(uint32_t tdsType, const uint64_t *data);
        DataObject(uint32_t tdsType, const char *data);
        DataObject(uint32_t tdsType, const bool *data);
        DataObject(uint32_t tdsType, const double *data);
        DataObject(uint32_t tdsType, const float *data);
        DataObject(uint32_t tdsType, const tdms::Timestamp *data);

        const int8_t *GetDataI8();
        const uint8_t *GetDataU8();
        const int16_t *GetDataI16();
        const uint16_t *GetDataU16();
        const int32_t *GetDataI32();
        const uint32_t *GetDataU32();
        const int64_t *GetDataI64();
        const uint64_t *GetDataU64();
        const char *GetDataChar();
        const bool *GetDataBool();
        const double *GetDataDouble();
        const float *GetDataFloat();
        const tdms::Timestamp *GetDataTimestamp();

    private:
        uint32_t tdsType;

    public:
        union DataPtr
        {
            const int8_t *i8;
            const int16_t *i16;
            const int32_t *i32;
            const int64_t *i64;
            const uint8_t *u8;
            const uint16_t *u16;
            const uint32_t *u32;
            const uint64_t *u64;
            const float *f;
            const double *d;
            const char *c; // string
            const tdms::Timestamp *ts;
            const bool *b;
        };
        DataPtr dataPtr;
    };

    class MetaDataObjectChannel
    {
    public:
        MetaDataObjectChannel();
        MetaDataObjectChannel(std::string groupName,
                              std::string name,
                              uint32_t tdstype,
                              uint32_t numOfRawDataValue);
        bool operator==(const MetaDataObjectChannel &other);
        uint32_t Size();
        size_t GetNumOfRawDataValue();

        template <typename T>
        void SetData(const T *data)
        {
            this->data = DataObject(dataTypeOfRaw, data);
        }

        template <typename T>
        void SetProperty(std::string name, uint32_t tdsType, T value)
        {
            isChanged = true;
            if (properties.count(name) == 0)
            {
                numberOfProperties += 1;
                properties.insert({name, {name, tdsType, value}});
                size += properties.at(name).Size();
                return;
            }
            size -= properties.at(name).Size();
            properties.at(name) = Property({name, tdsType, value});
            size += properties.at(name).Size();
        }

        uint32_t *GetLengthOfPath();
        std::string *GetPath();
        uint32_t *GetLengthOfIndexInfo();
        uint32_t *GetDataTypeOfRaw();
        uint32_t *GetDimentionOfRawArray();
        uint64_t *GetNumberOfRawDataValue();
        uint32_t *GetNumberOfProperties();
        std::unordered_map<std::string, Property> &GetProperties();
        DataObject &GetDataObject();

    private:
        std::string name;      // just for me and program logic
        std::string groupName; // just for me and program logic
        uint32_t lengthOfPath;
        std::string path;

        uint32_t lengthOfIndexInfo; // is size of 4 fields below

        uint32_t dataTypeOfRaw;
        uint32_t dimentionOfRawArray; // must be 1
        uint64_t numberOfRawDataValue;

        uint32_t numberOfProperties;
        std::unordered_map<std::string, Property> properties;

        uint32_t size;

        bool isChanged = false;

    private: // raw data
        DataObject data;
    };

} // tdms

#endif // TDMS_PARTS_H
