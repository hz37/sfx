/***************************************************************
 * Name:      sfxMain.cpp
 * Purpose:   Code for Application Frame
 * Author:    Hens Zimmerman (henszimmerman@gmail.com)
 * Created:   2014-01-07
 * Copyright: Hens Zimmerman (http://henszimmerman.com)
 * License:
 **************************************************************/

#include "sfxMain.h"
#include <wx/msgdlg.h>
#include <wx/dnd.h>
#include <wx/filename.h>
#include <wx/kbdstate.h>
#include <wx/tooltip.h>
#include <wx/regex.h>

//(*InternalHeaders(sfxFrame)
#include <wx/font.h>
#include <wx/intl.h>
#include <wx/string.h>
//*)

#include "MetadataFrame.h"

//helper functions
enum wxbuildinfoformat {
    short_f, long_f };

wxString wxbuildinfo(wxbuildinfoformat format)
{
    wxString wxbuild(wxVERSION_STRING);

    if (format == long_f )
    {
#if defined(__WXMSW__)
        wxbuild << _T("-Windows");
#elif defined(__UNIX__)
        wxbuild << _T("-Linux");
#endif

#if wxUSE_UNICODE
        wxbuild << _T("-Unicode build");
#else
        wxbuild << _T("-ANSI build");
#endif // wxUSE_UNICODE
    }

    return wxbuild;
}

//(*IdInit(sfxFrame)
const long sfxFrame::ID_SEARCHTEXT = wxNewId();
const long sfxFrame::ID_SEARCH = wxNewId();
const long sfxFrame::ID_ANDRADIOBUTTON = wxNewId();
const long sfxFrame::ID_ORRADIOBUTTON = wxNewId();
const long sfxFrame::ID_PLAYBUTTON = wxNewId();
const long sfxFrame::ID_STOPBUTTON = wxNewId();
const long sfxFrame::ID_METADATABUTTON = wxNewId();
const long sfxFrame::ID_TOPMOSTCHECKBOX = wxNewId();
const long sfxFrame::ID_SAVEFOLDERBUTTON = wxNewId();
const long sfxFrame::ID_SAVETOFOLDERBUTTON = wxNewId();
const long sfxFrame::ID_RESULTSLISTCTRL = wxNewId();
const long sfxFrame::idMenuQuit = wxNewId();
const long sfxFrame::idMenuAbout = wxNewId();
const long sfxFrame::ID_STATUSBAR1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(sfxFrame,wxFrame)
    //(*EventTable(sfxFrame)
    //*)
END_EVENT_TABLE()

sfxFrame::sfxFrame(wxWindow* parent,wxWindowID id)
{
    //(*Initialize(sfxFrame)
    wxMenuItem* MenuItem2;
    wxMenuItem* MenuItem1;
    wxBoxSizer* BoxSizer2;
    wxMenu* Menu1;
    wxBoxSizer* BoxSizer1;
    wxMenuBar* MenuBar1;
    wxBoxSizer* BoxSizer3;
    wxMenu* Menu2;

    Create(parent, wxID_ANY, _("SFX 1.32"), wxDefaultPosition, wxDefaultSize, wxSTAY_ON_TOP|wxDEFAULT_FRAME_STYLE, _T("wxID_ANY"));
    SetClientSize(wxSize(979,363));
    SetBackgroundColour(wxColour(137,234,72));
    BoxSizer1 = new wxBoxSizer(wxVERTICAL);
    BoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
    SearchTextCtrl = new wxTextCtrl(this, ID_SEARCHTEXT, wxEmptyString, wxDefaultPosition, wxSize(303,21), wxTE_PROCESS_ENTER, wxDefaultValidator, _T("ID_SEARCHTEXT"));
    SearchTextCtrl->SetMinSize(wxDLG_UNIT(this,wxSize(200,12)));
    SearchTextCtrl->SetMaxSize(wxDLG_UNIT(this,wxSize(1000,12)));
    BoxSizer2->Add(SearchTextCtrl, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
    SearchButton = new wxButton(this, ID_SEARCH, _("Search"), wxDefaultPosition, wxDLG_UNIT(this,wxSize(35,12)), 0, wxDefaultValidator, _T("ID_SEARCH"));
    SearchButton->SetMinSize(wxDLG_UNIT(this,wxSize(35,12)));
    SearchButton->SetMaxSize(wxDLG_UNIT(this,wxSize(35,12)));
    BoxSizer2->Add(SearchButton, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
    AndRadioButton = new wxRadioButton(this, ID_ANDRADIOBUTTON, _("AND"), wxDefaultPosition, wxDLG_UNIT(this,wxSize(30,12)), wxRB_GROUP, wxDefaultValidator, _T("ID_ANDRADIOBUTTON"));
    AndRadioButton->SetValue(true);
    AndRadioButton->SetMinSize(wxDLG_UNIT(this,wxSize(30,12)));
    AndRadioButton->SetMaxSize(wxDLG_UNIT(this,wxSize(30,12)));
    BoxSizer2->Add(AndRadioButton, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
    OrRadioButton = new wxRadioButton(this, ID_ORRADIOBUTTON, _("OR"), wxDefaultPosition, wxDLG_UNIT(this,wxSize(30,12)), 0, wxDefaultValidator, _T("ID_ORRADIOBUTTON"));
    OrRadioButton->SetMinSize(wxDLG_UNIT(this,wxSize(30,12)));
    OrRadioButton->SetMaxSize(wxDLG_UNIT(this,wxSize(30,12)));
    BoxSizer2->Add(OrRadioButton, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
    PlayButton = new wxButton(this, ID_PLAYBUTTON, _("Play"), wxDefaultPosition, wxDLG_UNIT(this,wxSize(35,12)), 0, wxDefaultValidator, _T("ID_PLAYBUTTON"));
    PlayButton->SetMinSize(wxDLG_UNIT(this,wxSize(35,12)));
    PlayButton->SetMaxSize(wxDLG_UNIT(this,wxSize(35,12)));
    BoxSizer2->Add(PlayButton, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
    StopButton = new wxButton(this, ID_STOPBUTTON, _("Stop"), wxDefaultPosition, wxDLG_UNIT(this,wxSize(35,12)), 0, wxDefaultValidator, _T("ID_STOPBUTTON"));
    StopButton->SetMinSize(wxDLG_UNIT(this,wxSize(35,12)));
    StopButton->SetMaxSize(wxDLG_UNIT(this,wxSize(35,12)));
    BoxSizer2->Add(StopButton, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
    MetadataButton = new wxButton(this, ID_METADATABUTTON, _("Meta"), wxDefaultPosition, wxDLG_UNIT(this,wxSize(35,12)), 0, wxDefaultValidator, _T("ID_METADATABUTTON"));
    MetadataButton->SetMinSize(wxDLG_UNIT(this,wxSize(35,12)));
    MetadataButton->SetMaxSize(wxDLG_UNIT(this,wxSize(35,12)));
    BoxSizer2->Add(MetadataButton, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
    TopmostCheckBox = new wxCheckBox(this, ID_TOPMOSTCHECKBOX, _("Topmost"), wxDefaultPosition, wxDLG_UNIT(this,wxSize(45,12)), 0, wxDefaultValidator, _T("ID_TOPMOSTCHECKBOX"));
    TopmostCheckBox->SetValue(true);
    TopmostCheckBox->SetMinSize(wxDLG_UNIT(this,wxSize(45,12)));
    TopmostCheckBox->SetMaxSize(wxDLG_UNIT(this,wxSize(45,12)));
    BoxSizer2->Add(TopmostCheckBox, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
    SaveFolderButton = new wxButton(this, ID_SAVEFOLDERBUTTON, _("..."), wxDefaultPosition, wxDLG_UNIT(this,wxSize(20,12)), 0, wxDefaultValidator, _T("ID_SAVEFOLDERBUTTON"));
    SaveFolderButton->SetMinSize(wxDLG_UNIT(this,wxSize(20,12)));
    SaveFolderButton->SetMaxSize(wxDLG_UNIT(this,wxSize(20,12)));
    BoxSizer2->Add(SaveFolderButton, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
    SaveToFolderButton = new wxButton(this, ID_SAVETOFOLDERBUTTON, _("Save to folder"), wxDefaultPosition, wxDLG_UNIT(this,wxSize(60,12)), 0, wxDefaultValidator, _T("ID_SAVETOFOLDERBUTTON"));
    SaveToFolderButton->SetMinSize(wxDLG_UNIT(this,wxSize(60,12)));
    SaveToFolderButton->SetMaxSize(wxDLG_UNIT(this,wxSize(60,12)));
    SaveToFolderButton->Disable();
    BoxSizer2->Add(SaveToFolderButton, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
    BoxSizer1->Add(BoxSizer2, 0, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 0);
    BoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
    ResultsListCtrl = new wxListCtrl(this, ID_RESULTSLISTCTRL, wxDefaultPosition, wxSize(1183,319), wxLC_REPORT|wxLC_NO_HEADER|wxLC_SINGLE_SEL|wxVSCROLL|wxHSCROLL, wxDefaultValidator, _T("ID_RESULTSLISTCTRL"));
    wxFont ResultsListCtrlFont(14,wxSWISS,wxFONTSTYLE_NORMAL,wxNORMAL,false,_T("Arial"),wxFONTENCODING_DEFAULT);
    ResultsListCtrl->SetFont(ResultsListCtrlFont);
    BoxSizer3->Add(ResultsListCtrl, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
    BoxSizer1->Add(BoxSizer3, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
    SetSizer(BoxSizer1);
    MenuBar1 = new wxMenuBar();
    Menu1 = new wxMenu();
    MenuItem1 = new wxMenuItem(Menu1, idMenuQuit, _("Quit\tAlt-F4"), _("Quit the application"), wxITEM_NORMAL);
    Menu1->Append(MenuItem1);
    MenuBar1->Append(Menu1, _("&File"));
    Menu2 = new wxMenu();
    MenuItem2 = new wxMenuItem(Menu2, idMenuAbout, _("About\tF1"), _("Show info about this application"), wxITEM_NORMAL);
    Menu2->Append(MenuItem2);
    MenuBar1->Append(Menu2, _("Help"));
    SetMenuBar(MenuBar1);
    StatusBar1 = new wxStatusBar(this, ID_STATUSBAR1, 0, _T("ID_STATUSBAR1"));
    int __wxStatusBarWidths_1[1] = { -1 };
    int __wxStatusBarStyles_1[1] = { wxSB_NORMAL };
    StatusBar1->SetFieldsCount(1,__wxStatusBarWidths_1);
    StatusBar1->SetStatusStyles(1,__wxStatusBarStyles_1);
    SetStatusBar(StatusBar1);
    DirDialog1 = new wxDirDialog(this, _("Select directory"), wxEmptyString, wxDD_DEFAULT_STYLE, wxDefaultPosition, wxDefaultSize, _T("wxDirDialog"));
    SetSizer(BoxSizer1);
    Layout();

    Connect(ID_SEARCHTEXT,wxEVT_COMMAND_TEXT_ENTER,(wxObjectEventFunction)&sfxFrame::OnSearchTextCtrlTextEnter);
    Connect(ID_SEARCH,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&sfxFrame::OnSearchButtonClick);
    Connect(ID_ANDRADIOBUTTON,wxEVT_COMMAND_RADIOBUTTON_SELECTED,(wxObjectEventFunction)&sfxFrame::OnAndRadioButtonSelect);
    Connect(ID_ORRADIOBUTTON,wxEVT_COMMAND_RADIOBUTTON_SELECTED,(wxObjectEventFunction)&sfxFrame::OnOrRadioButtonSelect);
    Connect(ID_PLAYBUTTON,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&sfxFrame::OnPlayButtonClick);
    Connect(ID_STOPBUTTON,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&sfxFrame::OnStopButtonClick);
    Connect(ID_METADATABUTTON,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&sfxFrame::OnMetadataButtonClick);
    Connect(ID_TOPMOSTCHECKBOX,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&sfxFrame::OnTopmostCheckBoxClick);
    Connect(ID_SAVEFOLDERBUTTON,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&sfxFrame::OnSaveFolderButtonClick);
    Connect(ID_SAVETOFOLDERBUTTON,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&sfxFrame::OnSaveToFolderButtonClick);
    Connect(ID_RESULTSLISTCTRL,wxEVT_COMMAND_LIST_BEGIN_DRAG,(wxObjectEventFunction)&sfxFrame::OnResultsListCtrlBeginDrag);
    Connect(ID_RESULTSLISTCTRL,wxEVT_COMMAND_LIST_ITEM_SELECTED,(wxObjectEventFunction)&sfxFrame::OnResultsListCtrlItemSelect);
    Connect(idMenuQuit,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&sfxFrame::OnQuit);
    Connect(idMenuAbout,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&sfxFrame::OnAbout);
    //*)

    // Report how many sounds were loaded.

    wxString s;
    s.Printf(_("%d sound effects found"), m_model.totalCount());
    StatusBar1->SetLabel(s);

    // Set the icon for this application (see .rc file).

    SetIcon(wxICON(aaaa));

    // Tooltips are annoying and slow us down for this application.

    wxToolTip::Enable(false);

    // That's not enough for the list control, so we disable tooltips the win32 lowlevel way too.

    const int LVM_SETEXTENDEDLISTVIEWSTYLE = 4150;
    const int LVM_GETEXTENDEDLISTVIEWSTYLE = 4151;
    const int LVS_EX_LABELTIP = 16384;

    long style = ::SendMessage((HWND)ResultsListCtrl->GetHandle(), LVM_GETEXTENDEDLISTVIEWSTYLE, 0, 0);
    style = style & (~LVS_EX_LABELTIP);
    ::SendMessage((HWND)ResultsListCtrl->GetHandle(), LVM_SETEXTENDEDLISTVIEWSTYLE, 0, style);

    // Check if model has correctly loaded everything.

    wxString description;

    if(m_model.hasDataError(description))
    {
        wxMessageBox(description, _("Error..."));
    }
}

sfxFrame::~sfxFrame()
{
    //(*Destroy(sfxFrame)
    //*)
}

// End the program from the menu.

void sfxFrame::OnQuit(wxCommandEvent& event)
{
    Close();
}

// Some info about this program.

void sfxFrame::OnAbout(wxCommandEvent& event)
{
    wxMessageBox
    (
        _("Programming: Hens Zimmerman\nEmail: hz37@xs4all.nl\nVersion: January 30, 2015."),
        _("About...")
    );
}

void sfxFrame::OnSearchTextCtrlTextEnter(wxCommandEvent& event)
{
    OnSearchButtonClick(event);
}

void sfxFrame::OnSearchButtonClick(wxCommandEvent& event)
{
    int resultCount = m_model.search(SearchTextCtrl->GetValue());

    wxString s;
    s.Printf(_("%d results"), resultCount);
    StatusBar1->SetLabel(s);

    // The result set can be quite lengthy, so freeze updates until we are done.

    ResultsListCtrl->Freeze();

    ResultsListCtrl->ClearAll();

    wxListItem col0;
    col0.SetId(0);

    ResultsListCtrl->InsertColumn(0, col0);

    for(int idx = 0; idx < resultCount; ++idx)
    {
        wxListItem item;
        item.SetId(idx);
        ResultsListCtrl->InsertItem(item);
        wxString description = m_model.searchedDescriptionAt(idx);
        ResultsListCtrl->SetItem(idx, 0, description);
    }

    ResultsListCtrl->SetColumnWidth(0, wxLIST_AUTOSIZE);

    // Unfreeze updates.

    ResultsListCtrl->Thaw();
}

void sfxFrame::OnAndRadioButtonSelect(wxCommandEvent& event)
{
    m_model.setAndSearch();
}

void sfxFrame::OnOrRadioButtonSelect(wxCommandEvent& event)
{
    m_model.setAndSearch(false);
}

void sfxFrame::OnPlayButtonClick(wxCommandEvent& event)
{
    playSelectedSound();
}

void sfxFrame::OnStopButtonClick(wxCommandEvent& event)
{
    m_sound.Stop();
}

void sfxFrame::OnTopmostCheckBoxClick(wxCommandEvent& event)
{
    this->SetWindowStyle
    (
        TopmostCheckBox->IsChecked() ? wxSTAY_ON_TOP | wxDEFAULT_FRAME_STYLE : wxDEFAULT_FRAME_STYLE
    );
}

void sfxFrame::OnSaveFolderButtonClick(wxCommandEvent& event)
{
    // Pressing the shift is an easy way to erase the save folder again.

    wxKeyboardState ks;

    if(ks.ShiftDown())
    {
        m_saveFolder = "";
        SaveToFolderButton->Disable();

        return;
    }

    if(DirDialog1->ShowModal())
    {
        m_saveFolder = includeTrailingBackslash(DirDialog1->GetPath());
        StatusBar1->SetLabel(m_saveFolder);

        if(m_saveFolder.Length())
        {
            SaveToFolderButton->Enable();
        }
        else
        {
            SaveToFolderButton->Disable();
        }
    }
}

// Save file to a special place. Useful when editors come to begg for sounds
// and want their requests saved to a network location.

void sfxFrame::OnSaveToFolderButtonClick(wxCommandEvent& event)
{
    // If they drag a file, they are sure they wanna use 'em.
    // No need to keep pestering a user with more of the same.

    m_sound.Stop();

    // First find index of selected item (if any).

    int index = ResultsListCtrl->GetNextItem
    (
        -1,
        wxLIST_NEXT_ALL,
        wxLIST_STATE_SELECTED
    );

    if(index < 0)
    {
        return;
    }

    // We turn the filename into something more human, based on its description.
    // This avoids directories filled with names like S_9872354987.WAV etc.

    wxString source = m_model.searchedFileNameAt(index);
    wxString description = m_model.searchedDescriptionAt(index);

//    wxFileName fileNameParser(source);
//    wxString destination = m_saveFolder + fileNameParser.GetName() + _(".wav");
    wxString destination = m_saveFolder + validFileName(description) + _(".wav");
    wxCopyFile(source, destination, true);
}

void sfxFrame::OnResultsListCtrlBeginDrag(wxListEvent& event)
{
    // First find index of selected item (if any).

    int index = ResultsListCtrl->GetNextItem
    (
        -1,
        wxLIST_NEXT_ALL,
        wxLIST_STATE_SELECTED
    );

    if(index < 0)
    {
        return;
    }

    wxFileDataObject* data = new wxFileDataObject();

    wxString fileName = m_model.searchedFileNameAt(index);

    StatusBar1->SetLabel(fileName);

    data->AddFile(fileName);

    wxDropSource dragSource(this);
    dragSource.SetData(*data);

    wxDragResult dragResult = dragSource.DoDragDrop();

    wxDELETE(data);

    if(dragResult == wxDragCopy)
    {
        // If they drag a file, they are sure they wanna use 'em.
        // No need to keep pestering a user with more of the same.

        m_sound.Stop();

        // Auto copy file if a directory is defined for this and user actually copied something.

        if(m_saveFolder.Length())
        {
            wxFileName fileNameParser(fileName);
            wxString destination = m_saveFolder + fileNameParser.GetName() + _(".wav");
            wxCopyFile(fileName, destination, true);
        }
    }
}

void sfxFrame::OnResultsListCtrlItemSelect(wxListEvent& event)
{
    playSelectedSound();
}

void sfxFrame::playSelectedSound()
{
    // First find index of selected item (if any).

    int index = ResultsListCtrl->GetNextItem
    (
        -1,
        wxLIST_NEXT_ALL,
        wxLIST_STATE_SELECTED
    );

    if(index < 0)
    {
        return;
    }

    // Get the file to play.

    wxString soundFileName = m_model.searchedFileNameAt(index);

    m_sound.Play(soundFileName, wxSOUND_ASYNC);
}

// Helper function to make sure paths end with a backslash.

wxString sfxFrame::includeTrailingBackslash(wxString folder)
{
    int len = folder.Length();

    if(len == 0)
    {
        return _("");
    }

    if(folder[len - 1] == '\\')
    {
        return folder;
    }

    return folder + _("\\");
}

// Turn description into a valid Windows filename.
// We take a bit of a sharp turn, because we disallow everything but chars, numbers and spaces.
// And use a max 128 char filename (MAX_PATH in Win32 is longer).

wxString sfxFrame::validFileName(const wxString description)
{
    wxString returnValue = description;
    wxRegEx regexp(_("[^A-Z0-9 ]"), wxRE_ICASE | wxRE_DEFAULT);
    regexp.ReplaceAll(&returnValue, _(""));

    return returnValue.Truncate(128);
}

void sfxFrame::OnMetadataButtonClick(wxCommandEvent& event)
{
    // First find index of selected item (if any).

    int index = ResultsListCtrl->GetNextItem
    (
        -1,
        wxLIST_NEXT_ALL,
        wxLIST_STATE_SELECTED
    );

    if(index < 0)
    {
        return;
    }

    wxString soundFileName = m_model.searchedFileNameAt(index);
    wxString text = m_model.metaData(soundFileName);
    MetadataFrame* metaDataFrame = new MetadataFrame(this, text);

    metaDataFrame->Show();
}
