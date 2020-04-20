/***************************************************************************
 * Filename: xma.h                                                         *
 * Begin: Su Jun 05 12:28:28 CET 2005                                      *
 * Description: Laden und wiedergeben von xma-Animationen                  *
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
 
#ifndef XMA_H
#define XMA_H 1

/***** Includes *****/

#include <string>
#include <cstring>
#include <cstdio>
#include <vector>

#include <allegro.h>
#include <alfont.h>
#include <tinyxml.h>

/***** Defines *****/
#ifndef XMA_DIR_SEP
 #ifdef __WIN32__
  #define XMA_DIR_SEP "\\"
 #else
  #define XMA_DIR_SEP "/"
 #endif
#endif


/* Animationsbefehle */
#define XMA_ANIMATION 0
#define XMA_GROUP 1
#define XMA_FADEIN 2
#define XMA_FADEOUT 3
#define XMA_MOVE 4
#define XMA_ZOOM 5
#define XMA_STRETCH 6
#define XMA_FZOOM 7
#define XMA_TEXT 8
#define XMA_IMG 9

#define XMA_FZOOM_IN 0
#define XMA_FZOOM_OUT 1

#define XMA_TEXT_LEFT 0
#define XMA_TEXT_CENTRE 1
#define XMA_TEXT_RIGHT 2
#define XMA_TEXT_LEFT_NOAA 3
#define XMA_TEXT_CENTRE_NOAA 4
#define XMA_TEXT_RIGHT_NOAA 5

#define XMA_IMG_TRANS 0
#define XMA_IMG_ALPHA 1
#define XMA_IMG_SOLID 2

/* Fehlercodes */
#define XMA_ERR_UNEXPECTED -1
#define XMA_ERR_NOERR 0
#define XMA_ERR_EXPECTED 1
#define XMA_ERR_DATATYPE 2
#define XMA_ERR_NOROOT 3
#define XMA_ERR_WRONGFORMAT 4

/***** Declarations *****/
struct xma_anim;

void render_xma (void);
void _render_xma (xma_anim *anim, BITMAP *draw_to, unsigned int step);
void _xma_step (void);
int play_xma (xma_anim *anim, BITMAP *draw_to, int x, int y);
int play_xma_blocking (xma_anim *anim, BITMAP *draw_to, int x, int y);
void stop_xma (void);
void pause_xma (void);
int continue_xma (void);
int toggle_xma (void);

/***** Misc *****/
using namespace std;

/***** Variables *****/
/* Globale xma-Variablen */
namespace xma {
  ALFONT_FONT *font; // Verwendete Schrift
  unsigned int step = 0; // Aktueller Zeitpunkt in der Animation
  xma_anim *played_anim; // Aktuell abgespielte Animation
  BITMAP *draw_to;
  string basedir; // Basis-Verzeichnis für relative Pfade
  bool playing = false; // Wird gerade eine Animation abgespielt?
  bool initialised = false; // Initialisiert?
  bool loop = false; // loopen?
}

namespace xma_err {
  int err_code = XMA_ERR_NOERR; // Fehlercode
  int line; // Zeile
  string details; // Details
}

//============================================================================
// Fehlerfunktion
//============================================================================
template <class ret_type> ret_type
xma_error (int err_code, int line = 0, string details = "",
           ret_type retval = XMA_ERR_UNEXPECTED)
{
  xma_err::err_code = err_code;
  xma_err::line = line;
  xma_err::details = details;
  return retval;
}

//============================================================================
// Deinitialisierungs-Funktion
//============================================================================
void
xma_exit (void)
{
  if (!xma::initialised)
    return;
    
  alfont_destroy_font (xma::font);
  xma::font = NULL;
  xma_err::err_code = XMA_ERR_NOERR;

  xma::initialised = false;

  alfont_exit ();
  return;
}

//============================================================================
// Initialisierungsfunktion
// Eingabe: font_file = Schriftdatei
// Rückgabe: kleiner 0 bei Fehler
//============================================================================
int
xma_init (string font_file)
{
  if (xma::initialised)
    return xma_error <int> (XMA_ERR_NOERR, 0, "", XMA_ERR_NOERR);

  if (install_timer () < 0)
    return xma_error <int> (XMA_ERR_UNEXPECTED);
  if (alfont_init () != ALFONT_OK)
    return xma_error <int> (XMA_ERR_UNEXPECTED);

  xma::font = alfont_load_font (font_file.c_str ());
  if (xma::font == NULL)
    return xma_error <int> (XMA_ERR_UNEXPECTED);

  LOCK_VARIABLE (xma::step);
  LOCK_FUNCTION (_xma_step);
  
  xma::initialised = true;

  atexit (xma_exit);
  return xma_error <int> (XMA_ERR_NOERR, 0, "", XMA_ERR_NOERR);
}

//============================================================================
// Struct für die Daten der Animation
//============================================================================
struct xma_anim {
  public:
    int action_type; // Art des Animationsbefehls
    unsigned int start, length; // Anfangszeitpunkt und Länge
    int x, y; // Position
    unsigned int w, h; // Breite, Höhe
    bool initialised; // Initialisiert?
    BITMAP *bmp; // gerenderte Grafik
    vector <void *> params; // Parameter
    vector <xma_anim *> underlying; // Untergeordnete Elemente
    
    xma_anim (void);
    ~xma_anim (void);
};

//----------------------------------------------------------------------------
// Konstruktor
//----------------------------------------------------------------------------
xma_anim::xma_anim (void)
{
  bmp = NULL;
  initialised = false;
}

//----------------------------------------------------------------------------
// Destruktor
//----------------------------------------------------------------------------
xma_anim::~xma_anim (void)
{
  destroy_bitmap (bmp);
  bmp = NULL;
}

//============================================================================
// destroy-Funktion
// Eingabe: anim = zu zerstörende Animation
//============================================================================
void
destroy_xma (xma_anim *anim)
{
  /*** Variables ***/
  unsigned int i; // Zählvariable

  /*** Commands ***/
  if (anim == NULL)
    return;
    
  for (i = 0; i < anim->underlying.size (); i++)
    {
      if (anim->underlying[i] == NULL)
        continue;
      destroy_xma (anim->underlying[i]);
    }

  switch (anim->action_type)
    {
      case XMA_ANIMATION:
        if (anim->params[0] != NULL)
          delete (string *) anim->params[0];
        break;
        
      case XMA_MOVE:
        if (anim->params[0] != NULL)
          delete (int *) anim->params[0];
        if (anim->params[1] != NULL)
          delete (int *) anim->params[1];
        break;
        
      case XMA_ZOOM:
        if (anim->params[0] != NULL)
          delete (int *) anim->params[0];
        if (anim->params[1] != NULL)
          delete (int *) anim->params[1];
        if (anim->params[2] != NULL)
          delete (int *) anim->params[2];
        if (anim->params[3] != NULL)
          delete (int *) anim->params[3];
        break;
        
      case XMA_STRETCH:
        if (anim->params[0] != NULL)
          delete (int *) anim->params[0];
        if (anim->params[1] != NULL)
          delete (int *) anim->params[1];
        break;
        
      case XMA_FZOOM:
        if (anim->params[0] != NULL)
          delete (int *) anim->params[0];
        if (anim->params[1] != NULL)
          delete (double *) anim->params[1];
        if (anim->params[2] != NULL)
          delete (double *) anim->params[2];
        if (anim->params[3] != NULL)
          delete (double *) anim->params[3];
        if (anim->params[4] != NULL)
          delete (double *) anim->params[4];
        if (anim->params[5] != NULL)
          delete (string *) anim->params[5];
        if (anim->params[6] != NULL)
          delete (double *) anim->params[6];
        if (anim->params[7] != NULL)
          delete (double *) anim->params[7];
        if (anim->params[8] != NULL)
          delete (double *) anim->params[8];
        if (anim->params[9] != NULL)
          delete (double *) anim->params[9];
        if (anim->params[10] != NULL)
          delete (string *) anim->params[10];
        destroy_bitmap ((BITMAP *) anim->params[11]);
        destroy_bitmap ((BITMAP *) anim->params[12]);
        break;

      case XMA_TEXT:
        if (anim->params[0] != NULL)
          delete (string *) anim->params[0];
        if (anim->params[1] != NULL)
          delete (int *) anim->params[1];
        if (anim->params[2] != NULL)
          delete (int *) anim->params[2];
        if (anim->params[3] != NULL)
          delete (int *) anim->params[3];
        if (anim->params[4] != NULL)
          delete (int *) anim->params[4];
        if (anim->params[5] != NULL)
          delete (int *) anim->params[5];
        break;
        
      case XMA_IMG:
        if (anim->params[0] != NULL)
          delete (string *) anim->params[0];
        if (anim->params[1] != NULL)
          delete (int *) anim->params[1];
        break;
    }
    
  delete anim;
  return;
}

//============================================================================
// load-Funktion
//============================================================================
//----------------------------------------------------------------------------
// convert-Funktion
// Eingabe:     elem = TinyXML-Element, das konvertiert werden soll
//               top = Übergeordnetes Element
//          previous = Vorhergehendes Element
// Rückgabe: Konvertiertes Element
//----------------------------------------------------------------------------
xma_anim *
_convert_xmlelem (TiXmlElement *elem, xma_anim *top, xma_anim *previous = NULL)
{
  /*** Variables ***/
  TiXmlElement *act_edited; // Aktuell und vorhergehend editiertes Element
  xma_anim *converted = new xma_anim (); // Konvertierte Animation

  /*** Commands ***/
  /* Standardattribute */
  if (elem->QueryIntAttribute ("start", (int *) &converted->start) != TIXML_SUCCESS)
    converted->start = 0;
  converted->start *= 4;
  if (top->action_type == XMA_ANIMATION)
    {
      if (previous != NULL)
        converted->start += previous->start + previous->length + 1;
    }
  else
    converted->start += top->start; // Wert soll absolut und nicht relativ sein
  
  if (elem->QueryIntAttribute ("length", (int *) &converted->length) != TIXML_SUCCESS)
    converted->length = top->length + top->start - converted->start;
  else
    converted->length *= 4;
  if (converted->length > top->length + top->start - converted->start)
    converted->length = top->length + top->start - converted->start;
  converted->length--;
 
  if (elem->QueryIntAttribute ("x", &converted->x) != TIXML_SUCCESS)
    converted->x = 0;
  if (elem->QueryIntAttribute ("y", &converted->y) != TIXML_SUCCESS)
    converted->y = 0;
    
  if (elem->QueryIntAttribute ("w", (int *) &converted->w) != TIXML_SUCCESS)
    converted->w = top->w;
  if (elem->QueryIntAttribute ("h", (int *) &converted->h) != TIXML_SUCCESS)
    converted->h = top->h;

  /* Animationsaktion */
  if (strcmp (elem->Value (), "group") == 0)
    converted->action_type = XMA_GROUP;
  else if (strcmp (elem->Value (), "fadein") == 0)
    converted->action_type = XMA_FADEIN;
  else if (strcmp (elem->Value (), "fadeout") == 0)
    converted->action_type = XMA_FADEOUT;
  else if (strcmp (elem->Value (), "move") == 0)
    converted->action_type = XMA_MOVE;
  else if (strcmp (elem->Value (), "zoom") == 0)
    converted->action_type = XMA_ZOOM;
  else if (strcmp (elem->Value (), "stretch") == 0)
    converted->action_type = XMA_STRETCH;
  else if (strcmp (elem->Value (), "fzoom") == 0)
    converted->action_type = XMA_FZOOM;
  else if (strcmp (elem->Value (), "text") == 0)
    converted->action_type = XMA_TEXT;
  else if (strcmp (elem->Value (), "img") == 0)
    converted->action_type = XMA_IMG;

  /* Spezifische Attribute */
  switch (converted->action_type)
    {
      case XMA_MOVE:
        converted->params.push_back (new int);
        converted->params.push_back (new int);
        
        if (elem->QueryIntAttribute ("endx", (int *) converted->params[0])
            != TIXML_SUCCESS)
          *((int *) converted->params[0]) = converted->x;
        if (elem->QueryIntAttribute ("endy",(int *)  converted->params[1])
            != TIXML_SUCCESS)
          *((int *) converted->params[1]) = converted->y;
        break;
        
      case XMA_ZOOM:
        converted->params.push_back (new int);
        converted->params.push_back (new int);
        converted->params.push_back (new int);
        converted->params.push_back (new int);

        if (elem->QueryIntAttribute ("endx", (int *) converted->params[0])
            != TIXML_SUCCESS)
          *((int *) converted->params[0]) = converted->x;
        if (elem->QueryIntAttribute ("endy", (int *) converted->params[1])
            != TIXML_SUCCESS)
          *((int *) converted->params[1]) = converted->y;
        if (elem->QueryIntAttribute ("endw", (int *) converted->params[2])
            != TIXML_SUCCESS)
          *((int *) converted->params[2]) = converted->w;
        if (elem->QueryIntAttribute ("endh", (int *) converted->params[3])
            != TIXML_SUCCESS)
          *((int *) converted->params[3]) = converted->h;
        break;
        
      case XMA_STRETCH:
        converted->params.push_back (new int);
        converted->params.push_back (new int);

        if (elem->QueryIntAttribute ("neww", (int *) converted->params[0])
            != TIXML_SUCCESS)
          *((int *) converted->params[0]) = converted->w;
        if (elem->QueryIntAttribute ("newh", (int *) converted->params[1])
            != TIXML_SUCCESS)
          *((int *) converted->params[1]) = converted->h;
        break;
        
      case XMA_FZOOM:
        converted->params.push_back (new int);
        converted->params.push_back (new double);
        converted->params.push_back (new double);
        converted->params.push_back (new double);
        converted->params.push_back (new double);
        converted->params.push_back (new string);
        converted->params.push_back (new double);
        converted->params.push_back (new double);
        converted->params.push_back (new double);
        converted->params.push_back (new double);
        converted->params.push_back (new string);
        converted->params.push_back (NULL);
        converted->params.push_back (NULL);
        
        if (elem->Attribute ("dir") != NULL)
          {
            if (strcmp (elem->Attribute ("dir"), "out") == 0)
              *((int *) converted->params[0]) = XMA_FZOOM_OUT;
            else
              *((int *) converted->params[0]) = XMA_FZOOM_IN;
          }
        else
          *((int *) converted->params[0]) = XMA_FZOOM_IN;

        if (elem->Attribute ("data0") != NULL)
          {
            if (sscanf (elem->Attribute ("data0"), "%lf;%lf;%lf;%lf",
                        (double *) converted->params[1],
                        (double *) converted->params[2],
                        (double *) converted->params[3],
			(double *) converted->params[4]) != 4)
              {
                if (sscanf (elem->Attribute ("data0"), "%lf;%lf;%lf",
                        (double *) converted->params[1],
                        (double *) converted->params[2],
                        (double *) converted->params[3]) != 3)
                  {
                    destroy_xma (converted);
                    return xma_error <xma_anim *> (XMA_ERR_DATATYPE, elem->Row (),
                                                   "data0", NULL);
                  }
		*((double *) converted->params[4]) = *((double *) converted->params[3])
                                      + ((double) converted->h / (double) converted->w)
                                         * (double) (*((double *) converted->params[2])
                                                  - *((double *) converted->params[1]));
              }
          }
        else
          {
            destroy_xma (converted);
            return xma_error <xma_anim *> (XMA_ERR_EXPECTED, elem->Row (),
                                           "data0", NULL);
          }

        if (elem->Attribute ("src0") == NULL)
          {
            destroy_xma (converted);
            return xma_error <xma_anim *> (XMA_ERR_EXPECTED, elem->Row (),
                                           "src0", NULL);
          }
        *((string *) converted->params[5]) = elem->Attribute ("src0");

        if (elem->Attribute ("data1") != NULL)
          {
            if (sscanf (elem->Attribute ("data1"), "%lf;%lf;%lf;%lf",
                        (double *) converted->params[6],
                        (double *) converted->params[7],
                        (double *) converted->params[8],
			(double *) converted->params[9]) != 4)
              {
                if (sscanf (elem->Attribute ("data1"), "%lf;%lf;%lf",
                        (double *) converted->params[6],
                        (double *) converted->params[7],
                        (double *) converted->params[8]) != 3)
                  {
                    destroy_xma (converted);
                    return xma_error <xma_anim *> (XMA_ERR_DATATYPE, elem->Row (),
                                                   "data1", NULL);
                  }
		*((double *) converted->params[9]) = *((double *) converted->params[8])
                                      + ((double) converted->h / (double) converted->w)
                                         * (double) (*((double *) converted->params[7])
                                                  - *((double *) converted->params[6]));
              }
          }
        else
          {
            destroy_xma (converted);
            return xma_error <xma_anim *> (XMA_ERR_EXPECTED, elem->Row (),
                                           "data1", NULL);
          }

        if (elem->Attribute ("src1") == NULL)
          {
            destroy_xma (converted);
            return xma_error <xma_anim *> (XMA_ERR_EXPECTED, elem->Row (),
                                           "src1", NULL);
          }
        *((string *) converted->params[10]) = elem->Attribute ("src1");
        break;
        
      case XMA_TEXT:
        converted->params.push_back (new string);
        converted->params.push_back (new int);
        converted->params.push_back (new int);
        converted->params.push_back (new int);
        converted->params.push_back (new int);
        converted->params.push_back (new int);

        if (elem->Attribute ("value") == NULL)
          {
            destroy_xma (converted);
            return xma_error <xma_anim *> (XMA_ERR_EXPECTED, elem->Row (),
                                           "value", NULL);
          }
        *((string *) converted->params[0]) = elem->Attribute ("value");

        if (elem->Attribute ("align") != NULL)
          {
            if (strcmp (elem->Attribute ("align"), "left") == 0)
              *((int *) converted->params[1]) = XMA_TEXT_LEFT;
            else if (strcmp (elem->Attribute ("align"), "right") == 0)
              *((int *) converted->params[1]) = XMA_TEXT_RIGHT;
            else
              *((int *) converted->params[1]) = XMA_TEXT_CENTRE;
          }
        else
          *((int *) converted->params[1]) = XMA_TEXT_CENTRE;
        if (elem->Attribute ("aa") != NULL)
          {
            if (strcmp (elem->Attribute ("aa"), "no") == 0)
              *((int *) converted->params[1]) += 3;
          }

        if (elem->QueryIntAttribute ("size", (int *) converted->params[2])
            != TIXML_SUCCESS)
          *((int *) converted->params[2]) = 30;

        if (elem->Attribute ("color") != NULL)
          {
            if (sscanf (elem->Attribute ("color"), "%i;%i;%i",
                        (int *) converted->params[3],
                        (int *) converted->params[4],
                        (int *) converted->params[5]) != 3)
              {
                destroy_xma (converted);
                return xma_error <xma_anim *> (XMA_ERR_DATATYPE, elem->Row (),
                                               "color", NULL);
              }
          }
        else
          {
            *((int *) converted->params[3]) = 255;
            *((int *) converted->params[4]) = 255;
            *((int *) converted->params[5]) = 255;
          }
        break;
        
      case XMA_IMG:
        converted->params.push_back (new string);
        converted->params.push_back (new int);

        if (elem->Attribute ("src") == NULL)
          {
            destroy_xma (converted);
            return xma_error <xma_anim *> (XMA_ERR_EXPECTED, elem->Row (),
                                           "src", NULL);
          }
        *((string *) converted->params[0]) = elem->Attribute ("src");

        if (elem->Attribute ("trans") != NULL)
          {
            if (strcmp (elem->Attribute ("trans"), "yes") == 0)
              *((int *) converted->params[1]) = XMA_IMG_TRANS;
            else if (strcmp (elem->Attribute ("trans"), "alpha") == 0)
              *((int *) converted->params[1]) = XMA_IMG_ALPHA;
            else
              *((int *) converted->params[1]) = XMA_IMG_SOLID;
          }
        else
          {
            if (((string *) converted->params[0])->compare (
                  ((string *) converted->params[0])->size () - 3, 3, "tga") == 0)
              *((int *) converted->params[1]) = XMA_IMG_ALPHA;
            else if (((string *) converted->params[0])->compare (
                  ((string *) converted->params[0])->size () - 3, 3, "jpg") == 0)
              *((int *) converted->params[1]) = XMA_IMG_SOLID;
            else
              *((int *) converted->params[1]) = XMA_IMG_TRANS;
          }
        break;
    }
  
  for (act_edited = elem->FirstChildElement (); act_edited != NULL;
       act_edited = act_edited->NextSiblingElement ())
    {
      if (converted->underlying.size () > 0)
        converted->underlying.push_back (_convert_xmlelem (act_edited, converted,
                        converted->underlying[converted->underlying.size () - 1]));
      else
        converted->underlying.push_back (_convert_xmlelem (act_edited, converted, NULL));
      if (converted->underlying[converted->underlying.size () - 1] == NULL)
        {
          destroy_xma (converted);
          return NULL;
        }
    }
  
  return xma_error <xma_anim *> (XMA_ERR_NOERR, 0, "", converted);
}

//----------------------------------------------------------------------------
// Die eigentliche load-Funktion
// Eingabe: file = Dateiname
// Rückgabe: Zeiger auf geöffnete Animation
//----------------------------------------------------------------------------
xma_anim *
load_xma (string file)
{
  /*** Variables ***/
  TiXmlDocument *xml_tree; // Aufbau der Datei aus der geladen wird
  TiXmlElement *root, *act_edited; // Root-Element, aktuell und vorhergehend
                                              // editiertes Element
  xma_anim *loaded_anim = new xma_anim ();

  /*** Commands ***/
  loaded_anim->params.push_back (new string);
  xml_tree = new TiXmlDocument (file);
  xml_tree->LoadFile ();
  root = xml_tree->RootElement ();
  if (root == NULL)
    {
      destroy_xma (loaded_anim);
      xml_tree->Clear ();
      delete xml_tree;
      return xma_error <xma_anim *> (XMA_ERR_NOROOT, 0, "", NULL);
    }
  if (strcmp (root->Value (), "animation") != 0)
    {
      destroy_xma (loaded_anim);
      xml_tree->Clear ();
      delete xml_tree;
      return xma_error <xma_anim *> (XMA_ERR_WRONGFORMAT, 0, "", NULL);
    }

  loaded_anim->start = 0;
  loaded_anim->action_type = XMA_ANIMATION;
  loaded_anim->x = 0;
  loaded_anim->y = 0;
  if (root->QueryIntAttribute ("length", (int *) &loaded_anim->length) != TIXML_SUCCESS
      || root->QueryIntAttribute ("w", (int *) &loaded_anim->w) != TIXML_SUCCESS
      || root->QueryIntAttribute ("h", (int *) &loaded_anim->h) != TIXML_SUCCESS)
    {
      destroy_xma (loaded_anim);
      xml_tree->Clear ();
      delete xml_tree;
      return xma_error <xma_anim *> (XMA_ERR_EXPECTED, 0, "length/w/h", NULL);
    }
  loaded_anim->length *= 4;
    
  *((string *) loaded_anim->params[0]) = file.substr (0, file.find_last_of (XMA_DIR_SEP))
                                         + XMA_DIR_SEP;
  
  for (act_edited = root->FirstChildElement (); act_edited != NULL;
       act_edited = act_edited->NextSiblingElement ())
    {
      if (loaded_anim->underlying.size () > 0)
        loaded_anim->underlying.push_back (_convert_xmlelem (act_edited, loaded_anim,
                        loaded_anim->underlying[loaded_anim->underlying.size () - 1]));
      else
        loaded_anim->underlying.push_back (_convert_xmlelem (act_edited, loaded_anim, NULL));
      if (loaded_anim->underlying[loaded_anim->underlying.size () - 1] == NULL)
        {
          destroy_xma (loaded_anim);
          xml_tree->Clear ();
          delete xml_tree;
          return NULL;
        }
    }

  xml_tree->Clear ();
  delete xml_tree;
  return xma_error <xma_anim *> (XMA_ERR_NOERR, 0, "", loaded_anim);
}

//============================================================================
// Play-, Stop-, Pause-Funktion
//============================================================================
//----------------------------------------------------------------------------
// render-Funktion
// Eingabe:    anim = abgespielete Animation
//          draw_to = auf welches Bitmap gerendert werden soll
//             step = Aktueller step
//----------------------------------------------------------------------------
void
render_xma (void)
{
  _render_xma (xma::played_anim, xma::draw_to, xma::step);
}

void
_render_xma (xma_anim *anim, BITMAP *draw_to, unsigned int step)
{
  /*** Variables ***/
  unsigned int i; // Zählvariable

  /*** Commands ***/
  /* loop */
  if (anim->action_type == XMA_ANIMATION && step >= anim->length - 4 && xma::loop)
    {
      xma::step = 0;
      _render_xma (anim, draw_to, xma::step);
      return;
    }

  /* Initialisierung */
  if (step >= anim->start && !anim->initialised && step <= anim->start + anim->length)
    {
      switch (anim->action_type)
        {
          case XMA_ANIMATION:
          case XMA_FADEIN:
          case XMA_FADEOUT:
          case XMA_MOVE:
          case XMA_ZOOM:
          case XMA_STRETCH:
            anim->bmp = create_bitmap (anim->w, anim->h);
            break;
            
          case XMA_GROUP:
            anim->bmp = draw_to;
            break;
            
          case XMA_FZOOM:
            anim->bmp = create_bitmap (anim->w, anim->h);
            anim->params[11] = load_bitmap (
                (xma::basedir + *((string *) anim->params[5])).c_str (), NULL);
            anim->params[12] = load_bitmap (
                (xma::basedir + *((string *) anim->params[10])).c_str (), NULL);
            break;

          case XMA_IMG:
            anim->bmp = load_bitmap ((xma::basedir + *((string *) anim->params[0])).c_str (),
                                     NULL);
            break;

          default:
            anim->bmp = NULL;
            break;
        }
      anim->initialised = true;
    }

  /** Zeichenfläche löschen */
  if (step >= anim->start && step <= anim->length + anim->start && anim->bmp != NULL)
    {
      if (anim->action_type == XMA_FADEIN || anim->action_type == XMA_FADEOUT)
        blit (draw_to, anim->bmp, anim->x, anim->y, 0, 0, anim->w, anim->h);
      else if (anim->action_type == XMA_MOVE)
        blit (draw_to, anim->bmp, (int) (((double) (step - anim->start))
                                         * (((double) (*((int *) anim->params[0]) - anim->x))
                                         / (double) anim->length)) + anim->x,
                                  (int) (((double) (step - anim->start))
                                         * (((double) (*((int *) anim->params[1]) - anim->y))
                                         / (double) anim->length)) + anim->y,
              0, 0, anim->w, anim->h);
      else if (anim->action_type == XMA_ZOOM || anim->action_type == XMA_STRETCH)
        clear_to_color (anim->bmp, makecol (255, 0 ,255));
      else if (anim->action_type == XMA_ANIMATION)
        clear_bitmap (anim->bmp);
    }

  /** Zeichnen von untergeordneten Elementen **/
  for (i = 0; i < anim->underlying.size () && anim->bmp != NULL; i++)
    _render_xma (anim->underlying[i], anim->bmp, step);

  /** Zeichnen **/
  if (step >= anim->start && step <= anim->length + anim->start)
    {
      switch (anim->action_type)
        {
          case XMA_FADEIN:
            if (anim->bmp == NULL)
              break;
            set_trans_blender (0, 0, 0, (int) ((((double) (step - anim->start))
                                                / (double) anim->length)
                                               * 255));
            draw_trans_sprite (draw_to, anim->bmp, anim->x, anim->y);
            break;

          case XMA_FADEOUT:
            if (anim->bmp == NULL)
              break;
            set_trans_blender (0, 0, 0, 255 - (int) ((((double) (step - anim->start))
                                                      / (double) anim->length)
                                                     * 255));
            draw_trans_sprite (draw_to, anim->bmp, anim->x, anim->y);
            break;

          case XMA_MOVE:
            if (anim->bmp == NULL)
              break;
            draw_sprite (draw_to, anim->bmp,
                         (int) (((double) (step - anim->start))
                                * (((double) (*((int *) anim->params[0]) - anim->x))
                                   / (double) anim->length)) + anim->x,
                         (int) (((double) (step - anim->start))
                                * (((double) (*((int *) anim->params[1]) - anim->y))
                                   / (double) anim->length)) + anim->y);
            break;
            
          case XMA_ZOOM:
            set_trans_blender (0, 0, 0, 0);
            stretch_sprite (draw_to, anim->bmp,
                            (int) ((signed) (*((int *) anim->params[0]) - anim->x)
                                   * (((double) (step - anim->start))
                                   / (double) anim->length))
                            + anim->x,
                            (int) ((signed) (*((int *) anim->params[1]) - anim->y)
                                   * (((double) (step - anim->start))
                                   / (double) anim->length))
                            + anim->y,
                            (int) ((signed) (*((int *) anim->params[2]) - anim->w)
                                   * (((double) (step - anim->start))
                                   / (double) anim->length))
                            + anim->w,
                            (int) ((signed) (*((int *) anim->params[3]) - anim->h)
                                   * (((double) (step - anim->start))
                                   / (double) anim->length))
                            + anim->h);
            break;
            
          case XMA_STRETCH:
            set_trans_blender (0, 0, 0, 0);
            stretch_sprite (draw_to, anim->bmp, anim->x, anim->y,
                            *((int *) anim->params[0]), *((int *) anim->params[1]));
            break;
            
          case XMA_FZOOM:
            {
              if (anim->bmp == NULL || anim->params[11] == NULL || anim->params[12] == NULL)
                break;

              double percent = ((double) (step - anim->start))
                               / (double) anim->length;
              double left_bound = (*((double *) anim->params[6])
                                 - *((double *) anim->params[1]))
                                * percent + *((double *) anim->params[1]);
              double right_bound =  - (*((double *) anim->params[2])
                                       - *((double *) anim->params[7]))
                                      * percent + *((double *) anim->params[2]);
              double bottom_bound = (*((double *) anim->params[8])
                                     - *((double *) anim->params[3]))
                                    * percent + *((double *) anim->params[3]);
              double top_bound = - (*((double *) anim->params[4])
                                    - *((double *) anim->params[9]))
                                   * percent + *((double *) anim->params[4]);
              int x1 = (int) (((*((double *) anim->params[1]) - left_bound)
                              / (right_bound - left_bound)) * (double) anim->w);
              int y1 = anim->h - (int) (((*((double *) anim->params[4]) - bottom_bound)
                                         / (top_bound - bottom_bound))
                                        * (double) anim->h);
              int h1 = anim->h - (int) (((*((double *) anim->params[3]) - bottom_bound)
                                         / (top_bound - bottom_bound)) * (double) anim->h);
              int w1 = (int) (((*((double *) anim->params[2]) - left_bound)
                                / (right_bound - left_bound)) * (double) anim->w);
              int x2 = (int) (((*((double *) anim->params[6]) - left_bound)
                              / (right_bound - left_bound)) * (double) anim->w);
              int y2 = anim->h - (int) (((*((double *) anim->params[9]) - bottom_bound)
                                         / (top_bound - bottom_bound))
                                        * (double) anim->h);
              int h2 = anim->h - (int) (((*((double *) anim->params[8]) - bottom_bound)
                                         / (top_bound - bottom_bound)) * (double) anim->h);
              int w2 = (int) (((*((double *) anim->params[7]) - left_bound)
                                / (right_bound - left_bound)) * (double) anim->w);

              if (*((int *) anim->params[0]) == XMA_FZOOM_IN)
                {
                  stretch_sprite (draw_to, (BITMAP *) anim->params[11], x1, y1,
                                  w1 - x1, h1 - y1);
                  stretch_sprite (draw_to, (BITMAP *) anim->params[12], x2, y2,
                                  w2 - x2, h2 - y2);
                }
              else
                {
                  stretch_sprite (draw_to, (BITMAP *) anim->params[12], x2, y2,
                                  w2 - x2, h2 - y2);
                  stretch_sprite (draw_to, (BITMAP *) anim->params[11], x1, y1,
                                  w1 - x1, h1 - y1);
                }
            }
            break;

          case XMA_TEXT:
            alfont_set_font_size (xma::font, *((int *) anim->params[2]));
            switch (*((int *) anim->params[1]))
              {
                case XMA_TEXT_LEFT:
                  alfont_textout_aa_ex (draw_to, xma::font,
                                        ((string *) anim->params[0])->c_str (),
                                        anim->x, anim->y,
                                        makecol (*((int *) anim->params[3]),
                                                 *((int *) anim->params[4]),
                                                 *((int *) anim->params[5])),
                                        makeacol (0, 0, 0, 255));
                  break;
                case XMA_TEXT_CENTRE:
                  alfont_textout_centre_aa_ex (draw_to, xma::font,
                                               ((string *) anim->params[0])->c_str (),
                                               anim->x, anim->y,
                                               makecol (*((int *) anim->params[3]),
                                                        *((int *) anim->params[4]),
                                                        *((int *) anim->params[5])),
                                               makeacol (0, 0, 0, 255));
                  break;
                case XMA_TEXT_RIGHT:
                  alfont_textout_right_aa_ex (draw_to, xma::font,
                                              ((string *) anim->params[0])->c_str (),
                                              anim->x, anim->y,
                                              makecol (*((int *) anim->params[3]),
                                                       *((int *) anim->params[4]),
                                                       *((int *) anim->params[5])),
                                              makeacol (0, 0, 0, 255));
                  break;
                case XMA_TEXT_LEFT_NOAA:
                  alfont_textout_ex (draw_to, xma::font,
                                     ((string *) anim->params[0])->c_str (),
                                     anim->x, anim->y,
                                     makecol (*((int *) anim->params[3]),
                                              *((int *) anim->params[4]),
                                              *((int *) anim->params[5])),
                                     makeacol (0, 0, 0, 255));
                  break;
                case XMA_TEXT_CENTRE_NOAA:
                  alfont_textout_centre_ex (draw_to, xma::font,
                                            ((string *) anim->params[0])->c_str (),
                                            anim->x, anim->y,
                                            makecol (*((int *) anim->params[3]),
                                                     *((int *) anim->params[4]),
                                                     *((int *) anim->params[5])),
                                            makeacol (0, 0, 0, 255));
                  break;
                case XMA_TEXT_RIGHT_NOAA:
                  alfont_textout_right_ex (draw_to, xma::font,
                                           ((string *) anim->params[0])->c_str (),
                                           anim->x, anim->y,
                                           makecol (*((int *) anim->params[3]),
                                                    *((int *) anim->params[4]),
                                                    *((int *) anim->params[5])),
                                           makeacol (0, 0, 0, 255));
                  break;
              }
            break;

          case XMA_IMG:
            if (anim->bmp == NULL)
              break;
            switch (*((int *) anim->params[1]))
              {
                case XMA_IMG_TRANS:
                  set_trans_blender (0, 0, 0, 0);
                  draw_trans_sprite (draw_to, anim->bmp, anim->x, anim->y);
                  break;

                case XMA_IMG_ALPHA:
                  set_alpha_blender ();
                  draw_trans_sprite (draw_to, anim->bmp, anim->x, anim->y);
                  break;

                case XMA_IMG_SOLID:
                  draw_sprite (draw_to, anim->bmp, anim->x, anim->y);
                  break;
              }
            break;

          default:
            if (anim->bmp == NULL)
              break;
            draw_sprite (draw_to, anim->bmp, anim->x, anim->y);
            break;
        }
    }
    
  /** Deinitialisierung **/
  if (step >= anim->length + anim->start && anim->initialised)
    {
      switch (anim->action_type)
        {
          case XMA_GROUP:
            anim->bmp = NULL;
            break;

          case XMA_FZOOM:
            destroy_bitmap ((BITMAP *) anim->params[11]);
            destroy_bitmap ((BITMAP *) anim->params[12]);
            anim->params[11] = NULL;
            anim->params[12] = NULL;
            break;
        }

      destroy_bitmap (anim->bmp);
      anim->bmp = NULL;

      anim->initialised = false;

      if (anim->action_type == XMA_ANIMATION && !xma::loop)
        stop_xma ();
    }

  return;
}

//----------------------------------------------------------------------------
// step-Funktion
//----------------------------------------------------------------------------
void
_xma_step (void)
{
  xma::step++;
}
END_OF_FUNCTION (_xma_step)

//----------------------------------------------------------------------------
// play-Funktion
// Eingabe:    anim = Animation, die abgespielt werden soll
//          draw_to = Bitmap, auf das die Animation gezeichnet werden soll
//             x, y = Koordinatan für die Darstellung
//----------------------------------------------------------------------------
int
play_xma (xma_anim *anim, BITMAP *draw_to, int x, int y)
{
  if (anim == NULL)
    return xma_error <int> (XMA_ERR_UNEXPECTED, 0, "", XMA_ERR_UNEXPECTED);
  xma::played_anim = anim;
  xma::draw_to = draw_to;
  anim->x = x;
  anim->y = y;
  xma::basedir = *((string *) anim->params[0]);
  xma::playing = true;
  if (install_int (_xma_step, 10) < 0)
    xma_error <int> (XMA_ERR_UNEXPECTED, 0, "", XMA_ERR_UNEXPECTED);
  return xma_error <int> (XMA_ERR_NOERR);
}

int
play_xma_blocking (xma_anim *anim, BITMAP *draw_to, int x, int y)
{
  if (anim == NULL)
    return xma_error <int> (XMA_ERR_UNEXPECTED, 0, "", XMA_ERR_UNEXPECTED);
  xma::played_anim = anim;
  xma::draw_to = draw_to;
  anim->x = x;
  anim->y = y;
  xma::basedir = *((string *) anim->params[0]);
  if (install_int (_xma_step, 10) < 0)
    return xma_error <int> (XMA_ERR_UNEXPECTED, 0, "", XMA_ERR_UNEXPECTED);;
  xma::playing = true;
  while (xma::playing)
    {
      _render_xma (anim, draw_to, xma::step);
      rest (1);
    }
  return xma_error <int> (XMA_ERR_NOERR);
}

//----------------------------------------------------------------------------
// stop-Funktion
//----------------------------------------------------------------------------
void
stop_xma (void)
{
  remove_int (_xma_step);
  xma::step = 0;
  xma::playing = false;
}

//----------------------------------------------------------------------------
// pause-Funktion
//----------------------------------------------------------------------------
void
pause_xma (void)
{
  remove_int (_xma_step);
  xma::playing = false;
}

//----------------------------------------------------------------------------
// continue-Funktion
//----------------------------------------------------------------------------
int
continue_xma (void)
{
  xma::playing = true;
  if (install_int (_xma_step, 10) < 0)
    return xma_error <int> (XMA_ERR_UNEXPECTED, 0, "", XMA_ERR_UNEXPECTED);;
  xma::playing = true;
  return xma_error <int> (XMA_ERR_NOERR);
}

//----------------------------------------------------------------------------
// toggle-Funktion
//----------------------------------------------------------------------------
int
toggle_xma (void)
{
  if (xma::playing)
    pause_xma ();
  else
    {
      if (install_int (_xma_step, 10) < 0)
        return xma_error <int> (XMA_ERR_UNEXPECTED, 0, "", XMA_ERR_UNEXPECTED);
      xma::playing = true;
    }
  
  return xma_error <int> (XMA_ERR_NOERR);
}

#endif // XMA_H
