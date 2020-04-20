/***************************************************************************
 * Project: FractalViewer                                                  *
 * Filename: gui_colsel.h                                                  *
 * Begin: Mo May 16 12:37:12 CET 2005                                      *
 * Description: Farbenwähler                                               *
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

#ifndef GUI_COLSEL_H
#define GUI_COLSEL_H 1

/***** Includes *****/
#include <cstdio>
#include <allegro.h>
#include <alfont.h>
#include "gfxgui.h"
#include <palette.h>

/***** Functions *****/
//----------------------------------------------------------------------------
// Namespace mit Zeigern auf Funktionsinterne Variablen von gui_colsel
//----------------------------------------------------------------------------
namespace gui_colsel_vars {
  char (*red)[24], (*green)[24], (*blue)[24];
  colpal *rgradient, *ggradient, *bgradient;
  vector <color_pos> *rslider, *gslider, *bslider;
  DIALOG *prev;
}

//----------------------------------------------------------------------------
// Handler für Änderungen der Werte
//----------------------------------------------------------------------------
void
gui_colsel_onChange (DIALOG *d)
{
  /*** Variables ***/
  int r, g, b; // Neue Farbwerte

  /*** Commands ***/
  if (d->proc == d_gfxgui_edit_proc)
    {
      r = atoi (*gui_colsel_vars::red);
      g = atoi (*gui_colsel_vars::green);
      b = atoi (*gui_colsel_vars::blue);

      if (r < 0)
        r = 0;
      if (g < 0)
        g = 0;
      if (b < 0)
        b = 0;

      if (r > 255)
        r = 255;
      if (g > 255)
        g = 255;
      if (b > 255)
        b = 255;
    }
  else
    {
      r = (*gui_colsel_vars::rslider)[0].pos;
      g = (*gui_colsel_vars::gslider)[0].pos;
      b = (*gui_colsel_vars::bslider)[0].pos;
    }

  sprintf (*gui_colsel_vars::red, "%i", r);
  sprintf (*gui_colsel_vars::green, "%i", g);
  sprintf (*gui_colsel_vars::blue, "%i", b);

  (*gui_colsel_vars::rslider)[0].pos = r;
  (*gui_colsel_vars::gslider)[0].pos = g;
  (*gui_colsel_vars::bslider)[0].pos = b;

  gui_colsel_vars::rgradient->bg_col = makecol (0, g, b);
  gui_colsel_vars::rgradient->colors[0].color = makecol (255, g, b);

  gui_colsel_vars::ggradient->bg_col = makecol (r, 0, b);
  gui_colsel_vars::ggradient->colors[0].color = makecol (r, 255, b);

  gui_colsel_vars::bgradient->bg_col = makecol (r, g, 0);
  gui_colsel_vars::bgradient->colors[0].color = makecol (r, g, 255);
  
  gui_colsel_vars::prev->bg = makecol (r, g, b);

  gui_colsel_vars::rgradient->calc_pal ();
  gui_colsel_vars::bgradient->calc_pal ();
  gui_colsel_vars::ggradient->calc_pal ();
  broadcast_dialog_message (MSG_DRAW, 0);
}

//----------------------------------------------------------------------------
// Anzeige eines Farbwählers
//----------------------------------------------------------------------------
int
gui_colsel (int old_col)
{
  /*** Variables ***/
  char red[24], green[24], blue[24]; // Farbwerteingabe
  int exit_status; // Durch welchen Button wurde der Dialog beendet?

  /*** GUI-Elements ***/
  gfxgui_button ok_but (but_normal, but_hover, but_selected, but_clicked, _("OK"),
                        main_font, BUT_FNT_SIZE, BUT_H_OFFSET);
  gfxgui_button cancel_but (but_normal, but_hover, but_selected, but_clicked,
                            _("Cancel"), main_font, BUT_FNT_SIZE, BUT_H_OFFSET);
                            
  gfxgui_edit rval (small_edit, _("Red: "), main_font, main_font, EDIT_FNT_SIZE,
                    red, 3, EDIT_V_BORDER, EDIT_H_BORDER, EDIT_H_BORDER,
                    EDIT_V_BORDER);
  gfxgui_edit gval (small_edit, _("Green: "), main_font, main_font, EDIT_FNT_SIZE,
                    green, 3, EDIT_V_BORDER, EDIT_H_BORDER, EDIT_H_BORDER,
                    EDIT_V_BORDER);
  gfxgui_edit bval (small_edit, _("Blue: "), main_font, main_font, EDIT_FNT_SIZE,
                    blue, 3, EDIT_V_BORDER, EDIT_H_BORDER, EDIT_H_BORDER,
                    EDIT_V_BORDER);
                    
  /* Paletten */
  colpal rgradient (makecol (0, getg (old_col), getb (old_col)));
  rgradient.insert_color (255, makecol (255, getg (old_col), getb (old_col)));
  rgradient.calc_pal ();
  vector <color_pos> rslider;
  rslider.push_back (color_pos ());
  rslider[0].pos = getr (old_col);
  rslider[0].color = makecol (255, 0, 0);
  
  colpal ggradient (makecol (getr (old_col), 0, getb (old_col)));
  ggradient.insert_color (255, makecol (getr (old_col), 255, getb (old_col)));
  ggradient.calc_pal ();
  vector <color_pos> gslider;
  gslider.push_back (color_pos ());
  gslider[0].pos = getg (old_col);
  gslider[0].color = makecol (0, 255, 0);
  
  colpal bgradient (makecol (getr (old_col), getg (old_col), 0));
  bgradient.insert_color (255, makecol (getr (old_col), getg (old_col), 255));
  bgradient.calc_pal ();
  vector <color_pos> bslider;
  bslider.push_back (color_pos ());
  bslider[0].pos = getb (old_col);
  bslider[0].color = makecol (0, 0, 255);
  
  /* Regler */
  colslider_data rdata (&rgradient, &rslider);
  colslider_data gdata (&ggradient, &gslider);
  colslider_data bdata (&bgradient, &bslider);
  
  /*** Initialisation ***/
  /* Namespace */
  gui_colsel_vars::red = &red;
  gui_colsel_vars::green = &green;
  gui_colsel_vars::blue = &blue;
  gui_colsel_vars::rgradient = &rgradient;
  gui_colsel_vars::ggradient = &ggradient;
  gui_colsel_vars::bgradient = &bgradient;
  gui_colsel_vars::rslider = &rslider;
  gui_colsel_vars::gslider = &gslider;
  gui_colsel_vars::bslider = &bslider;

  /* Weiteres */
  sprintf (red, "%i", getr (old_col));
  sprintf (green, "%i", getg (old_col));
  sprintf (blue, "%i", getb (old_col));

  alfont_set_font_size (main_font, BUT_FNT_SIZE);
  DIALOG dlg[] = {
    { d_gfxgui_area_proc, 0, 0, 355, 175, 0, 0, 0, 0, 0, 0, popup_back, NULL, NULL },
    { d_gfxgui_edit_proc, 65, 30, 0, 0, EDT_FNT_COL, EDT_FNT_COL_BG,
      0, 0, 0, 0, &rval, (void *) gui_colsel_onChange, NULL },
    { d_gfxgui_edit_proc, 65, 60, 0, 0, EDT_FNT_COL, EDT_FNT_COL_BG,
      0, 0, 0, 0, &gval, (void *) gui_colsel_onChange, NULL },
    { d_gfxgui_edit_proc, 65, 90, 0, 0, EDT_FNT_COL, EDT_FNT_COL_BG,
      0, 0, 0, 0, &bval, (void *) gui_colsel_onChange, NULL },
    { d_colslider_proc, 130, 35, 145, 20, BUT_FNT_COL_ACT, BUT_FNT_COL,
      0, 0, COLSLIDER_INSTCALC, 0, &rdata, (void *) gui_colsel_onChange, NULL },
    { d_colslider_proc, 130, 65, 145, 20, BUT_FNT_COL_ACT, BUT_FNT_COL,
      0, 0, COLSLIDER_INSTCALC, 0, &gdata, (void *) gui_colsel_onChange, NULL },
    { d_colslider_proc, 130, 95, 145, 20, BUT_FNT_COL_ACT, BUT_FNT_COL,
      0, 0, COLSLIDER_INSTCALC, 0, &bdata, (void *) gui_colsel_onChange, NULL },
    { d_box_proc, 280, 30, 50, 85, makecol (0, 0, 0), old_col, 0, 0, 0, 0,
      NULL, NULL, NULL },
    { d_gfxgui_button_proc, 75, 125, 0, 0, BUT_FNT_COL, BUT_FNT_COL_ACT,
      scancode_to_ascii (KEY_ENTER), D_EXIT, 0, 0, &ok_but, NULL, NULL },
    { d_gfxgui_button_proc, 180, 125, 0, 0, BUT_FNT_COL, BUT_FNT_COL_ACT, 0,
      D_EXIT, 0, 0, &cancel_but, NULL, NULL },
    { NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, NULL, NULL, NULL } };
    
  gui_colsel_vars::prev = &dlg[7];

  centre_dialog (dlg);
  exit_status = popup_dialog (dlg, 1);
  
  if (exit_status == 8)
    return dlg[7].bg;
  else
    return old_col;
}

#endif // GUI_COLSEL_H
