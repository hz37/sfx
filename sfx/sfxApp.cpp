/***************************************************************
 * Name:      sfxApp.cpp
 * Purpose:   Code for Application Class
 * Author:    Hens Zimmerman (henszimmerman@gmail.com)
 * Created:   2014-01-07
 * Copyright: Hens Zimmerman (http://henszimmerman.com)
 * License:
 **************************************************************/

#include "sfxApp.h"

//(*AppHeaders
#include "sfxMain.h"
#include <wx/image.h>
//*)

#include <wx/dir.h>
#include <wx/filefn.h>
#include <wx/stdpaths.h>

#include "utility.h"

IMPLEMENT_APP(sfxApp);

void sfxApp::DeleteTempFiles()
{
    // Delete temp files we created.

    wxString storageDirectory = wxStandardPaths::Get().GetTempDir();

    wxArrayString tempFiles;

    wxDir::GetAllFiles(storageDirectory, &tempFiles, wxEmptyString, wxDIR_FILES);

    size_t tempFileCount = tempFiles.GetCount();

    for(size_t idx = 0; idx < tempFileCount; ++idx)
    {
        wxString file = tempFiles[idx];
        if(file.Contains(c_tempPrefix))
        {
            wxRemoveFile(file);
        }
    }
}

//! \brief OnExit() virtual function override.
//!
//! \return Returns 0 at all times.

int sfxApp::OnExit()
{
    DeleteTempFiles();

    // Clean up mutex.

    delete m_checker;

    return 0;
}

//! \brief OnInit() machine generated code to start application + added support for single instance mutex.
//!
//! \return false if already running, else wxsOK.

bool sfxApp::OnInit()
{
    //! Guarantee to run only one instance of this application at all times.
    //! Supplied string is a UUID.

    m_checker = new wxSingleInstanceChecker(_("7EB25BC2-F4BD-4A7B-9D0F-70CA0A77A198"));

    if(m_checker->IsAnotherRunning())
    {
        wxLogError(_("Another program instance is already running, aborting."));
        return false;
    }

    //(*AppInitialize
    bool wxsOK = true;
    wxInitAllImageHandlers();
    if ( wxsOK )
    {
    	sfxFrame* Frame = new sfxFrame(0);
    	Frame->Show();
    	SetTopWindow(Frame);
    }
    //*)

    return wxsOK;
}
