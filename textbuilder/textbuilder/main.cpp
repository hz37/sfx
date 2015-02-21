/*

A simple Windows console program that scans a sound effects directory
and turns it into the right data.txt and config.txt files for sfx to run.
It assumes the filenames are the labels of the sound effect files.

data.txt is written in its proper location, i.e. at the start of the
tree where all the sound effects are located.

config.txt is written in the current directory.

Hens Zimmerman, Code::Blocks 13.12.
henszimmerman@gmail.com
Jan. 30, 2015.
Update Feb. 20, 2015: Reads and corrects existing data file. Does not overwrite existing entries.
Update feb. 21, 2015. Backs up old data.txt before overwriting.

*/

#include <algorithm>
#include <ctime>
#include <fstream>
#include <iostream>
#include <string>
#include <sys/stat.h>
#include <vector>

// This console program is platform specific for Windows.

#include <windows.h>


// A simple struct to populate a vector.

struct TSFXRecord
{
    std::string label;
    std::string filename;
    friend bool operator < (const TSFXRecord& rec1, const TSFXRecord& rec2);
    friend bool operator == (const TSFXRecord& rec1, const TSFXRecord& rec2);
};


bool operator < (const TSFXRecord& rec1, const TSFXRecord& rec2);
bool operator == (const TSFXRecord& rec1, const TSFXRecord& rec2);
bool file_exists(const std::string filename);
size_t find_file_ext_idx(const char* fileName);
std::string get_file_ext(const char* fileName);
std::string include_trailing_backslash(const std::string directory);
bool is_directory(const char* directory);
std::string filename_to_label(const std::string filename);
void scan(const std::string sfx_directory, const std::string data_filename, std::vector <TSFXRecord>& out);
void tree(const std::string directory, std::vector <TSFXRecord>& out);


// Non-class (friend) function, needed by STL sort() algorithm.

bool operator < (const TSFXRecord& rec1, const TSFXRecord& rec2)
{
    return rec1.label < rec2.label;
}


// Non-class (friend) function, needed by STL find() algorithm.
// In this case (woohee!), equality means the records point to the same existing file.
// So they can (and will often) have different labels.

bool operator == (const TSFXRecord& rec1, const TSFXRecord& rec2)
{
    // Do a case insensitive compare.

    return stricmp(rec1.filename.c_str(), rec2.filename.c_str()) == 0;
}

// Return true if fileName exists.

bool file_exists(const std::string filename)
{
    struct stat buffer;
    return (stat (filename.c_str(), &buffer) == 0);
}

// Helper function to extract a file extension.

size_t find_file_ext_idx(const char* fileName)
{
    size_t len = strlen(fileName);
    size_t idx = len - 1;

    for(size_t i = 0; *(fileName+i); i++)
    {
        if (*(fileName+i) == '.')
        {
            idx = i;
        } else if (*(fileName + i) == '/' || *(fileName + i) == '\\')
        {
            idx = len - 1;
        }
    }

    return idx + 1;
}


// Function to extract file extension.

std::string get_file_ext(const char* fileName)
{
    std::string ext = std::string(fileName).substr(find_file_ext_idx(fileName));

    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);

    return ext;
}


// Guarantee that directory ends with a backslash.

std::string include_trailing_backslash(const std::string directory)
{
    if(*directory.rbegin() != '\\')
    {
        return directory + "\\";
    }

    return directory;
}


// Return true if directory is an existing directory.

bool is_directory(const char* directory)
{
    WIN32_FIND_DATA win32FindData;

    HANDLE result = FindFirstFile(directory, &win32FindData);

    if(result == INVALID_HANDLE_VALUE)
    {
        return false;
    }

    FindClose(result);

    return (win32FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY);
}


// Strip a full path plus filename and turn it into a label.

std::string filename_to_label(const std::string filename)
{
    std::string label = filename;

    // Extract filename.

    const size_t sep = label.find_last_of("\\/");

    if (sep != std::string::npos)
    {
        label = label.substr(sep + 1, label.size() - sep - 1);
    }

    // Remove extension.

    size_t dot = label.find_last_of(".");

    if (dot != std::string::npos)
    {
        label = label.substr(0, dot);
    }

    // Turn into lowercase.

    std::transform(label.begin(), label.end(), label.begin(), ::tolower);

    return label;
}


// Read data filename into out and remove referenced files that do not exist.

void scan(const std::string sfx_directory, const std::string data_filename, std::vector <TSFXRecord>& out)
{
    std::ifstream input_file(data_filename.c_str());

    std::string line;

    int line_counter = 0;

    std::string description;

    while (std::getline(input_file, line))
    {
        if(line_counter == 0)
        {
            // First line can be ignored.
            ++line_counter;
            continue;
        }
        else if(line_counter % 2)
        {
            // Uneven lines are descriptions.
            description = line;
        }
        else
        {
            // Even lines are filenames.
            // Build the proper file name (path + name).

            std::string full_path = sfx_directory + line;

            if(file_exists(full_path))
            {
                TSFXRecord sfx_record;
                sfx_record.filename = full_path;
                sfx_record.label = description;
                out.push_back(sfx_record);
            }
            else
            {
                // Report a missing file.
                std::cout << "Removing missing [" << full_path << "] from data file" << std::endl;
            }
        }

        ++line_counter;
    }
}


// Recursive function to scan directory for .wav files.
// Found files are stored in out, together with their label.

void tree(const std::string directory, std::vector <TSFXRecord>& out)
{
    WIN32_FIND_DATA win32FindData;

    const std::string closed_path = include_trailing_backslash(directory);

    HANDLE FindHandle = FindFirstFile
    (
        (closed_path + "*.*").c_str(),
        &win32FindData
    );

    bool find_success = FindHandle != INVALID_HANDLE_VALUE;

    while(find_success)
    {
        const std::string short_name = win32FindData.cFileName;
        const std::string long_name = closed_path + short_name;

        if(win32FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
            if((short_name != ".") && (short_name != ".."))
            {
                tree(long_name, out);
            }
        }
        else
        {
            // It's a file. But is it a *.wav file ?

            if(get_file_ext(short_name.c_str()) == "wav")
            {
                // Is it already in the vector?

                TSFXRecord sfx_record;
                sfx_record.filename = long_name;

                if(std::find(out.begin(), out.end(), sfx_record) == out.end())
                {
                    std::cout << "Adding " << long_name << std::endl;
                    sfx_record.label = filename_to_label(short_name);
                    out.push_back(sfx_record);
                }
            }
        }

        find_success = FindNextFile(FindHandle, &win32FindData);
    }

    FindClose(FindHandle);
}


// main()

int main(int argc, char* argv[])
{
    if(argc != 2)
    {
        std::cout << "Usage: textbuilder name_of_sfx_directory" << std::endl;
        return 0;
    }

    if(!is_directory(argv[1]))
    {
        std::cout << argv[1] << "is not a directory" << std::endl;
        return 0;
    }

    std::string sfx_directory = include_trailing_backslash(argv[1]);
    std::string data_filename = sfx_directory + "data.txt";

    // If there is already a data file, load it and remove missing entries.

    std::vector <TSFXRecord> sfx_records;

    bool has_old_data_file = file_exists(data_filename);

    if(has_old_data_file)
    {
        std::cout << "Reading and checking existing data file" << std::endl;

        scan(sfx_directory, data_filename, sfx_records);
    }

    std::cout << "Scanning " << sfx_directory << std::endl;

    tree(sfx_directory, sfx_records);

    std::sort(sfx_records.begin(), sfx_records.end());

    if(has_old_data_file)
    {
        // Back up old data file.

        std::cout << "Backing up old data.txt" << std::endl;

        std::string backup_filename = sfx_directory + "data_backup.txt";

        CopyFile(data_filename.c_str(), backup_filename.c_str(), false /* overwrite */);
    }

    // Write datafile.

    std::cout << "Writing data.txt in " << sfx_directory << std::endl;

    std::ofstream data_file(data_filename.c_str());

    // Print first line to file with some info.

    time_t rawtime;
    struct tm* timeinfo;
    char buffer[80];

    time(&rawtime);
    timeinfo = localtime (&rawtime);

    strftime (buffer, 80, "%d-%m-%Y", timeinfo);

    data_file << "# machine generated data file - " << buffer << std::endl;

    // Now for every record we will chop off the common start directory portion at the beginning of the filename.

    int pos = sfx_directory.length();

    for(std::vector <TSFXRecord>::iterator idx = sfx_records.begin(); idx != sfx_records.end(); ++idx)
    {
        TSFXRecord rec = *idx;
        data_file << rec.label << std::endl << rec.filename.substr(pos) << std::endl;
    }

    data_file.close();

    // Write config file.

    std::cout << "Writing config.txt in current directory" << std::endl;

    std::ofstream config_file("config.txt");

    config_file << sfx_directory << std::endl;

    config_file.close();

    std::cout << "All done! Make sure config.txt is in the same dir as sfx.exe" << std::endl;

    return 0;
}
