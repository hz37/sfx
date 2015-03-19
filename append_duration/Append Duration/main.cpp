/***************************************************************
 * Name:      Append Duration
 * Purpose:   Append duration to the descriptions of sound effects for SFX
 * Author:    Hens Zimmerman (henszimmerman@gmail.com)
 * Created:   2014 - 2015
 * Copyright: Hens Zimmerman
 * License:   The MIT License (MIT)
 **************************************************************/

//---------------------------------------------------------------------------

#include <windows.h>

#include <cstdio>
#include <fstream>
#include <iostream>
#include <memory>
#include <sndfile.h>
#include <string>
#include <time.h>

//---------------------------------------------------------------------------

bool createDurationTag(const std::string filename, std::string& durationTag);
bool fileExists(const std::string path);
std::string includeTrailingBackslash(const std::string folder);
bool isDirectory(const std::string folder);

//---------------------------------------------------------------------------

/** \brief Create a duration tag from input file. E.g. __12:34
 *
 * \param filename Path to *.wav file to read duration from
 * \param durationTag Tag to update.
 * \return True if successful.
 */

bool createDurationTag(const std::string filename, std::string& durationTag)
{
    SF_INFO sf_info;
    sf_info.format = 0;

    SNDFILE* snd_file = sf_open(filename.c_str(), SFM_READ, &sf_info);

    if(snd_file)
    {
        int seconds = sf_info.frames / sf_info.samplerate;

        if(!seconds)
        {
            /** Avoid 0 seconds duration; we round that to 1 */

            seconds = 1;
        }

        char buffer[16];
        sprintf(buffer, "__%.02d:%.02d", seconds / 60, seconds % 60);

        durationTag = buffer;

        sf_close(snd_file);

        return true;
    }

    return false;
}

//---------------------------------------------------------------------------

/** \brief Check if file exists.
 *
 * \param path Path to file to check for existence.
 * \return True if path exists as a file.
 */

bool fileExists(const std::string path)
{
    DWORD dwAttrib = GetFileAttributes(path.c_str());

    return  (dwAttrib != INVALID_FILE_ATTRIBUTES &&
            !(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}

//---------------------------------------------------------------------------

/** \brief Helper function to make sure paths end with a backslash.
 *
 * \param folder Folder to return with a backslash.
 * \return Input folder with a guaranteed backslash
 */

std::string includeTrailingBackslash(const std::string folder)
{
    size_t len = folder.length();

    if(!len || folder[len - 1] == '\\')
    {
        return folder;
    }

    return folder + "\\";
}

//---------------------------------------------------------------------------

/** \brief Return true if input is an existing directory.
 *
 * \param folder Input to check for existence.
 * \return True if folder exists.
 */

bool isDirectory(const std::string folder)
{
    DWORD fileType = GetFileAttributesA(folder.c_str());

    if(fileType == INVALID_FILE_ATTRIBUTES)
    {
        return false;  //something is wrong with your path!
    }

    if(fileType & FILE_ATTRIBUTE_DIRECTORY)
    {
        return true;   // this is a directory!
    }

    return false;    // this is not a directory!}
}

//---------------------------------------------------------------------------

int main()
{
    /** Read sfx data directory from config file. */

    std::ifstream configFile("config.txt");

    if(!configFile)
    {
        std::cout << "Can't open config.txt. Run this program in the same directory as sfx.exe." << std::endl;

        return 0;
    }

    std::string dataDirectory;
    std::getline(configFile, dataDirectory);

    configFile.close();

    if(!isDirectory(dataDirectory))
    {
        std::cout   << "Illegal directory read from config.txt. ["
                    << dataDirectory
                    << "]. Check if drive is available/mounted."
                    << std::endl;

        return 0;
    }

    /** Rename data.txt (which will then become a backup). */

    std::string dataName = includeTrailingBackslash(dataDirectory) + "data.txt";
    std::string backupName = includeTrailingBackslash(dataDirectory) + "data_prior_to_duration_appending.txt";

    /** Check if data file exists. */

    if(!fileExists(dataName))
    {
        std::cout << dataName << " does not exist. Fix that first." << std::endl;

        return 0;
    }

    /** Delete existing backup silently. */

    if(fileExists(backupName))
    {
        ::DeleteFile(backupName.c_str());
    }

    int result = std::rename(dataName.c_str(), backupName.c_str());

    if(result)
    {
        std::cout << "Can't rename " << dataName << ". Aborting, sorry it didn't work out." << std::endl;

        return 0;
    }

    /** Open datafile for reading. */

    std::ifstream inputFile(backupName.c_str());

    if(!inputFile)
    {
        std::cout << "Can't open " << backupName << ". Aborting. " << std::endl;

        return 0;
    }

    /** Open new datafile for writing. */

    std::ofstream outputFile(dataName.c_str());

    if(!outputFile)
    {
        std::cout << "Can't create " << dataName << ". Aborting. " << std::endl;

        inputFile.close();

        return 0;
    }

    /** Ignore the first line of the input file. */

    std::string ignore;

    std::getline(inputFile, ignore);

    /** Write the first line of the output file. */

    time_t rawtime;
    struct tm* timeinfo;
    char timeStamp[80];

    time (&rawtime);
    timeinfo = localtime (&rawtime);

    strftime (timeStamp, sizeof(timeStamp), "%Y-%m-%d %H:%M", timeinfo);

    outputFile << "# Machine generated file. Appended durations. " << timeStamp << std::endl;

    /** Scan through input file and append durations to output file. */

    std::string description;

    while(std::getline(inputFile, description))
    {
        std::string shortFileName;

        /** We are guessing the datafile's integrity is good, so the next line exists. */

        std::getline(inputFile, shortFileName);

        /** Figure out duration of this file. */

        std::string durationTag;

        if(createDurationTag(includeTrailingBackslash(dataDirectory) + shortFileName, durationTag))
        {
            /** Append it to the description. */

            description += durationTag;
        }

        /** Write both lines back to the new data file. */

        std::cout << description << std::endl;

        outputFile << description << std::endl << shortFileName << std::endl;
    }

    /** Done with the files. */

    inputFile.close();
    outputFile.close();

    return 0;
}

//---------------------------------------------------------------------------
