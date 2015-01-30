#ifndef METADATAFRAME_H
#define METADATAFRAME_H

//(*Headers(MetadataFrame)
#include <wx/sizer.h>
#include <wx/textctrl.h>
#include <wx/button.h>
#include <wx/frame.h>
//*)

class MetadataFrame: public wxFrame
{
	public:

		MetadataFrame(wxWindow* parent, wxString text);
		virtual ~MetadataFrame();

		//(*Declarations(MetadataFrame)
		wxTextCtrl* MetadataTextCtrl;
		wxButton* CloseButton;
		//*)

	protected:

		//(*Identifiers(MetadataFrame)
		static const long ID_METADATATEXTCTRL;
		static const long ID_CLOSEBUTTON;
		//*)

	private:

		//(*Handlers(MetadataFrame)
		void OnCloseButtonClick(wxCommandEvent& event);
		//*)

		DECLARE_EVENT_TABLE()
};

#endif
