#include "MetadataFrame.h"

//(*InternalHeaders(MetadataFrame)
#include <wx/settings.h>
#include <wx/font.h>
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(MetadataFrame)
const long MetadataFrame::ID_METADATATEXTCTRL = wxNewId();
const long MetadataFrame::ID_CLOSEBUTTON = wxNewId();
//*)

BEGIN_EVENT_TABLE(MetadataFrame,wxFrame)
	//(*EventTable(MetadataFrame)
	//*)
END_EVENT_TABLE()

MetadataFrame::MetadataFrame(wxWindow* parent, wxString text)
{
	//(*Initialize(MetadataFrame)
	wxBoxSizer* BoxSizer1;

	Create(parent, wxID_ANY, _("Metadata"), wxDefaultPosition, wxDefaultSize, wxSTAY_ON_TOP|wxDEFAULT_FRAME_STYLE|wxRESIZE_BORDER|wxCLOSE_BOX, _T("wxID_ANY"));
	SetClientSize(wxSize(453,361));
	BoxSizer1 = new wxBoxSizer(wxVERTICAL);
	MetadataTextCtrl = new wxTextCtrl(this, ID_METADATATEXTCTRL, wxEmptyString, wxDefaultPosition, wxSize(438,317), wxTE_MULTILINE|wxTE_READONLY|wxTE_DONTWRAP|wxNO_BORDER|wxVSCROLL|wxHSCROLL, wxDefaultValidator, _T("ID_METADATATEXTCTRL"));
	MetadataTextCtrl->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_SCROLLBAR));
	wxFont MetadataTextCtrlFont(10,wxSWISS,wxFONTSTYLE_NORMAL,wxNORMAL,false,_T("Arial"),wxFONTENCODING_DEFAULT);
	MetadataTextCtrl->SetFont(MetadataTextCtrlFont);
	BoxSizer1->Add(MetadataTextCtrl, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
	CloseButton = new wxButton(this, ID_CLOSEBUTTON, _("Close"), wxDefaultPosition, wxSize(310,32), 0, wxDefaultValidator, _T("ID_CLOSEBUTTON"));
	CloseButton->SetDefault();
	BoxSizer1->Add(CloseButton, 0, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	SetSizer(BoxSizer1);
	SetSizer(BoxSizer1);
	Layout();

	Connect(ID_CLOSEBUTTON,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MetadataFrame::OnCloseButtonClick);
	//*)

    MetadataTextCtrl->SetValue(text);
}

MetadataFrame::~MetadataFrame()
{
	//(*Destroy(MetadataFrame)
	//*)
}


void MetadataFrame::OnCloseButtonClick(wxCommandEvent& event)
{
    Close();
}
