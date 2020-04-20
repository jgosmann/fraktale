/***************************************************************************
 * Project: Fraktale                                                       *
 * Filename: menus.h                                                       *
 * Begin: Th Jul 07 15:06:59 CET 2005                                      *
 * Description: Menüs des Fraktalprogramms                                 *
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

#ifndef MENUS_H
#define MENUS_H 1

#include "gfxgui.h"
#include "interface.h"

//============================================================================
// opent-content-Funktion
//============================================================================
void
open_content (DIALOG *d)
{
  load_content ((char *) d->dp2);
  draw_sprite (screen, bg, 0, 0);
  broadcast_dialog_message (MSG_DRAW, 0);
}

//============================================================================
// Grundlagenmenü
//============================================================================
void
do_grundlagen_menu (DIALOG *d)
{
  gfxgui_button ch1_but (longbut_normal, longbut_hover, longbut_selected, longbut_clicked,
                         "2.1) Iteration", main_font, LONGBUT_FNT_SIZE, BUT_H_OFFSET, BUT_V_OFFSET,
			 open_content);
  gfxgui_button ch2_but (longbut_normal, longbut_hover, longbut_selected, longbut_clicked,
                         "2.2) Grafische Iteration", main_font, LONGBUT_FNT_SIZE, BUT_H_OFFSET, BUT_V_OFFSET,
			 open_content);
  gfxgui_button ch3_but (longbut_normal, longbut_hover, longbut_selected, longbut_clicked,
                         "2.3) Fixpunkte", main_font, LONGBUT_FNT_SIZE, BUT_H_OFFSET,
			 BUT_V_OFFSET, open_content);
  gfxgui_button ch4_but (longbut_normal, longbut_hover, longbut_selected, longbut_clicked,
                         "2.4) Logistische Funktion", main_font, LONGBUT_FNT_SIZE, BUT_H_OFFSET,
			 BUT_V_OFFSET, open_content);
  gfxgui_button ch5_but (longbut_normal, longbut_hover, longbut_selected, longbut_clicked,
                         "2.5) Das Feigenbaum-Diagramm", main_font, LONGBUT_FNT_SIZE, BUT_H_OFFSET,
			 BUT_V_OFFSET, open_content);
  gfxgui_button back_but (longbut_normal, longbut_hover, longbut_selected, longbut_clicked,
                          "Zurück", main_font, LONGBUT_FNT_SIZE, BUT_H_OFFSET,
			  BUT_V_OFFSET);

  DIALOG dlg[] = {
    { d_yield_proc, 0, 0, 800, 600, 0, 0, 0, 0, 0, 0, NULL, NULL, NULL },
    { d_gfxgui_button_proc, 200, 200, 0, 0, BUT_FNT_COL, BUT_FNT_COL_ACT, 0, 0, 0, 0,
      &ch1_but, (void *) ("content" DIR_SEP "01_grundlagen" DIR_SEP "00_iteration"), NULL },
    { d_gfxgui_button_proc, 200, 250, 0, 0, BUT_FNT_COL, BUT_FNT_COL_ACT, 0, 0, 0, 0,
      &ch2_but, (void *) ("content" DIR_SEP "01_grundlagen" DIR_SEP "01_grafische_iteration"), NULL },
    { d_gfxgui_button_proc, 200, 300, 0, 0, BUT_FNT_COL, BUT_FNT_COL_ACT, 0, 0, 0, 0,
      &ch3_but, (void *) ("content" DIR_SEP "01_grundlagen" DIR_SEP "02_fixpunkte"), NULL },
    { d_gfxgui_button_proc, 200, 350, 0, 0, BUT_FNT_COL, BUT_FNT_COL_ACT, 0, 0, 0, 0,
      &ch4_but, (void *) ("content" DIR_SEP "01_grundlagen" DIR_SEP "03_logistische_funktion"), NULL },
    { d_gfxgui_button_proc, 200, 400, 0, 0, BUT_FNT_COL, BUT_FNT_COL_ACT, 0, 0, 0, 0,
      &ch5_but, (void *) ("content" DIR_SEP "01_grundlagen" DIR_SEP "04_feigenbaum"), NULL },
    { d_gfxgui_button_proc, 200, 500, 0, 0, BUT_FNT_COL, BUT_FNT_COL_ACT, 0, D_EXIT, 0, 0,
      &back_but, NULL, NULL },
    { NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, NULL, NULL, NULL } };

  draw_sprite (screen, bg, 0, 0);
  popup_dialog (dlg, -1);
  draw_sprite (screen, bg, 0, 0);
  broadcast_dialog_message (MSG_DRAW, 0);
  return;
}

//============================================================================
// Mandelbrot- und Juliamengen-Menü
//============================================================================
void
do_mandelbrot_menu (DIALOG *d)
{
  gfxgui_button ch1_but (longbut_normal, longbut_hover, longbut_selected, longbut_clicked,
                         "3.1) Komplexe Zahlen", main_font, LONGBUT_FNT_SIZE, BUT_H_OFFSET, BUT_V_OFFSET,
			 open_content);
  gfxgui_button ch2_but (longbut_normal, longbut_hover, longbut_selected, longbut_clicked,
                         "3.2) Die Mandelbrotmenge", main_font, LONGBUT_FNT_SIZE, BUT_H_OFFSET, BUT_V_OFFSET,
			 open_content);
  gfxgui_button ch3_but (longbut_normal, longbut_hover, longbut_selected, longbut_clicked,
                         "3.3) Juliamengen", main_font, LONGBUT_FNT_SIZE, BUT_H_OFFSET,
			 BUT_V_OFFSET, open_content);
  gfxgui_button ch4_but (longbut_normal, longbut_hover, longbut_selected, longbut_clicked,
                         "3.4) Weitere Mandelbrot- und Juliamengen", main_font, LONGBUT_FNT_SIZE, BUT_H_OFFSET,
			 BUT_V_OFFSET, open_content);
  gfxgui_button back_but (longbut_normal, longbut_hover, longbut_selected, longbut_clicked,
                          "Zurück", main_font, LONGBUT_FNT_SIZE, BUT_H_OFFSET,
			  BUT_V_OFFSET);

  DIALOG dlg[] = {
    { d_yield_proc, 0, 0, 800, 600, 0, 0, 0, 0, 0, 0, NULL, NULL, NULL },
    { d_gfxgui_button_proc, 200, 200, 0, 0, BUT_FNT_COL, BUT_FNT_COL_ACT, 0, 0, 0, 0,
      &ch1_but, (void *) ("content" DIR_SEP "02_mandelbrot_julia" DIR_SEP "00_komplexe_zahlen"), NULL },
    { d_gfxgui_button_proc, 200, 250, 0, 0, BUT_FNT_COL, BUT_FNT_COL_ACT, 0, 0, 0, 0,
      &ch2_but, (void *) ("content" DIR_SEP "02_mandelbrot_julia" DIR_SEP "01_mandelbrot"), NULL },
    { d_gfxgui_button_proc, 200, 300, 0, 0, BUT_FNT_COL, BUT_FNT_COL_ACT, 0, 0, 0, 0,
      &ch3_but, (void *) ("content" DIR_SEP "02_mandelbrot_julia" DIR_SEP "02_julia"), NULL },
    { d_gfxgui_button_proc, 200, 350, 0, 0, BUT_FNT_COL, BUT_FNT_COL_ACT, 0, 0, 0, 0,
      &ch4_but, (void *) ("content" DIR_SEP "02_mandelbrot_julia" DIR_SEP "03_more"), NULL },
    { d_gfxgui_button_proc, 200, 500, 0, 0, BUT_FNT_COL, BUT_FNT_COL_ACT, 0, D_EXIT, 0, 0,
      &back_but, NULL, NULL },
    { NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, NULL, NULL, NULL } };

  draw_sprite (screen, bg, 0, 0);
  popup_dialog (dlg, -1);
  draw_sprite (screen, bg, 0, 0);
  broadcast_dialog_message (MSG_DRAW, 0);
  return;
}

//============================================================================
// MoreFractals-Menü
//============================================================================
void
do_morefractals_menu (DIALOG *d)
{
  gfxgui_button ch1_but (longbut_normal, longbut_hover, longbut_selected, longbut_clicked,
                         "4.1) Die Cantormenge", main_font, LONGBUT_FNT_SIZE, BUT_H_OFFSET, BUT_V_OFFSET,
			 open_content);
  gfxgui_button ch2_but (longbut_normal, longbut_hover, longbut_selected, longbut_clicked,
                         "4.2) Die Kochkurve", main_font, LONGBUT_FNT_SIZE, BUT_H_OFFSET, BUT_V_OFFSET,
			 open_content);
  gfxgui_button ch3_but (longbut_normal, longbut_hover, longbut_selected, longbut_clicked,
                         "4.3) Das Newton-Fraktal", main_font, LONGBUT_FNT_SIZE, BUT_H_OFFSET,
			 BUT_V_OFFSET, open_content);
  gfxgui_button ch4_but (longbut_normal, longbut_hover, longbut_selected, longbut_clicked,
                         "4.4) Der Heighway-Drachen", main_font, LONGBUT_FNT_SIZE, BUT_H_OFFSET,
			 BUT_V_OFFSET, open_content);
  gfxgui_button ch5_but (longbut_normal, longbut_hover, longbut_selected, longbut_clicked,
                         "4.5) Das Sierpinski-Dreieck", main_font, LONGBUT_FNT_SIZE, BUT_H_OFFSET,
			 BUT_V_OFFSET, open_content);
  gfxgui_button ch6_but (longbut_normal, longbut_hover, longbut_selected, longbut_clicked,
                         "4.6) Mit dem Sierpinski-Dreieck verwandte Fraktale", main_font,
			 LONGBUT_FNT_SIZE, BUT_H_OFFSET, BUT_V_OFFSET, open_content);
  gfxgui_button back_but (longbut_normal, longbut_hover, longbut_selected, longbut_clicked,
                          "Zurück", main_font, LONGBUT_FNT_SIZE, BUT_H_OFFSET,
			  BUT_V_OFFSET);

  DIALOG dlg[] = {
    { d_yield_proc, 0, 0, 800, 600, 0, 0, 0, 0, 0, 0, NULL, NULL, NULL },
    { d_gfxgui_button_proc, 200, 200, 0, 0, BUT_FNT_COL, BUT_FNT_COL_ACT, 0, 0, 0, 0,
      &ch1_but, (void *) ("content" DIR_SEP "03_more_fractals" DIR_SEP "00_cantor"), NULL },
    { d_gfxgui_button_proc, 200, 250, 0, 0, BUT_FNT_COL, BUT_FNT_COL_ACT, 0, 0, 0, 0,
      &ch2_but, (void *) ("content" DIR_SEP "03_more_fractals" DIR_SEP "01_koch"), NULL },
    { d_gfxgui_button_proc, 200, 300, 0, 0, BUT_FNT_COL, BUT_FNT_COL_ACT, 0, 0, 0, 0,
      &ch3_but, (void *) ("content" DIR_SEP "03_more_fractals" DIR_SEP "02_newton_fraktal"), NULL },
    { d_gfxgui_button_proc, 200, 350, 0, 0, BUT_FNT_COL, BUT_FNT_COL_ACT, 0, 0, 0, 0,
      &ch4_but, (void *) ("content" DIR_SEP "03_more_fractals" DIR_SEP "03_heighway"), NULL },
    { d_gfxgui_button_proc, 200, 400, 0, 0, BUT_FNT_COL, BUT_FNT_COL_ACT, 0, 0, 0, 0,
      &ch5_but, (void *) ("content" DIR_SEP "03_more_fractals" DIR_SEP "04_sierpinski_dreieck"), NULL },
    { d_gfxgui_button_proc, 200, 450, 0, 0, BUT_FNT_COL, BUT_FNT_COL_ACT, 0, 0, 0, 0,
      &ch6_but, (void *) ("content" DIR_SEP "03_more_fractals" DIR_SEP "05_more_sierpinski"), NULL },
    { d_gfxgui_button_proc, 200, 500, 0, 0, BUT_FNT_COL, BUT_FNT_COL_ACT, 0, D_EXIT, 0, 0,
      &back_but, NULL, NULL },
    { NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, NULL, NULL, NULL } };

  draw_sprite (screen, bg, 0, 0);
  popup_dialog (dlg, -1);
  draw_sprite (screen, bg, 0, 0);
  broadcast_dialog_message (MSG_DRAW, 0);
  return;
}

//============================================================================
// Des Rätsels Lösung-Menü
//============================================================================
void
do_the_end_menu (DIALOG *d)
{
  gfxgui_button ch1_but (longbut_normal, longbut_hover, longbut_selected, longbut_clicked,
                         "5.1) Fraktale Dimension", main_font, LONGBUT_FNT_SIZE, BUT_H_OFFSET, BUT_V_OFFSET,
			 open_content);
  gfxgui_button ch2_but (longbut_normal, longbut_hover, longbut_selected, longbut_clicked,
                         "5.2) Und Englands Küste...?", main_font, LONGBUT_FNT_SIZE, BUT_H_OFFSET, BUT_V_OFFSET,
			 open_content);
  gfxgui_button back_but (longbut_normal, longbut_hover, longbut_selected, longbut_clicked,
                          "Zurück", main_font, LONGBUT_FNT_SIZE, BUT_H_OFFSET,
			  BUT_V_OFFSET);

  DIALOG dlg[] = {
    { d_yield_proc, 0, 0, 800, 600, 0, 0, 0, 0, 0, 0, NULL, NULL, NULL },
    { d_gfxgui_button_proc, 200, 200, 0, 0, BUT_FNT_COL, BUT_FNT_COL_ACT, 0, 0, 0, 0,
      &ch1_but, (void *) ("content" DIR_SEP "04_ende" DIR_SEP "00_fraktale_dimension"), NULL },
    { d_gfxgui_button_proc, 200, 250, 0, 0, BUT_FNT_COL, BUT_FNT_COL_ACT, 0, 0, 0, 0,
      &ch2_but, (void *) ("content" DIR_SEP "04_ende" DIR_SEP "01_englands_kueste"), NULL },
    { d_gfxgui_button_proc, 200, 500, 0, 0, BUT_FNT_COL, BUT_FNT_COL_ACT, 0, D_EXIT, 0, 0,
      &back_but, NULL, NULL },
    { NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, NULL, NULL, NULL } };

  draw_sprite (screen, bg, 0, 0);
  popup_dialog (dlg, -1);
  draw_sprite (screen, bg, 0, 0);
  broadcast_dialog_message (MSG_DRAW, 0);
  return;
}

//============================================================================
// Kapitelmenü
//============================================================================
void
do_chapter_menu (DIALOG *d)
{
  gfxgui_button ch1_but (longbut_normal, longbut_hover, longbut_selected, longbut_clicked,
                         "1) Einleitung", main_font, LONGBUT_FNT_SIZE, BUT_H_OFFSET, BUT_V_OFFSET,
			 open_content);
  gfxgui_button ch2_but (longbut_normal, longbut_hover, longbut_selected, longbut_clicked,
                         "2) Grundlagen", main_font, LONGBUT_FNT_SIZE, BUT_H_OFFSET, BUT_V_OFFSET,
			 do_grundlagen_menu);
  gfxgui_button ch3_but (longbut_normal, longbut_hover, longbut_selected, longbut_clicked,
                         "3) Mandelbrot- und Juliamengen", main_font, LONGBUT_FNT_SIZE, BUT_H_OFFSET,
			 BUT_V_OFFSET, do_mandelbrot_menu);
  gfxgui_button ch4_but (longbut_normal, longbut_hover, longbut_selected, longbut_clicked,
                         "4) Noch mehr Fraktale...", main_font, LONGBUT_FNT_SIZE, BUT_H_OFFSET,
			 BUT_V_OFFSET, do_morefractals_menu);
  gfxgui_button ch5_but (longbut_normal, longbut_hover, longbut_selected, longbut_clicked,
                         "5) Des Rätsels Lösung", main_font, LONGBUT_FNT_SIZE, BUT_H_OFFSET,
			 BUT_V_OFFSET, do_the_end_menu);
  gfxgui_button back_but (longbut_normal, longbut_hover, longbut_selected, longbut_clicked,
                          "Zurück", main_font, LONGBUT_FNT_SIZE, BUT_H_OFFSET,
			  BUT_V_OFFSET);

  DIALOG dlg[] = {
    { d_yield_proc, 0, 0, 800, 600, 0, 0, 0, 0, 0, 0, NULL, NULL, NULL },
    { d_gfxgui_button_proc, 200, 200, 0, 0, BUT_FNT_COL, BUT_FNT_COL_ACT, 0, 0, 0, 0,
      &ch1_but, (void *) ("content" DIR_SEP "00_einleitung"), NULL },
    { d_gfxgui_button_proc, 200, 250, 0, 0, BUT_FNT_COL, BUT_FNT_COL_ACT, 0, 0, 0, 0,
      &ch2_but, NULL, NULL },
    { d_gfxgui_button_proc, 200, 300, 0, 0, BUT_FNT_COL, BUT_FNT_COL_ACT, 0, 0, 0, 0,
      &ch3_but, NULL, NULL },
    { d_gfxgui_button_proc, 200, 350, 0, 0, BUT_FNT_COL, BUT_FNT_COL_ACT, 0, 0, 0, 0,
      &ch4_but, NULL, NULL },
    { d_gfxgui_button_proc, 200, 400, 0, 0, BUT_FNT_COL, BUT_FNT_COL_ACT, 0, 0, 0, 0,
      &ch5_but, NULL, NULL },
    { d_gfxgui_button_proc, 200, 500, 0, 0, BUT_FNT_COL, BUT_FNT_COL_ACT, 0, D_EXIT, 0, 0,
      &back_but, NULL, NULL },
    { NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, NULL, NULL, NULL } };

  draw_sprite (screen, bg, 0, 0);
  popup_dialog (dlg, -1);
  draw_sprite (screen, bg, 0, 0);
  broadcast_dialog_message (MSG_DRAW, 0);
  return;
}

//============================================================================
// Main-Menu
//============================================================================
//----------------------------------------------------------------------------
// Callbacks
//----------------------------------------------------------------------------
void
show_credits (DIALOG *d)
{
  xma_anim *animation = load_xma ("credits" DIR_SEP "credits.xma");
  play_xma (animation, screen, 0, 0);
  while (!key[KEY_ESC] && xma::playing)
    render_xma ();
  stop_xma ();
  destroy_xma (animation);

  clear_keybuf ();
  draw_sprite (screen, bg, 0, 0);
  broadcast_dialog_message (MSG_DRAW, 0);
  return;
}

void
toggle_fullscreen (DIALOG *d)
{
  fullscreen = !fullscreen;

  if (!fullscreen)
    {
      if (set_gfx_mode (GFX_AUTODETECT_WINDOWED, WIN_WIDTH, WIN_HEIGHT, 0, 0) < 0)
        fullscreen = true;
      else
        d->flags ^= D_SELECTED;
    }
  if (fullscreen)
    {
      if (set_gfx_mode (GFX_AUTODETECT_FULLSCREEN, WIN_WIDTH, WIN_HEIGHT, 0, 0) < 0)
        exit_error ("Ausnahmefehler: Grafik-Modus konnte nicht initialisiert werden.",
                    allegro_error);
      d->flags |= D_SELECTED;
    }

  set_config_int ("graphics", "fullscreen", (int) fullscreen);

  draw_sprite (screen, bg, 0, 0);
  broadcast_dialog_message (MSG_DRAW, 0);
  show_mouse (screen);
}

//----------------------------------------------------------------------------
// Menü-Aufruf
//----------------------------------------------------------------------------
void
do_main_menu (void)
{
  gfxgui_button chapter_but (longbut_normal, longbut_hover, longbut_selected, longbut_clicked,
                             "Kapitelauswahl", main_font, LONGBUT_FNT_SIZE, BUT_H_OFFSET, BUT_V_OFFSET,
			     do_chapter_menu);
  gfxgui_button credits_but (longbut_normal, longbut_hover, longbut_selected, longbut_clicked,
                             "Credits", main_font, LONGBUT_FNT_SIZE, BUT_H_OFFSET, BUT_V_OFFSET,
			     show_credits);
  gfxgui_button fullscr_but (longbut_normal, longbut_hover, longbut_selected, longbut_clicked,
                             "Vollbildmodus", main_font, LONGBUT_FNT_SIZE, BUT_H_OFFSET, BUT_V_OFFSET,
			     toggle_fullscreen);
  gfxgui_button exit_but (longbut_normal, longbut_hover, longbut_selected, longbut_clicked,
                          "Beenden", main_font, LONGBUT_FNT_SIZE, BUT_H_OFFSET, BUT_V_OFFSET);

  DIALOG dlg[] = {
    { d_yield_proc, 0, 0, 800, 600, 0, 0, 0, 0, 0, 0, NULL, NULL, NULL },
    { d_gfxgui_button_proc, 200, 200, 0, 0, BUT_FNT_COL, BUT_FNT_COL_ACT, 0, 0, 0, 0,
      &chapter_but, NULL, NULL },
    { d_gfxgui_button_proc, 200, 250, 0, 0, BUT_FNT_COL, BUT_FNT_COL_ACT, 0, 0, 0, 0,
      &credits_but, NULL, NULL },
    { d_gfxgui_button_proc, 200, 300, 0, 0, BUT_FNT_COL, BUT_FNT_COL_ACT, 0,
      (get_config_int ("graphics", "fullscreen", 1) == 1) ? D_SELECTED : 0, 0, 0,
      &fullscr_but, NULL, NULL },
    { d_gfxgui_button_proc, 200, 500, 0, 0, BUT_FNT_COL, BUT_FNT_COL_ACT, 0, D_EXIT, 0, 0,
      &exit_but, NULL, NULL },
    { NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, NULL, NULL, NULL } };

  draw_sprite (screen, bg, 0, 0);
  popup_dialog (dlg, -1);
}

#endif
