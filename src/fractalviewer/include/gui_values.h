/***************************************************************************
 * Project: FractalViewer                                                  *
 * Filename: gui_values.h                                                  *
 * Begin: We May 04 15:23:13 CET 2005                                      *
 * Description: Dialog für Parametereingabe                                *
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

#ifndef GUI_VALUES_H
#define GUI_VALUES_H 1

/***** Includes *****/
#include <cstdio>
#include <fractal.h>
#include "gfxgui.h"

#include "gui_alert.h"

/***** Defines *****/
#define GUI_VALUES_ELEMENTS 15 // Zahl der (max.) GUI-Elemente

#define GUI_VALUES_EDIT_MAXC 31 // Zahl der maximalen Zeichen in edit-Feldern
#define GUI_VALUES_EDIT_SIZE (GUI_VALUES_EDIT_MAXC + 1) * 6 // Minimale Größen des
                                                 // char-Arrays für ein edit-Feld

/* Positionen wichtiger Elemente */
#define GUI_VALUES_FIRST_EDIT 1
#define GUI_VALUES_FIRST_BUT 8

/***** Variables *****/
DIALOG gui_values[GUI_VALUES_ELEMENTS]; // The gui itself
bool gui_values_loaded = false; // Wurde die GUI initialisiert?
undo_fractal *gui_values_edited_fractal;

/* Eingabewerte */
char mina[GUI_VALUES_EDIT_SIZE], maxa[GUI_VALUES_EDIT_SIZE], minb[GUI_VALUES_EDIT_SIZE],
     maxb[GUI_VALUES_EDIT_SIZE];
char const1[GUI_VALUES_EDIT_SIZE], const2[GUI_VALUES_EDIT_SIZE];
char iters[GUI_VALUES_EDIT_SIZE];

/*** Objects ***/
gfxgui_button *gui_values_but_ok;
gfxgui_button *gui_values_but_cancel;
gfxgui_button *gui_values_but_startvalues;

gfxgui_button *gui_values_but_mandelbrot;
gfxgui_button *gui_values_but_newton;
gfxgui_button *gui_values_but_spider;

gfxgui_edit *gui_values_edit_mina;
gfxgui_edit *gui_values_edit_maxa;
gfxgui_edit *gui_values_edit_minb;
gfxgui_edit *gui_values_edit_maxb;

gfxgui_edit *gui_values_edit_const1;
gfxgui_edit *gui_values_edit_const2;

gfxgui_edit *gui_values_edit_iter;

/***** Functions *****/
void gui_values_unload (void);
void gui_values_load (void);
void gui_values_init (undo_fractal *init_for_frac, bool reinit = false);

//============================================================================
// Misc-Functions
//============================================================================
//----------------------------------------------------------------------------
// Überprüft eine Eingabe
// Eingabe:         input = Zu prüfende Eingabe
//          should_be_int = Wenn true wird geprüft, ob int, sonst ob double
// Rückgabe: true, bei korrekter Eingabe
//----------------------------------------------------------------------------
bool
check_input (char *input, bool should_be_int = false)
{
  unsigned int i; // Zählvariable
  
  if (should_be_int && atoi (input) == 0)
    {
      for (i = 0; input[i] != '\0'; i++)
        {
          if (input[i] !=  '0')
            return false;
        }
    }
  else if (!should_be_int && atof (input) == 0)
    {
      for (i = 0; input[i] != '\0'; i++)
        {
          if (input[i] !=  '0' && input[i] !=  '.')
            return false;
        }
    }
    
  return true;
}

//============================================================================
// Callback-Functions
//============================================================================
void
gui_values_but_ok_click (DIALOG *d)
{
  if (!check_input (mina))
    gui_alert (_("Min. a contains a forbidden value!"), _("OK"));
  else if (!check_input (maxa))
    gui_alert (_("Max. a contains a forbidden value!"), _("OK"));
  else if (!check_input (minb))
    gui_alert (_("Min. b contains a forbidden value!"), _("OK"));

  else if (atoi (const1) < 2 && gui_values_edited_fractal->iter_func == mandelbrot_x)
    gui_alert (_("Exponent contains a forbidden value!"), _("OK"));
  else if (!check_input (const1) && gui_values_edited_fractal->iter_func == julia_x)
    gui_alert (_("c (real) contains a forbidden value!"), _("OK"));
  else if (!check_input (const2) && gui_values_edited_fractal->iter_func == julia_x)
    gui_alert (_("c (imag) contains a forbidden value!"), _("OK"));
  else if (atoi (iters) < 1)
    gui_alert (_("Iterations contains a forbidden value!"), _("OK"));

  else
    {
      gui_values_edited_fractal->min_x = atof (mina);
      gui_values_edited_fractal->max_x = atof (maxa);
      gui_values_edited_fractal->min_y = atof (minb);
      gui_values_edited_fractal->max_y = atof (maxb);
      gui_values_edited_fractal->iterations = atoi (iters);
      if (gui_values_edited_fractal->iter_func == mandelbrot_x)
        {
          main_fractal->c = atof (const1);
          sec_fractal->c = atof (const1);
          preview_fractal->c = atof (const1);
        }
      if (gui_values_edited_fractal->iter_func == julia_x)
        {
          gui_values_edited_fractal->a = atof (const1);
          gui_values_edited_fractal->b = atof (const2);
        }
      gui_values_edited_fractal->render (loader);
      gui_values_edited_fractal->deactivate_undo ();
      d->flags |= D_EXIT;
    }
}

void
gui_values_but_mandelbrot_click (DIALOG *d)
{
  unsigned int i;
  gui_values_edited_fractal->iter_func = mandelbrot_x;
  broadcast_dialog_message (MSG_RADIO, d->d1);
  gui_values_init (gui_values_edited_fractal, true);
  gui_values[0].d1 = 1;
  for (i = GUI_VALUES_FIRST_EDIT; i < GUI_VALUES_FIRST_EDIT + 7; i++)
    gui_values[i].d2 = 1;
  broadcast_dialog_message (MSG_DRAW, 0);
}

void
gui_values_but_newton_click (DIALOG *d)
{
  unsigned int i;
  gui_values_edited_fractal->iter_func = newton;
  broadcast_dialog_message (MSG_RADIO, d->d1);
  gui_values_init (gui_values_edited_fractal, true);
  gui_values[0].d1 = 1;
  for (i = GUI_VALUES_FIRST_EDIT; i < GUI_VALUES_FIRST_EDIT + 7; i++)
    gui_values[i].d2 = 1;
  broadcast_dialog_message (MSG_DRAW, 0);
}

void
gui_values_but_spider_click (DIALOG *d)
{
  unsigned int i;
  gui_values_edited_fractal->iter_func = spider;
  broadcast_dialog_message (MSG_RADIO, d->d1);
  gui_values_init (gui_values_edited_fractal, true);
  gui_values[0].d1 = 1;
  for (i = GUI_VALUES_FIRST_EDIT; i < GUI_VALUES_FIRST_EDIT + 7; i++)
    gui_values[i].d2 = 1;
  broadcast_dialog_message (MSG_DRAW, 0);
}

void
gui_values_but_startvalues_click (DIALOG *d)
{
  sprintf (mina, "%0.16f", FRAC_XMIN);
  sprintf (maxa, "%0.16f", FRAC_XMAX);
  sprintf (minb, "%0.16f", FRAC_YMIN);
  sprintf (maxb, "%0.16f", FRAC_YMIN + ((double) gui_values_edited_fractal->h
                                        / gui_values_edited_fractal->w)
                           * (FRAC_XMAX - FRAC_XMIN));
  sprintf (iters, "%0i", FRAC_STD_ITER);
  broadcast_dialog_message (MSG_DRAW, 0);
}

void
gui_values_edit_minb_lostfocus (DIALOG *d)
{
  sprintf (maxb, "%0.16f", atof (minb) + ((double) gui_values_edited_fractal->h
                                          / gui_values_edited_fractal->w)
                           * (atof (maxa) - atof (mina)));
  object_message (&gui_values[GUI_VALUES_FIRST_EDIT + 3], MSG_DRAW, 0);
}

//============================================================================
// load-Function, unload-Function, init-Function
//============================================================================
void
gui_values_unload (void)
{
  if (gui_values_loaded)
    {
      if (gui_values_but_ok != NULL)
        delete gui_values_but_ok;
      if (gui_values_but_cancel != NULL)
        delete gui_values_but_cancel;
      if (gui_values_but_startvalues != NULL)
        delete gui_values_but_startvalues;
        
      if (gui_values_but_mandelbrot != NULL)
        delete gui_values_but_mandelbrot;
      if (gui_values_but_newton != NULL)
        delete gui_values_but_newton;
      if (gui_values_but_spider != NULL)
        delete gui_values_but_spider;
        
      if (gui_values_edit_mina != NULL)
        delete gui_values_edit_mina;
      if (gui_values_edit_maxa != NULL)
        delete gui_values_edit_maxa;
      if (gui_values_edit_minb != NULL)
        delete gui_values_edit_minb;
      if (gui_values_edit_maxb != NULL)
        delete gui_values_edit_maxb;

      if (gui_values_edit_const1 != NULL)
        delete gui_values_edit_const1;
      if (gui_values_edit_const2 != NULL)
        delete gui_values_edit_const2;
        
      if (gui_values_edit_iter != NULL)
        delete gui_values_edit_iter;
    }
  return;
}

//----------------------------------------------------------------------------
void
gui_values_load (void)
{
  if (!gui_values_loaded)
    {
      atexit (gui_values_unload);

      gui_values_but_ok = new gfxgui_button (but_normal, but_hover, but_selected,
                                             but_clicked, _("OK"), main_font,
                                             BUT_FNT_SIZE, BUT_H_OFFSET,
                                             gui_values_but_ok_click);
      gui_values_but_cancel = new gfxgui_button (but_normal, but_hover, but_selected,
                                                 but_clicked, _("Cancel"), main_font,
                                                 BUT_FNT_SIZE, BUT_H_OFFSET);
      gui_values_but_startvalues = new gfxgui_button (but_normal, but_hover,
                                                      but_selected, but_clicked,
                                                      _("Start values"), main_font,
                                                      BUT_FNT_SIZE, BUT_H_OFFSET,
                                                      gui_values_but_startvalues_click);

      gui_values_but_mandelbrot = new gfxgui_button (but_normal, but_hover,
                                                     but_selected, but_clicked,
                                                     _("Mandelbrot"), main_font,
                                                     BUT_FNT_SIZE, BUT_H_OFFSET,
                                                     gui_values_but_mandelbrot_click);
      gui_values_but_newton = new gfxgui_button (but_normal, but_hover, but_selected,
                                                 but_clicked, _("Newton"), main_font,
                                                 BUT_FNT_SIZE, BUT_H_OFFSET,
                                                 gui_values_but_newton_click);
      gui_values_but_spider = new gfxgui_button (but_normal, but_hover, but_selected,
                                                 but_clicked, _("Spider"), main_font,
                                                 BUT_FNT_SIZE, BUT_H_OFFSET,
                                                 gui_values_but_spider_click);

      gui_values_edit_mina = new gfxgui_edit (edit, _("Min. a: "), main_font,
                                              main_font, EDIT_FNT_SIZE, mina,
                                              GUI_VALUES_EDIT_MAXC,
                                              EDIT_V_BORDER, EDIT_H_BORDER,
                                              EDIT_H_BORDER, EDIT_V_BORDER);
      gui_values_edit_maxa = new gfxgui_edit (edit, _("Max. a: "), main_font,
                                              main_font, EDIT_FNT_SIZE, maxa,
                                              GUI_VALUES_EDIT_MAXC,
                                              EDIT_V_BORDER, EDIT_H_BORDER,
                                              EDIT_H_BORDER, EDIT_V_BORDER);
      gui_values_edit_minb = new gfxgui_edit (edit, _("Min. b: "), main_font,
                                              main_font, EDIT_FNT_SIZE, minb,
                                              GUI_VALUES_EDIT_MAXC,
                                              EDIT_V_BORDER, EDIT_H_BORDER,
                                              EDIT_H_BORDER, EDIT_V_BORDER);
      gui_values_edit_maxb = new gfxgui_edit (edit, _("Max. b: "), main_font,
                                              main_font, EDIT_FNT_SIZE, maxb,
                                              GUI_VALUES_EDIT_MAXC,
                                              EDIT_V_BORDER, EDIT_H_BORDER,
                                              EDIT_H_BORDER, EDIT_V_BORDER);

      gui_values_edit_const1 = new gfxgui_edit (edit, _("Const 1: "), main_font,
                                                main_font, EDIT_FNT_SIZE, const1,
                                                GUI_VALUES_EDIT_MAXC,
                                                EDIT_V_BORDER, EDIT_H_BORDER,
                                                EDIT_H_BORDER, EDIT_V_BORDER);
      gui_values_edit_const2 = new gfxgui_edit (edit, _("Const 2: "), main_font,
                                                main_font, EDIT_FNT_SIZE, const2,
                                                GUI_VALUES_EDIT_MAXC,
                                                EDIT_V_BORDER, EDIT_H_BORDER,
                                                EDIT_H_BORDER, EDIT_V_BORDER);

      gui_values_edit_iter = new gfxgui_edit (edit, _("Iterations: "), main_font,
                                              main_font, EDIT_FNT_SIZE, iters,
                                              GUI_VALUES_EDIT_MAXC,
                                              EDIT_V_BORDER, EDIT_H_BORDER,
                                              EDIT_H_BORDER, EDIT_V_BORDER);

      DIALOG tmp[] = {
        { d_gfxgui_area_proc, 0, 0, 370, 335, 0, 0, 0, 0, 0, 0, popup_back,
          NULL, NULL },
        { d_gfxgui_edit_proc, 125, 25, 0, 0, EDT_FNT_COL, EDT_FNT_COL_BG,
          0, 0, 0, 0, gui_values_edit_mina, NULL, NULL },
        { d_gfxgui_edit_proc, 125, 55, 0, 0, EDT_FNT_COL, EDT_FNT_COL_BG,
          0, 0, 0, 0, gui_values_edit_maxa, NULL, NULL },
        { d_gfxgui_edit_proc, 125, 85, 0, 0, EDT_FNT_COL, EDT_FNT_COL_BG,
          0, 0, 0, 0, gui_values_edit_minb, (void *) gui_values_edit_minb_lostfocus, NULL },
        { d_gfxgui_edit_proc, 125, 115, 0, 0, EDT_FNT_COL, EDT_FNT_COL_BG,
          0, D_DISABLED, 0, 0, gui_values_edit_maxb, NULL, NULL },
        { d_gfxgui_edit_proc, 125, 145, 0, 0, EDT_FNT_COL, EDT_FNT_COL_BG,
          0, 0, 0, 0, gui_values_edit_const1, NULL, NULL },
        { d_gfxgui_edit_proc, 125, 175, 0, 0, EDT_FNT_COL, EDT_FNT_COL_BG,
          0, 0, 0, 0, gui_values_edit_const2, NULL, NULL },
        { d_gfxgui_edit_proc, 125, 205, 0, 0, EDT_FNT_COL, EDT_FNT_COL_BG,
          0, 0, 0, 0, gui_values_edit_iter, NULL, NULL },
        { d_gfxgui_button_proc, 40, 245, 0, 0, BUT_FNT_COL, BUT_FNT_COL_ACT,
          0, 0, 1, 0, gui_values_but_mandelbrot, NULL, NULL },
        { d_gfxgui_button_proc, 140, 245, 0, 0, BUT_FNT_COL, BUT_FNT_COL_ACT,
          0, 0, 1, 0, gui_values_but_newton, NULL, NULL },
        { d_gfxgui_button_proc, 240, 245, 0, 0, BUT_FNT_COL, BUT_FNT_COL_ACT,
          0, 0, 1, 0, gui_values_but_spider, NULL, NULL },
        { d_gfxgui_button_proc, 40, 285, 0, 0, BUT_FNT_COL, BUT_FNT_COL_ACT,
          scancode_to_ascii (KEY_ENTER), 0, 0, 0, gui_values_but_ok, NULL, NULL },
        { d_gfxgui_button_proc, 140, 285, 0, 0, BUT_FNT_COL, BUT_FNT_COL_ACT,
          0, D_EXIT, 0, 0, gui_values_but_cancel, NULL, NULL },
        { d_gfxgui_button_proc, 240, 285, 0, 0, BUT_FNT_COL, BUT_FNT_COL_ACT,
          0, 0, 0, 0, gui_values_but_startvalues, NULL, NULL },
        { NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, NULL, NULL, NULL } };

      unsigned int i;
      for (i = 0; i < GUI_VALUES_ELEMENTS; i++)
        gui_values[i] = tmp[i];
       
      centre_dialog (gui_values);
    }
  return;
}

//----------------------------------------------------------------------------
void
gui_values_init (undo_fractal *init_for_frac, bool reinit)
{
  if (!reinit)
    {
      sprintf (mina, "%0.16f", init_for_frac->min_x);
      sprintf (maxa, "%0.16f", init_for_frac->max_x);
      sprintf (minb, "%0.16f", init_for_frac->min_y);
      sprintf (maxb, "%0.16f", init_for_frac->max_y);
      sprintf (iters, "%0i", init_for_frac->iterations);
      gui_values_edited_fractal = init_for_frac;
    }
  
  gui_values[GUI_VALUES_FIRST_BUT + 3].flags &= ~D_EXIT;

  if (init_for_frac->iter_func == mandelbrot_x)
    {
      gui_values[GUI_VALUES_FIRST_BUT + 0].flags |= D_SELECTED;
      gui_values[GUI_VALUES_FIRST_BUT + 1].flags &= ~D_SELECTED;
      gui_values[GUI_VALUES_FIRST_BUT + 2].flags &= ~D_SELECTED;

      gui_values[GUI_VALUES_FIRST_BUT + 0].flags &= ~D_HIDDEN;
      gui_values[GUI_VALUES_FIRST_BUT + 1].flags &= ~D_HIDDEN;
      gui_values[GUI_VALUES_FIRST_BUT + 2].flags &= ~D_HIDDEN;

      gui_values[GUI_VALUES_FIRST_EDIT + 4].flags &= ~D_HIDDEN;
      gui_values[GUI_VALUES_FIRST_EDIT + 5].flags |= D_HIDDEN;

      ((gfxgui_edit *) gui_values[GUI_VALUES_FIRST_EDIT + 4].dp)->label = _("Exponent: ");
        sprintf (const1, "%0.0f", init_for_frac->c);
    }
  if (init_for_frac->iter_func == julia_x)
    {
      gui_values[GUI_VALUES_FIRST_BUT + 0].flags |= D_HIDDEN;
      gui_values[GUI_VALUES_FIRST_BUT + 1].flags |= D_HIDDEN;
      gui_values[GUI_VALUES_FIRST_BUT + 2].flags |= D_HIDDEN;

      gui_values[GUI_VALUES_FIRST_EDIT + 4].flags &= ~D_HIDDEN;
      gui_values[GUI_VALUES_FIRST_EDIT + 5].flags &= ~D_HIDDEN;

      ((gfxgui_edit *) gui_values[GUI_VALUES_FIRST_EDIT + 4].dp)->label = _("c (real): ");
        sprintf (const1, "%0.16f", init_for_frac->a);
      ((gfxgui_edit *) gui_values[GUI_VALUES_FIRST_EDIT + 5].dp)->label = _("c (imag): ");
        sprintf (const2, "%0.16f", init_for_frac->b);
    }
  if (init_for_frac->iter_func == newton)
    {
      gui_values[GUI_VALUES_FIRST_BUT + 0].flags &= ~D_SELECTED;
      gui_values[GUI_VALUES_FIRST_BUT + 1].flags |= D_SELECTED;
      gui_values[GUI_VALUES_FIRST_BUT + 2].flags &= ~D_SELECTED;

      gui_values[GUI_VALUES_FIRST_BUT + 0].flags &= ~D_HIDDEN;
      gui_values[GUI_VALUES_FIRST_BUT + 1].flags &= ~D_HIDDEN;
      gui_values[GUI_VALUES_FIRST_BUT + 2].flags &= ~D_HIDDEN;

      gui_values[GUI_VALUES_FIRST_EDIT + 4].flags |= D_HIDDEN;
      gui_values[GUI_VALUES_FIRST_EDIT + 5].flags |= D_HIDDEN;
    }
  if (init_for_frac->iter_func == spider)
    {
      gui_values[GUI_VALUES_FIRST_BUT + 0].flags &= ~D_SELECTED;
      gui_values[GUI_VALUES_FIRST_BUT + 1].flags &= ~D_SELECTED;
      gui_values[GUI_VALUES_FIRST_BUT + 2].flags |= D_SELECTED;

      gui_values[GUI_VALUES_FIRST_BUT + 0].flags &= ~D_HIDDEN;
      gui_values[GUI_VALUES_FIRST_BUT + 1].flags &= ~D_HIDDEN;
      gui_values[GUI_VALUES_FIRST_BUT + 2].flags &= ~D_HIDDEN;

      gui_values[GUI_VALUES_FIRST_EDIT + 4].flags |= D_HIDDEN;
      gui_values[GUI_VALUES_FIRST_EDIT + 5].flags |= D_HIDDEN;
    }
  return;
}

#endif // ifndef GUI_VALUES_H
