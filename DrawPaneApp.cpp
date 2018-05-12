/***************************************************************
 * Name:      DrawPaneApp.cpp
 * Purpose:   Code for Application Class
 * Author:    Kevin Grifo ()
 * Created:   2018-02-11
 * Copyright: Kevin Grifo ()
 * License:
 **************************************************************/

#include "wx_pch.h"
#include "DrawPaneApp.h"

//(*AppHeaders
#include "DrawPaneMain.h"
#include <wx/image.h>
//*)

IMPLEMENT_APP(DrawPaneApp);

bool DrawPaneApp::OnInit()
{
    //(*AppInitialize
    bool wxsOK = true;
    wxInitAllImageHandlers();
    if ( wxsOK )
    {
    	DrawPaneDialog Dlg(0);
    	SetTopWindow(&Dlg);
    	Dlg.ShowModal();
    	wxsOK = false;
    }
    //*)
    return wxsOK;

}
