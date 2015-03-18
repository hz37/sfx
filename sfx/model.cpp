#include "model.h"

#include <wx/stdpaths.h>
#include <wx/filefn.h>
#include <wx/textfile.h>
#include <wx/tokenzr.h>
#include <wx/filename.h>
#include <wx/volume.h>

#include "utility.h"

const wxString c_configFile = "config.txt";
const wxString c_dataFile = "data.txt";

model::model() :
    m_andSearch(true),
    m_dataError(false),
    m_dataSize(0),
    m_searchSize(0)
{
    // Load data from exe directory.

    wxStandardPaths sp = wxStandardPaths::Get();

    wxString exePath = Utility::includeTrailingBackslash(wxPathOnly(sp.GetExecutablePath()));
    wxString configFilename = exePath + c_configFile;

    if(wxFileExists(configFilename))
    {
        wxTextFile configFile;
        configFile.Open(configFilename);
        m_sfxRootDirectory = configFile.GetFirstLine();
        configFile.Close();
    }
    else
    {
        m_dataError = true;
        m_dataErrorDescription = _("config file not found");

        return;
    }

    // Now the actual data.
    // Update 01-06-2014: file is now read from sfx root.

    wxString dataFilename = m_sfxRootDirectory + c_dataFile;

    // Drive may be mounted but (temporarily) on a different drive designator.
    // Let sfx enumerate all available drives to see if this is the case
    // and if so, silently repair this situation for now. Failure is not an option. 19-09-2014.

    if(!wxFileExists(dataFilename))
    {
        wxArrayString availableVolumes;

        availableVolumes = wxFSVolume::GetVolumes(wxFS_VOL_MOUNTED, 0);

        for(wxArrayString::iterator idx = availableVolumes.begin(); idx != availableVolumes.end(); ++idx)
        {
            wxString drive = *idx;

            if(drive.length() > 0)
            {
                dataFilename[0] = drive[0];

                if(wxFileExists(dataFilename))
                {
                    // Done, we've hunted down the (temporary?) new location of our sound effect drive.

                    break;
                }
            }
        }
    }

    if(wxFileExists(dataFilename))
    {
        wxTextFile dataFile;
        dataFile.Open(dataFilename);

        wxString firstLine = dataFile.GetFirstLine();

        // First line can be anything.
        // Most likely a description. HZ, jan 5, 2014.

        // Read all data and turn into lowercase.
        // This will speed up our searches later on.

        while(!dataFile.Eof())
        {
            wxString description = dataFile.GetNextLine().Lower();

            if(description.length() == 0)
            {
                // Done scanning.

                break;
            }

            m_descriptions.push_back(description.Trim());

            wxString fileName = dataFile.GetNextLine();

            m_fileNames.push_back(fileName.Trim());
        }

        dataFile.Close();

        m_dataSize = static_cast <int> (m_descriptions.size());

        if(m_dataSize == 0)
        {
            m_dataError = true;
            m_dataErrorDescription = _("no sound effects were listed in the datafile");
        }
    }
    else
    {
        m_dataError = true;
        m_dataErrorDescription = _("data file not found (error in config? drive not mounted?)");
    }

    // Added 23-04-2014 HZ: check if first file can be found.
    // If not, the designated wav file drive may not be mounted. This is also an error.

    // Removed 01-06-2014: we now only check for the data file in the root of the sfx directory.

    /*
    if(m_dataSize > 0 && !wxFileExists(fileNameAt(0)))
    {
        m_dataError = true;
        m_dataErrorDescription = _("file(s) not found! Drive not mounted?");
    }
    */

}

model::~model()
{
    //dtor
}

wxString model::dataFileName()
{
    return m_sfxRootDirectory + c_dataFile;
}

bool model::hasDataError(wxString& description)
{
    description = m_dataErrorDescription;
    return m_dataError;
}

int model::totalCount()
{
    return m_dataSize;
}

// Internal private function; returns description at absolute vector index.

wxString model::descriptionAt(const int index)
{
    if(index < 0 || index >= m_dataSize)
    {
        return _("");
    }

    return m_descriptions[index];
}

// Internal private function; returns filename at absolute vector index.

wxString model::fileNameAt(const int index)
{
    if(index < 0 || index >= m_dataSize)
    {
        return _("");
    }

    return m_sfxRootDirectory + m_fileNames[index];
}

// Return metadata of this file using TagLib.

//wxString model::metaData(wxString filename)
//{
    /*
    wxString metaData;

    const char* filenameAsCharBuffer = filename.mb_str(wxConvUTF8);
    TagLib::FileRef fileRef(filenameAsCharBuffer);

    if(!fileRef.isNull() && fileRef.audioProperties())
    {
        TagLib::AudioProperties* properties = fileRef.audioProperties();

        const int seconds = properties->length() % 60;
        const int minutes = (properties->length() - seconds) / 60;
        const int samplerate = properties->sampleRate();
        const int channels = properties->channels();

        metaData << filename << ":\n"
                 << "Duration: " << wxString::Format("%.02d:%.02d", minutes, seconds) << "\n"
                 << samplerate << " kHz\n"
                 << channels << (channels == 1 ? " channel\n" : " channels\n");

        TagLib::PropertyMap tags = fileRef.file()->properties();

        for(TagLib::PropertyMap::ConstIterator i = tags.begin(); i != tags.end(); ++i)
        {
            for(TagLib::StringList::ConstIterator j = i->second.begin(); j != i->second.end(); ++j)
            {
                metaData << i->first.toCString() << ": " << j->toCString() << "\n";
            }
        }
    }
    else
    {
        metaData = "Could not read metadata for this file.\n" + filename;
    }

    return metaData;
    */

//    return "Not implemented";
//}

// Do case insensitive search.
// Descriptions have already been forced into lowercase on reading the datafile!

int model::search(wxString searchString)
{
    wxArrayString tokens = wxStringTokenize(searchString.Lower());

    int tokenCount = tokens.Count();

    if(tokenCount == 0)
    {
        // User typed empty string or spaces.

        return 0;
    }

    // Clear any previous results.

    m_searchResults.clear();

    // Scan entire descriptions vector for results.
    // Because we remember absolute vector indices, we do not use an std::iterator
    // but iterate by absolute index instead.

    if(m_andSearch) // AND SEARCH
    {
        for(int idx = 0; idx < m_dataSize; ++idx)
        {
            wxString description = (m_descriptions[idx]);

            // Start out optimistically. If one token is not found, the AND fails.

            bool allFound = true;

            for(int jdx = 0; jdx < tokenCount; ++jdx)
            {
                wxString token = tokens[jdx];

                if(!description.Contains(token))
                {
                    allFound = false;

                    // No need to carry on.

                    break;
                }
            }
            if(/* still */ allFound)
            {
                m_searchResults.push_back(idx);
            }
        }
    }
    else // OR SEARCH
    {
        for(int idx = 0; idx < m_dataSize; ++idx)
        {
            wxString description = (m_descriptions[idx]);

            for(int jdx = 0; jdx < tokenCount; ++jdx)
            {
                wxString token = tokens[jdx];

                if(description.Contains(token))
                {

                    // Got one! No need to search for other tokens.

                    m_searchResults.push_back(idx);

                    break;
                }
            }
        }
    }

    m_searchSize = static_cast <int> (m_searchResults.size());

    return m_searchSize;
}

// Map relative index from interface to absolute index and return description.

wxString model::searchedDescriptionAt(const int index)
{
    if(index < 0 || index >= m_searchSize)
    {
        return _("");
    }

    int absoluteIndex = m_searchResults[index];

    return descriptionAt(absoluteIndex);
}

// Map relative index from interface to absolute index and return filename.

wxString model::searchedFileNameAt(const int index)
{
    if(index < 0 || index >= m_searchSize)
    {
        return _("");
    }

    int absoluteIndex = m_searchResults[index];

    return fileNameAt(absoluteIndex);
}

// Set AND search (andSearch == true) or OR search (andSearch == false).
// This is done by the user via the interface.

void model::setAndSearch(const bool andSearch)
{
    m_andSearch = andSearch;
}
