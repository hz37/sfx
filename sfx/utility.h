/***************************************************************
 * Name:      utility.h
 * Purpose:   Utility class functions.
 * Author:    Hens Zimmerman (henszimmerman@gmail.com)
 * Created:   2014 - 2015
 * Copyright: Hens Zimmerman
 * License:   The MIT License (MIT)
 **************************************************************/

//---------------------------------------------------------------------------

#ifndef UTILITY_H
#define UTILITY_H

//---------------------------------------------------------------------------

#include <wx/string.h>

/**< Constants that are used at more than one place in the code. */

const wxString c_tempPrefix = "__sfx_tmp_"; /**< What to add to our temp files to make them distinguishable */

//---------------------------------------------------------------------------

class Utility
{
public:
    /** \brief Ensure a folder ends with a backslash.
     *
     * \param folder Input folder with or without trailing backslash.
     * \return wxString of folder with a trailing backslash.
     */

    static wxString includeTrailingBackslash(const wxString folder);


    /** \brief Turn description into a valid Windows filename.
     *
     * \param description Input to turn into a filename
     * \return Description in such a form that it can be used as a filename under Windows OS.
     *
     * We take a bit of a sharp turn, because we disallow everything but chars, numbers and spaces.
     * And use a max 128 char filename (MAX_PATH in Win32 is longer).
     */

    static wxString validFileName(const wxString description);

};

//---------------------------------------------------------------------------

#endif

//---------------------------------------------------------------------------
