#ifndef TDMS_DATA_STRUCT_H
#define TDMS_DATA_STRUCT_H

#include <cstdint>
#include <vector>
#include <unordered_map>

#include "tdmsParts.h"

#define VERSION_NUMBER 4713

namespace tdms
{

    struct LeadIn
    {
        const char *tag = "TDSm";
        uint32_t tocMask = 0;
        const uint32_t version = VERSION_NUMBER;
        uint64_t nextSegmentOffset;
        uint64_t rawDataOffset;

        void SegmentContainMetaData();
        void SegmentContainRawData();
        void SegmentContainDAQmxRawData();
        void RawDataInSegmentIsInterleaved();
        void BigEnding();
        void SegmentContainsNewObject();
    };

    struct MetaData
    {
        uint32_t numberOfOjects; // file + groups + channels count
        MetaDataObjectFile *fileObject;
        std::vector<MetaDataObjectGroup *> groupObjects;     // file + group
        std::vector<MetaDataObjectChannel *> channelObjects; // objects only
    };

    class TDMSDataStruct
    {
    public:
        TDMSDataStruct();
        char *GetData();
        void SetFile(std::string name, std::string description);
        void SetGroup(std::string name, std::string description);
        void SetChannel(std::string groupName,
                        std::string channelName,
                        uint32_t tdsType,
                        uint32_t numOfRawDataValue);

        template <typename T>
        void SetData(std::string groupName,
                     std::string channelName,
                     const T *data);

        void PrepareToFirstWrite();
        void PrepareToMetaChannelsAndRawData();
        void PrepareForRawDataOnly();
        void PrepareToWrite();

    public:
        LeadIn leadIn;
        MetaData metaData;
        // RawData rawData;
        char *data;

    public:
        MetaDataObjectFile metaFile;
        std::unordered_map<std::string, MetaDataObjectGroup> metaGroupObjects;     // key: groupName
        std::unordered_map<std::string, MetaDataObjectChannel> metaChannelObjects; // key: groupName + "/" + channelName
    };

    template <typename T>
    void TDMSDataStruct::SetData(std::string groupName,
                                 std::string channelName,
                                 const T *data)
    {
        metaChannelObjects.at(groupName + "/" + channelName).SetData(data);
    }

} // tdms

#endif // TDMS_DATA_STRUCT_H
