#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

#include <string>
#include <stdio.h>

#include "tdmsDataStruct.h"

namespace tdms
{

    class TDMSFileHandler
    {
    public:
        TDMSFileHandler(std::string fileName);
        ~TDMSFileHandler();
        void WriteMetaDataOnly(tdms::TDMSDataStruct *tdmsData);
        void WriteChannelsWithRawData(tdms::TDMSDataStruct *tdmsData);
        void WriteRawDataOnly(tdms::TDMSDataStruct *tdmsData);

    private:
        void OpenFile();
        void OpenFile(std::string fileName);
        void CloseFile();

        void Write(); // Что райт? какие сделать аргументы
        void WriteLeadIn(tdms::TDMSDataStruct *tdmsData);
        void WriteMeta(tdms::TDMSDataStruct *tdmsData);

        void WriteMetaFile(tdms::TDMSDataStruct *tdmsData);
        void WriteMetaGroups(tdms::TDMSDataStruct *tdmsData);
        void WriteMetaChannels(tdms::TDMSDataStruct *tdmsData);
        void WriteRawData(tdms::TDMSDataStruct *tdmsData);
        void WriteMetaProperties(std::unordered_map<std::string,
                                                    tdms::Property> &properties);

    private:
        FILE *file;
    };

} // tdms

#endif // FILE_HANDLER_H
