#ifndef TDMS_WRITER_H
#define TDMS_WRITER_H

#include <memory>
#include <string>
#include <unordered_map>

#include "fileHandler.h"
// #include "tdmsParts.h"
// #include "tdmsFile.h"
// #include "tdmsGroup.h"
#include "tdmsDataStruct.h"

namespace tdms
{

    class TDMSWriter
    {
    public:
        TDMSWriter(const std::string filePath,
                   const std::string fileName,
                   const std::string fileDescription);
        ~TDMSWriter();

        void AddGroup(const std::string name, const std::string description);
        void AddChannelInGroup(const std::string groupName,
                               const std::string name,
                               const uint32_t tdsType,
                               const uint32_t numOfRawDataValue);

        template <typename T>
        void CreateFileProperty(const std::string name,
                                const uint32_t tdsType,
                                const T value);

        template <typename T>
        void CreateGroupProperty(const std::string groupName,
                                 const std::string name,
                                 const uint32_t tdsType,
                                 const T value);

        template <typename T>
        void CreateChannelProperty(const std::string groupName,
                                   const std::string channelName,
                                   const std::string name,
                                   const uint32_t tdsType,
                                   const T value);

        template <typename T>
        void AddRawDataInChannel(std::string groupName,
                                 std::string channelName,
                                 const T *data);

        void GenerateFirstFilePart();
        void GenerateChannelsWithRawData();
        void GenerateRawData();

        void ConfigureToRawData();

    private:
        std::shared_ptr<TDMSFileHandler> fileHandlerPtr;

    private:
        const size_t LeadInLength = 28;

    private:
        TDMSDataStruct tdmsStruct;
    };

    template <typename T>
    void TDMSWriter::CreateFileProperty(const std::string name,
                                        const uint32_t tdsType,
                                        const T value)
    {
        tdmsStruct.metaFile.SetProperty(name, tdsType, value);
    }

    template <typename T>
    void TDMSWriter::CreateGroupProperty(const std::string groupName,
                                         const std::string name,
                                         const uint32_t tdsType,
                                         const T value)
    {
        tdmsStruct.metaGroupObjects.at(groupName).SetProperty(name, tdsType, value);
    }

    template <typename T>
    void TDMSWriter::CreateChannelProperty(const std::string groupName,
                                           const std::string channelName,
                                           const std::string name,
                                           const uint32_t tdsType,
                                           const T value)
    {
        tdmsStruct.metaChannelObjects.at(groupName + "/" + channelName).SetProperty(name, tdsType, value);
    }

    template <typename T>
    void TDMSWriter::AddRawDataInChannel(std::string groupName,
                                         std::string channelName,
                                         const T *data)
    {
        tdmsStruct.SetData(groupName, channelName, data);
    }

} // tdms

#endif // TDMS_WRITER_H
