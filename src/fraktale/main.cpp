/***************************************************************************
 * Project: Fraktale                                                       *
 * Filename: main.cpp                                                      *
 * Begin: We Jul 06 18:04:43 CET 2005                                      *
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

#include <allegro.h>
#include <alfont.h>
#include <jpgalleg.h>

#include "include/gfxgui.h"
#include <xma.h>

/***** Defines *****/
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
BITMAP *but_normal, *but_hover, *but_selected, *but_clicked; // Std-Button-Grafiken
BITMAP *longbut_normal, *longbut_hover, *longbut_selected, *longbut_clicked; // Lange Button-Grafiken
BITMAP *edit, *small_edit; // Edit-Feld-Grafiken
BITMAP *bg; // Hintergrund
gfxgui_area *popup_back; // Fensterhintergründe

bool fullscreen;

/***** Functions *****/
void exit_error (const char *msg, const char *msg2 = NULL);
void deinit (void);

/***** Prgm-Includes *****/
#include "include/gui_alert.h"
#include "include/interface.h"
#include "include/menus.h"

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

  /* System-Start */
  set_uformat (U_ASCII);
  set_config_file (CONFIG_FILE);

  if (allegro_init () < 0)
    exit_error ("Ausnahmefehler: Allegro konnte nicht initialisiert werden.");

  set_window_title (PRGM_NAME);

  set_color_depth (32);

  fullscreen = (bool) get_config_int ("graphics", "fullscreen", 1);

  if (!fullscreen)
    {
      if (set_gfx_mode (GFX_AUTODETECT_WINDOWED, WIN_WIDTH, WIN_HEIGHT, 0, 0) < 0)
        fullscreen = true;
    }
  if (fullscreen)
    {
      if (set_gfx_mode (GFX_AUTODETECT_FULLSCREEN, WIN_WIDTH, WIN_HEIGHT, 0, 0) < 0)
        exit_error ("Ausnahmefehler: Grafik-Modus konnte nicht initialisiert werden.",
                    allegro_error);
    }

  set_config_int ("graphics", "fullscreen", (int) fullscreen);

  loading_screen = load_bitmap (LOADING_SCREEN, NULL);
  if (loading_screen == NULL)
    exit_error ("Folgende Grafikdatei konnte nicht geladen werden:", LOADING_SCREEN);
  draw_sprite (screen, loading_screen, 0, 0);

  if (install_keyboard () < 0)
    exit_error ("Ausnahmefehler: Tastatur konnte nicht initialisiert werden.");
  if (install_timer () < 0)
    exit_error ("Ausnahmefehler: Timer konnte nicht initialisiert werden.");
  if (install_mouse () < 0)
    exit_error ("Ausnahmefehler: Maus konnte nicht initialisiert werden.");
  enable_hardware_cursor ();
  
  if (install_sound (DIGI_AUTODETECT, MIDI_NONE, "") < 0)
    exit_error ("Ausnahmefehler: Sound konnte nicht initialisiert werden.");

  if (alfont_init () != ALFONT_OK)
    exit_error ("Ausnamefehler: AlFont konnte nicht initialisiert werden.");
  if (atexit (alfont_exit) != 0)
    {
      alfont_exit ();
      exit_error ("Ausnahmefehler: alfont_exit () konnte nicht als "
                    "atexit-Funktion registriert werden!");
    }

  if (atexit (deinit) != 0)
    {
      alfont_exit ();
      exit_error ("Ausnahmefehler: deinit () konnte nicht als atexit-Funktion registriert werden!");
    }

  /* JPEG-Unterstützung */
  if (jpgalleg_init () >= 0)
    jpg_support = true;

  /* Schriften */
  main_font = alfont_load_font (FNT_FILE);
  if (main_font == NULL)
    exit_error ("Schrift konnte nicht geladen werden:", FNT_FILE);

  /* XMA-Unterstützung */
  if (xma_init (FNT_FILE) < 0)
    exit_error ("XMA-Unterstützung konnte nicht initialisiert werden!");

  /* Grafiken */
  bg = load_bitmap (BG_PATH, NULL);
  if (bg == NULL)
    exit_error ("Folgende Grafikdatei konnte nicht geladen werden:", BG_PATH);

  but_normal = load_bitmap (BUT_NORMAL_PATH, NULL);
  if (but_normal == NULL)
    exit_error ("Folgende Grafikdatei konnte nicht geladen werden:", BUT_NORMAL_PATH);
  but_hover = load_bitmap (BUT_HOVER_PATH, NULL);
  if (but_hover == NULL)
    exit_error ("Folgende Grafikdatei konnte nicht geladen werden:", BUT_HOVER_PATH);
  but_selected = load_bitmap (BUT_SELECTED_PATH, NULL);
  if (but_selected == NULL)
    exit_error ("Folgende Grafikdatei konnte nicht geladen werden:", BUT_SELECTED_PATH);
  but_clicked = load_bitmap (BUT_CLICKED_PATH, NULL);
  if (but_clicked == NULL)
    exit_error ("Folgende Grafikdatei konnte nicht geladen werden:", BUT_CLICKED_PATH);

  longbut_normal = load_bitmap (LONGBUT_NORMAL_PATH, NULL);
  if (longbut_normal == NULL)
    exit_error ("Folgende Grafikdatei konnte nicht geladen werden:", LONGBUT_NORMAL_PATH);
  longbut_hover = load_bitmap (LONGBUT_HOVER_PATH, NULL);
  if (longbut_hover == NULL)
    exit_error ("Folgende Grafikdatei konnte nicht geladen werden:", LONGBUT_HOVER_PATH);
  longbut_selected = load_bitmap (LONGBUT_SELECTED_PATH, NULL);
  if (longbut_selected == NULL)
    exit_error ("Folgende Grafikdatei konnte nicht geladen werden:", LONGBUT_SELECTED_PATH);
  longbut_clicked = load_bitmap (LONGBUT_CLICKED_PATH, NULL);
  if (longbut_clicked == NULL)
    exit_error ("Folgende Grafikdatei konnte nicht geladen werden:", LONGBUT_CLICKED_PATH);

  edit = load_bitmap (EDIT_PATH, NULL);
  if (edit == NULL)
    exit_error ("Folgende Grafikdatei konnte nicht geladen werden:", EDIT_PATH);
  small_edit = load_bitmap (SMALL_EDIT_PATH, NULL);
  if (small_edit == NULL)
    exit_error ("Folgende Grafikdatei konnte nicht geladen werden:", SMALL_EDIT_PATH);

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
    }
  catch (bad_alloc)
    {
      exit_error ("Ausnahmefehler: Speicher für Daten konnte nicht belegt werden!");
    }
  catch (...)
    {
      exit_error ("Ein unvorhergesehener Fehler ist aufgetreten!");
    }
    
  try
    {
      autoinit_interface ();
    }
  catch (int i)
    {
      switch (i)
        {
          case 0:
            exit_error ("Ausnahmefehler: deinit_interface () konnte nicht"
                        " als atexit-Funktion registriert werden!");
            break;
          case 1:
            exit_error ("Einige Grafikdateien konnten nicht geladen werden!");
            break;
        }
    }
  catch (bad_alloc)
    {
      exit_error ("Ausnahmefehler: Speicher für Daten konnte nicht belegt werden!");
    }
    
  /* "No-Warranty"-Message */
  if (get_config_int ("global", "first_start", 1) == 1)
    {
      alfont_set_font_size (main_font, 36);
      alfont_textout_centre_aa_ex (screen, main_font, "Bitte eine Taste drücken...", SCREEN_W / 2,
                                   440, makecol (0, 0, 0), makeacol (0, 0, 0, 255));
      readkey ();
      set_config_int ("global", "first_start", 0);
    }

  if (!jpg_support)
    gui_alert ("Fehler: JPEG-Unterstützung konnte nicht initialisiert werden.", "OK");

  destroy_bitmap (loading_screen);

  do_main_menu ();

  deinit ();
  return EXIT_SUCCESS;
}
END_OF_MAIN ();

//----------------------------------------------------------------------------
// Gibt eine Fehlermeldung aus und beendet das Programm
//----------------------------------------------------------------------------
void
exit_error (const char *msg, const char *msg2)
{
  if (set_gfx_mode (GFX_TEXT, 0, 0, 0, 0) < 0)
    {
      cerr << "Es sind Fehler aufgetreten:" << endl;
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

  alfont_destroy_font (main_font);
  main_font = NULL;

  flush_config_file();
}
