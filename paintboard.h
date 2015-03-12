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

#ifndef PAINTBOARD_H
#define PAINTBOARD_H

#include <wx/wx.h>
#include <wx/graphics.h>
#include <cmath>
#include <set>
#include <sstream>
#include "board.h"

/* For cursor. */
//#include "icons/hand.xpm"


/* Available tools. */
enum
{
  TOOL_TRACK = 30,
  TOOL_MOVE = 31,
  TOOL_ERASE = 32,
  TOOL_HAND = 33,
  TOOL_PAD = 34, 
};

/* Class for painting the board in a scrolled window. */
class PaintBoard: public wxScrolled <wxWindow>
{
  public:
    void SetBoard(Board *board); // Set the board data store. 
    Board* GetBoard(void);
    Segment *tempSegment; // Temporary store segment when drawing.
    void PaintSegment(wxGraphicsContext *gc, Segment *segment); // Paint a segment.
    void PaintGrid(wxGraphicsContext *gc, double size);
    bool IsMouseOver(int id);
    wxRealPoint GetGridPoint(int x, int y);
    void SetTool(int toolId);
    void SetZoom(double z, double x, double y);
    void SetGridSize(double s);
    
  private:
    void ScreenToBoard(double x, double y, double *toX, double *toY);
    void BoardToScreen(double x, double y, double *toX, double *toY);
    int tool;
    double zoom = 1;
    wxGraphicsPath SegmentToPath(Segment *segment);
    std::set<int> mouseOver; // Id of items the mouse is over. 
    Board *board; // Where we store the data.
    double gridSize=30;
    
  /* Events. */
  private:
    void OnPaint(wxPaintEvent &event); // Paint window. 
    void OnClick(wxMouseEvent &event); // Click mouse. 
    void OnMotion(wxMouseEvent &event); // Move mouse. 
    void OnMouseLeftUp(wxMouseEvent &event); // Lift mouse.
    void OnMouseWheel(wxMouseEvent &event); // Mouse wheel. 
    wxDECLARE_EVENT_TABLE(); // Need for events. 
};

#endif // PAINTBOARD_H
