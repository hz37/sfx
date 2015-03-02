/***************************************************************
 * Name:      Bitmap_builderMain.h
 * Purpose:   Defines Application Frame
 * Author:    Hens Zimmerman (henszimmerman@gmail.com)
 * Created:   2015-03-02
 * Copyright: Hens Zimmerman (http://henszimmerman.com)
 * License:
 **************************************************************/

#ifndef BITMAP_BUILDERMAIN_H
#define BITMAP_BUILDERMAIN_H

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "Bitmap_builderApp.h"

class Bitmap_builderFrame: public wxFrame
{
    public:
        Bitmap_builderFrame(wxFrame *frame, const wxString& title);
        ~Bitmap_builderFrame();
    private:
        enum
        {
            idMenuQuit = 1000,
            idMenuAbout
        };
        void OnClose(wxCloseEvent& event);
        void OnQuit(wxCommandEvent& event);
        void OnAbout(wxCommandEvent& event);
        DECLARE_EVENT_TABLE()
};


#endif // BITMAP_BUILDERMAIN_H
