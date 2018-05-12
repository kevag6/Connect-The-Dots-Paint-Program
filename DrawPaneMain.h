/***************************************************************
 * Name:      DrawPaneMain.h
 * Purpose:   Defines Application Frame
 * Author:    Kevin Grifo ()
 * Created:   2018-02-11
 * Copyright: Kevin Grifo ()
 * License:
 **************************************************************/

#ifndef DRAWPANEMAIN_H
#define DRAWPANEMAIN_H

//(*Headers(DrawPaneDialog)
#include <wx/spinctrl.h>
#include <wx/dialog.h>
#include <wx/sizer.h>
#include <wx/button.h>
#include <wx/panel.h>
#include <wx/statline.h>
//*)

class DrawPaneDialog: public wxDialog
{
    public:



        DrawPaneDialog(wxWindow* parent,wxWindowID id = -1);
        virtual ~DrawPaneDialog();

    private:

        //(*Handlers(DrawPaneDialog)
        void OnQuit(wxCommandEvent& event);
        void OnAbout(wxCommandEvent& event);
        void OnPanel1LeftDown(wxMouseEvent& event);
        void OnPanel1LeftDClick(wxMouseEvent& event);
        void OnPanel1Paint(wxPaintEvent& event);
        void OnButton1Click(wxCommandEvent& event);
        void OnButton4Click(wxCommandEvent& event);
        void OnSpinCtrl1Change(wxSpinEvent& event);
        void Save(wxCommandEvent& event);
        void OnButton3Click(wxCommandEvent& event);
        void OnOpenClick(wxCommandEvent& event);
        //*)

        //(*Identifiers(DrawPaneDialog)
        static const long ID_PANEL1;
        static const long ID_BUTTON1;
        static const long ID_SPINCTRL1;
        static const long ID_STATICLINE1;
        static const long ID_BUTTON2;
        static const long ID_BUTTON3;
        static const long ID_BUTTON4;
        //*)

        //(*Declarations(DrawPaneDialog)
        wxPanel* Panel1;
        wxButton* Button4;
        wxButton* Button1;
        wxButton* Button2;
        wxButton* Button3;
        wxBoxSizer* BoxSizer2;
        wxStaticLine* StaticLine1;
        wxBoxSizer* BoxSizer1;
        wxSpinCtrl* SpinCtrl1;
        //*)

        DECLARE_EVENT_TABLE()
};

#endif // DRAWPANEMAIN_H
