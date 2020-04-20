/***************************************************************************
 * Filename: fractal.h                                                     *
 * Begin: Su Apr 17 09:11:29 CET 2005                                      *
 * Description: Berechnung von Fraktalen                                   *
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
 
#ifndef FRACTAL_H
#define FRACTAL_H 1

/***** Includes *****/
#include <cmath>
#include <cstdlib>
#include <new>

#include <allegro.h>

#ifdef FRACTAL_USE_XMA
 #include <xma.h>
#endif

/***** Defines *****/
/*** Aktionsbezeichnungen für d_frac_proc ***/
#define D_FRAC_ACT_NOTHING 0
#define D_FRAC_ACT_ZOOMIN 1
#define D_FRAC_ACT_ZOOMOUT 2
#define D_FRAC_ACT_MOVE 3
#define D_FRAC_ACT_JULIA 4

/*** Statusbezeichnungen für d_frac_proc ***/
#define D_FRAC_ST_STD 0
#define D_FRAC_ST_JULIA 1
#define D_FRAC_ST_USER 2

/***** Misc *****/
using namespace std;

//============================================================================
// Klasse für (Escape-Time-)Fraktale und deren Berechnung
//============================================================================
class fractal {
  public:
    /*** Variables ***/
    BITMAP *act_view; // Aktuelle Ansicht
    unsigned int w, h; // Breite und Höhe
    unsigned int iterations; // Zahl der Iterationen
    double min_x, max_x, min_y, max_y; // Koordinatend es angezeigten Bereichs
    double a, b, c; // Konstante Werte für Fraktalerzeugung
    int bg; // Farbe für das Fraktalinnere
    int *colors; // Farbpalette
    unsigned int disp_cols; // Zahl der darzustellenden Farben
    unsigned int col_num; // Zahl der Farben
    void (*iter_func)(double, double, double, double, double,
                      unsigned int, unsigned int, unsigned int, int, int *,
                      unsigned int, unsigned int, BITMAP *); // Iterations-Funktion
    
    /*** Functions ***/
    fractal (unsigned int _w, unsigned int _h, double _min_x, double _max_x,
             double _min_y, double _a, double _b, double _c,
             unsigned int _iterations, int _bg, int *_colors, unsigned int _col_num,
             unsigned int _disp_cols,
             void (*_iter_func)(double, double, double, double, double,
                                unsigned int, unsigned int, unsigned int, int,
                                int *, unsigned int, unsigned int, BITMAP *),
             void (*procbar_func)(unsigned int, unsigned int) = NULL);
    ~fractal (void);
    void reinit (double _min_x, double _max_x, double _min_y, double _a, double _b,
                 void (*procbar_func)(unsigned int, unsigned int) = NULL);
    void render (void (*procbar_func)(unsigned int, unsigned int) = NULL);
    void render_area (unsigned int start_x, unsigned int start_y,
                      unsigned int end_x, unsigned int end_y,
                      void (*procbar_func)(unsigned int, unsigned int) = NULL);
    void move (int x_offset, int y_offset, int col);

    friend int d_fractal_proc (int msg, DIALOG *d, int c);
};

//----------------------------------------------------------------------------
// Konstruktor
// Eingabe:                 _w, _h = Breite und Höhe
//          _min_x, _max_x, _min_y = Zu berechnender Bereich
//                      _a, _b, _c = Konstante Werte für Iterator-Funktion
//                     _iterations = Zahl der Iterationen
//                             _bg = Hauptfarbe
//                         _colors = weitere Farben
//                      _iter_func = Iterator-Funktion
//                    procbar_func = Zeichenfunktion für Fortschrittsbalken
//----------------------------------------------------------------------------
fractal::fractal (unsigned int _w, unsigned int _h, double _min_x, double _max_x,
                  double _min_y, double _a, double _b, double _c,
                  unsigned int _iterations, int _bg, int *_colors,
                  unsigned int _col_num, unsigned int _disp_cols,
                  void (*_iter_func)(double, double, double, double, double,
                                     unsigned int, unsigned int, unsigned int, int,
                                     int *, unsigned int, unsigned int, BITMAP *),
                  void (*procbar_func)(unsigned int, unsigned int))
{
  act_view = create_bitmap (_w, _h);
  if (act_view == NULL)
    throw bad_alloc ();
  w = _w;
  h = _h;
  iterations = _iterations;
  min_x = _min_x;
  max_x = _max_x;
  min_y = _min_y;
  max_y = _min_y + ((double) _h / (double) _w) * (double) (_max_x - _min_x);
  a = _a;
  b = _b;
  c = _c;
  bg = _bg;
  colors = _colors;
  col_num = _col_num;
  disp_cols = _disp_cols;
  iter_func = _iter_func;
  render (procbar_func);
}

//----------------------------------------------------------------------------
// Destruktor
//----------------------------------------------------------------------------
fractal::~fractal (void)
{
  destroy_bitmap (act_view);
}

//----------------------------------------------------------------------------
// Neu initialisieren
// Eingabe: siehe Konstruktor
//----------------------------------------------------------------------------
void
fractal::reinit (double _min_x, double _max_x, double _min_y, double _a, double _b,
                 void (*procbar_func)(unsigned int, unsigned int))
{
  min_x = _min_x;
  max_x = _max_x;
  min_y = _min_y;
  max_y = _min_y + ((double) h / (double) w) * (double) (_max_x - _min_x);
  a = _a;
  b = _b;
  render (procbar_func);
}

//----------------------------------------------------------------------------
// Funktion zum rendern des Fraktals
// Eingabe: procbar_func = Zeiger auf Funktion, die einen Fortschrittsbalken
//                         zeichnet; darf NULL sein
//----------------------------------------------------------------------------
void
fractal::render (void (*procbar_func)(unsigned int act, unsigned int max))
{
  render_area (0, 0, w, h, procbar_func);
  return;
}

//----------------------------------------------------------------------------
// Funktion zum rendern eines Bereiches des Fraktals
// Eingabe: start_x, starty, end_x, end_y = Pixelkoordinaten des darzustell-
//                                          enden Bereichs
//          procbar_func = Zeiger auf Funktion, die einen Fortschrittsbalken
//                         zeichnet; darf NULL sein
//----------------------------------------------------------------------------
void
fractal::render_area (unsigned int start_x, unsigned int start_y, unsigned int end_x,
                      unsigned int end_y,
                      void (*procbar_func)(unsigned int act, unsigned int max))
{
  /*** Variables ***/
  register unsigned int i, j; // Zählvariablen
  register double x, y; // Aktuelle Kooridinaten
  register const double x_step = (max_x - min_x) / w; // Schrittweite
  register const double y_step = (max_y - min_y) / h; // Schrittweite

  /*** Commands ***/
  for (i = start_x, x = min_x + x_step * start_x; i < end_x; i++, x += x_step)
    {
      for (j = start_y, y = min_y + y_step * end_y; j < end_y; j++, y -= y_step)
        iter_func (x, y, a, b, c, iterations, i, j, bg, colors, col_num, disp_cols,
                   act_view);
      if (procbar_func != NULL && i % 50 == 0)
        procbar_func (i - start_x, end_x - start_x);
    }

  if (procbar_func != NULL)
    procbar_func (i - start_x, end_x - start_x);
  return;
}

//----------------------------------------------------------------------------
// Bewegt das Fraktal um x_offset nach rechts und um y_offset nach unten
// Eingabe: x_offset, y_offset = Offset der Verschiebung
//                         col = Farbe mit der nicht berechnete Bereiche an-
//                               gezeigt werden.
//----------------------------------------------------------------------------
void
fractal::move (int x_offset, int y_offset, int col)
{
  /*** Variables ***/
  const double x_step = (max_x - min_x) / w; // Schrittweite
  const double y_step = (max_y - min_y) / h; // Schrittweite
  
  /*** Commands ***/
  blit (act_view, act_view, 0, 0, x_offset, y_offset, w, h);
  min_x -= x_offset * x_step;
  max_x -= x_offset * x_step;
  min_y += y_offset * y_step;
  max_y += y_offset * y_step;

  if (x_offset > 0)
    rectfill (act_view, 0, 0, x_offset, h, col);
  else
    rectfill (act_view, w + x_offset, 0, w, h, col);

  if (y_offset > 0)
    rectfill (act_view, 0, 0, w, y_offset, col);
  else
    rectfill (act_view, 0, h + y_offset, w, h, col);
}

//============================================================================
// Klasse für ein Fraktal mit Undo-Funktion
//============================================================================
class undo_fractal : public fractal {
  public:
    undo_fractal (unsigned int _w, unsigned int _h, double _min_x, double _max_x,
                  double _min_y, double _a, double _b, double _c,
                  unsigned int _iterations, int _bg, int *_colors,
                  unsigned int _col_num, unsigned int _disp_cols,
                  void (*_iter_func)(double, double, double, double, double,
                                     unsigned int, unsigned int, unsigned int, int,
                                     int *, unsigned int, unsigned int, BITMAP *),
                  void (*procbar_func)(unsigned int, unsigned int) = NULL);
    void save (void);
    bool undo (void);
    void deactivate_undo (void);
                  
  protected:
    bool undo_possible; // Undo-Aktion möglich?
    /* Undo-Variablen */
    unsigned int u_iterations; // Zahl der Iterationen
    double u_min_x, u_max_x, u_min_y, u_max_y; // Koordinatend es angezeigten Bereichs
    double u_a, u_b, u_c; // Konstante Werte für Fraktalerzeugung
    void (*u_iter_func)(double, double, double, double, double,
                        unsigned int, unsigned int, unsigned int, int, int *,
                        unsigned int, unsigned int, BITMAP *); // Iterations-Funktion
    
};

//----------------------------------------------------------------------------
// Konstruktor
//----------------------------------------------------------------------------
undo_fractal::undo_fractal (unsigned int _w, unsigned int _h, double _min_x,
                            double _max_x, double _min_y, double _a, double _b,
                            double _c, unsigned int _iterations, int _bg,
                            int *_colors, unsigned int _col_num,
                            unsigned int _disp_cols,
                            void (*_iter_func)(double, double, double, double,
                                               double, unsigned int, unsigned int,
                                               unsigned int, int, int *,
                                               unsigned int, unsigned int, BITMAP *),
                            void (*procbar_func)(unsigned int, unsigned int))
                 : fractal (_w, _h, _min_x, _max_x, _min_y, _a, _b, _c, _iterations,
                            _bg, _colors, _col_num, _disp_cols, _iter_func, procbar_func)
{
  undo_possible = false;
}

//----------------------------------------------------------------------------
// Aktuellen Status des Fraktals speichern
//----------------------------------------------------------------------------
void
undo_fractal::save (void)
{
  u_iterations = iterations;
  u_min_x = min_x;
  u_max_x = max_x;
  u_min_y = min_y;
  u_max_y = max_y;
  u_a = a;
  u_b = b;
  u_c = c;
  u_iter_func = iter_func;
  undo_possible = true;
  return;
}

//----------------------------------------------------------------------------
// Alten Fraktalstatus wiederherstellen
//----------------------------------------------------------------------------
bool
undo_fractal::undo (void)
{
  if (!undo_possible)
    return false;

  iterations = u_iterations;
  min_x = u_min_x;
  max_x = u_max_x;
  min_y = u_min_y;
  max_y = u_max_y;
  a = u_a;
  b = u_b;
  c = u_c;
  iter_func = u_iter_func;
  undo_possible = false;
  return true;
}

//----------------------------------------------------------------------------
// Undo-Funktion deaktivieren
//----------------------------------------------------------------------------
void
undo_fractal::deactivate_undo (void)
{
  undo_possible = false;
}

//============================================================================
// Struct, für Übergabe von Daten zu d_fractal_proc und um Rückgabewerte zu
//   speichern.
//============================================================================
struct d_fractal_proc_com {
  void (*procbar_func)(unsigned int, unsigned int); // Ladebalkenfunktion
  double a, b; // Koordinate
  int status; // Rückgabestatus
  d_fractal_proc_com (void (*_procbar_func)(unsigned int, unsigned int) = NULL);
};

d_fractal_proc_com::d_fractal_proc_com (void (*_procbar_func)(unsigned int,
                                                              unsigned int))
{
  procbar_func = _procbar_func;
  status = D_FRAC_ST_STD;
  return;
}

//============================================================================
// Struct, um Daten für d_fractal_proc zu speichern
//============================================================================
struct _d_fractal_proc_data {
  bool action_start; // Aktion gestartet? (Zoom in, Zoom out, etc)
  unsigned int zoom_start_x, zoom_start_y, zoom_end_x, zoom_end_y; // Zoom-Koordinaten
  unsigned int move_start_x, move_start_y; // Koordinaten für Verschiebung
  _d_fractal_proc_data (void);
};

_d_fractal_proc_data::_d_fractal_proc_data (void)
{
  action_start = false;
}

//============================================================================
// Allegro-Dialog-Handler zur Ausgabe etc. eines Fraktals
//============================================================================
int
d_fractal_proc (int msg, DIALOG *d, int c)
{
  /*** Variables ***/
  undo_fractal *obj = (undo_fractal *) d->dp; // Daten zum Fraktal
  d_fractal_proc_com *com = (d_fractal_proc_com *) d->dp2; // Com-Daten
  _d_fractal_proc_data *data = (_d_fractal_proc_data *) d->dp3; // Daten
  
  const double x_step = (obj->max_x - obj->min_x) / d->w; // Schrittweite
  const double y_step = (obj->max_y - obj->min_y) / d->h; // Schrittweite
  
  /* Variablen für Zoom in */
  int last_x = 0, last_y = 0; // Mausposition beim letzten Schleifendurchlauf
  BITMAP *buffer;
  
  /* Variablen für Zoom out */
  const double x_offset = ((d->w / 2) - (gui_mouse_x () - d->x)) * x_step;
  const double y_offset = ((d->h / 2) - (gui_mouse_y () - d->y)) * y_step;
  const double x_diff = abs (obj->max_x - obj->min_x) / 2;
  const double y_diff = abs (obj->max_y - obj->min_y) / 2;

  /*** Commands ***/
  switch (msg)
    {
      case MSG_START:
        d->dp3 = new(nothrow) _d_fractal_proc_data ();
        if (d->dp3 == NULL)
          return D_CLOSE;
        com->status = D_FRAC_ST_STD;
        break;
        
      case MSG_END:
        if (d->dp3 != NULL)
          delete (_d_fractal_proc_data *) d->dp3;
        break;

      case MSG_LPRESS:
        switch (d->d1)
          {
            case D_FRAC_ACT_ZOOMIN:
              data->zoom_start_x = gui_mouse_x ();
              data->zoom_start_y = gui_mouse_y ();
	            buffer = create_bitmap (d->w, d->h);
	            scare_mouse ();
	            blit (gui_get_screen (), buffer, d->x, d->y, 0, 0, d->w, d->h);
	            unscare_mouse ();
              break;
            case D_FRAC_ACT_ZOOMOUT:
            case D_FRAC_ACT_JULIA:
              break;
            case D_FRAC_ACT_MOVE:
              obj->save ();
              data->move_start_x = gui_mouse_x ();
              data->move_start_y = gui_mouse_y ();
              break;
            default:
              return D_O_K;
              break;
          }
          
        while (gui_mouse_b () & 1)
          {
            if (d->d1 == D_FRAC_ACT_ZOOMIN)
              {
                if (last_x == gui_mouse_x () && last_y == gui_mouse_y ())
		              {
		                broadcast_dialog_message (MSG_IDLE, 0);
                    continue;
		              }

                if (gui_mouse_x () - d->x > d->w - 1)
                  data->zoom_end_x = d->w + d->x - 1;
                else if (gui_mouse_x () - d->x < 0)
                  data->zoom_end_x = d->x;
                else
                  data->zoom_end_x = gui_mouse_x ();

                if (gui_mouse_y () - d->y > d->h - 1)
                  data->zoom_end_y = d->h + d->y - 1;
                else if (gui_mouse_y () - d->y < 0)
                  data->zoom_end_y = d->y;
                else
                  data->zoom_end_y = gui_mouse_y ();

                int mirror_x = (data->zoom_end_x < data->zoom_start_x) ? -1 : 1;
                int mirror_y = (data->zoom_end_y < data->zoom_start_y) ? -1 : 1;
                if (((double) d->h / (double) d->w)
                    * abs ((signed) (data->zoom_start_x - data->zoom_end_x))
                    > abs ((signed) (data->zoom_start_y - data->zoom_end_y)))
                  data->zoom_end_y = data->zoom_start_y + mirror_y
                                     * (int) ceil (((double) d->h / (double) d->w)
                                     * abs ((signed) (data->zoom_start_x - data->zoom_end_x)));
                else
                  data->zoom_end_x = data->zoom_start_x + mirror_x
                                     * (int) ceil (((double) d->w / (double) d->h)
                                     * abs ((signed) (data->zoom_start_y
                                                      - data->zoom_end_y)));

                if ((signed) data->zoom_end_x - d->x > d->w - 1)
                  {
                    data->zoom_end_x = d->w + d->x - 1;
                    data->zoom_end_y = data->zoom_start_y + mirror_y
                                       * (int) ceil (((double) d->h / (double) d->w)
                                       * abs ((signed) (data->zoom_start_x
                                                        - data->zoom_end_x)));
                  }
                else if ((signed) data->zoom_end_x - d->x < 0)
                  {
                    data->zoom_end_x = d->x;
                    data->zoom_end_y = data->zoom_start_y + mirror_y
                                       * (int) ceil (((double) d->h / (double) d->w)
                                       * abs ((signed) (data->zoom_start_x
                                                        - data->zoom_end_x)));
                  }
                if ((signed) data->zoom_end_y - d->y > d->h - 1)
                  {
                    data->zoom_end_y = d->h + d->y - 1;
                    data->zoom_end_x = data->zoom_start_x + mirror_x
                                       * (int) ceil (((double) d->w / (double) d->h)
                                       * abs ((signed) (data->zoom_start_y
                                                        - data->zoom_end_y)));
                  }
                else if ((signed) data->zoom_end_y - d->y < 0)
                  {
                    data->zoom_end_y = d->y;
                    data->zoom_end_x = data->zoom_start_x + mirror_x
                                       * (int) ceil (((double) d->w / (double) d->h)
                                       * abs ((signed) (data->zoom_start_y
                                                        - data->zoom_end_y)));
                  }

		            draw_sprite (obj->act_view, buffer, 0, 0);
		            fastline (obj->act_view, data->zoom_start_x - d->x,
                          data->zoom_start_y - d->y, data->zoom_start_x - d->x,
                          data->zoom_end_y, d->fg);
                fastline (obj->act_view, data->zoom_start_x - d->x,
                          data->zoom_start_y - d->y, data->zoom_end_x - d->x,
                          data->zoom_start_y - d->y, d->fg);
                fastline (obj->act_view, data->zoom_start_x - d->x,
                          data->zoom_end_y - d->y, data->zoom_end_x - d->x,
                          data->zoom_end_y - d->y, d->fg);
                fastline (obj->act_view, data->zoom_end_x - d->x,
                          data->zoom_start_y - d->y, data->zoom_end_x - d->x,
                          data->zoom_end_y - d->y, d->fg);
                object_message (d, MSG_DRAW, 0);

                last_x = gui_mouse_x ();
                last_y = gui_mouse_y ();
              }
            else if (d->d1 == D_FRAC_ACT_MOVE)
              {
                obj->move (gui_mouse_x () - data->move_start_x,
                           gui_mouse_y () - data->move_start_y, d->bg);
                data->move_start_x = gui_mouse_x ();
                data->move_start_y = gui_mouse_y ();
                stretch_sprite (gui_get_screen (), obj->act_view, d->x, d->y, d->w, d->h);
                if (gui_mouse_x () < d->x || gui_mouse_x () > d->x + d->w
                    || gui_mouse_y () < d->y || gui_mouse_y () > d->y + d->h)
                  break;
              }
              
            broadcast_dialog_message (MSG_IDLE, 0);
          }
          
        switch (d->d1)
          {
            case D_FRAC_ACT_ZOOMIN:
              if (data->zoom_end_x < data->zoom_start_x)
                {
                  unsigned int swp_val;
                  swp_val = data->zoom_start_x;
                  data->zoom_start_x = data->zoom_end_x;
                  data->zoom_end_x = swp_val;
                }

               if (data->zoom_end_y < data->zoom_start_y)
                 {
                   unsigned int swp_val;
                   swp_val = data->zoom_start_y;
                   data->zoom_start_y = data->zoom_end_y;
                   data->zoom_end_y = swp_val;
                 }

               if (data->zoom_start_x - data->zoom_end_x != 0
                   && data->zoom_start_y - data->zoom_end_y != 0)
                 {
                   obj->save ();
                   
                   #ifdef FRACTAL_USE_XMA
                     xma_anim *anim = new xma_anim ();
                     anim->action_type = XMA_ANIMATION;
                     anim->start = 0;
                     anim->length = 4 * 25;
                     anim->w = obj->w;
                     anim->h = obj->h;
                     anim->params.push_back (new string);
                     anim->underlying.push_back (new xma_anim ());
                     anim->underlying[0]->action_type = XMA_FZOOM;
                     anim->underlying[0]->x = 0;
                     anim->underlying[0]->y = 0;
                     anim->underlying[0]->w = obj->w;
                     anim->underlying[0]->h = obj->h;
                     anim->underlying[0]->start = 0;
                     anim->underlying[0]->length = anim->length;
                     anim->underlying[0]->initialised = true;
                     anim->underlying[0]->bmp = create_bitmap (obj->w, obj->h);
                     anim->underlying[0]->params.push_back (new int);
                     *((int *) anim->underlying[0]->params[0]) = XMA_FZOOM_IN;
                     anim->underlying[0]->params.push_back (new double);
                     *((double *) anim->underlying[0]->params[1]) = obj->min_x;
                     anim->underlying[0]->params.push_back (new double);
                     *((double *) anim->underlying[0]->params[2]) = obj->max_x;
                     anim->underlying[0]->params.push_back (new double);
                     *((double *) anim->underlying[0]->params[3]) = obj->min_y;
                     anim->underlying[0]->params.push_back (new double);
                     *((double *) anim->underlying[0]->params[4]) = obj->max_y;
                     anim->underlying[0]->params.push_back (new string);
                   #endif
                   
                   obj->max_x = (data->zoom_end_x - d->x) * x_step + obj->min_x;
                   obj->min_x = (data->zoom_start_x - d->x) * x_step + obj->min_x;
                   obj->max_y = - ((data->zoom_start_y - d->y) * y_step - obj->max_y);
                   obj->min_y = obj->max_y - ((double) obj->h / obj->w)
                                * (obj->max_x - obj->min_x);

                   #ifdef FRACTAL_USE_XMA
                     anim->underlying[0]->params.push_back (new double);
                     *((double *) anim->underlying[0]->params[6]) = obj->min_x;
                     anim->underlying[0]->params.push_back (new double);
                     *((double *) anim->underlying[0]->params[7]) = obj->max_x;
                     anim->underlying[0]->params.push_back (new double);
                     *((double *) anim->underlying[0]->params[8]) = obj->min_y;
                     anim->underlying[0]->params.push_back (new double);
                     *((double *) anim->underlying[0]->params[9]) = obj->max_y;
                     anim->underlying[0]->params.push_back (new string);

                     anim->underlying[0]->params.push_back (create_bitmap (obj->w, obj->h));
                     draw_sprite (obj->act_view, buffer, 0, 0);
                     draw_sprite ((BITMAP *) anim->underlying[0]->params[11],
                                  obj->act_view, 0, 0);
                   #endif
                   
                   destroy_bitmap (buffer);
                   obj->render (com->procbar_func);
                   
                   #ifdef FRACTAL_USE_XMA
                     anim->underlying[0]->params.push_back (create_bitmap (obj->w, obj->h));
                     draw_sprite ((BITMAP *) anim->underlying[0]->params[12],
                                  obj->act_view, 0, 0);
                     play_xma_blocking (anim, gui_get_screen (), d->x, d->y);
                     destroy_xma (anim);
                   #endif
                 }
               return object_message (d, MSG_DRAW, 0);
               break;
             case D_FRAC_ACT_ZOOMOUT:
               if (gui_mouse_x () >= d->x && gui_mouse_x () <= d->x + d->w
                   && gui_mouse_y () >= d->y && gui_mouse_y () <= d->y + d->h)
                 {
                   obj->save ();

                   #ifdef FRACTAL_USE_XMA
                     xma_anim *anim = new xma_anim ();
                     anim->action_type = XMA_ANIMATION;
                     anim->start = 0;
                     anim->length = 4 * 12;
                     anim->w = obj->w;
                     anim->h = obj->h;
                     anim->params.push_back (new string);
                     anim->underlying.push_back (new xma_anim ());
                     anim->underlying[0]->action_type = XMA_FZOOM;
                     anim->underlying[0]->x = 0;
                     anim->underlying[0]->y = 0;
                     anim->underlying[0]->w = obj->w;
                     anim->underlying[0]->h = obj->h;
                     anim->underlying[0]->start = 0;
                     anim->underlying[0]->length = anim->length;
                     anim->underlying[0]->initialised = true;
                     anim->underlying[0]->bmp = create_bitmap (obj->w, obj->h);
                     anim->underlying[0]->params.push_back (new int);
                     *((int *) anim->underlying[0]->params[0]) = XMA_FZOOM_OUT;
                     anim->underlying[0]->params.push_back (new double);
                     *((double *) anim->underlying[0]->params[1]) = obj->min_x;
                     anim->underlying[0]->params.push_back (new double);
                     *((double *) anim->underlying[0]->params[2]) = obj->max_x;
                     anim->underlying[0]->params.push_back (new double);
                     *((double *) anim->underlying[0]->params[3]) = obj->min_y;
                     anim->underlying[0]->params.push_back (new double);
                     *((double *) anim->underlying[0]->params[4]) = obj->max_y;
                     anim->underlying[0]->params.push_back (new string);
                   #endif

                   obj->min_x -= x_diff + x_offset;
                   obj->max_x += x_diff - x_offset;
                   obj->min_y -= y_diff - y_offset;
                   obj->max_y += y_diff + y_offset;

                   #ifdef FRACTAL_USE_XMA
                     anim->underlying[0]->params.push_back (new double);
                     *((double *) anim->underlying[0]->params[6]) = obj->min_x;
                     anim->underlying[0]->params.push_back (new double);
                     *((double *) anim->underlying[0]->params[7]) = obj->max_x;
                     anim->underlying[0]->params.push_back (new double);
                     *((double *) anim->underlying[0]->params[8]) = obj->min_y;
                     anim->underlying[0]->params.push_back (new double);
                     *((double *) anim->underlying[0]->params[9]) = obj->max_y;
                     anim->underlying[0]->params.push_back (new string);

                     anim->underlying[0]->params.push_back (create_bitmap (obj->w, obj->h));
                     draw_sprite ((BITMAP *) anim->underlying[0]->params[11],
                                  obj->act_view, 0, 0);
                   #endif

                   obj->render (com->procbar_func);

                   #ifdef FRACTAL_USE_XMA
                     anim->underlying[0]->params.push_back (create_bitmap (obj->w, obj->h));
                     draw_sprite ((BITMAP *) anim->underlying[0]->params[12],
                                  obj->act_view, 0, 0);
                     play_xma_blocking (anim, gui_get_screen (), d->x, d->y);
                     destroy_xma (anim);
                   #endif

                   return object_message (d, MSG_DRAW, 0);
                 }
               break;
             case D_FRAC_ACT_MOVE:
               obj->render (com->procbar_func);
               return object_message (d, MSG_DRAW, 0);
               break;
             case D_FRAC_ACT_JULIA:
               if (gui_mouse_x () >= d->x && gui_mouse_x () <= d->x + d->w
                   && gui_mouse_y () >= d->y && gui_mouse_y () <= d->y + d->h)
                 {
                   com->a = obj->min_x + (double) (gui_mouse_x () - d->x) * x_step;
                   com->b = obj->min_y + (double) (gui_mouse_y () - d->y) * y_step;
                   com->status = D_FRAC_ST_JULIA;
                   return D_CLOSE;
                 }
               break;
          }
        break;

      case MSG_DRAW:
        stretch_sprite (gui_get_screen (), obj->act_view, d->x, d->y, d->w, d->h);
        break;
    }

  return D_O_K;
}

//============================================================================
// Allegro-Dialog-Handler zur Ausgabe etc. eines Preview-Fraktals (v.a. J-M.)
//============================================================================
int
d_preview_fractal_proc (int msg, DIALOG *d, int c)
{
  /*** Variables ***/
  fractal *obj = (fractal *) d->dp; // Daten zum Fraktal
  DIALOG *grabber = (DIALOG *) d->dp2; // Auswahlfeld für Konstante
  fractal *grabber_obj = (fractal *) grabber->dp; // Daten des Auswahlfraktals

  const double grabber_x_step = (grabber_obj->max_x - grabber_obj->min_x)
                                / grabber->w; // Schrittweite beim Grabber
  const double grabber_y_step = (grabber_obj->max_y - grabber_obj->min_y)
                                / grabber->h; // Schrittweite beim Grabber

  const int mouse_xpos = gui_mouse_x (); // Mauspostition
  const int mouse_ypos = gui_mouse_y (); // Mauspostition

  /*** Commands ***/
  switch (msg)
    {
      case MSG_IDLE:
        if (mouse_xpos > grabber->x && mouse_xpos < grabber->x + grabber->w
            && mouse_ypos > grabber->y && mouse_ypos < grabber->y + grabber->h)
          {
            obj->a = grabber_obj->min_x + (double) (mouse_xpos - grabber->x)
                     * grabber_x_step;
            obj->b = grabber_obj->min_y + (double) (mouse_ypos - grabber->y)
                     * grabber_y_step;
            obj->render ();
            return object_message (d, MSG_DRAW, 0);
          }
        break;

      case MSG_DRAW:
        stretch_sprite (gui_get_screen (), obj->act_view, d->x, d->y, d->w, d->h);
        break;
    }

  return D_O_K;
}

//============================================================================
// Iterator-Functions
// Eingabe:        x, y = Aktuell iterierte Koordinate
//              a, b, c = Konstante Werte
//                 iter = Iterationen
//           xpos, ypos = Pixel-Position
//                   bg = Hauptfarbe
//               colors = Weitere Farben
//              col_num = Zahl der Farben in colors
//                  bmp = Zeichen-Bitmap
//============================================================================

//----------------------------------------------------------------------------
// Funktion zur Multiplikation komplexer Zahlen
// Eingabe: a1, b1 = Zahl 1
//          a2, b2 = Zahl 2
//          a3, b3 = Zur Rückgabe des Ergebnisses
//----------------------------------------------------------------------------
inline void
multiply_complex (double _a1, double _b1, double _a2, double _b2, double &a3,
                  double &b3)
{
  register double a1 = _a1, b1 = _b1, a2 = _a2, b2 = _b2;
  
  a3 = a1 * a2 - b1 * b2;
  b3 = a1 * b2 + a2 * b1;

  return;
}

//----------------------------------------------------------------------------
// Standard-Mandelbrotmenge
//----------------------------------------------------------------------------
inline void
mandelbrot (double x, double y, double _a, double _b, double _c, unsigned int iter,
            unsigned int xpos, unsigned int ypos, int bg, int *colors,
            unsigned int col_num, unsigned int disp_cols, BITMAP *bmp)
{
  /*** Variables ***/
  register unsigned int i; // Zählvariable
  register double a, b; // Für die Rechnung

  /*** Commands ***/
  for (a = _a, b = _b, i = 0; i < iter; i++)
    {
      multiply_complex (a, b, a, b, a, b);
      a += x;
      b += y;
      
      if (a * a + b * b > 4)
        break;
    }

  if (a * a + b * b <= 4)
    _putpixel32 (bmp, xpos, ypos, bg);
  else
    _putpixel32 (bmp, xpos, ypos, colors[((i % disp_cols) * col_num) / disp_cols]);
             
  return;
}

//----------------------------------------------------------------------------
// Mandelbrotmenge mit dem Exponent _c
//----------------------------------------------------------------------------
inline void
mandelbrot_x (double x, double y, double _a, double _b, double _c, unsigned int iter,
              unsigned int xpos, unsigned int ypos, int bg, int *colors,
              unsigned int col_num, unsigned int disp_cols, BITMAP *bmp)
{
  /*** Variables ***/
  register unsigned int i, j; // Zählvariable
  register double a1, b1, a2, b2; // Für die Rechnung

  /*** Commands ***/
  for (a1 = _a, b1 = _b, i = 0; i < iter; i++)
    {
      for (a2 = a1, b2 = b1, j = 1; j < _c; j++)
        multiply_complex (a1, b1, a2, b2, a2, b2);
      a1 = a2 + x;
      b1 = b2 + y;

      if (a1 * a1 + b1 * b1 > 4)
        break;
    }

  if (a1 * a1 + b1 * b1 <= 4)
    _putpixel32 (bmp, xpos, ypos, bg);
  else
    _putpixel32 (bmp, xpos, ypos, colors[((i % disp_cols) * col_num) / disp_cols]);

  return;
}

//----------------------------------------------------------------------------
// Standard Julia-Menge
//----------------------------------------------------------------------------
inline void
julia (double _x, double _y, double a, double b, double c, unsigned int iter,
       unsigned int xpos, unsigned int ypos, int bg, int *colors,
       unsigned int col_num, unsigned int disp_cols, BITMAP *bmp)
{
  /*** Variables ***/
  register unsigned int i; // Zählvariable
  register double x, y; // Für die Rechnung

  /*** Commands ***/
  for (x = _x, y = _y, i = 0; i < iter; i++)
    {
      multiply_complex (x, y, x, y, x, y);
      x += a;
      y += b;

      if (x * x + y * y > 4)
        break;
    }

  if (x * x + y * y <= 4)
    _putpixel32 (bmp, xpos, ypos, bg);
  else
    _putpixel32 (bmp, xpos, ypos, colors[((i % disp_cols) * col_num) / disp_cols]);

  return;
}

//----------------------------------------------------------------------------
// Julia-Menge mit dem Exponenten c
//----------------------------------------------------------------------------
inline void
julia_x (double _x, double _y, double a, double b, double c, unsigned int iter,
         unsigned int xpos, unsigned int ypos, int bg, int *colors,
         unsigned int col_num, unsigned int disp_cols, BITMAP *bmp)
{
  /*** Variables ***/
  register unsigned int i, j; // Zählvariable
  register double x1, y1, x2, y2; // Für die Rechnung

  /*** Commands ***/
  for (x1 = _x, y1 = _y, i = 0; i < iter; i++)
    {
      for (x2 = x1, y2 = y1, j = 1; j < c; j++)
        multiply_complex (x1, y1, x2, y2, x2, y2);
      x1 = x2 + a;
      y1 = y2 + b;

      if (x1 * x1 + y1 * y1 > 4)
        break;
    }

  if (x1 * x1 + y1 * y1 <= 4)
    _putpixel32 (bmp, xpos, ypos, bg);
  else
    _putpixel32 (bmp, xpos, ypos, colors[((i % disp_cols) * col_num) / disp_cols]);

  return;
}

//----------------------------------------------------------------------------
// Spinnnen-Fraktal
//----------------------------------------------------------------------------
inline void
spider (double _x, double _y, double _a, double _b, double c, unsigned int iter,
        unsigned int xpos, unsigned int ypos, int bg, int *colors,
        unsigned int col_num, unsigned int disp_cols, BITMAP *bmp)
{
  /*** Variables ***/
  register unsigned int i; // Zählvariable
  register double x, y, a, b; // Für die Rechnung

  /*** Commands ***/
  for (x = _x, y = _y, a = _x, b = _y, i = 0; i < iter; i++)
    {
      multiply_complex (x, y, x, y, x, y);
      x += a;
      y += b;
      a = a / 2 + x;
      b = b / 2 + y;
      
      if (x * x + y * y > 4)
        break;
    }

  if (x * x + y * y <= 4)
    _putpixel32 (bmp, xpos, ypos, bg);
  else
    _putpixel32 (bmp, xpos, ypos, colors[((i % disp_cols) * col_num) / disp_cols]);

  return;
}

//----------------------------------------------------------------------------
// Newton-Fraktal
//----------------------------------------------------------------------------
inline void
newton (double _x, double _y, double a, double b, double c, unsigned int iter,
        unsigned int xpos, unsigned int ypos, int bg, int *colors,
        unsigned int col_num, unsigned int disp_cols, BITMAP *bmp)
{
  /*** Variables ***/
  register unsigned int i; // Zählvariable
  register double x, y, x2, nenner; // Für die Rechnung

  /*** Commands ***/
  for (x = _x, y = _y, i = 0; i < iter; i++)
    {
      nenner = (x * x + y * y) * (x * x + y * y) *(x * x + y * y);
      x2 = 3.0/4.0 * x + x/4.0 * (x * x - 3.0 * y * y) / nenner;
      y = 3.0/4.0 * y - y/4.0 * (3.0 * x * x - y * y) / nenner;
      x = x2;

      if (pow (x - 0, 2) + pow (y - 1, 2) < 0.01
          || pow (x + 1, 2) + pow (y - 0, 2) < 0.01
          || pow (x - 0, 2) + pow (y + 1, 2) < 0.01
          || pow (x - 1, 2) + pow (y - 0, 2) < 0.01)
        break;
    }

  _putpixel32 (bmp, xpos, ypos, colors[((i % disp_cols) * col_num) / disp_cols]);

  return;
}

#endif // ifndef FRACTAL_H
