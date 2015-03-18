/***************************************************************
 * Name:      DetailDialog.cpp
 * Purpose:   Show details of a sound effect in a modal dialog.
 * Author:    Hens Zimmerman (henszimmerman@gmail.com)
 * Created:   2014 - 2015
 * Copyright: Hens Zimmerman
 * License:   The MIT License (MIT)
 **************************************************************/

//---------------------------------------------------------------------------

#include "DetailDialog.h"

//(*InternalHeaders(DetailDialog)
#include <wx/intl.h>
#include <wx/string.h>
//*)

#include <wx/bitmap.h>
#include <wx/cursor.h>
#include <wx/dcbuffer.h>
#include <wx/dnd.h>
#include <wx/filefn.h>
#include <wx/msgdlg.h>
#include <wx/stdpaths.h>

#include <sndfile.h>
#include "utility.h"

//---------------------------------------------------------------------------

/** Constants. */

//(*IdInit(DetailDialog)
const long DetailDialog::ID_STATICBOX1 = wxNewId();
const long DetailDialog::ID_STATICTEXT3 = wxNewId();
const long DetailDialog::ID_STATICTEXT4 = wxNewId();
const long DetailDialog::ID_STATICTEXT1 = wxNewId();
const long DetailDialog::ID_STATICTEXT5 = wxNewId();
const long DetailDialog::ID_PANEL1 = wxNewId();
const long DetailDialog::ID_BUTTON1 = wxNewId();
const long DetailDialog::ID_BUTTON2 = wxNewId();
const long DetailDialog::ID_BUTTON3 = wxNewId();
const long DetailDialog::ID_STATICTEXT7 = wxNewId();
const long DetailDialog::ID_STATICTEXT8 = wxNewId();
//*)

/** These are the fixed dimensions of the waveform display. */

const int c_waveDisplayWidth = 500;
const int c_waveDisplayHeight = 100;

/** These are the pixel bounds of the selection. */

const int c_minSel = 0;
const int c_maxSel = c_waveDisplayWidth - 1;

//---------------------------------------------------------------------------

/** Machine generated event table. */

BEGIN_EVENT_TABLE(DetailDialog,wxDialog)
	//(*EventTable(DetailDialog)
	//*)
END_EVENT_TABLE()

//---------------------------------------------------------------------------

/** \brief Constructor.
 *
 * \param parent Parent window (SFX main window).
 * \param filename Full path to filename to load.
 * \param description Description of this sound effect as loaded from datafile.
 * \param id Machine genereated window ID; gets value as def arg.
 * \param pos Machine generated position; gets value as def arg.
 *
 */


DetailDialog::DetailDialog
(
    wxWindow* parent,
    wxString filename,
    wxString description,
    wxWindowID id,
    const wxPoint& pos
) :
    m_filename(filename),
    m_channels(0),
    m_frameCount(0),
    m_sampleRate(0),
    m_format(0),
    m_sel1(0),
    m_sel2(0),
    m_selStart(c_minSel),
    m_selStop(c_maxSel),
    m_selecting(false)
{
    /** Create buffers in which we will draw the waveform and its inverse. */

    m_drawBuffer = new wxBitmap(c_waveDisplayWidth, c_waveDisplayHeight, -1);
    m_invertedBuffer = new wxBitmap(c_waveDisplayWidth, c_waveDisplayWidth, -1);

	//(*Initialize(DetailDialog)
	Create(parent, id, _("Detail"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("id"));
	SetClientSize(wxSize(517,247));
	Move(wxDefaultPosition);
	StaticBox1 = new wxStaticBox(this, ID_STATICBOX1, wxEmptyString, wxPoint(8,8), wxSize(500,88), 0, _T("ID_STATICBOX1"));
	StaticText3 = new wxStaticText(this, ID_STATICTEXT3, _("Channels:"), wxPoint(16,48), wxDefaultSize, 0, _T("ID_STATICTEXT3"));
	StaticText4 = new wxStaticText(this, ID_STATICTEXT4, _("Sample rate:"), wxPoint(16,72), wxDefaultSize, 0, _T("ID_STATICTEXT4"));
	StaticText1 = new wxStaticText(this, ID_STATICTEXT1, _("Description:"), wxPoint(16,24), wxDefaultSize, 0, _T("ID_STATICTEXT1"));
	DescriptionStaticText = new wxStaticText(this, ID_STATICTEXT5, _("--------"), wxPoint(88,24), wxDefaultSize, 0, _T("ID_STATICTEXT5"));
	Panel1 = new wxPanel(this, ID_PANEL1, wxPoint(8,104), wxSize(500,100), wxTAB_TRAVERSAL, _T("ID_PANEL1"));
	Panel1->SetBackgroundColour(wxColour(0,0,0));
	PlayButton = new wxButton(this, ID_BUTTON1, _("Play"), wxPoint(8,216), wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON1"));
	StopButton = new wxButton(this, ID_BUTTON2, _("Stop"), wxPoint(96,216), wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON2"));
	ExitButton = new wxButton(this, ID_BUTTON3, _("Exit"), wxPoint(432,216), wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON3"));
	ChannelsStaticText = new wxStaticText(this, ID_STATICTEXT7, _("-------"), wxPoint(88,48), wxDefaultSize, 0, _T("ID_STATICTEXT7"));
	SamplerateStaticText = new wxStaticText(this, ID_STATICTEXT8, _("-------"), wxPoint(88,72), wxDefaultSize, 0, _T("ID_STATICTEXT8"));
	Center();

	Panel1->Connect(wxEVT_PAINT,(wxObjectEventFunction)&DetailDialog::OnPanel1Paint,0,this);
	Panel1->Connect(wxEVT_ERASE_BACKGROUND,(wxObjectEventFunction)&DetailDialog::OnPanel1EraseBackground,0,this);
	Panel1->Connect(wxEVT_LEFT_DOWN,(wxObjectEventFunction)&DetailDialog::OnPanel1LeftDown,0,this);
	Panel1->Connect(wxEVT_LEFT_UP,(wxObjectEventFunction)&DetailDialog::OnPanel1LeftUp,0,this);
	Panel1->Connect(wxEVT_MOTION,(wxObjectEventFunction)&DetailDialog::OnPanel1MouseMove,0,this);
	Panel1->Connect(wxEVT_LEAVE_WINDOW,(wxObjectEventFunction)&DetailDialog::OnPanel1MouseLeave,0,this);
	Connect(ID_BUTTON1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&DetailDialog::OnPlayButtonClick);
	Connect(ID_BUTTON2,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&DetailDialog::OnStopButtonClick);
	Connect(ID_BUTTON3,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&DetailDialog::OnExitButtonClick);
	//*)

	/** Show description of this sound effect. */

	DescriptionStaticText->SetLabel(description);

	/** Set up a path for the temp files. */

    m_storageDirectory = Utility::includeTrailingBackslash(wxStandardPaths::Get().GetTempDir());

    /** Load the WAV file that was passed to this dialog. */

    LoadWavFile(m_filename);

    /** Show info we read from the WAV file. */

    wxString channels;
    channels << m_channels;
    ChannelsStaticText->SetLabel(channels);

    wxString samplerate;
    samplerate << m_sampleRate << " Hz";
    SamplerateStaticText->SetLabel(samplerate);

    /** Remember description to use as a prefix for the WAV filename we generate. */

    m_filePrefix = Utility::validFileName(description);
}

//---------------------------------------------------------------------------

DetailDialog::~DetailDialog()
{
    m_sound.Stop();

	//(*Destroy(DetailDialog)
	//*)

    /** Delete background buffers. */

    delete m_drawBuffer;
    delete m_invertedBuffer;

}

//---------------------------------------------------------------------------

bool DetailDialog::CreateWavFile(wxString& fileName)
{
    static int tempWavCounter = 0; /** For every time a user Plays a sound, a new file is created with this counter in its name. */

    if(!m_waveData.get() || (m_selStart >= m_selStop))
    {
        /** Nothing to do. */

        return false;
    }

    /** Build a temp file name for this wav. */

    fileName << m_storageDirectory
             << c_tempPrefix
             << m_filePrefix
             << tempWavCounter++
             << ".wav";

    SF_INFO sfInfo;

    sfInfo.channels = m_channels;
    sfInfo.format = m_format;
    sfInfo.samplerate = m_sampleRate;

    SNDFILE* sndFile = sf_open(fileName.c_str(), SFM_WRITE, &sfInfo);

    if(!sndFile)
    {
        int result = sf_error(sndFile);

        switch (result)
        {
        case 0:
            wxMessageBox("SF_ERR_NO_ERROR", fileName);
            break;
        case 1:
            wxMessageBox("SF_ERR_UNRECOGNISED_FORMAT");
            break;
        case 2:
            wxMessageBox("SF_ERR_SYSTEM", fileName);
            break;
        case 3:
            wxMessageBox("SF_ERR_MALFORMED_FILE");
            break;
        case 4:
            wxMessageBox("SF_ERR_UNSUPPORTED_ENCODING");
            break;
        default:
            wxMessageBox("SOMETHING WENT AWRY");

        }

        return false;
    }

    int delta = m_frameCount / c_waveDisplayWidth;

    int start = m_selStart * delta;
    int frames = (m_selStop - m_selStart) * delta;

    sf_writef_double(sndFile, m_waveData.get() + (start * m_channels), frames);

    sf_write_sync(sndFile);

    sf_close(sndFile);

    return true;
}

//---------------------------------------------------------------------------

void DetailDialog::LoadWavFile(wxString fileName)
{
    SF_INFO sf_info;

    sf_info.format = 0;

    SNDFILE* snd_file = sf_open(fileName.c_str(), SFM_READ, &sf_info);

    if(snd_file)
    {
        /** Create buffer. */

        m_frameCount = sf_info.frames;
        m_channels = sf_info.channels;
        m_sampleRate = sf_info.samplerate;
        m_format = sf_info.format;

        if(m_channels < 1 || m_channels > 2)
        {
            wxMessageBox("Only mono and stereo channels are supported");
            sf_close(snd_file);
            return;
        }

        m_waveData.reset(new double[m_frameCount * m_channels]);

        int read = sf_readf_double(snd_file, m_waveData.get(), m_frameCount);

        if(read != m_frameCount)
        {
            wxMessageBox("Read error");
            return;
        }

        /** Draw wave data to background buffer. */

        /** Create a memory device context */

        wxMemoryDC dc;

        /** Select the bitmap into the DC. */

        dc.SelectObject(*m_drawBuffer);

        /** If an image exists, load it. If not, create and save it for next time. */
        /** To speed up this program, generally all these BMP files should already have been generated */
        /** by a batch program. */

        wxString mapFile = fileName + ".bmp";

        if(wxFileExists(mapFile))
        {
            wxBitmap bitmap;
            bitmap.LoadFile(mapFile, wxBITMAP_TYPE_BMP);
            dc.DrawBitmap(bitmap, 0, 0);
        }
        else
        {
            /** This can be quite a lengthy operation, so change cursor. */

            SetCursor(wxCURSOR_WAIT);

            /** Set the background. */

            dc.SetBackground(*wxBLACK_BRUSH);

            /** Color the bitmap. */

            dc.Clear();

            /** Draw the left or only channel. */

            dc.SetPen(wxPen(*wxWHITE, 2, wxSOLID));
            int prevX = 0;
            int prevY = 50;

            double yDiv = (double) c_waveDisplayWidth / (double) m_frameCount;

            for(int idx = 0; idx < m_frameCount; ++idx)
            {
                double sample = (m_waveData.get())[idx * m_channels]; /** LEFT CHANNEL or the only channel if mono. */

                int newY = (int)(sample * 50.0) + 50;
                int newX = (double)idx * yDiv;

                dc.DrawLine(prevX, prevY, newX, newY);

                prevX = newX;
                prevY = newY;
            }

            /** Save to disk. */

            wxBitmap bitmap = dc.GetAsBitmap();
            bitmap.SaveFile(mapFile, wxBITMAP_TYPE_BMP);

            /** Carry on as usual. */

            SetCursor(wxCURSOR_DEFAULT);
        }

        /** Render an inverted bitmap for selecting. */

        wxMemoryDC inverted_dc;

        inverted_dc.SelectObject(*m_invertedBuffer);

        inverted_dc.Blit
        (
            0,
            0,
            c_waveDisplayWidth,
            c_waveDisplayHeight,
            &dc,
            0,
            0,
            wxSRC_INVERT,
            false
        );

        /** Now done with the DC's. */

        dc.SelectObject(wxNullBitmap);
        inverted_dc.SelectObject(wxNullBitmap);

        /** Select everything. */

        m_selStart = c_minSel;
        m_selStop = c_maxSel;

        /** Redraw panel. */

        Panel1->Refresh();
        Panel1->Update();
    }
    else
    {
        int result = sf_error(snd_file);

        switch (result)
        {
        case 0:
            wxMessageBox("SF_ERR_NO_ERROR", fileName);
            break;
        case 1:
            wxMessageBox("SF_ERR_UNRECOGNISED_FORMAT");
            break;
        case 2:
            wxMessageBox("SF_ERR_SYSTEM", fileName);
            break;
        case 3:
            wxMessageBox("SF_ERR_MALFORMED_FILE");
            break;
        case 4:
            wxMessageBox("SF_ERR_UNSUPPORTED_ENCODING");
            break;
        }
    }

    sf_close(snd_file);
}

//---------------------------------------------------------------------------

void DetailDialog::OnExitButtonClick(wxCommandEvent& event)
{
    Close();
}

//---------------------------------------------------------------------------

void DetailDialog::PlayCurrentSelection()
{
    wxString fileName;

    if(CreateWavFile(fileName))
    {
        m_sound.Create(fileName);

        m_sound.Play(wxSOUND_ASYNC);
    }
}

//---------------------------------------------------------------------------

void DetailDialog::OnPlayButtonClick(wxCommandEvent& event)
{
    PlayCurrentSelection();
}

//---------------------------------------------------------------------------

void DetailDialog::OnStopButtonClick(wxCommandEvent& event)
{
    m_sound.Stop();
}

//---------------------------------------------------------------------------

void DetailDialog::OnPanel1Paint(wxPaintEvent& event)
{
    wxMemoryDC memDC;
    memDC.SelectObject(*m_drawBuffer);

    /** First draw the entire normal bitmap on the destination DC. */

    wxBufferedPaintDC destDC(Panel1);

    destDC.Blit
    (
        0,
        0,
        c_waveDisplayWidth,
        c_waveDisplayHeight,
        &memDC,
        0,
        0,
        wxCOPY,
        false
    );

    memDC.SelectObject(wxNullBitmap);

    /** Now copy the selected portion over it. */

    memDC.SelectObject(*m_invertedBuffer);

    destDC.Blit
    (
        m_selStart,
        0,
        m_selStop - m_selStart,
        c_waveDisplayHeight,
        &memDC,
        m_selStart,
        0,
        wxCOPY,
        false
    );
}

//---------------------------------------------------------------------------

void DetailDialog::OnPanel1EraseBackground(wxEraseEvent& event)
{
    /** Do nothing on purpose. Entire thing is drawn in OnPanel1Paint(). */
}

//---------------------------------------------------------------------------

void DetailDialog::OnPanel1LeftDown(wxMouseEvent& event)
{
    if(event.GetY() > (c_waveDisplayHeight * .5))
    {
        /** Lower half is for grabbing a selection, much like Pro Tools multi tool. */

        /** If user starts a'dragging, we can be sure they no longer want to hear the selection playing. */

        m_sound.Stop();

        wxString fileName;

        if(CreateWavFile(fileName))
        {
            wxFileDataObject* data = new wxFileDataObject();

            data->AddFile(fileName);

            wxDropSource dragSource(this);
            dragSource.SetData(*data);

            /*wxDragResult dragResult = */ dragSource.DoDragDrop();

            wxDELETE(data);
        }
    }
    else
    {
        /** Upper half of waveform is for selecting, much like Pro Tools multi tool. */

        m_selecting = true;

        /** With Shift pressed we can extend selection. */

        if(GetAsyncKeyState(VK_SHIFT))
        {
            m_sel2 = event.GetX();

            if(m_sel2 < m_selStart)
            {
                m_sel1 = m_selStop;
            }
            else if(m_sel2 > m_selStop)
            {
                m_sel1 = m_selStart;
            }
        }
        else
        {
            m_sel1 = event.GetX();
        }
    }
}

//---------------------------------------------------------------------------

void DetailDialog::OnPanel1LeftUp(wxMouseEvent& event)
{
    m_selecting = false;

    /** Auto play if lifting the mouse in selection mode. */

    if(event.GetY() <= (c_waveDisplayHeight * .5))
    {
        PlayCurrentSelection();
    }
}

//---------------------------------------------------------------------------

void DetailDialog::OnPanel1MouseMove(wxMouseEvent& event)
{
    if(m_selecting)
    {
        m_sel2 = event.GetX();

        /** Which is sel start and which is sel stop now? */

        m_selStart = std::min(m_sel1, m_sel2);
        m_selStop = std::max(m_sel1, m_sel2);

        /** Guard out of bounds. */

        m_selStart = std::max(m_selStart, c_minSel);
        m_selStop = std::min(m_selStop, c_maxSel);

        Panel1->Update();
        Panel1->Refresh();
    }
    else
    {
        SetCursor((event.GetY() > (c_waveDisplayHeight * .5)) ? wxCURSOR_HAND : wxCURSOR_IBEAM);
    }
}

//---------------------------------------------------------------------------

void DetailDialog::OnPanel1MouseLeave(wxMouseEvent& event)
{
    /** It's better if selection stops when the user leaves the boundaries of the panel. */

    m_selecting = false;

    /** In the panel we mimic the Pro Tools multi tool, but when we leave it, we should return to normal. */

    SetCursor(wxCURSOR_DEFAULT);

    /** Auto play if lifting the mouse in selection mode. */
    /** Bug/feature: plays every time the user leaves the waveform */

//    if(event.GetY() <= (c_waveDisplayHeight * .5))
//    {
//        PlayCurrentSelection();
//    }
}

//---------------------------------------------------------------------------
