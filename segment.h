/*
 * segment.h
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

#ifndef SEGMENT_H
#define SEGMENT_H

#include <memory>
#include "boarditem.h"
#include "point.h"

class Segment : public BoardItem
{
  public:
    void SetStart(std::shared_ptr<Point> start);
    void SetEnd(std::shared_ptr<Point> end);
    double GetStartX() const;
    double GetStartY() const;
    double GetEndX() const;
    double GetEndY() const;
    std::shared_ptr<Point> GetStart() const;
    std::shared_ptr<Point> GetEnd() const;
    double GetWidth() const;
    void SetWidth(double w);
    
  private:
    std::shared_ptr<Point> start;
    std::shared_ptr<Point> end;
    double width;
};

#endif // SEGMENT_H
