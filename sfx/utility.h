/***************************************************************
 * Name:      utility.h
 * Purpose:   Utility non class functions.
 * Author:    Hens Zimmerman (henszimmerman@gmail.com)
 * Created:   2014-02-13
 * Copyright: Hens Zimmerman (http://henszimmerman.com)
 * License:
 **************************************************************/

//---------------------------------------------------------------------------

#ifndef UTILITY_H
#define UTILITY_H

//---------------------------------------------------------------------------

#include <wx/string.h>

//---------------------------------------------------------------------------

/**< Constants that are used at more than one place in the code. */

const wxString c_tempPrefix = "__sfx_tmp_";

//---------------------------------------------------------------------------

wxString includeTrailingBackslash(const wxString folder);
wxString validFileName(const wxString description);

//---------------------------------------------------------------------------

#endif

//---------------------------------------------------------------------------
