/***************************************************************
 * Name:      sfxApp.h
 * Purpose:   Defines Application Class
 * Author:    Hens Zimmerman (henszimmerman@gmail.com)
 * Created:   2014-01-07
 * Copyright: Hens Zimmerman (http://henszimmerman.com)
 * License:
 **************************************************************/

#ifndef SFXAPP_H
#define SFXAPP_H

#include <wx/app.h>
#include <wx/snglinst.h>

/** \brief Defines Application Class. Machine generated.
 */

class sfxApp : public wxApp
{
private:
    void DeleteTempFiles();

    wxSingleInstanceChecker* m_checker; /** Guarantee to run only one instance. */

public:
    virtual bool OnInit();
    virtual int OnExit();
};

#endif // SFXAPP_H
