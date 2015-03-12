/*
 * board.cpp
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

#include "board.h"

void Board::AddSegment(Segment *segment)
{
  segments.push_back(segment);
}

void Board::AddItem(BoardItem *item)
{
  items.push_back(item);
}

bool Board::EraseSegment(int id)
{
  std::vector<Segment*>::iterator it = segments.begin();
  while (it != segments.end())
  {
    if ((*it)->GetId() == id)
    {
      std::cout << "Erasing " << id << "\n";
      it = segments.erase(it);
      return 1; // A segment was erased. 
    }
    else
    {
      std::cout << "Not erasing " << id << "\n";
      it++;
    }
  }
  return 0; // No segment erased. 
}

std::vector <Segment*> Board::GetSegments(void) const
{
  return segments; 
}
