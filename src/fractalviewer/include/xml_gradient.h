/***************************************************************************
 * Project: FractalViewer                                                  *
 * Filename: xml_gradient.h                                                *
 * Begin: Sa May 28 09:19:12 CET 2005                                      *
 * Description: Konvertierung eines Farbverlaufes zu XML und umgekehrt mit *
 *              Hilfe der TinyXML-Lib                                      *
 ***************************************************************************
 * Authors: Jan Gosmann (blubb@bbmy.ath.cx)                                *
 ***************************************************************************
 * Copyright (C) 2005 Jan Gosmann                                          *
 ***************************************************************************
 *  This program is free software; you can redistribute it and/or modify   *
 *  it under the terms of the GNU General Public License as published by   *
 *  the Free Software Foundation; either version 2 of the License, or      *
 *  (at your option) any later version.                                    *
 *                                                                         *
 *  This program is distributed in the hope that it will be useful,        *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.                   *
 *                                                                         *
 *  See the GNU General Public License for more details.                   *
 *                                                                         *
 *  You should have received a copy of the GNU General Public License      *
 *  along with this program; if not, write to the Free Software            *
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.              *
 ***************************************************************************/

#ifndef XML_GRADIENT_H
#define XML_GRADIENT_H 1

/***** Includes *****/
#include <cstring>
#include <tinyxml.h>
#include "gui_alert.h"

/***** Functions *****/
//----------------------------------------------------------------------------
// Farbverlauf zu XML konvetieren
// Eingabe: gradient = Der zu konvertierende Farbverlauf
// Rückgabe: TiXmlElement mit konvertierter Farbpalette
//----------------------------------------------------------------------------
TiXmlElement*
gradient_to_xml (colpal &gradient)
{
  /*** Variables ***/
  TiXmlElement *root, *act_edited; // Root- und aktuell editiertes Element
  unsigned int i; // Zählvariable
  
  /*** Commands ***/
  root = new TiXmlElement ("palette");
  root->SetAttribute ("red", getr (gradient.bg_col));
  root->SetAttribute ("green", getg (gradient.bg_col));
  root->SetAttribute ("blue", getb (gradient.bg_col));

  for (i = 0; i < gradient.colors.size (); i++)
    {
      act_edited = (TiXmlElement *) root->LinkEndChild (new TiXmlElement ("color"));
      act_edited->SetAttribute ("pos", gradient.colors[i].pos);
      act_edited->SetAttribute ("red", getr (gradient.colors[i].color));
      act_edited->SetAttribute ("green", getg (gradient.colors[i].color));
      act_edited->SetAttribute ("blue", getb (gradient.colors[i].color));
    }
    
  return root;
}

//----------------------------------------------------------------------------
// XML zu Farbverlauf konvertieren
// Eingabe:     root = Root-Element "palette"
//          gradient = Die Farbpalette in die gespeichert wird
// Rückgabe: true, bei Erfolg
//----------------------------------------------------------------------------
bool
xml_to_gradient (TiXmlElement *root, colpal &gradient)
{
  /*** Variables ***/
  TiXmlElement *act_edited; // Aktuell konvertiertes Element
  colpal loaded_gradient; // Der konvertierte Farbverlauf
  int r, g, b, pos; // Aktuell konvertierte Werte für Farbe und Position
  
  /*** Commands ***/
  if (strcmp (root->Value (), "palette") != 0)
    {
      gui_alert (_("Incorrect data format!"), _("OK"));
      return false;
    }

  if (root->QueryIntAttribute ("red", &r) != TIXML_SUCCESS
      || root->QueryIntAttribute ("green", &g) != TIXML_SUCCESS
      || root->QueryIntAttribute ("blue", &b) != TIXML_SUCCESS)
    {
      gui_alert (_("File is corrupted!"), _("OK"));
      return false;
    }
  if (r > 255 || r < 0 || g > 255 || g < 0 || b > 255 || b < 0)
    {
      gui_alert (_("File is corrupted!"), _("OK"));
      return false;
    }

  loaded_gradient.bg_col = makecol (r, g, b);

  if (root->FirstChildElement ("color") == NULL)
    {
      gui_alert (_("File is corrupted!"), _("OK"));
      return false;
    }

  for (act_edited = root->FirstChildElement ("color");
       act_edited != NULL;
       act_edited = act_edited->NextSiblingElement ("color"))
    {
      if (act_edited->QueryIntAttribute ("red", &r) != TIXML_SUCCESS
         || act_edited->QueryIntAttribute ("green", &g) != TIXML_SUCCESS
         || act_edited->QueryIntAttribute ("blue", &b) != TIXML_SUCCESS
         || act_edited->QueryIntAttribute ("pos", &pos) != TIXML_SUCCESS)
        {
          gui_alert (_("File is corrupted!"), _("OK"));
          return false;
        }
      if (r > 255 || r < 0 || g > 255 || g < 0 || b > 255 || b < 0 || pos > 255
          || pos < 0)
        {
          gui_alert (_("File is corrupted!"), _("OK"));
          return false;
        }

      loaded_gradient.insert_color (pos, makecol (r, g, b));
    }
    
  gradient = loaded_gradient;
  return true;
}

#endif // XML_GRADIENT
