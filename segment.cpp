/*
 * segment.cpp
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

#include "segment.h"


void Segment::SetStart(std::shared_ptr<Point> start)
{
  this->start = start;
}

void Segment::SetEnd(std::shared_ptr<Point> end)
{
  this->end = end;
}

std::shared_ptr<Point> Segment::GetStart(void) const
{
  return start;
}

std::shared_ptr<Point> Segment::GetEnd(void) const
{
  return end;
}

void Segment::SetWidth(double w)
{
  this->width = w;
}

double Segment::GetWidth(void) const
{
  return width;
}

double Segment::GetStartX(void) const
{
  return start->GetX();
}

double Segment::GetStartY(void) const
{
  return start->GetY();
}

double Segment::GetEndX(void) const
{
  return end->GetX();
}

double Segment::GetEndY(void) const
{
  return end->GetY();
}
