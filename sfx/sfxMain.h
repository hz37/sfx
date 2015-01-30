/***************************************************************
 * Name:      sfxMain.h
 * Purpose:   Defines Application Frame
 * Author:    Hens Zimmerman (henszimmerman@gmail.com)
 * Created:   2014-01-07
 * Copyright: Hens Zimmerman (http://henszimmerman.com)
 * License:
 **************************************************************/

#ifndef SFXMAIN_H
#define SFXMAIN_H

#include "model.h"

//(*Headers(sfxFrame)
#include <wx/listctrl.h>
#include <wx/sizer.h>
#include <wx/menu.h>
#include <wx/textctrl.h>
#include <wx/checkbox.h>
#include <wx/radiobut.h>
#include <wx/button.h>
#include <wx/dirdlg.h>
#include <wx/frame.h>
#include <wx/statusbr.h>
//*)

#include <wx/sound.h>

class sfxFrame: public wxFrame
{
    public:

        sfxFrame(wxWindow* parent,wxWindowID id = -1);
        virtual ~sfxFrame();

    private:

        //(*Handlers(sfxFrame)
        void OnQuit(wxCommandEvent& event);
        void OnAbout(wxCommandEvent& event);
        void OnSearchTextCtrlTextEnter(wxCommandEvent& event);
        void OnSearchButtonClick(wxCommandEvent& event);
        void OnAndRadioButtonSelect(wxCommandEvent& event);
        void OnOrRadioButtonSelect(wxCommandEvent& event);
        void OnPlayButtonClick(wxCommandEvent& event);
        void OnStopButtonClick(wxCommandEvent& event);
        void OnTopmostCheckBoxClick(wxCommandEvent& event);
        void OnSaveFolderButtonClick(wxCommandEvent& event);
        void OnSaveToFolderButtonClick(wxCommandEvent& event);
        void OnResultsListCtrlBeginDrag(wxListEvent& event);
        void OnResultsListCtrlItemSelect(wxListEvent& event);
        void OnMetadataButtonClick(wxCommandEvent& event);
        //*)

        void playSelectedSound();
        wxString includeTrailingBackslash(wxString folder);
        wxString validFileName(const wxString description);

        //(*Identifiers(sfxFrame)
        static const long ID_SEARCHTEXT;
        static const long ID_SEARCH;
        static const long ID_ANDRADIOBUTTON;
        static const long ID_ORRADIOBUTTON;
        static const long ID_PLAYBUTTON;
        static const long ID_STOPBUTTON;
        static const long ID_METADATABUTTON;
        static const long ID_TOPMOSTCHECKBOX;
        static const long ID_SAVEFOLDERBUTTON;
        static const long ID_SAVETOFOLDERBUTTON;
        static const long ID_RESULTSLISTCTRL;
        static const long idMenuQuit;
        static const long idMenuAbout;
        static const long ID_STATUSBAR1;
        //*)

        //(*Declarations(sfxFrame)
        wxButton* PlayButton;
        wxTextCtrl* SearchTextCtrl;
        wxRadioButton* OrRadioButton;
        wxButton* SaveToFolderButton;
        wxButton* MetadataButton;
        wxCheckBox* TopmostCheckBox;
        wxButton* StopButton;
        wxButton* SearchButton;
        wxListCtrl* ResultsListCtrl;
        wxStatusBar* StatusBar1;
        wxDirDialog* DirDialog1;
        wxRadioButton* AndRadioButton;
        wxButton* SaveFolderButton;
        //*)

        model m_model;
        wxSound m_sound;
        wxString m_saveFolder;

        DECLARE_EVENT_TABLE()
};

#endif // SFXMAIN_H
