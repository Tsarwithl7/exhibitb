#pragma once
#include <dcmtk/config/osconfig.h>
#include <dcmtk/dcmdata/dctk.h>
#include <iostream>

class LoadInformation {
private:

public:
    DcmFileFormat fileformat;
    
    bool loadDICOMFile(const std::string& filePath) {
        OFCondition status = fileformat.loadFile(filePath.c_str());
        if (!status.good()) {
            std::cout << "Load DICOM File Error: " << status.text() << std::endl;
            return false;
        }

        // 加载所有数据到内存
        status = fileformat.loadAllDataIntoMemory();
        if (!status.good()) {
            std::cout << "Load DICOM Data Error: " << status.text() << std::endl;
            return false;
        }

        return true;
    }


    // 读取患者姓名
        std::string readName(DcmFileFormat& fileformat, const std::string& filePath)
    {
        OFCondition status = fileformat.loadFile(filePath.c_str());
        if (!status.good())
        {
            std::cout << "Load DICOM File Error: " << status.text() << std::endl;
            return "";
        }

        DcmDataset* dataset = fileformat.getDataset();
        if (!dataset->tagExistsWithValue(DCM_PatientName))
        {
            std::cout << "Patient Name not found in DICOM file." << std::endl;
            return "";
        }

        OFString patientName;
        status = dataset->findAndGetOFString(DCM_PatientName, patientName);
        if (status.good())
        {
            return std::string(patientName.c_str());
        }
        else
        {
            std::cout << "Error getting Patient Name: " << status.text() << std::endl;
            return "";
        }
    }

    // 读取患者年龄
        std::string readAge(DcmFileFormat& fileformat, const std::string& filePath)
        {
            OFCondition status = fileformat.loadFile(filePath.c_str());
            if (!status.good())
            {
                std::cout << "Load DICOM File Error: " << status.text() << std::endl;
                return "";
            }

            DcmDataset* dataset = fileformat.getDataset();
            if (!dataset->tagExistsWithValue(DCM_PatientAge))
            {
                std::cout << "Patient Age not found in DICOM file." << std::endl;
                return "";
            }

            OFString patientAge;
            status = dataset->findAndGetOFString(DCM_PatientAge, patientAge);
            if (status.good())
            {
                return std::string(patientAge.c_str());
            }
            else
            {
                std::cout << "Error getting Patient Age: " << status.text() << std::endl;
                return "";
            }
        }

    // 读取患者性别
        std::string readSex(DcmFileFormat& fileformat, const std::string& filePath)
        {
            OFCondition status = fileformat.loadFile(filePath.c_str());
            if (!status.good())
            {
                std::cout << "Load DICOM File Error: " << status.text() << std::endl;
                return "";
            }

            DcmDataset* dataset = fileformat.getDataset();
            if (!dataset->tagExistsWithValue(DCM_PatientSex))
            {
                std::cout << "Patient Sex not found in DICOM file." << std::endl;
                return "";
            }

            OFString patientSex;
            status = dataset->findAndGetOFString(DCM_PatientSex, patientSex);
            if (status.good())
            {
                return std::string(patientSex.c_str());
            }
            else
            {
                std::cout << "Error getting Patient Sex: " << status.text() << std::endl;
                return "";
            }
        }

    
    //读取RIS号
        std::string readAccessionNumber(DcmFileFormat& fileformat, const std::string& filePath)
        {
            OFCondition status = fileformat.loadFile(filePath.c_str());
            if (!status.good())
            {
                std::cout << "Load DICOM File Error: " << status.text() << std::endl;
                return "";
            }

            DcmDataset* dataset = fileformat.getDataset();
            if (!dataset->tagExistsWithValue(DCM_AccessionNumber))
            {
                std::cout << "Accession Number not found in DICOM file." << std::endl;
                return "";
            }

            OFString accessionNumber;
            status = dataset->findAndGetOFString(DCM_AccessionNumber, accessionNumber);
            if (status.good())
            {
                return std::string(accessionNumber.c_str());
            }
            else
            {
                std::cout << "Error getting Accession Number: " << status.text() << std::endl;
                return "";
            }
        }
     
        //读取检查日期
        std::string readSeriesDate(DcmFileFormat& fileformat, const std::string& filePath)
        {
            OFCondition status = fileformat.loadFile(filePath.c_str());
            if (!status.good())
            {
                std::cout << "Load DICOM File Error: " << status.text() << std::endl;
                return "";
            }

            DcmDataset* dataset = fileformat.getDataset();
            if (!dataset->tagExistsWithValue(DCM_SeriesDate))
            {
                std::cout << "Series Date not found in DICOM file." << std::endl;
                return "";
            }

            OFString seriesDate;
            status = dataset->findAndGetOFString(DCM_SeriesDate, seriesDate);
            if (status.good())
            {
                return std::string(seriesDate.c_str());
            }
            else
            {
                std::cout << "Error getting Series Date: " << status.text() << std::endl;
                return "";
            }
        }

        //读取检查时间
        time_t readSeriesTime(DcmFileFormat& fileformat, const std::string& filePath)
{
    OFCondition status = fileformat.loadFile(filePath.c_str());
    if (!status.good())
    {
        std::cout << "Load DICOM File Error: " << status.text() << std::endl;
        return 0;
    }

    DcmDataset* dataset = fileformat.getDataset();
    if (!dataset->tagExistsWithValue(DCM_SeriesTime))
    {
        std::cout << "Series Time not found in DICOM file." << std::endl;
        return 0;
    }

    OFString seriesTime;
    status = dataset->findAndGetOFString(DCM_SeriesTime, seriesTime);
    if (status.good())
    {
        // Convert DICOM time string to time_t
        std::string timeStr(seriesTime.c_str());
        tm timeStruct = {};
        std::istringstream iss(timeStr);
        iss >> std::get_time(&timeStruct, "%H%M%S");
        time_t seriesTimeT = mktime(&timeStruct);
        return seriesTimeT;
    }
    else
    {
        std::cout << "Error getting Series Time: " << status.text() << std::endl;
        return 0;
    }
}


     //读取数据模态
        std::string readModality(DcmFileFormat& fileformat, const std::string& filePath)
        {
            OFCondition status = fileformat.loadFile(filePath.c_str());
            if (!status.good())
            {
                std::cout << "Load DICOM File Error: " << status.text() << std::endl;
                return "";
            }

            DcmDataset* dataset = fileformat.getDataset();
            if (!dataset->tagExistsWithValue(DCM_Modality))
            {
                std::cout << "Modality not found in DICOM file." << std::endl;
                return "";
            }

            OFString modality;
            status = dataset->findAndGetOFString(DCM_Modality, modality);
            if (status.good())
            {
                return std::string(modality.c_str());
            }
            else
            {
                std::cout << "Error getting Modality: " << status.text() << std::endl;
                return "";
            }
        }

    //读取病人检查描述
        std::string readDescription(DcmFileFormat& fileformat, const std::string& filePath)
        {
            OFCondition status = fileformat.loadFile(filePath.c_str());
            if (!status.good())
            {
                std::cout << "Load DICOM File Error: " << status.text() << std::endl;
                return "";
            }

            DcmDataset* dataset = fileformat.getDataset();
            if (!dataset->tagExistsWithValue(DCM_StudyDescription))
            {
                std::cout << "Study Description not found in DICOM file." << std::endl;
                return "";
            }

            OFString studyDescription;
            status = dataset->findAndGetOFString(DCM_StudyDescription, studyDescription);
            if (status.good())
            {
                return std::string(studyDescription.c_str());
            }
            else
            {
                std::cout << "Error getting Study Description: " << status.text() << std::endl;
                return "";
            }
        }
}; 