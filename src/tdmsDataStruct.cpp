#include "tdmsDataStruct.h"

using namespace std;

namespace tdms
{

    TDMSDataStruct::TDMSDataStruct()
    {
        metaData.numberOfOjects = 0;
    }

    char *TDMSDataStruct::GetData()
    {
        return data;
    }

    void TDMSDataStruct::SetFile(string name, string description)
    {
        metaFile = MetaDataObjectFile(name, description);
    }
    void TDMSDataStruct::SetGroup(std::string name, std::string description)
    {
        metaGroupObjects.insert({name, {name, description}});
    }
    void TDMSDataStruct::SetChannel(std::string groupName,
                                    std::string channelName,
                                    uint32_t tdsType,
                                    uint32_t numOfRawDataValue)
    {
        metaChannelObjects.insert({groupName + "/" + channelName, {groupName, channelName, tdsType, numOfRawDataValue}});
    }

    void TDMSDataStruct::PrepareToFirstWrite()
    {
        leadIn.SegmentContainMetaData();
        leadIn.SegmentContainsNewObject();
        leadIn.nextSegmentOffset = 0;
        // leadIn.rawDataOffset = leadIn.nextSegmentOffset;
        metaData.fileObject = &metaFile;

        leadIn.nextSegmentOffset = 4 + metaData.fileObject->Size(); // numberOfObjs

        metaData.numberOfOjects = 1 + metaGroupObjects.size() + metaChannelObjects.size();
        for (auto &m : metaGroupObjects)
        {
            metaData.groupObjects.push_back(&(m.second));
            leadIn.nextSegmentOffset += m.second.Size();
        }
        for (auto &m : metaChannelObjects)
        {
            metaData.channelObjects.push_back(&(m.second));
            leadIn.nextSegmentOffset += m.second.Size();
        }
        leadIn.rawDataOffset = leadIn.nextSegmentOffset;
    }

    // Рассчет при условии, что структура путей и их метаданных
    // меняться не будет, а будут меняться лишь их значения
    void TDMSDataStruct::PrepareToMetaChannelsAndRawData()
    {

        metaData.groupObjects.clear();
        metaData.channelObjects.clear();

        leadIn.tocMask = 0;
        leadIn.SegmentContainMetaData();
        leadIn.SegmentContainRawData();
        leadIn.nextSegmentOffset = 4; // numberOfObjs

        metaData.numberOfOjects = metaChannelObjects.size();
        size_t tmpSize = 0;
        size_t tmp = 0;
        for (auto &m : metaChannelObjects)
        {
            metaData.channelObjects.push_back(&(m.second));
            leadIn.nextSegmentOffset += m.second.Size();
            tmp = *m.second.GetNumberOfRawDataValue();
            switch (*m.second.GetDataTypeOfRaw())
            {
            case (tdsTypeI8):
                tmp *= 1;
                break;
            case (tdsTypeU8):
                tmp *= 1;
                break;
            case (tdsTypeI16):
                tmp *= 2;
                break;
            case (tdsTypeU16):
                tmp *= 2;
                break;
            case (tdsTypeI32):
                tmp *= 4;
                break;
            case (tdsTypeU32):
                tmp *= 4;
                break;
            case (tdsTypeI64):
                tmp *= 8;
                break;
            case (tdsTypeU64):
                tmp *= 8;
                break;
            case (tdsTypeBoolean):
                tmp *= 1;
                break;
            case (tdsTypeSingleFloat):
                tmp *= sizeof(float);
                break;
            case (tdsTypeDoubleFloat):
                tmp *= sizeof(double);
                break;
            case (tdsTypeString):
                tmp *= 1;
                break;
            }
            tmpSize += tmp;
        }

        leadIn.rawDataOffset = leadIn.nextSegmentOffset;
        leadIn.nextSegmentOffset += tmpSize;
    }

    // Рассчет при условии, что структура путей и их метаданных
    // меняться не будет, use before GenerateRawData()
    void TDMSDataStruct::PrepareForRawDataOnly()
    {
        metaData.groupObjects.clear();
        metaData.channelObjects.clear();

        leadIn.tocMask = 0;
        leadIn.SegmentContainRawData();

        leadIn.nextSegmentOffset = 0;
        metaData.numberOfOjects = 0;

        size_t tmpSize = 0;
        size_t tmp = 0;
        for (auto &m : metaChannelObjects)
        {
            metaData.channelObjects.push_back(&(m.second));
            leadIn.nextSegmentOffset += m.second.Size();
            tmp = *m.second.GetNumberOfRawDataValue();
            switch (*m.second.GetDataTypeOfRaw())
            {
            case (tdsTypeI8):
                tmp *= 1;
                break;
            case (tdsTypeU8):
                tmp *= 1;
                break;
            case (tdsTypeI16):
                tmp *= 2;
                break;
            case (tdsTypeU16):
                tmp *= 2;
                break;
            case (tdsTypeI32):
                tmp *= 4;
                break;
            case (tdsTypeU32):
                tmp *= 4;
                break;
            case (tdsTypeI64):
                tmp *= 8;
                break;
            case (tdsTypeU64):
                tmp *= 8;
                break;
            case (tdsTypeBoolean):
                tmp *= 1;
                break;
            case (tdsTypeSingleFloat):
                tmp *= sizeof(float);
                break;
            case (tdsTypeDoubleFloat):
                tmp *= sizeof(double);
                break;
            case (tdsTypeString):
                tmp *= 1;
                break;
            }
            tmpSize += tmp;
        }

        leadIn.rawDataOffset = leadIn.nextSegmentOffset;
        leadIn.nextSegmentOffset += tmpSize;
    }

    void LeadIn::SegmentContainMetaData()
    {
        tocMask |= (1 << 1);
    }
    void LeadIn::SegmentContainRawData()
    {
        tocMask |= (1 << 3);
    }
    void LeadIn::SegmentContainDAQmxRawData()
    {
        tocMask |= (1 << 7);
    }
    void LeadIn::RawDataInSegmentIsInterleaved()
    {
        tocMask |= (1 << 5);
    }
    // dont use this gunc
    void LeadIn::BigEnding()
    {
        tocMask |= (1 << 6);
    }
    void LeadIn::SegmentContainsNewObject()
    {
        tocMask |= (1 << 2);
    }

} // tdms
