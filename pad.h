/*
 * pad.h
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

#ifndef PAD_H
#define PAD_H

#include "point.h"

enum shape {
  SHAPE_ROUND,
  SHAPE_SQUARE,
}

class Pad : public BoardItem
{
  public:
    SetSize(double width, double height);
    SetPoint(double x, double y);
    
  private:
    Point* point;
    double width;
    double height;
    double diameter;
    int shape;
    bool surfaceMount;
};

#endif // PAD_H
