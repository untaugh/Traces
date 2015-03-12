/*
 * main.cpp
 * 
 * Copyright 2015 Oskari Rundgren <orundg@gmail.com>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */

#include <wx/wx.h>
#include <wx/xml/xml.h>
#include <wx/graphics.h>
#include <string>
#include "board.h"
#include "paintboard.h"
#include "xml.h"

/* Bitmaps. */
#include "icons/track.xpm"
#include "icons/move.xpm"
#include "icons/erase.xpm"
#include "icons/hand.xpm"
#include "icons/pad.xpm"

class TracesApp: public wxApp
{
public:
    virtual bool OnInit();
};

class MyFrame: public wxFrame
{
  public:
    MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size);
  private:
    PaintBoard *paintboard; 
    wxToolBar *toolBar;
    void OnExit(wxCommandEvent& event);
    void OnTool(wxCommandEvent& event);
    void OnHello(wxCommandEvent& event);
    void OnSave(wxCommandEvent& event);
    void OnOpen(wxCommandEvent& event);
    wxDECLARE_EVENT_TABLE();
};

enum
{
  ID_Hello = 1,
  ID_Open = 2,
  ID_Save = 3,
};

wxBEGIN_EVENT_TABLE(MyFrame, wxFrame)
    EVT_MENU(ID_Hello,   MyFrame::OnHello)
    EVT_MENU(ID_Save,   MyFrame::OnSave)
    EVT_MENU(ID_Open,   MyFrame::OnOpen)
    EVT_MENU(wxID_EXIT,  MyFrame::OnExit)
    EVT_TOOL_RANGE(TOOL_TRACK, TOOL_HAND, MyFrame::OnTool)
wxEND_EVENT_TABLE()

wxIMPLEMENT_APP(TracesApp);



bool TracesApp::OnInit()
{
    MyFrame *frame = new MyFrame("Traces", wxPoint(50, 50), wxSize(600, 400) );
    frame->Show(true);
    return true;
}

MyFrame::MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
        : wxFrame(NULL, wxID_ANY, title, pos, size)
{
  /* Create menu. */
  wxMenu *menuFile = new wxMenu;
  menuFile->Append(ID_Hello, "&Hello...\tCtrl-H",
                   "Help string shown in status bar for this menu item");
  menuFile->Append(ID_Open, "Open...", "Open file. ");
  menuFile->AppendSeparator();
  menuFile->Append(ID_Save, "Save", "Save file. ");
  menuFile->AppendSeparator();
  menuFile->Append(wxID_EXIT);
  wxMenuBar *menuBar = new wxMenuBar;
  menuBar->Append( menuFile, "&File" );
  SetMenuBar( menuBar );
  
  /* Create toolbar. */
  toolBar = CreateToolBar();
  wxString str("Test");
  
  toolBar->AddTool(TOOL_TRACK, wxString("Track"), wxBitmap(track_xpm), wxString("Create new track. "),wxITEM_RADIO);
  toolBar->AddTool(TOOL_MOVE, wxString("Move"), wxBitmap(move_xpm), wxString("Move track. "),wxITEM_RADIO);
  toolBar->AddTool(TOOL_ERASE, wxString("Erase"), wxBitmap(erase_xpm), wxString("Erase Track. "),wxITEM_RADIO);
  toolBar->AddTool(TOOL_HAND, wxString("Hand"), wxBitmap(hand_xpm), wxString("Hand. "),wxITEM_RADIO);
  toolBar->AddTool(TOOL_HAND, wxString("Pad"), wxBitmap(pad_xpm), wxString("New pad. "),wxITEM_RADIO);

  toolBar->Realize();

  /* Create box sizer. */
  wxBoxSizer *box = new wxBoxSizer(wxVERTICAL);
  paintboard = new PaintBoard();
  paintboard->Create(this, wxID_ANY);
  paintboard->SetBackgroundColour( *wxWHITE );
  paintboard->SetScrollRate( 1, 1 );
  //paintboard->SetVirtualSize(1000,1000);
  paintboard->SetBoard(new Board);
  box->Add(paintboard, 1, wxALL | wxEXPAND, 0);
  box->SetMinSize(300,300);
  SetSizer(box);

  /* Status bar. */
  CreateStatusBar();
  SetStatusText("Welcome to statusbar!");
}

void MyFrame::OnTool(wxCommandEvent& event)
{
  if (paintboard)
    paintboard->SetTool(event.GetId());
}

void MyFrame::OnExit(wxCommandEvent& event)
{
    Close( true );
}

void printNodeInfo(wxXmlNode* node)
{
  std::cout << "*** Node info ***\n";
  std::cout << "Name:" << node->GetName() << "\n";
  std::cout << "Type:" << node->GetType() << "\n";
  std::cout << "Content:" << node->GetContent() << "\n";
}

void BoardSaveToFile(Board* board, const char* fileName)
{
  wxXmlDocument doc;

  wxXmlNode* rootNode = new wxXmlNode(wxXML_ELEMENT_NODE,"board");

  doc.SetRoot(rootNode);

  for (Segment* segment: board->GetSegments())
  {
    std::cout << "Adding segment:" << segment->GetId() << "\n";
    rootNode->AddChild(segmentToNode(segment));
  }

  doc.Save("save2.xml");
}

void MyFrame::OnSave(wxCommandEvent& event)
{
  BoardSaveToFile(paintboard->GetBoard(), "file");
}

void MyFrame::OnOpen(wxCommandEvent& event)
{
  wxXmlDocument doc;

  if (!doc.Load("save2.xml"))
  {
    std::cerr << "Could not open document.";
    return;
  }

  if (doc.GetRoot()->GetName() != "board")
  {
    std::cerr << "Root element not found.";
    return;
  }

  wxXmlNode *child = doc.GetRoot()->GetChildren();
  while (child) {
  if (child->GetName() == "segment")
  {
    Segment* segment = new Segment();
    
    wxXmlNode *nodeSegment = child->GetChildren();
    while (nodeSegment)
    {
      if (nodeSegment->GetName() == "start")
      {
        std::shared_ptr <Point> pointStart(new Point);
        wxString xs = nodeSegment->GetAttribute("x");
        wxString ys = nodeSegment->GetAttribute("y");
        pointStart->Set(std::stod(xs.ToStdString()),std::stod(ys.ToStdString()));
        segment->SetStart(pointStart);
      }
      else if (nodeSegment->GetName() == "end")
      {
        std::shared_ptr <Point> pointEnd(new Point);
        wxString xs = nodeSegment->GetAttribute("x");
        wxString ys = nodeSegment->GetAttribute("y");
        pointEnd->Set(std::stod(xs.ToStdString()),std::stod(ys.ToStdString()));
        segment->SetEnd(pointEnd);
      }
      segment->SetWidth(10.0);

      nodeSegment = nodeSegment->GetNext();
    }
    paintboard->GetBoard()->AddSegment(segment);
  }

  child = child->GetNext();
  }

  Refresh();
  Update();
}

void MyFrame::OnHello(wxCommandEvent& event)
{
  std::cout << "Hello!\n";
}
