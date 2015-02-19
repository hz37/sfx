#ifndef MODEL_H
#define MODEL_H

#include <wx/string.h>
#include <vector>

class model
{
public:
    /** Default constructor */
    model();
    /** Default destructor */
    ~model();

    wxString dataFileName();
    bool hasDataError(wxString& description);
    int totalCount();
    int search(wxString searchString);
    void setAndSearch(const bool andSearch = true);
    wxString searchedDescriptionAt(const int index);
    wxString searchedFileNameAt(const int index);
    // wxString metaData(wxString filename);

private:
    wxString descriptionAt(const int index);
    wxString fileNameAt(const int index);

    bool m_andSearch;
    bool m_dataError;
    wxString m_dataErrorDescription;
    wxString m_sfxRootDirectory;
    std::vector <wxString> m_descriptions;
    std::vector <wxString> m_fileNames;
    std::vector <int> m_searchResults;
    int m_dataSize; // Amount of fields read on program startup.
    int m_searchSize; // Amount of fields in search result set.
};

#endif // MODEL_H
