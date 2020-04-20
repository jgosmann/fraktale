/***************************************************************************
 * Project: FractalViewer                                                  *
 * Filename: main.cpp                                                      *
 * Begin: Su Apr 17 09:11:29 CET 2005                                      *
 * Description: Programm zur Berechnung von Fraktalen                      *
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

/***** Includes *****/
#include "settings.h"

#include <iostream>
#include <string>
#include <cstring>
#include <clocale>

#include <alFileDlg.h> // alFileDlg.h inkludiert allegro.h
#include <alfont.h>
#include <jpgalleg.h>

#include <libintl.h> // Erst nach Allegro inkludieren

#ifdef __WIN32__
 #include <winalleg.h>
#endif

#define FRACTAL_USE_XMA 1
#define XMA_DIR_SEP "/"
#include <fractal.h>
#include "include/gfxgui.h"
#include <palette.h>

/***** Defines *****/
#define LOAD_JULIA D_FRAC_ST_USER
#define UNLOAD_JULIA D_FRAC_ST_USER + 1

/* Gettext */
#ifndef _
 #define _(string) gettext (string)
#endif

/* Rückgabewerte */
#ifndef EXIT_SUCCESS
 #define EXIT_SUCCESS 0
#endif
#ifndef EXIT_FAILURE
 #define EXIT_FAILURE 1
#endif

/* Newline-Char, Dir-Seperator */
#ifdef __WIN32__
 #define NL "\r\n"
 #define DIR_SEP "\\"
#else
 #define NL "\n"
 #define DIR_SEP "/"
#endif

/***** Misc *****/
using namespace std;

/***** Variables ***/
ALFONT_FONT *main_font; // Hauptschrift
colpal *colors; // Farbpalette
undo_fractal *main_fractal, *sec_fractal; // Klassen für Fraktale
fractal *preview_fractal; // Vorschau für Julia-Menge
undo_fractal *shown_frac; // Aktuell gezeigtes Fraktal
d_fractal_proc_com *main_fractal_com, *sec_fractal_com; // Für Ret-Info von d_fractal_proc
BITMAP *but_normal, *but_hover, *but_selected, *but_clicked; // Std-Button-Grafiken
BITMAP *edit, *small_edit; // Edit-Feld-Grafiken

gfxgui_area *popup_back; // Fensterhintergründe

/***** Functions *****/
void loader (unsigned int act, unsigned int max);
void exit_without_error (void);
void exit_error (const char *msg, const char *msg2 = NULL);
void deinit (void);

/***** Prgm-Includes *****/
#include "include/gui_main.h"

//----------------------------------------------------------------------------
// main ()
//----------------------------------------------------------------------------
int
main (int argc, char *argv[])
{
  /*** Variables ***/
  bool jpg_support = false; // Ist JPG-Support geladen worden?
  BITMAP *loading_screen; // Lade Bildschirm

  /*** Commands ***/
  /** Initialisation **/
  /* Localisation */
  setlocale (LC_ALL, "");
  bindtextdomain (LOCALE_NAME, "." DIR_SEP "locale" DIR_SEP);
  textdomain (LOCALE_NAME);

  /* System-Start */
  set_uformat (U_ASCII);
  set_config_file (CONFIG_FILE);

  if (allegro_init () < 0)
    exit_error (_("Fatal error: Allegro could not be initialised!"));

  set_window_title (PRGM_NAME);

  set_color_depth (32);

  if (set_gfx_mode (GFX_AUTODETECT_WINDOWED, WIN_WIDTH, WIN_HEIGHT, 0, 0) < 0)
    {
      if (set_gfx_mode (GFX_AUTODETECT_FULLSCREEN, WIN_WIDTH, WIN_HEIGHT, 0, 0) < 0)
        exit_error (_("Fatal error: Graphic mode could not be initialised!"),
                    allegro_error);
    }

  loading_screen = load_bitmap (_("load_en.tga"), NULL);
  if (loading_screen == NULL)
    exit_error (_("Following graphics file could not be loaded:"), _("load_en.tga"));
  draw_sprite (screen, loading_screen, 0, 0);

  if (install_keyboard () < 0)
    exit_error (_("Fatal error: Keyboard could not be initialised!"));
  if (install_timer () < 0)
    exit_error (_("Fatal error: Timer could not be initialised!"));
  if (install_mouse () < 0)
    exit_error (_("Fatal error: Mouse could not be initialised!"));

  if (alfont_init () != ALFONT_OK)
    exit_error (_("Fatal error: Alfont could not be initialised!"));
  if (atexit (alfont_exit) != 0)
    {
      alfont_exit ();
      exit_error (_("Fatal error: alfont_exit () could not be registred as "
                    "atexit-function!"));
    }

  if (atexit (deinit) != 0)
    {
      alfont_exit ();
      exit_error (_("Fatal error: deinit () could not be registred as atexit-function!"));
    }

  /* JPEG-Unterstützung */
  if (jpgalleg_init () >= 0)
    jpg_support = true;

  /* Schriften */
  main_font = alfont_load_font (FNT_FILE);
  if (main_font == NULL)
    exit_error (_("Font could not be loaded:"), FNT_FILE);

  /* XMA-Unterstützung */
  #ifdef FRACTAL_USE_XMA
    if (xma_init (FNT_FILE) < 0)
      exit_error ("XMA support could not be initialised!");
  #endif
    
  /* Grafiken */
  but_normal = load_bitmap (BUT_NORMAL_PATH, NULL);
  if (but_normal == NULL)
    exit_error (_("Following graphics file could not be loaded:"), BUT_NORMAL_PATH);
  but_hover = load_bitmap (BUT_HOVER_PATH, NULL);
  if (but_hover == NULL)
    exit_error (_("Following graphics file could not be loaded:"), BUT_HOVER_PATH);
  but_selected = load_bitmap (BUT_SELECTED_PATH, NULL);
  if (but_selected == NULL)
    exit_error (_("Following graphics file could not be loaded:"), BUT_SELECTED_PATH);
  but_clicked = load_bitmap (BUT_CLICKED_PATH, NULL);
  if (but_clicked == NULL)
    exit_error (_("Following graphics file could not be loaded:"), BUT_CLICKED_PATH);

  edit = load_bitmap (EDIT_PATH, NULL);
  if (edit == NULL)
    exit_error (_("Following graphics file could not be loaded:"), EDIT_PATH);
  small_edit = load_bitmap (SMALL_EDIT_PATH, NULL);
  if (small_edit == NULL)
    exit_error (_("Following graphics file could not be loaded:"), SMALL_EDIT_PATH);

  /* Fraktale, GUI-Elements */
  try
    {
      popup_back = new gfxgui_area ();
      popup_back->left = load_bitmap (POPUPBACK_LEFT_PATH, NULL);
      popup_back->right = load_bitmap (POPUPBACK_RIGHT_PATH, NULL);
      popup_back->top = load_bitmap (POPUPBACK_TOP_PATH, NULL);
      popup_back->bottom = load_bitmap (POPUPBACK_BOTTOM_PATH, NULL);
      popup_back->left_top = load_bitmap (POPUPBACK_LEFT_TOP_PATH, NULL);
      popup_back->right_top = load_bitmap (POPUPBACK_RIGHT_TOP_PATH, NULL);
      popup_back->left_bottom = load_bitmap (POPUPBACK_LEFT_BOTTOM_PATH, NULL);
      popup_back->right_bottom = load_bitmap (POPUPBACK_RIGHT_BOTTOM_PATH, NULL);
      popup_back->middle = load_bitmap (POPUPBACK_MIDDLE_PATH, NULL);

      colors = new colpal (makecol (0, 0, 0));
      colors->insert_color (0, makecol (255, 0, 0));
      colors->insert_color (42, makecol (255, 255, 0));
      colors->insert_color (84, makecol (0, 255, 0));
      colors->insert_color (126, makecol (0, 255, 255));
      colors->insert_color (168, makecol (0, 0, 255));
      colors->insert_color (210, makecol (255, 0, 254));
      colors->insert_color (255, makecol (255, 0, 0));
      colors->calc_pal ();
      
      main_fractal = new undo_fractal (FRAC_WIDTH, FRAC_HEIGHT, FRAC_XMIN, FRAC_XMAX,
                                       FRAC_YMIN, 0, 0, 2, 160, colors->bg_col,
                                       colors->pal, COL_PER_PAL, 64, mandelbrot_x);
      preview_fractal = new fractal (PREV_FRAC_WIDTH, PREV_FRAC_HEIGHT,
                                     FRAC_XMIN, FRAC_XMAX, FRAC_YMIN, 0, 0, 2,
                                     40, colors->bg_col, colors->pal, COL_PER_PAL,
                                     64, julia_x);
      sec_fractal = new undo_fractal (FRAC_WIDTH, FRAC_HEIGHT, FRAC_XMIN, FRAC_XMAX,
                                      FRAC_YMIN, 0, 0, 2, 160, colors->bg_col,
                                      colors->pal, COL_PER_PAL, 64, julia_x);

      main_fractal_com = new d_fractal_proc_com (loader);
      sec_fractal_com = new d_fractal_proc_com (loader);
      
      gui_main_load ();
    }
  catch (bad_alloc)
    {
      exit_error (_("Fatal error: Memory for data could not be allocated!"));
    }
  catch (...)
    {
      exit_error (_("An unexpected error occured!"));
    }
    
#ifndef __WIN32__
  set_close_button_callback (exit_without_error);
#endif
    
  /* "No-Warranty"-Message */
  if (get_config_int ("global", "first_start", 1) == 1)
    {
      alfont_set_font_size (main_font, 36);
      alfont_textout_centre_aa_ex(screen, main_font, _("Press any key..."), SCREEN_W / 2,
                               440, makecol (0, 0, 0), makeacol (0, 0, 0, 255));
      readkey ();
      set_config_int ("global", "first_start", 0);
    }

  if (!jpg_support)
    gui_alert (_("Error: JPEG support could not be initialised."), _("OK"));

  destroy_bitmap (loading_screen);

  while (1) // Hauptschleife
    { 
      shown_frac = (undo_fractal *) gui_main[GUI_MAIN_FRACTAL].dp;
      do_dialog (gui_main, -1);

      if (main_fractal_com->status == D_FRAC_ST_JULIA)
        {
          sec_fractal->reinit (FRAC_XMIN, FRAC_XMAX, FRAC_YMIN, main_fractal_com->a,
                               main_fractal_com->b);
          gui_main[GUI_MAIN_FRACTAL].dp = (void *) sec_fractal;
          gui_main[GUI_MAIN_FRACTAL].d1 = D_FRAC_ACT_ZOOMIN;
          gui_main[GUI_MAIN_BUT_ZOOMIN].flags |= D_SELECTED;
          gui_main[GUI_MAIN_PREVFR].flags |= D_HIDDEN;
          gui_main[GUI_MAIN_BUT_JULIA].flags |= D_HIDDEN;
          gui_main[GUI_MAIN_BUT_JULIA].flags &= ~D_SELECTED;
          gui_main_but_exit->label = _("Back");
        }
      else if (main_fractal_com->status == LOAD_JULIA)
        {
          sec_fractal->render (loader);
          gui_main[GUI_MAIN_FRACTAL].dp = (void *) sec_fractal;
          gui_main[GUI_MAIN_FRACTAL].d1 = D_FRAC_ACT_ZOOMIN;
          gui_main[GUI_MAIN_BUT_ZOOMIN].flags |= D_SELECTED;
          gui_main[GUI_MAIN_PREVFR].flags |= D_HIDDEN;
          gui_main[GUI_MAIN_BUT_JULIA].flags |= D_HIDDEN;
          gui_main[GUI_MAIN_BUT_JULIA].flags &= ~D_SELECTED;
          gui_main_but_exit->label = _("Back");
        }
      else if (main_fractal_com->status == UNLOAD_JULIA
               || shown_frac->iter_func == julia_x)
        {
          main_fractal->render (loader);
          gui_main[GUI_MAIN_FRACTAL].dp = (void *) main_fractal;
          gui_main[GUI_MAIN_BUT_JULIA].flags &= ~D_HIDDEN;
          gui_main_but_exit->label = _("Exit");
        }
      else
        break;
    }

  exit_without_error ();
  return EXIT_SUCCESS; // Not reached
}
END_OF_MAIN ();

//----------------------------------------------------------------------------
// Zeigt den Ladeprozess für eine Fraktalberechnung an
//----------------------------------------------------------------------------
void
loader (unsigned int act, unsigned int max)
{
  alfont_set_font_size (main_font, 20);
  alfont_textprintf_aa_ex(screen, main_font, 5, WIN_HEIGHT - 5
                          - alfont_text_height (main_font), makecol (0, 0, 0),
                          BG_COL, "%s %0.0f%%     ", _("Calculating..."),
                          ((double) act / (double) max) * 100.0);
}

//----------------------------------------------------------------------------
// Beendet das Programm
//----------------------------------------------------------------------------
void
exit_without_error (void)
{
  gui_main_unload ();
  deinit ();
  exit (EXIT_SUCCESS);
}

//----------------------------------------------------------------------------
// Gibt eine Fehlermeldung aus und beendet das Programm
//----------------------------------------------------------------------------
void
exit_error (const char *msg, const char *msg2)
{
  if (set_gfx_mode (GFX_TEXT, 0, 0, 0, 0) < 0)
    {
      cerr << _("Some errors occured:") << endl;
      cerr << msg << endl << msg2 << endl << allegro_error << endl << endl;
      exit (EXIT_FAILURE);
    }

  if (msg2 == NULL)
    allegro_message (msg);
  else
    allegro_message ("%s%s%s", msg, NL, msg2);
  exit (EXIT_FAILURE);
}

//----------------------------------------------------------------------------
// Deinitialiersiert das Programm
//----------------------------------------------------------------------------
void
deinit (void)
{
  if (main_fractal != NULL)
    { delete main_fractal; main_fractal = NULL; }
  if (preview_fractal != NULL)
    { delete preview_fractal; preview_fractal = NULL; }
  if (sec_fractal != NULL)
    { delete sec_fractal; sec_fractal = NULL; }
    
  if (but_normal != NULL)
    { destroy_bitmap (but_normal); but_normal = NULL; }
  if (but_hover != NULL)
    { destroy_bitmap (but_hover); but_hover = NULL; }
  if (but_selected != NULL)
    { destroy_bitmap (but_selected); but_selected = NULL; }
  if (but_clicked != NULL)
    { destroy_bitmap (but_clicked); but_clicked = NULL; }

  if (edit != NULL)
    { destroy_bitmap (edit); edit = NULL; }
  if (small_edit != NULL)
    { destroy_bitmap (small_edit); small_edit = NULL; }

  if (popup_back != NULL)
    { delete popup_back; popup_back = NULL; }

  if (colors != NULL)
    { delete colors; colors = NULL; }

  alfont_destroy_font (main_font);
  main_font = NULL;
  
  flush_config_file();
}
