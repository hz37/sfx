/***************************************************************
 * Name:      utility.h
 * Purpose:   Utility class functions.
 * Author:    Hens Zimmerman (henszimmerman@gmail.com)
 * Created:   2014 - 2015
 * Copyright: Hens Zimmerman
 * License:   The MIT License (MIT)
 **************************************************************/

//---------------------------------------------------------------------------

#include "utility.h"

#include <wx/regex.h>

//---------------------------------------------------------------------------

/** \brief Ensure a folder ends with a backslash.
 *
 * \param folder Input folder with or without trailing backslash.
 * \return wxString of folder with a trailing backslash.
 */

wxString Utility::includeTrailingBackslash(const wxString folder)
{
    int len = folder.Length();

    if(len == 0)
    {
        return "";
    }

    if(folder[len - 1] == '\\')
    {
        return folder;
    }

    return folder + "\\";
}

//---------------------------------------------------------------------------

/** \brief Turn description into a valid Windows filename.
 *
 * \param description Input to turn into a filename
 * \return Description in such a form that it can be used as a filename under Windows OS.
 *
 * We take a bit of a sharp turn, because we disallow everything but chars, numbers and spaces.
 * And use a max 128 char filename (MAX_PATH in Win32 is longer).
 */

wxString Utility::validFileName(const wxString description)
{
    wxString returnValue = description;
    wxRegEx regexp("[^A-Z0-9 ]", wxRE_ICASE | wxRE_DEFAULT);
    regexp.ReplaceAll(&returnValue, "");

    return returnValue.Truncate(128);
}

//---------------------------------------------------------------------------
