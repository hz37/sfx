/***************************************************************
 * Name:      Bitmap_builderApp.cpp
 * Purpose:   Code for Application Class
 * Author:    Hens Zimmerman (henszimmerman@gmail.com)
 * Created:   2015-03-02
 * Copyright: Hens Zimmerman (http://henszimmerman.com)
 * License:
 **************************************************************/

#ifdef WX_PRECOMP
#include "wx_pch.h"
#endif

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

#include "Bitmap_builderApp.h"

#include <wx/bitmap.h>
#include <wx/dcbuffer.h>
#include <wx/filefn.h>
#include <wx/filename.h>
#include <wx/stdpaths.h>
#include <wx/textfile.h>

#include <iostream>
#include <memory>

#include <sndfile.h>

wxString includeTrailingBackslash(wxString folder);
void makeAndSaveBitmap(wxString filename);

static const int waveDisplayWidth = 500;
static const int waveDisplayHeight = 100;

IMPLEMENT_APP(Bitmap_builderApp);

// Helper function to make sure paths end with a backslash.

wxString includeTrailingBackslash(wxString folder)
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



void makeAndSaveBitmap(wxString filename)
{
    if(!wxFileExists(filename))
    {
        std::cout << "Skipping " << filename << " (file not found)" << std::endl;
        return;
    }

    wxString mapFile = filename + ".bmp";

    // No need to recreate existing files.

    if(wxFileExists(mapFile))
    {
        std::cout << "Skipping " << mapFile << " (exists)" << std::endl;
        return;
    }

    SF_INFO sf_info;

    sf_info.format = 0;

    SNDFILE* snd_file = sf_open(filename.c_str(), SFM_READ, &sf_info);

    if(snd_file)
    {
        // Create buffer.

        int m_frameCount = sf_info.frames;
        int m_channels = sf_info.channels;

        if(m_channels < 1 || m_channels > 2)
        {
            std::cout << "Skipping " << filename << " (multi channel)" << std::endl;
            sf_close(snd_file);
            return;
        }

        std::auto_ptr <double> m_waveData;
        m_waveData.reset(new double[m_frameCount * m_channels]);

        int read = sf_readf_double(snd_file, m_waveData.get(), m_frameCount);

        sf_close(snd_file);

        if(read != m_frameCount)
        {
            std::cout << "Error reading " << filename << std::endl;
            return;
        }

        std::cout << "Creating " << mapFile << std::endl;

        wxBitmap* m_drawBuffer = new wxBitmap(waveDisplayWidth, waveDisplayHeight, -1);

        // Create a memory device context
        wxMemoryDC dc;
        dc.SelectObject(*m_drawBuffer);

        // Set the background
        dc.SetBackground(*wxBLACK_BRUSH);

        // Color the bitmap green
        dc.Clear();

        // Draw the left channel;

        dc.SetPen(wxPen(*wxWHITE, 2, wxSOLID));

        int prevX = 0;
        int prevY = 50;

        double yDiv = (double) waveDisplayWidth / (double) m_frameCount;

        for(int idx = 0; idx < m_frameCount; ++idx)
        {
            double sample = (m_waveData.get())[idx * m_channels]; // LEFT CHANNEL or the only channel if mono

            int newY = (int)(sample * 50.0) + 50;
            int newX = (double)idx * yDiv;

            dc.DrawLine(prevX, prevY, newX, newY);

            prevX = newX;
            prevY = newY;
        }

        // Save to disk.

        wxBitmap bitmap = dc.GetAsBitmap();
        bitmap.SaveFile(mapFile, wxBITMAP_TYPE_BMP);

        delete m_drawBuffer;
    }
}


bool Bitmap_builderApp::OnInit()
{
    // Load data from exe directory.

    wxStandardPaths sp = wxStandardPaths::Get();

    wxString exePath = includeTrailingBackslash(wxPathOnly(sp.GetExecutablePath()));
    wxString configFilename = exePath + _("config.txt");

    wxString m_sfxRootDirectory;

    if(wxFileExists(configFilename))
    {
        wxTextFile configFile;
        configFile.Open(configFilename);
        m_sfxRootDirectory = configFile.GetFirstLine();
        configFile.Close();
    }
    else
    {
        std::cout << "config file not found" << std::endl;
        Exit();
    }

    wxString dataFilename = m_sfxRootDirectory + "data.txt";

    if(!wxFileExists(dataFilename))
    {
        std::cout << "data file not found in designated location" << std::endl;
        Exit();
    }

    wxTextFile dataFile;
    dataFile.Open(dataFilename);

    wxString firstLine = dataFile.GetFirstLine();

    // First line can be anything.
    // Most likely a description. HZ, jan 5, 2014.

    while(!dataFile.Eof())
    {
        // Skip description. We ain't needin' that.

        wxString description = dataFile.GetNextLine().Lower();

        if(description.length() == 0)
        {
            // Done scanning.

            break;
        }

        wxString fileName = m_sfxRootDirectory + dataFile.GetNextLine();

        makeAndSaveBitmap(fileName);
    }

    // Done!

    dataFile.Close();

    std::cout << "Done." << std::endl;

    Exit();

    return true;
}
