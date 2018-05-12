/***************************************************************
 * Name:      DrawPaneMain.cpp
 * Purpose:   Code for Application Frame
 * Author:    Kevin Grifo ()
 * Created:   2018-02-11
 * Copyright: Kevin Grifo ()
 * License:
 **************************************************************/



#include "wx_pch.h"
#include "DrawPaneMain.h"
#include <wx/msgdlg.h>
#include <wx/colordlg.h>
#include <wx/filedlg.h>
#include "tinyxml.h"
#include <wx/wfstream.h>

#include <iostream>

#define TIXML_USE_STL

using namespace std;

//The defaults for the pen.
int penWidth = 1;
wxPen penGlobal(*wxBLACK, penWidth);
int x = 0;
int y = 0;


//(*InternalHeaders(DrawPaneDialog)
#include <wx/string.h>
#include <wx/intl.h>
//*)

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

//(*IdInit(DrawPaneDialog)
const long DrawPaneDialog::ID_PANEL1 = wxNewId();
const long DrawPaneDialog::ID_BUTTON1 = wxNewId();
const long DrawPaneDialog::ID_SPINCTRL1 = wxNewId();
const long DrawPaneDialog::ID_STATICLINE1 = wxNewId();
const long DrawPaneDialog::ID_BUTTON2 = wxNewId();
const long DrawPaneDialog::ID_BUTTON3 = wxNewId();
const long DrawPaneDialog::ID_BUTTON4 = wxNewId();
//*)

BEGIN_EVENT_TABLE(DrawPaneDialog,wxDialog)
    //(*EventTable(DrawPaneDialog)
    //*)
END_EVENT_TABLE()

DrawPaneDialog::DrawPaneDialog(wxWindow* parent,wxWindowID id)
{
    //(*Initialize(DrawPaneDialog)
    Create(parent, wxID_ANY, _("wxWidgets app"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("wxID_ANY"));
    BoxSizer1 = new wxBoxSizer(wxVERTICAL);
    Panel1 = new wxPanel(this, ID_PANEL1, wxDefaultPosition, wxSize(295,215), wxTAB_TRAVERSAL, _T("ID_PANEL1"));
    BoxSizer1->Add(Panel1, 1, wxALL|wxEXPAND, 5);
    BoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
    Button1 = new wxButton(this, ID_BUTTON1, _("Set Color"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON1"));
    BoxSizer2->Add(Button1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    SpinCtrl1 = new wxSpinCtrl(this, ID_SPINCTRL1, _T("1"), wxDefaultPosition, wxDefaultSize, 0, 1, 10, 1, _T("ID_SPINCTRL1"));
    SpinCtrl1->SetValue(_T("1"));
    BoxSizer2->Add(SpinCtrl1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticLine1 = new wxStaticLine(this, ID_STATICLINE1, wxDefaultPosition, wxSize(10,-1), wxLI_VERTICAL, _T("ID_STATICLINE1"));
    BoxSizer2->Add(StaticLine1, 0, wxALL|wxEXPAND, 4);
    Button2 = new wxButton(this, ID_BUTTON2, _("Quit"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON2"));
    BoxSizer2->Add(Button2, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 4);
    Button3 = new wxButton(this, ID_BUTTON3, _("Save"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON3"));
    BoxSizer2->Add(Button3, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    Button4 = new wxButton(this, ID_BUTTON4, _("Open"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON4"));
    BoxSizer2->Add(Button4, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    BoxSizer1->Add(BoxSizer2, 0, wxEXPAND, 4);
    SetSizer(BoxSizer1);
    BoxSizer1->Fit(this);
    BoxSizer1->SetSizeHints(this);

    Panel1->Connect(wxEVT_PAINT,(wxObjectEventFunction)&DrawPaneDialog::OnPanel1Paint,0,this);
    Panel1->Connect(wxEVT_LEFT_DOWN,(wxObjectEventFunction)&DrawPaneDialog::OnPanel1LeftDown,0,this);
    Panel1->Connect(wxEVT_LEFT_DCLICK,(wxObjectEventFunction)&DrawPaneDialog::OnPanel1LeftDClick,0,this);
    Connect(ID_BUTTON1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&DrawPaneDialog::OnButton1Click);
    Connect(ID_SPINCTRL1,wxEVT_COMMAND_SPINCTRL_UPDATED,(wxObjectEventFunction)&DrawPaneDialog::OnSpinCtrl1Change);
    Connect(ID_BUTTON2,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&DrawPaneDialog::OnQuit);
    Connect(ID_BUTTON3,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&DrawPaneDialog::OnButton3Click);
    Connect(ID_BUTTON4,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&DrawPaneDialog::OnOpenClick);
    //*)
}

wxPoint pointBuffer[150];
int bufferPos = 0;
bool eraseMode = false;

//const char* pFilename

void write_app_settings_doc()
{
    TiXmlDocument doc;
    //TiXmlElement* msg;
    TiXmlDeclaration* dec1 = new TiXmlDeclaration("1.0", "", "");
    doc.LinkEndChild(dec1);

    TiXmlElement* root = new TiXmlElement("MyDrawing");
    doc.LinkEndChild(root);

    TiXmlElement* color = new TiXmlElement("PenColor");
    root->LinkEndChild(color);
    color->SetAttribute("Color", "wxRED");

    TiXmlElement* width = new TiXmlElement("PenWidth");
    root->LinkEndChild(width);
    width->SetAttribute("Width", penWidth);

    for(int i = 0; i < bufferPos; i++){
        TiXmlElement* point = new TiXmlElement("Point");
        root->LinkEndChild(point);
        point->SetAttribute("x", pointBuffer[i].x);
        point->SetAttribute("y", pointBuffer[i].y);
    }



    doc.SaveFile("testing");
}


DrawPaneDialog::~DrawPaneDialog()
{
    //(*Destroy(DrawPaneDialog)
    //*)
}

void DrawPaneDialog::OnQuit(wxCommandEvent& event)
{
    Close();
}

void DrawPaneDialog::OnAbout(wxCommandEvent& event)
{

}



void DrawPaneDialog::OnPanel1LeftDown(wxMouseEvent& event)
{
    //if eraseMode is not activated draw a point
    //where the user clicks in the panel.
    if(!eraseMode){
        wxClientDC dc(Panel1);
        //Gets the position from the mouseclicsk event.
        //and draws the point on the panel.
//        wxPen pen(*wxRED);
//        pen.SetWidth(100);
        dc.SetPen(penGlobal);
        dc.DrawPoint(event.GetPosition());

        //Saves the point position in a buffer for
        //the double click event below.
        pointBuffer[bufferPos] = event.GetPosition();

        //Points to next index of bufferPos.
        bufferPos++;
    }
}

void DrawPaneDialog::OnPanel1LeftDClick(wxMouseEvent& event)
{
    wxClientDC dc(Panel1);

    //If eraseMode if false then when the
    //user double clicks the points will be connected.
    if(!eraseMode){
        //Draws a line from one point to the other,
        //and continues to do so until all points are connected.
        for(int i = 1; i < bufferPos; i++){
            dc.SetPen(penGlobal);
            dc.DrawLine(pointBuffer[i - 1], pointBuffer[i]);

        }
        //goes from connecting the points to
        //eraseM mode. When the user double clicks
        //the else block below will be executed.
        eraseMode = !eraseMode;
    }
    else{
        //set pointBuffer back to a empty array.
        for(int i = 0; i < bufferPos; i++){
            pointBuffer[i] = {};
        }
    dc.Clear();
    bufferPos = 0;
    eraseMode = false;
    }
}

void DrawPaneDialog::OnPanel1Paint(wxPaintEvent& event)
{
}

void DrawPaneDialog::OnButton1Click(wxCommandEvent& event)
{
    wxColourData data;
    data.SetChooseFull(true);

    for(int i = 0; i < 16; i++){
        wxColour color(i * 8, i * 8, i * 8);
        data.SetCustomColour(i, color);
    }

    wxColourDialog dialog(this, &data);
    if(dialog.ShowModal() == wxID_OK){
        wxColourData retData = dialog.GetColourData();
        wxColour col = retData.GetColour();

        penGlobal.SetColour(col);




    }
}

wxString path;

void DrawPaneDialog::OnSpinCtrl1Change(wxSpinEvent& event)
{
    penWidth = SpinCtrl1->GetValue();
    penGlobal.SetWidth(penWidth);
}



//Save Button
void DrawPaneDialog::OnButton3Click(wxCommandEvent& event)
{
    wxMessageBox(wxT("Drawing saved as testing.xml xml file click open to restore it"), wxT("Save Description"), wxOK | wxCENTRE,this);

    write_app_settings_doc();




//    wxString caption = wxT("Save your file");
//    wxString wildCard = wxT("XML files (*.xml)|*.xml");
//    wxString defaultDir = defaultDir = wxT("c:\\temp");
//    wxString defaultFilename = wxEmptyString;
//
//    wxFileDialog dialog(this, caption, defaultDir, defaultFilename, wildCard, wxFD_SAVE|wxFD_OVERWRITE_PROMPT);
//
//    if (dialog.ShowModal() == wxID_CANCEL){
//        return;
//    }



//    if(dialog.ShowModal() == wxID_SAVE|wxID_SAVEAS){
//        wxString path = dialog.GetPath();
        //wxMessageBox(dialog.GetPath(), wxT("The path"), wxOK | wxCENTRE | wxICON_ERROR, this);
//        int filterIndex = dialog.GetFilterIndex();
//    }




    //write_app_settings_doc(dialog.GetPath().fn_str());

}

//Open Button
void DrawPaneDialog::OnOpenClick(wxCommandEvent& event)
{
    TiXmlDocument doc("testing");
    if(!doc.LoadFile()){
        wxMessageBox(wxT("error loading file"), wxT("The path"), wxOK | wxCENTRE | wxICON_ERROR,this);
        return;
    }

    //This is the root element.
    TiXmlElement* root = doc.FirstChildElement();
    if(root == NULL){
        cerr << "Failed to load file: No root element." << endl;
        doc.Clear();
        return;
    }


    bufferPos = 0;
    for(TiXmlElement* elem = root->FirstChildElement(); elem != NULL; elem = elem->NextSiblingElement()){
        string elemName = elem->Value();
        const char* attr;
        int x;
        int y;
        int width;

        if(elemName == "PenColor"){
            attr = elem->Attribute("Color");
        }
        else if(elemName == "PenWidth"){
            elem->QueryIntAttribute("Width", &penWidth);
            penGlobal.SetWidth(penWidth);
        }
        else if(elemName == "Point"){
            elem->QueryIntAttribute("x", &pointBuffer[bufferPos].x);
            elem->QueryIntAttribute("y", &pointBuffer[bufferPos].y);
            bufferPos++;

        }
    }

    wxClientDC dc(Panel1);

    //If eraseMode if false then when the

        //Draws a line from one point to the other,
        //and continues to do so until all points are connected.
        for(int i = 1; i < bufferPos; i++){
            dc.SetPen(penGlobal);
            dc.DrawLine(pointBuffer[i - 1], pointBuffer[i]);

        }
        //goes from connecting the points to
        //eraseM mode. When the user clicks open
        //eraseMode will be set to true, meaning the next doubleclick will clear the screen.
        bufferPos = 0;
        eraseMode = true;





//    wxString caption = wxT("Save your file");
//    wxString wildCard = wxT("XML files (*.xml)|*.xml");
//    wxString defaultDir = defaultDir = wxT("c:\\temp");
//    wxString defaultFilename = wxEmptyString;
//
//    wxFileDialog dialog(this, caption, defaultDir, defaultFilename, wildCard, wxOPEN);
//
//    if (dialog.ShowModal() == wxID_CANCEL)
//        return;
//
//    if(dialog.ShowModal() == wxID_OK){
//        wxString path = dialog.GetPath();
//        int filterIndex = dialog.GetFilterIndex();
//    }
}
