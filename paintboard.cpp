/*
 * paintboard.h
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

#include "paintboard.h"

/* WX Event Table. */
wxBEGIN_EVENT_TABLE(PaintBoard, wxScrolled <wxWindow>)
    EVT_PAINT(PaintBoard::OnPaint)
    EVT_LEFT_DOWN(PaintBoard::OnClick)
    EVT_LEFT_UP(PaintBoard::OnMouseLeftUp)
    EVT_MOTION(PaintBoard::OnMotion)
    EVT_MOUSEWHEEL(PaintBoard::OnMouseWheel)
wxEND_EVENT_TABLE()

void PaintBoard::SetBoard(Board *board)
{
  this->board = board;
}

Board* PaintBoard::GetBoard(void)
{
  return board;
}

/* Paint event. */
void PaintBoard::OnPaint(wxPaintEvent &event)
{  
  wxPaintDC dc(this);
  DoPrepareDC(dc);
  wxGraphicsContext *gc = wxGraphicsContext::Create( dc );
  PaintGrid(gc,30*zoom);
  if (gc)
  {
    wxGraphicsPath path = gc->CreatePath();
    for (Segment *segment : this->GetBoard()->GetSegments())
    {
      PaintSegment(gc, segment);
    }
    delete gc;
  }
}

/* Mouse is clicked on window */
void PaintBoard::OnClick(wxMouseEvent &event)
{
  if (tool == TOOL_ERASE)
  {
    for (int id: mouseOver)
    {
      if (GetBoard()->EraseSegment(id))
      {
        Refresh(); // Optimize here if graphics slow. 
        Update();
      }
    }
  }
  else
  {    
    if (board && mouseOver.empty())
    {
      std::shared_ptr<Point> p(new Point);
      double screenX, screenY;
      int scrollX, scrollY;
      screenX = event.GetX();
      screenY = event.GetY();
      CalcUnscrolledPosition(screenX,screenY,&scrollX,&scrollY);
      wxRealPoint gridPoint = GetGridPoint(screenX, screenY);
      double boardX, boardY;
      ScreenToBoard(gridPoint.x,gridPoint.y,&boardX,&boardY);
      p->Set(boardX, boardY);
      tempSegment = new Segment; // Start drawing a new segment. 
      tempSegment->SetStart(p); // Starting point of segment. 
      tempSegment->SetWidth(10.0);
    }
  }
}

/* Paint a segment in the window. */
void PaintBoard::PaintSegment(wxGraphicsContext *gc, Segment *segment)
{
  if (IsMouseOver(segment->GetId()))
    gc->SetBrush(*wxGREEN_BRUSH);
  else
    gc->SetBrush(*wxRED_BRUSH);

  gc->SetPen( *wxBLACK_PEN);
  wxGraphicsPath path = gc->CreatePath();

  path.AddPath(SegmentToPath(segment));
  
  gc->FillPath(path);
  gc->StrokePath(path);
}

/* Return closest grid point. */
wxRealPoint PaintBoard::GetGridPoint(int x, int y)
{
  int gridX, gridY, xScroll, yScroll, gs=gridSize*zoom;

  CalcUnscrolledPosition(x,y,&xScroll,&yScroll);

  if (xScroll%gs < gs/2)
    gridX = xScroll - xScroll%gs;
  else
    gridX = xScroll - xScroll%gs + gs;

  if (yScroll%gs < gs/2)
    gridY = yScroll - yScroll%gs;
  else
    gridY = yScroll - yScroll%gs + gs;

  return wxRealPoint(gridX, gridY);
}

/* Paint a grid */
void PaintBoard::PaintGrid(wxGraphicsContext *gc, double gridSize)
{
  int sizeX, sizeY, x, y;

  gc->SetBrush(*wxRED_BRUSH);

  gc->SetPen( *wxBLACK_PEN);
  
  wxGraphicsPath path = gc->CreatePath();

  GetVirtualSize(&sizeX, &sizeY);

  for (x = 0; x<=sizeX; x+=gridSize)
  {
    for (y = 0; y<=sizeY; y+=gridSize)
    {
      path.MoveToPoint(x,y);
      path.AddLineToPoint(x,y);
    }
  }

  gc->FillPath(path);
  gc->StrokePath(path);

}

/* Mouse button is lifted. */
void PaintBoard::OnMouseLeftUp(wxMouseEvent &event)
{
  if (tempSegment)
  {
    wxPoint gridPoint = GetGridPoint(event.GetX(),event.GetY()); // Nearest grid point.
    double boardX, boardY;
    ScreenToBoard(gridPoint.x,gridPoint.y,&boardX,&boardY);
    tempSegment->GetEnd()->Set(boardX, boardY);
    GetBoard()->AddSegment(tempSegment); // Add segment to board. 
    tempSegment = NULL; // Remove the temporary segment used for drawing. 
  }
}

void PaintBoard::ScreenToBoard(double x, double y, double *toX, double *toY)
{
  *toX = x/zoom;
  *toY = y/zoom;
}

void PaintBoard::BoardToScreen(double x, double y, double *toX, double *toY)
{
  *toX = x*zoom;
  *toY = y*zoom;
}

/* Mouse is moved over window. */
void PaintBoard::OnMotion(wxMouseEvent &event)
{
  double boardX, boardY, screenX, screenY;

  int scrollX, scrollY;

  /* Calculate all coords. */
  screenX = event.GetX();
  screenY = event.GetY();
  
  CalcUnscrolledPosition(screenX,screenY,&scrollX,&scrollY);

  ScreenToBoard(scrollX, scrollY, &boardX, &boardY); // Convert screen coords to board coords. 

  wxPoint gridPoint = GetGridPoint(screenX,screenY);



  /* Print status bar. */
  std::stringstream ss;
  
  ss  << "Virtual (" << scrollX << "," << scrollY
      << "), Screen (" << event.GetX() << "," << event.GetY()
      << "), Board (" << boardX << "," << boardY
      << "), Zoom (" << zoom
      << ")\n";

  ((wxFrame*)GetParent())->SetStatusText(ss.str());

  
  /* Check if mouse is over segments. */
  mouseOver.clear();
  for (Segment *segment : this->GetBoard()->GetSegments())
  {
    wxGraphicsPath path = SegmentToPath(segment);

    if (path.Contains(scrollX,scrollY))
    {
      mouseOver.insert(segment->GetId());
    }
  }

  Refresh(); // Optimize here if graphics slow. 
  Update();

  /* Draw circle on grid. */
  wxClientDC dc2(this);
  DoPrepareDC(dc2);
  wxGraphicsContext *gc2 = wxGraphicsContext::Create( dc2 );

  gc2->SetPen( *wxBLACK_PEN);
  gc2->SetBrush(*wxRED_BRUSH);

  if (gc2)
  {
    wxGraphicsPath path = gc2->CreatePath();
    path.AddCircle(gridPoint.x, gridPoint.y, 5.0);
    gc2->StrokePath(path);
    gc2->FillPath(path);
    delete gc2;
  }

  
  if (event.Dragging())
  {
    if (tempSegment)
    {
      double boardX, boardY;
      ScreenToBoard(gridPoint.x,gridPoint.y,&boardX,&boardY);

      if (!tempSegment->GetEnd())
      {
        std::shared_ptr<Point> pTemp(new Point);
        pTemp->Set(boardX, boardY);
        tempSegment->SetEnd(pTemp);
      }
      else
      {
        tempSegment->GetEnd()->Set(boardX, boardY);
      }
      
      wxClientDC dc(this);
      DoPrepareDC(dc);
      wxGraphicsContext *gc = wxGraphicsContext::Create( dc );
      if (gc)
      {
        PaintSegment(gc, tempSegment);
        delete gc;
      }
    }
  }
}

/* Make path out of segment data. */
wxGraphicsPath PaintBoard::SegmentToPath(Segment *segment)
{
  wxGraphicsContext *gc = wxGraphicsContext::Create();
  wxGraphicsPath path = gc->CreatePath();
      
  double xDelta = ( segment->GetEndX() - segment->GetStartX() );
  double yDelta = ( segment->GetEndY() - segment->GetStartY() );

  if (xDelta == 0) xDelta = 0.0001; // Avoid divide by zero. 
  
  double angle = atan( yDelta/xDelta );

  if (xDelta < 0)
  {
    angle += M_PI;
  }

  // p1 and p4 not needed for arc drawing. 
  //double p1x = cos(angle + M_PI_2)*segment->GetWidth() + segment->GetStartX();
  //double p1y = sin(angle + M_PI_2)*segment->GetWidth() + segment->GetStartY();
  double p2x = cos(angle - M_PI_2)*segment->GetWidth() + segment->GetStartX();
  double p2y = sin(angle - M_PI_2)*segment->GetWidth() + segment->GetStartY();

  double p3x = cos(angle - M_PI_2)*segment->GetWidth() + segment->GetEndX();
  double p3y = sin(angle - M_PI_2)*segment->GetWidth() + segment->GetEndY();
  //double p4x = cos(angle + M_PI_2)*segment->GetWidth() + segment->GetEndX();
  //double p4y = sin(angle + M_PI_2)*segment->GetWidth() + segment->GetEndY();  

  path.AddArc(segment->GetStartX(),segment->GetStartY(),segment->GetWidth(), angle + M_PI_2, angle - M_PI_2, TRUE);
  path.AddLineToPoint(p2x, p2y);
  path.AddLineToPoint(p3x, p3y);  
  path.AddArc(segment->GetEndX(),segment->GetEndY(),segment->GetWidth(), angle - M_PI_2, angle + M_PI_2, TRUE);

  path.CloseSubpath();

  wxGraphicsMatrix matrix = gc->CreateMatrix();
  matrix.Scale(zoom,zoom);

  path.Transform(matrix);

  return path;
}

/* Check if the mouse is over item with this id. */
bool PaintBoard::IsMouseOver(int id)
{
  std::set<int>::iterator it;
  it = mouseOver.find(id);

  if (it != mouseOver.end())
  {
    return TRUE; // Id was found in the set. 
  }
  else
  {
    return FALSE; // Not found. 
  }
}

/* Mouse wheel scroll. */
void PaintBoard::OnMouseWheel(wxMouseEvent &event)
{
  double boardX, boardY, screenX, screenY;

  int scrollX, scrollY;
  
  screenX = event.GetX();
  screenY = event.GetY();
  
  CalcUnscrolledPosition(screenX,screenY,&scrollX,&scrollY);

  ScreenToBoard(scrollX, scrollY, &boardX, &boardY); // Convert screen coords to board coords. 

  double newZoom = zoom + (double)event.GetWheelRotation()/1200;
  if (newZoom < 0.1) newZoom = 0.1;
  if (newZoom > 5) newZoom = 5;

  SetZoom(newZoom, screenX, screenY);
}

/* Set the selected tool. */
void PaintBoard::SetTool(int toolId)
{
  tool = toolId;

  /* Set cursors. */
  switch (tool)
  {
    case TOOL_TRACK:
      wxSetCursor(wxCURSOR_PENCIL);
      break;
    case TOOL_ERASE:
      wxSetCursor(wxCURSOR_CROSS);
      break;
    default:
      wxSetCursor(wxCURSOR_ARROW);
      break;
  }
}

/* Set zoom. */
void PaintBoard::SetZoom(double z, double screenX, double screenY)
{
  double boardX, boardY;

  int scrollX, scrollY;

  CalcUnscrolledPosition(screenX,screenY,&scrollX,&scrollY);
  
  ScreenToBoard(scrollX, scrollY, &boardX, &boardY); // Convert screen coords to board coords. 
  
  zoom = z;

  SetVirtualSize(1000*zoom,1000*zoom);

  double virtualX, virtualY;

  BoardToScreen(boardX, boardY, &virtualX, &virtualY);

  Scroll(virtualX - screenX, virtualY - screenY);

  std::cout << "Zoom is  " << zoom << "\n";
  Refresh();
  Update();
}

void PaintBoard::SetGridSize(double s)
{
  gridSize = s; 
}
