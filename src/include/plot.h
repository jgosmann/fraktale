/***************************************************************************
 * Filename: plot.h                                                        *
 * Begin: Th Jun 23 15:44:20 CET 2005                                      *
 * Description: Klasse zum Zeichnen von Graphen und grafischen Iterationen *
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

#ifndef PLOT_H
#define PLOT_H 1

/***** Includes *****/
#include <cstdlib>
#include <cmath>

#include <allegro.h>
#include <alfont.h>

/***** Functions *****/
//----------------------------------------------------------------------------
// Rundet eine Zahl
// Eingabe: val = Zu rundender Wert
// Rückgabe: Der gerundete Wert
//----------------------------------------------------------------------------
int
plot_round (double val)
{
  /*** Variables ***/
  double int_part; // ganzzahliger Teil
  
  /*** Commands ***/
  if (abs (modf (val, &int_part)) < 0.5)
    return (int) int_part;
  
  if (val < 0)
      return (int) int_part - 1;
      
  return (int) int_part + 1;
}

/***** Classes *****/
//============================================================================
// Die Plot-Klasse
//============================================================================
class plot {
  public:
    BITMAP *bmp; // Zeichenfläche
    ALFONT_FONT *font; // Schriftart
    double xmin, xmax, ymin, ymax; // Bereichsvariablen
    double x_lbl, y_lbl; // Werte für Beschriftung
    
    plot (double _xmin, double _xmax, double _ymin, double _ymax, unsigned int w,
          unsigned int h, ALFONT_FONT *_font);
    ~plot (void);
    
    void draw_axes (int color = makecol (0, 0, 0));
    void graph (double (*func) (double), int color = makecol (0, 0, 0));
    void iterate (double (*func) (double), double x0, unsigned int n,
                  int color = makecol (0, 0, 0));
    void clear (int color = makecol (255, 255, 255));
};

//----------------------------------------------------------------------------
// Konstruktor, Destruktor
//----------------------------------------------------------------------------
plot::plot (double _xmin, double _xmax, double _ymin, double _ymax, unsigned int w,
            unsigned int h, ALFONT_FONT *_font)
{
  /*** Variables ***/
  double x_step = (_xmax - _xmin) / (double) (w - 60); // x-Schrittweite
  double y_step = (_ymax - _ymin) / (double) (h - 60); // y-Schrittweite
  
  /*** Commands ***/
  xmin = _xmin - 10.0 * x_step;
  xmax = _xmax + 50.0 * x_step;
  ymin = _ymin - 10.0 * y_step;
  ymax = _ymax + 50.0 * y_step;
  x_lbl = _xmax;
  y_lbl = _ymax;
  bmp = create_bitmap (w, h);
  clear_to_color (bmp, makecol (255, 255, 255));
  font = _font;
}

plot::~plot (void)
{
  destroy_bitmap (bmp);
}

//----------------------------------------------------------------------------
// Achsen einzeichnen
// Eingabe: color = Zeichenfarbe
//----------------------------------------------------------------------------
void
plot::draw_axes (int color)
{
  /*** Variables ***/
  double x_step = (xmax - xmin) / (double) bmp->w; // Pixelbreite (x)
  double y_step = (ymax - ymin) / (double) bmp->h; // Pixelbreite (y)
  int x_zero_line = plot_round ((0.0 - xmin) / x_step); // Position der x-Achse
  int y_zero_line = bmp->h - plot_round ((0.0 - ymin) / y_step); // Position der x-Achse
  
  /*** Commands ***/
  alfont_set_font_size (font, 15);
  
  /* y-Achse zeichnen */
  fastline (bmp, x_zero_line, bmp->h - 10, x_zero_line, 10, color);
  line (bmp, x_zero_line, 10, x_zero_line - 4, 26, color);
  line (bmp, x_zero_line, 10, x_zero_line + 4, 26, color);
  fastline (bmp, x_zero_line, 50, x_zero_line - 5, 50, color);
  alfont_textprintf_right_aa_ex (bmp, font, x_zero_line - 7, 42, color,
                                 makeacol (0, 0, 0, 255), "%0.2lf", y_lbl);
                                 
  /* x-Achse zeichnen */
  fastline (bmp, 10, y_zero_line, bmp->w - 10, y_zero_line, color);
  line (bmp, bmp->w - 10, y_zero_line, bmp->w - 26, y_zero_line - 4, color);
  line (bmp, bmp->w - 10, y_zero_line, bmp->w - 26, y_zero_line + 4, color);
  fastline (bmp, bmp->w - 50, y_zero_line, bmp->w - 50, y_zero_line + 5, color);
  alfont_textprintf_centre_aa_ex (bmp, font, bmp->w - 50, y_zero_line + 10,
                                  color, makeacol (0, 0, 0, 255), "%0.2lf", x_lbl);
}

//----------------------------------------------------------------------------
// Funktion zum Zeichnen eines Graphen
// Eingabe:  func = Funktion des Graph
//          color = Zeichenfarbe
//----------------------------------------------------------------------------
void
plot::graph (double (*func) (double), int color)
{
  /*** Variables ***/
  double x_step = (xmax - xmin) / (double) bmp->w; // Pixelbreite (x)
  double y_step = (ymax - ymin) / (double) bmp->h; // Pixelbreite (y)
  double x; // Aktuell berechneter x-Wert
  int old_y; // Vorheriger y-Wert als Pixel-Koordinate
  int new_y; // Neuer y-Wert als Pixel-Koordinate
  unsigned int i; // Zählvariable
  
  /*** Commands ***/
  old_y = bmp->h - plot_round ((func (xmin) - ymin) / y_step);
  for (i = 0, x = xmin; i < (unsigned) bmp->w; i++, x += x_step)
    {
      new_y = bmp->h - plot_round ((func (x) - ymin) / y_step);
      line (bmp, i - 1, old_y, i, new_y, color);
      old_y = new_y;
    }
}

//----------------------------------------------------------------------------
// Funktion zum grafischen Iterierens einer Funktion
// Eingabe:  func = Zu iterierende Funktion
//             x0 = Startwert
//              n = Zahl der Iterationen
//          color = Zeichenfarbe
//----------------------------------------------------------------------------
void
plot::iterate (double (*func) (double), double x0, unsigned int n, int color)
{
  /*** Variables ***/
  double x_step = (xmax - xmin) / (double) bmp->w; // Pixelbreite (x)
  double y_step = (ymax - ymin) / (double) bmp->h; // Pixelbreite (y)
  double x = x0; // Aktuell berechneter x-Wert
  double y = 0; // Aktuell berechneter y-Wert
  double old_x, old_y; // Alter x- u. y-Wert
  
  unsigned int i; // Zählvariable
  
  /*** Commands ***/
  for (i = 0; i < n; i++)
    {
      old_y = y;
      y = func (x);
      fastline (bmp, plot_round ((x - xmin) / x_step),
                bmp->h - plot_round ((old_y - ymin) / y_step),
                plot_round ((x - xmin) / x_step),
                bmp->h - plot_round ((y - ymin) / y_step), color);
      if (i < n - 1)
        {
          old_x = x;
          x = y;
          fastline (bmp, plot_round ((old_x - xmin) / x_step),
                    bmp->h - plot_round ((y - ymin) / y_step),
                    plot_round ((x - xmin) / x_step),
                    bmp->h - plot_round ((y - ymin) / y_step), color);
        }
    }
}

//----------------------------------------------------------------------------
// Löscht den Plot
// Eingabe: color = Farbe
//----------------------------------------------------------------------------
void
plot::clear (int color)
{
  clear_to_color (bmp, color);
}

#endif // PLOT_H
