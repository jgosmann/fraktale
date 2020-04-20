/***************************************************************************
 * Filename: palette.h                                                     *
 * Begin: Sa May 14 10:47:24 CET 2005                                      *
 * Description: Klassen für Paletten und entsprechende GUI-Routinen        *
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

#ifndef PALETTE_H
#define PALETTE_H 1

/***** Includes *****/
#include <new>
#include <vector>

/***** Defines *****/
#define COL_PER_PAL 256

#define COLSLIDER_CHANGEABLE 1
#define COLSLIDER_INSTCALC 2

/***** Misc *****/
using namespace std;

/***** Variables *****/
int (*col_select)(int) = NULL; // Farbwähler für d_colslider_proc

/***** Structs, Classes, Functions *****/
//============================================================================
// Struct für Farbposition im Farbverlauf
//============================================================================
struct color_pos {
  int pos; // Position (0 - 255)
  int color; // Farbe
  int id; // Id
  
  color_pos (void) { };
  color_pos (unsigned int _pos, int _color);
};

//----------------------------------------------------------------------------
// Konstruktor
//----------------------------------------------------------------------------
color_pos::color_pos (unsigned int _pos, int _color)
{
  pos = _pos;
  color = _color;
}

//----------------------------------------------------------------------------
// Compare-Funktion für den sort-Algorithmus der STL
//----------------------------------------------------------------------------
bool
color_pos_comp_func (color_pos val1, color_pos val2)
{
  if (val1.pos < val2.pos)
    return true;
  return false;
}

//============================================================================
// Klasse für eine Palette/einen Farbverlauf
//============================================================================
class colpal {
  public:
    int pal[COL_PER_PAL]; // Die einzelnen Farben des Farbverlaufes
    int bg_col; // Hintergrundsfarbe
    BITMAP *preview; // Gerenderter Farbverlauf
    vector <color_pos> colors; // Die Farben aus denen der Verlauf berechnet wird
  
    colpal (int _bg_col = 0);
    colpal (colpal *source);
    ~colpal (void);

    void insert_color (unsigned int pos, int color);
    void sort_colors (void);
    void calc_pal (bool sort_cols = true);
    
    void operator = (colpal &source);

  private:
    int max_id;
};

//----------------------------------------------------------------------------
// Konstruktoren, Destruktor
//----------------------------------------------------------------------------
colpal::colpal (int _bg_col)
{
  preview = create_bitmap (COL_PER_PAL, 1);
  if (preview == NULL)
    throw bad_alloc ();
  bg_col = _bg_col;
  if (bg_col == makecol (255, 0, 255))
    bg_col = makecol (255, 0, 254);
  max_id = 0;
}

colpal::colpal (colpal *source)
{
  preview = create_bitmap (COL_PER_PAL, 1);
  if (preview == NULL)
    throw bad_alloc ();
    
  *this = *source;
}

void
colpal::operator = (colpal &source)
{
  unsigned int i;

  for (i = 0; i < COL_PER_PAL; i++)
    pal[i] = source.pal[i];

  bg_col = source.bg_col;
  max_id = 0;

  colors.clear ();
  for (i = 0; i < source.colors.size (); i++)
    insert_color (source.colors[i].pos, source.colors[i].color);
}

colpal::~colpal (void)
{
  destroy_bitmap (preview);
}

//----------------------------------------------------------------------------
// Fügt dem Farbverlauf eine Farbe hinzu
// Eingabe:   pos = Position der Farbe
//          color = Farbwert
//----------------------------------------------------------------------------
void
colpal::insert_color (unsigned int pos, int color)
{
  colors.push_back (color_pos ());
  (colors.back ()).pos = pos;
  (colors.back ()).color = color;
  if ((colors.back ()).color == makecol (255, 0, 255))
    (colors.back ()).color = makecol (255, 0, 254);
  (colors.back ()).id = max_id++;
}

//----------------------------------------------------------------------------
// Sortiert die Farbe in colors für die Palettenerzeugung richtig
//----------------------------------------------------------------------------
void
colpal::sort_colors (void)
{
  sort (colors.begin (), colors.end (), color_pos_comp_func);
}

//----------------------------------------------------------------------------
// Berechnet die Palette
// Eingabe: sort_cols = Sollen die Farben vorher sotiert werden (default: true)
//----------------------------------------------------------------------------
void
colpal::calc_pal (bool sort_cols)
{
  /*** Variables ***/
  double r_step, g_step, b_step; // Schrittweiten
  double r_part, g_part, b_part; // Farbanteile
  int act_pos = 0;
  unsigned int i; // Zählvariable
  
  /*** Commands ***/
  if (sort_cols)
    sort_colors ();
    
  r_part = getr (bg_col);
  g_part = getg (bg_col);
  b_part = getb (bg_col);
  
  for (i = 0; i < colors.size (); i++)
    {
      if (colors[i].pos - act_pos <= 0)
      {
        r_part = getr (colors[i].color);
        g_part = getg (colors[i].color);
        b_part = getb (colors[i].color);
        continue;
      }

      r_step = (double) (getr (colors[i].color) - r_part)
                 / (double) (colors[i].pos - act_pos);
      g_step = (double) (getg (colors[i].color) - g_part)
                 / (double) (colors[i].pos - act_pos);
      b_step = (double) (getb (colors[i].color) - b_part)
                 / (double) (colors[i].pos - act_pos);

      for (; act_pos <= colors[i].pos; act_pos++)
        {
          r_part += r_step;
          g_part += g_step;
          b_part += b_step;
          
          if (r_part > 255)
            r_part = 255;
          if (g_part > 255)
            g_part = 255;
          if (b_part > 255)
            b_part = 255;

          if (r_part < 0)
            r_part = 0;
          if (g_part < 0)
            g_part = 0;
          if (b_part < 0)
            b_part = 0;
            
          if (r_part == 255 && b_part == 255)
            b_part = 254;
          
          pal[act_pos] = makecol ((int) r_part, (int) g_part, (int) b_part);
          putpixel (preview, act_pos, 0, pal[act_pos]);
        }
    }

  r_step = (double) (getr (bg_col) - r_part)
             / (double) (COL_PER_PAL - 1 - act_pos);
  g_step = (double) (getg (bg_col) - g_part)
             / (double) (COL_PER_PAL - 1 - act_pos);
  b_step = (double) (getb (bg_col) - b_part)
             / (double) (COL_PER_PAL - 1 - act_pos);

  for (; act_pos < COL_PER_PAL; act_pos++)
    {
      r_part += r_step;
      g_part += g_step;
      b_part += b_step;
      
      if (r_part > 255)
        r_part = 255;
      if (g_part > 255)
        g_part = 255;
      if (b_part > 255)
        b_part = 255;

      if (r_part < 0)
        r_part = 0;
      if (g_part < 0)
        g_part = 0;
      if (b_part < 0)
        b_part = 0;
        
      if (r_part == 255 && b_part == 255)
        b_part = 254;

      pal[act_pos] = makecol ((int) r_part, (int) g_part, (int) b_part);
      putpixel (preview, act_pos, 0, pal[act_pos]);
    }
}

//============================================================================
// Struct um Daten für den d_colslider_proc zu speichern
//============================================================================
struct colslider_data {
  public:
    colpal *pal; // Palette für den Farbverlauf
    vector <color_pos> *sliders; // Schieberegler

    colslider_data (colpal *_pal, vector <color_pos> *_sliders = NULL);
    friend int d_colslider_proc (int msg, DIALOG *d, int c);

  protected:
    BITMAP *back, *bmp; // back = Überzeichneter Hintergrund, bmp = Zeichenbuffer
    bool first_draw; // Wurde der Slider noch nicht gezeichnet?
};

//----------------------------------------------------------------------------
// Konstruktor
//----------------------------------------------------------------------------
colslider_data::colslider_data (colpal *_pal, vector <color_pos> *_sliders)
{
  first_draw = true;
  pal = _pal;
  if (_sliders == NULL)
    sliders = &pal->colors;
  else
    sliders = _sliders;
}

//============================================================================
// Allegro-Dialog-Handler
//============================================================================
//----------------------------------------------------------------------------
// Farbverlaufswähler
//----------------------------------------------------------------------------
int
d_colslider_proc (int msg, DIALOG *d, int c)
{
  /*** Variables ***/
  colslider_data *data = (colslider_data *) d->dp; // Daten
  void (*onChange) (DIALOG *) = (void (*)(DIALOG *)) d->dp2;
  int lcol; // Linienfarbe
  vector <color_pos>::iterator act_slider; // Position des aktuell gewählten Sliders
  vector <color_pos>::iterator iter; // Iterator für diverse Zwecke
  int last_x = 0; // Anti-Flicker
  unsigned int i; // Zählvariable

  /*** Commands ***/
  for (act_slider = data->sliders->begin (); act_slider != data->sliders->end ();
       act_slider++)
    {
      if (act_slider->id == d->d2)
        break;
    }

  switch (msg)
    {
      case MSG_START:
        data->back = create_bitmap (d->w, d->h);
        if (data->back == NULL)
          return D_CLOSE;
        data->bmp = create_bitmap (d->w, d->h);
        if (data->bmp == NULL)
          return D_CLOSE;
        d->d2 = (*data->sliders)[0].id;
        break;
          
      case MSG_END:
        destroy_bitmap (data->back);
        destroy_bitmap (data->bmp);
        break;

      case MSG_WANTFOCUS:
        return D_WANTFOCUS;
        break;
        
      case MSG_CHAR:
        if (data->sliders->size () > 0)
          {
            if ((c >> 8) == KEY_LEFT && act_slider->pos > 0)
              act_slider->pos--;
            if ((c >> 8) == KEY_RIGHT && act_slider->pos < 255)
              act_slider->pos++;
            if ((c >> 8) == KEY_DEL && (d->d1 & COLSLIDER_CHANGEABLE)
                                       == COLSLIDER_CHANGEABLE)
              {
                act_slider = data->sliders->erase (act_slider);
                d->d2 = act_slider->id;
              }

            if (onChange == NULL)
              {
                data->pal->calc_pal ();
              }
            else
              onChange (d);

            object_message (d, MSG_DRAW, 0);
            return D_USED_CHAR;
          }
        break;
        
      case MSG_LPRESS:
        if (gui_mouse_y () >= d->y + d->h - 15 && gui_mouse_y () < d->y + d->h)
          {
            for (iter = data->sliders->begin (); iter != data->sliders->end (); iter++)
              {
                if (2 * abs ((d->x + 8 + ((d->w - 15) * iter->pos) / 255 - gui_mouse_x ()))
                    < 2 * 8 - (d->y + d->h - gui_mouse_y ()))
                act_slider = iter;
              }
            d->d2 = act_slider->id;
              
            while (gui_mouse_b () == 1)
              {
                act_slider->pos = (255 * (gui_mouse_x () - d->x - 8)) / (d->w - 15);
                if (act_slider->pos < 0)
                  act_slider->pos = 0;
                if (act_slider->pos > 255)
                  act_slider->pos = 255;

                if ((d->d1 & COLSLIDER_INSTCALC) == COLSLIDER_INSTCALC)
                  {
                    if (onChange == NULL)
                      {
                        data->pal->calc_pal ();
                      }
                    else
                      onChange (d);
                  }
		if (last_x != gui_mouse_x ())
		  {
                    object_message (d, MSG_DRAW, 0);
		    last_x = gui_mouse_x ();
		  }
                broadcast_dialog_message (MSG_IDLE, 0);
              }
              
            if (onChange == NULL)
              {
                data->pal->calc_pal ();
              }
            else
              onChange (d);

            object_message (d, MSG_DRAW, 0);
          }
        break;
        
      case MSG_RPRESS:
        if (gui_mouse_y () >= d->y + d->h - 15 && gui_mouse_y () < d->y + d->h)
          {
            for (iter = data->sliders->begin (); iter != data->sliders->end (); iter++)
              {
                if (2 * abs ((d->x + 8 + ((d->w - 15) * iter->pos) / 255 - gui_mouse_x ()))
                    < 2 * 8 - (d->y + d->h - gui_mouse_y ()))
                  act_slider = iter;
              }
            d->d2 = act_slider->id;

            object_message (d, MSG_DRAW, 0);

            while (gui_mouse_b () == 2)
              broadcast_dialog_message (MSG_IDLE, 0);

            if (col_select != NULL && (d->d1 & COLSLIDER_CHANGEABLE) == COLSLIDER_CHANGEABLE
                && 2 * abs ((d->x + 8 + ((d->w - 15) * act_slider->pos) / 255
                            - gui_mouse_x ()))
                   < 2 * 8 - (d->y + d->h - gui_mouse_y ()))
              {
                act_slider->color = col_select (act_slider->color);
                if (onChange == NULL)
                  {
                    data->pal->calc_pal ();
                  }
                else
                  onChange (d);

                object_message (d, MSG_DRAW, 0);
              }
          }
        break;
        
      case MSG_DRAW:
        if (data->first_draw)
          blit (gui_get_screen (), data->back, d->x, d->y, 0, 0, d->w, d->h);

        data->first_draw = false;
        draw_sprite (data->bmp, data->back, 0, 0);
        stretch_sprite (data->bmp, data->pal->preview, 8, 0, d->w - 16, d->h - 7);
        for (i = 0; i < data->sliders->size (); i++)
          {
            triangle (data->bmp, ((d->w - 16) * (*data->sliders)[i].pos) / COL_PER_PAL,
                      d->h - 1,
                      ((d->w - 16) * (*data->sliders)[i].pos) / COL_PER_PAL + 16,
                      d->h - 1,
                      ((d->w - 16) * (*data->sliders)[i].pos) / COL_PER_PAL + 8,
                      d->h - 16,
                      (*data->sliders)[i].color);

            lcol = ((*data->sliders)[i].id == d->d2
                    && (d->flags & D_GOTFOCUS) == D_GOTFOCUS)
                   ? d->fg : d->bg;
            line (data->bmp, ((d->w - 16) * (*data->sliders)[i].pos) / COL_PER_PAL,
                  d->h - 1, ((d->w - 16) * (*data->sliders)[i].pos) / COL_PER_PAL + 16,
                  d->h - 1, lcol);
            line (data->bmp, ((d->w - 16) * (*data->sliders)[i].pos) / COL_PER_PAL,
                  d->h - 1, ((d->w - 16) * (*data->sliders)[i].pos) / COL_PER_PAL + 8,
                  d->h - 16, lcol);
            line (data->bmp, ((d->w - 16) * (*data->sliders)[i].pos) / COL_PER_PAL + 16,
                  d->h - 1, ((d->w - 16) * (*data->sliders)[i].pos) / COL_PER_PAL + 8,
                  d->h - 16, lcol);
          }

        draw_sprite (gui_get_screen (), data->bmp, d->x, d->y);
        break;
    }
    
  return D_O_K;
}

#endif // ifndef PALETTE_H
