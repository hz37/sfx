#ifndef DETAILDIALOG_H
#define DETAILDIALOG_H

//(*Headers(DetailDialog)
#include <wx/stattext.h>
#include <wx/statbox.h>
#include <wx/panel.h>
#include <wx/button.h>
#include <wx/dialog.h>
//*)

#include <wx/bitmap.h>
#include <wx/sound.h>
#include <memory>
#include <vector>

class DetailDialog: public wxDialog
{
	public:

		DetailDialog
		(
            wxWindow* parent,
            wxString filename,
            wxString description,
            wxString storageDirectory,
            wxWindowID id=wxID_ANY,
            const wxPoint& pos=wxDefaultPosition
        );
		virtual ~DetailDialog();

		//(*Declarations(DetailDialog)
		wxButton* PlayButton;
		wxStaticText* DescriptionStaticText;
		wxPanel* Panel1;
		wxStaticText* StaticText1;
		wxButton* ExitButton;
		wxStaticText* StaticText3;
		wxStaticBox* StaticBox1;
		wxStaticText* ChannelsStaticText;
		wxButton* StopButton;
		wxStaticText* SamplerateStaticText;
		wxStaticText* StaticText4;
		//*)

	protected:

		//(*Identifiers(DetailDialog)
		static const long ID_STATICBOX1;
		static const long ID_STATICTEXT3;
		static const long ID_STATICTEXT4;
		static const long ID_STATICTEXT1;
		static const long ID_STATICTEXT5;
		static const long ID_PANEL1;
		static const long ID_BUTTON1;
		static const long ID_BUTTON2;
		static const long ID_BUTTON3;
		static const long ID_STATICTEXT7;
		static const long ID_STATICTEXT8;
		//*)

	private:

		//(*Handlers(DetailDialog)
		void OnExitButtonClick(wxCommandEvent& event);
		void OnPlayButtonClick(wxCommandEvent& event);
		void OnStopButtonClick(wxCommandEvent& event);
		void OnPanel1Paint(wxPaintEvent& event);
		void OnPanel1EraseBackground(wxEraseEvent& event);
		void OnPanel1LeftDown(wxMouseEvent& event);
		void OnPanel1LeftUp(wxMouseEvent& event);
		void OnPanel1MouseMove(wxMouseEvent& event);
		void OnPanel1MouseLeave(wxMouseEvent& event);
		//*)

		bool CreateWavFile(wxString& fileName, bool cueForDeletion = true);
		void LoadWavFile(wxString fileName);

        wxString m_filename; /**< File we are currently looking at. */
        wxString m_storageDirectory; /**< Directory where files will be stored. */

        int m_tempWavCounter; /**< For every time a user Plays a sound, a new file is created with this counter in its name. */

        wxBitmap* m_drawBuffer; /**< Drawing buffer for waveform view. */
        wxBitmap* m_invertedBuffer; /**< Auto generated inverted version of m_drawBuffer. */

        wxSound m_sound; /**< Sound (current selected part of waveform) to play. */

        std::auto_ptr <double> m_waveData; /**< Raw wav data to play with. */

        int m_channels; /**< Channels in currently loaded wav. */
        int m_frameCount; /**< Frames in currently loaded wav. */
        int m_sampleRate;/**< Sample rate of currently loaded wav. */
        int m_format; /**< Wav format of currently loaded wav (of which we are blissfully ignorant). */

        int m_sel1; /**< Border 1 of selection (can be left or right). */
        int m_sel2; /**< Border 2 of selection (can be left or right). */

        int m_selStart; /**< Computed left border of selection. */
        int m_selStop; /**< Computed right border of selection. */
        bool m_selecting; /**< True if user is currently selecting. */

        bool m_customDirectoryProvided; /**< If a user saves files into a custom dir, we don't delete the dragged files. */

        wxString m_filePrefix;/**< Description to tag on to filename we generate (easier for Pro Tools timeline) */

        std::vector <wxString> m_tempFiles; /**< Store all temp files to clean up on exit. */

		DECLARE_EVENT_TABLE()
};

#endif
