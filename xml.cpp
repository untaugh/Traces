/*
 * xml.cpp
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

#include "xml.h"

wxXmlNode* segmentToNode(Segment* segment)
{
  wxXmlNode *nodeSegment = new wxXmlNode(wxXML_ELEMENT_NODE, "segment");
  
  nodeSegment->AddChild(pointToNode(segment->GetStart(), "start"));
  nodeSegment->AddChild(pointToNode(segment->GetEnd(),"end"));
  
  return nodeSegment;
}

wxXmlNode* pointToNode(std::shared_ptr<Point> point, const char* name)
{
  wxXmlNode *node = new wxXmlNode(wxXML_ELEMENT_NODE, name);
  
  wxXmlAttribute* attrX = new wxXmlAttribute("x", std::to_string(point->GetX()));
  wxXmlAttribute* attrY = new wxXmlAttribute("y", std::to_string(point->GetY()));
  
  node->AddAttribute(attrX);
  node->AddAttribute(attrY);
  
  return node;
}


