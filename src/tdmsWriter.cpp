#include "tdmsWriter.h"

namespace tdms
{

    /// @brief
    /// @param filePath - Path of file
    /// @param fileName - Name property of file
    /// @param fileDescription - Description property of file
    TDMSWriter::TDMSWriter(const std::string filePath,
                           const std::string fileName,
                           const std::string fileDescription)
    {
        fileHandlerPtr = std::make_shared<TDMSFileHandler>(filePath);
        tdmsStruct.SetFile(fileName, fileDescription);
    }

    TDMSWriter::~TDMSWriter()
    {
        fileHandlerPtr.reset();
    }

    void TDMSWriter::AddGroup(const std::string name, const std::string description)
    {
        tdmsStruct.SetGroup(name, description);
    }

    void TDMSWriter::AddChannelInGroup(const std::string groupName,
                                       const std::string name,
                                       const uint32_t tdsType,
                                       const uint32_t numOfRawDataValue)
    {
        tdmsStruct.SetChannel(groupName, name, tdsType, numOfRawDataValue);
    }

    // leadIn + metaData of file,group,channel
    void TDMSWriter::GenerateFirstFilePart()
    {
        fileHandlerPtr->WriteMetaDataOnly(&tdmsStruct);
    }

    void TDMSWriter::GenerateChannelsWithRawData()
    {
        // raw data
        fileHandlerPtr->WriteChannelsWithRawData(&tdmsStruct);
    }

    void TDMSWriter::ConfigureToRawData()
    {
        tdmsStruct.PrepareForRawDataOnly();
    }

    void TDMSWriter::GenerateRawData()
    {
        fileHandlerPtr->WriteRawDataOnly(&tdmsStruct);
    }

} // tdms
