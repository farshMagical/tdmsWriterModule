#include "fileHandler.h"
#include "tdmsProperty.h"

namespace tdms
{

    TDMSFileHandler::TDMSFileHandler(std::string fileName)
    {
        OpenFile(fileName);
    }

    TDMSFileHandler::~TDMSFileHandler()
    {
        CloseFile();
    }

    void TDMSFileHandler::OpenFile()
    {
        file = fopen("kek.tdms", "wb");
        if (!file)
        {
            std::cout << "kek.tdms can't be open" << std::endl;
            throw;
        }
    }
    void TDMSFileHandler::OpenFile(std::string fileName)
    {
        // fileName += ".tdms";
        file = fopen(fileName.data(), "wb");
    }
    void TDMSFileHandler::CloseFile()
    {
        fclose(file);
    }

    void TDMSFileHandler::WriteMetaDataOnly(tdms::TDMSDataStruct *tdmsData)
    {
        tdmsData->PrepareToFirstWrite();
        WriteLeadIn(tdmsData);
        WriteMeta(tdmsData);
        tdmsData->PrepareToMetaChannelsAndRawData();
    }

    void TDMSFileHandler::WriteChannelsWithRawData(tdms::TDMSDataStruct *tdmsData)
    {
        WriteLeadIn(tdmsData);
        fwrite(&tdmsData->metaData.numberOfOjects, 4, 1, file);
        WriteMetaChannels(tdmsData);
        WriteRawData(tdmsData);
    }

    void TDMSFileHandler::WriteRawDataOnly(tdms::TDMSDataStruct *tdmsData)
    {
        WriteLeadIn(tdmsData);
        WriteRawData(tdmsData);
        RewriteNextSegmentOffsetValue(tdmsData);
    }

    void TDMSFileHandler::RewriteNextSegmentOffsetValue(tdms::TDMSDataStruct *tdmsData)
    {
        fseek(file, nextSegmentOffsetPosition, SEEK_SET);
        fwrite(&tdmsData->leadIn.nextSegmentOffset + nextSegmentOffsetPositionOffset,
               8, 1, file);
        fseek(file, 0, SEEK_END);
    }

    void TDMSFileHandler::WriteLeadIn(tdms::TDMSDataStruct *tdmsData)
    {
        fwrite(tdmsData->leadIn.tag, 1, 4, file);
        fwrite(&tdmsData->leadIn.tocMask, 4, 1, file);
        fwrite(&tdmsData->leadIn.version, 4, 1, file);
        nextSegmentOffsetPosition = ftell(file); // save position of nextSegmentOffset()
        fwrite(&tdmsData->leadIn.nextSegmentOffset, 8, 1, file);
        fwrite(&tdmsData->leadIn.rawDataOffset, 8, 1, file);
    }
    void TDMSFileHandler::WriteMeta(tdms::TDMSDataStruct *tdmsData)
    {
        fwrite(&tdmsData->metaData.numberOfOjects, 4, 1, file);
        WriteMetaFile(tdmsData);
        WriteMetaGroups(tdmsData);
        WriteMetaChannels(tdmsData);
    }

    void TDMSFileHandler::WriteMetaFile(tdms::TDMSDataStruct *tdmsData)
    {

        uint32_t tmpSize = 1;
        fwrite(&tmpSize, 4, 1, file);
        fwrite(tdmsData->metaFile.GetPath()->data(), 1, 1, file);
        fwrite(tdmsData->metaFile.GetRawDataIndex(), 4, 1, file);
        fwrite(tdmsData->metaFile.GetNumberOfProperties(), 4, 1, file);

        WriteMetaProperties(tdmsData->metaFile.GetProperties());
    }

    void TDMSFileHandler::WriteMetaGroups(tdms::TDMSDataStruct *tdmsData)
    {

        for (auto &m : tdmsData->metaData.groupObjects)
        {
            fwrite(m->GetLengthOfPath(), 4, 1, file);
            auto tmp = m->GetPath();
            fwrite(tmp->data(), 1, tmp->size(), file);
            fwrite(m->GetRawDataIndex(), 4, 1, file);
            fwrite(m->GetNumberOfProperty(), 4, 1, file);

            WriteMetaProperties(m->GetProperty());
        }
    }

    void TDMSFileHandler::WriteMetaChannels(tdms::TDMSDataStruct *tdmsData)
    {
        for (auto &m : tdmsData->metaData.channelObjects)
        {
            fwrite(m->GetLengthOfPath(), 4, 1, file);
            auto tmp = m->GetPath();
            fwrite(tmp->data(), 1, tmp->size(), file);
            fwrite(m->GetLengthOfIndexInfo(), 4, 1, file);
            fwrite(m->GetDataTypeOfRaw(), 4, 1, file);
            fwrite(m->GetDimentionOfRawArray(), 4, 1, file);
            fwrite(m->GetNumberOfRawDataValue(), 8, 1, file);
            fwrite(m->GetNumberOfProperties(), 4, 1, file);
            WriteMetaProperties(m->GetProperties());
        }
    }

    void TDMSFileHandler::WriteRawData(tdms::TDMSDataStruct *tdmsData)
    {
        uint64_t bytes = 0;
        for (auto &m : tdmsData->metaData.channelObjects)
        {
            switch (*m->GetDataTypeOfRaw())
            {
            case (tdsTypeBoolean):
                fwrite(m->GetDataObject().GetDataBool(),
                       1,
                       *m->GetNumberOfRawDataValue(),
                       file);
                bytes += 1 * *m->GetNumberOfRawDataValue();
                break;

            case (tdsTypeI8):
                fwrite(m->GetDataObject().GetDataI8(),
                       1,
                       *m->GetNumberOfRawDataValue(),
                       file);
                bytes += 1 * *m->GetNumberOfRawDataValue();
                break;
            case (tdsTypeU8):
                fwrite(m->GetDataObject().GetDataU8(),
                       1,
                       *m->GetNumberOfRawDataValue(),
                       file);
                bytes += 1 * *m->GetNumberOfRawDataValue();
                break;
            case (tdsTypeI16):
                fwrite(m->GetDataObject().GetDataI16(),
                       2,
                       *m->GetNumberOfRawDataValue(),
                       file);
                bytes += 2 * *m->GetNumberOfRawDataValue();
                break;
            case (tdsTypeU16):
                fwrite(m->GetDataObject().GetDataI16(),
                       2,
                       *m->GetNumberOfRawDataValue(),
                       file);
                bytes += 2 * *m->GetNumberOfRawDataValue();
                break;

            case (tdsTypeI32):
                fwrite(m->GetDataObject().GetDataI32(),
                       4,
                       *m->GetNumberOfRawDataValue(),
                       file);
                bytes += 4 * *m->GetNumberOfRawDataValue();
                break;
            case (tdsTypeU32):
                fwrite(m->GetDataObject().GetDataU32(),
                       4,
                       *m->GetNumberOfRawDataValue(),
                       file);
                bytes += 4 * *m->GetNumberOfRawDataValue();
                break;
            case (tdsTypeI64):
                fwrite(m->GetDataObject().GetDataI64(),
                       8,
                       *m->GetNumberOfRawDataValue(),
                       file);
                bytes += 8 * *m->GetNumberOfRawDataValue();
                break;
            case (tdsTypeU64):
                fwrite(m->GetDataObject().GetDataU64(),
                       8,
                       *m->GetNumberOfRawDataValue(),
                       file);
                bytes += 8 * *m->GetNumberOfRawDataValue();
                break;
            case (tdsTypeString):
                fwrite(m->GetDataObject().GetDataChar(),
                       1,
                       *m->GetNumberOfRawDataValue(),
                       file);
                bytes += *m->GetNumberOfRawDataValue();
                break;
            case (tdsTypeSingleFloat):
                fwrite(m->GetDataObject().GetDataFloat(),
                       sizeof(float),
                       *m->GetNumberOfRawDataValue(),
                       file);
                bytes += sizeof(float) * *m->GetNumberOfRawDataValue();
                break;
            case (tdsTypeDoubleFloat):
                fwrite(m->GetDataObject().GetDataDouble(),
                       sizeof(double),
                       *m->GetNumberOfRawDataValue(),
                       file);
                bytes += sizeof(double) * *m->GetNumberOfRawDataValue();
                break;
                // case (tdsTypeTimeStamp): // TODO
                //     fwrite(m->GetDataObject().GetDataTimestamp(),
                //            1,
                //            m->GetDataObject().GetDataTimestamp()->Size(),
                //            file);
            }
        }
        nextSegmentOffsetPositionOffset += bytes;
    }

    void TDMSFileHandler::WriteMetaProperties(std::unordered_map<std::string,
                                                                 tdms::Property> &properties)
    {
        for (auto &p : properties)
        {
            fwrite(p.second.GetNameLength(), 4, 1, file);
            fwrite(p.second.GetName(), 1, *p.second.GetNameLength(), file);
            fwrite(p.second.GetDataTypeOfProperty(), 4, 1, file);
            if (*p.second.GetDataTypeOfProperty() == tdsTypeString)
            {
                fwrite(p.second.GetLengthStringProperty(), 4, 1, file);
                fwrite(p.second.GetValueString()->data(), 1, p.second.GetValueString()->size(), file);
            }
            else
            {
                fwrite(p.second.GetValue(), 1, p.second.GetDataSizeIfNotString(), file);
            }
        }
    }

} // tdms
