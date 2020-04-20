/***************************************************************************
 * Project: Fraktale                                                       *
 * Filename: interface.h                                                   *
 * Begin: Th Jul 07 15:06:59 CET 2005                                      *
 * Description: Interface des Fraktalprogramms                             *
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

#ifndef INTERFACE_H
#define INTERFACE_H 1

/***** Includes *****/
#include <cstdio>
#include <cstring>

#include <allegro.h>
#include <jpgalleg.h>
#include <almp3.h>

#include "gfxgui.h"
#include <xma.h>
#include <plot.h>

/***** Defines *****/
#define INTERFACE_NONE 0
#define INTERFACE_BACK 1
#define INTERFACE_NEXT 2
#define INTERFACE_SOUND 3
#define INTERFACE_MOREINFO 4
#define INTERFACE_QUIT 5

/***** Misc *****/
using namespace std;

/***** Variables *****/
namespace interface_gui {
  BITMAP *back_normal, *back_hover, *back_clicked;
  BITMAP *next_normal, *next_hover, *next_clicked;
  BITMAP *sound_normal, *sound_hover, *sound_clicked;
  BITMAP *moreinfo_normal, *moreinfo_hover, *moreinfo_clicked;
  BITMAP *quit_normal, *quit_hover, *quit_clicked;
  BITMAP *checkbox_normal, *checkbox_hover, *checkbox_clicked, *checkbox_selected;
  BITMAP *radio_normal, *radio_hover, *radio_clicked, *radio_selected;
  BITMAP *edit_small, *edit_large;
  gfxgui_button *back, *next, *sound, *moreinfo, *quit;
  
  vector <gfxgui_button *> buttons; // Klassen-Instanzen für Buttons
  vector <int> buttons_dlg_pos; // Postion der Buttons im DIALOG-Array
  vector <bool> buttons_right; // Ist der Button eine richtige Antwort?
  vector <gfxgui_edit *> edit_fields; // Klassen-Instanzen für Edit-Felder
  vector <char *> edit_input; // Eingabe-Buffer für Edit-Felder
  vector <int> edit_dlg_pos; // Position der Edit-Felder im DIALOG-Array
  vector <const char *> edit_right; // Richtige Antwort für Edit-Felder
  
  xma_anim *animation = NULL; // XMA-Animation
  int animation_x, animation_y; // x-, y-Koordinate der Animation
  vector <gfxgui_button *> xma_buttons; // Kontrollschaltflächen

  /* Für grafische Iteration */
  plot *plot_area = NULL;
  double my;
  int iter;
  int plot_x, plot_y;
  bool have_lower, have_greater;
  double lowert, greatert;
  gfxgui_edit *plot_edt = NULL;
  char *plot_edt_buf = NULL;
  int plot_edt_dlg_pos;
  gfxgui_button *plot_but = NULL;
  int plot_but_dlg_pos;

  DIALOG *fin_dialog; // Der finale Dialog
}

namespace interface_sound {
  char sound_filename[16];
  FILE *sound_file;  // Sound-Datei-Stream
  ALMP3_MP3STREAM *sound_stream; // ALMP3-Sound-Stream
  const unsigned int sound_buffer_size = 32768; // Buffergröße
  char sound_buffer[sound_buffer_size]; // Sound-Buffer
  unsigned int mp3poll_dlg_obj; // Objekt für mp3-polling
  bool playing = false; // Wird gerade eine Sound-Datei wiedergegeben?
}

/***** Functions *****/
//============================================================================
// Iterate-Callbacks
//============================================================================
double
arc_divide (double x)
{
  return x;
}

double
logistic (double x)
{
  return interface_gui::my * x * (1 - x);
}

//============================================================================
// GUI-Objekte
//============================================================================
//----------------------------------------------------------------------------
// Objekt für mp3-Wiedergabe (polling)
//----------------------------------------------------------------------------
int
d_mp3poll_proc (int msg, DIALOG *d, int c)
{
  bool stop_playing = false;
  using namespace interface_sound;
  unsigned int read;
  switch (msg)
    {
      case MSG_IDLE:
        if (playing)
          {
            if ((read = fread(sound_buffer, 1, sound_buffer_size, sound_file))
                < sound_buffer_size)
              {
                if (feof (sound_file) == 0)
                  stop_playing = true;
              }
            if (almp3_poll_mp3stream (sound_stream) != ALMP3_OK)
              stop_playing = true;
          }
        break;
        
      case MSG_END:
        if (playing)
          stop_playing = true;
        break;
    }
    
  if (stop_playing)
    {
      playing = false;
      almp3_stop_mp3stream (sound_stream);
      almp3_destroy_mp3stream (sound_stream);
      fclose (sound_file);
    }
    
  return D_O_K;
}

//----------------------------------------------------------------------------
// Objekt für Animationswiedergabe
//----------------------------------------------------------------------------
int
d_renderxma_proc (int msg, DIALOG *d, int c)
{
  using namespace interface_gui;

  if (msg == MSG_IDLE && xma::playing)
    {
      render_xma ();
    }
  else if (msg == MSG_DRAW && !xma::playing)
    rectfill (gui_get_screen (), animation_x, animation_y, animation_x + animation->w - 1,
              animation_y + animation->h - 1, makecol (0, 0, 0));

  return D_O_K;
}

//----------------------------------------------------------------------------
// Objekt für die Iteration
//----------------------------------------------------------------------------
int
d_iter_proc (int msg, DIALOG *d, int c)
{
  using namespace interface_gui;
  if (msg == MSG_DRAW && plot_area != NULL)
    {
      plot_area->clear ();
      plot_area->graph (arc_divide);
      plot_area->graph (logistic, makecol (0, 0, 255));
      plot_area->draw_axes ();
      draw_sprite (gui_get_screen (), plot_area->bmp, plot_x, plot_y);
    }

  return D_O_K;
}

//============================================================================
// GUI-Callbacks
//============================================================================
void
iterate (DIALOG *d)
{
  using namespace interface_gui;

  if ((have_lower && lowert >= atof (plot_edt_buf)) || (have_greater && greatert <= atof (plot_edt_buf)))
    {
      gui_alert ("Bitte gib einen anderen Wert ein!", "OK");
      return;
    }

  plot_area->clear ();
  plot_area->graph (arc_divide);
  plot_area->graph (logistic, makecol (0, 0, 255));
  plot_area->draw_axes ();
  plot_area->iterate (logistic, atof (plot_edt_buf), iter, makecol (255, 0, 0));
  draw_sprite (gui_get_screen (), plot_area->bmp, plot_x, plot_y);
}

void
interface_sound_but_click (DIALOG *d)
{
  using namespace interface_sound;

  if (playing)
    {
      playing = false;
      almp3_stop_mp3stream (sound_stream);
      almp3_destroy_mp3stream (sound_stream);
      fclose (sound_file);
    }

  sound_file = fopen (sound_filename, "rb");
  if (sound_file == NULL)
    {
      gui_alert ("Die Sounddatei konnte nicht geöffnet werden!", "OK");
      return;
    }
  if (fread (sound_buffer, 1, sound_buffer_size, sound_file) < sound_buffer_size)
    {
      if (feof (sound_file) != 0)
        {
          gui_alert ("Fehler beim Laden der Sounddatei!", "OK");
          fclose (sound_file);
          return;
        }
    }
  sound_stream = almp3_create_mp3stream (sound_buffer, sound_buffer_size,
                                         (feof (sound_file) == 0) ? FALSE : TRUE);
   if (sound_stream == NULL)
     {
       gui_alert ("Stream für den Sound konnte nicht erstellt werden!", "OK");
       fclose (sound_file);
       return;
     }
   if (almp3_play_mp3stream(sound_stream, sound_buffer_size, 255, 127) != ALMP3_OK)
     {
       gui_alert ("Sounddatei konnte nicht abgespielt werden!", "OK");
       fclose (sound_file);
       return;
     }

   playing = true;
}

void
interface_chkbox_click (DIALOG *d)
{
  d->flags ^= D_SELECTED;
}

void
interface_radio_click (DIALOG *d)
{
  broadcast_dialog_message (MSG_RADIO, d->d1);
  d->flags |= D_SELECTED;
}

void
interface_next_but_click (DIALOG *d)
{
  unsigned int i; // Zählvariable
  bool all_right = true; // Ist alles richtig?
  bool xma_was_playing = false; // Wurde eine xma-Animation gespielt?
  
  using namespace interface_gui;
  using namespace interface_sound;

  if (playing)
    {
      playing = false;
      almp3_stop_mp3stream (sound_stream);
      almp3_destroy_mp3stream (sound_stream);
      fclose (sound_file);
    }

  for (i = 0; i < buttons_dlg_pos.size () && all_right; i++)
    {
      if (((fin_dialog[buttons_dlg_pos[i]].flags &= D_SELECTED) == D_SELECTED)
          != buttons_right[i])
        all_right = false;
    }
  for (i = 0; i < edit_dlg_pos.size () && all_right; i++)
    {
      if (ustrcmp (edit_input[i], edit_right[i]) != 0)
        all_right = false;
    }

  if (xma::playing)
    {
      pause_xma ();
      xma_was_playing = true;
    }

  if (all_right)
    d->flags |= D_EXIT;
  else
    gui_alert ("Leider ist deine Antwort falsch.", "OK");

  if (xma_was_playing)
    play_xma (animation, gui_get_screen (), animation_x, animation_y);
}

void
interface_playxma_but_click (DIALOG *d)
{
  using namespace interface_gui;
  play_xma (animation, gui_get_screen (), animation_x, animation_y);
}

void
interface_pausexma_but_click (DIALOG *d)
{
  pause_xma ();
}

void
interface_stopxma_but_click (DIALOG *d)
{
  using namespace interface_gui;
  stop_xma ();
  rectfill (gui_get_screen (), animation_x, animation_y, animation_x + animation->w - 1,
            animation_y + animation->h - 1, makecol (0, 0, 0));
}

//============================================================================
// Allgemeine Funktionen
//============================================================================
//----------------------------------------------------------------------------
// Deinit
//----------------------------------------------------------------------------
void
deinit_interface (void)
{
  using namespace interface_gui;

  if (back != NULL)
    { delete back; back = NULL; }
  if (next != NULL)
    { delete next; next = NULL; }
  if (sound != NULL)
    { delete sound; sound = NULL; }
  if (moreinfo != NULL)
    { delete moreinfo; moreinfo = NULL; }
  if (quit != NULL)
    { delete quit; quit = NULL; }
  if (plot_area != NULL)
    { delete plot_area; plot_area = NULL; }

  if (plot_area != NULL)
    { delete plot_edt; plot_area = NULL; }
  if (plot_edt != NULL)
    { delete plot_edt; plot_edt = NULL; }
  if (plot_edt_buf != NULL)
    { delete plot_edt_buf; plot_edt_buf = NULL; }
  if (plot_but != NULL)
    { delete plot_but; plot_but = NULL; }

  destroy_bitmap (back_normal);
  destroy_bitmap (back_hover);
  destroy_bitmap (back_clicked);
  
  destroy_bitmap (next_normal);
  destroy_bitmap (next_hover);
  destroy_bitmap (next_clicked);
  
  destroy_bitmap (sound_normal);
  destroy_bitmap (sound_hover);
  destroy_bitmap (sound_clicked);
  
  destroy_bitmap (moreinfo_normal);
  destroy_bitmap (moreinfo_hover);
  destroy_bitmap (moreinfo_clicked);
  
  destroy_bitmap (quit_normal);
  destroy_bitmap (quit_hover);
  destroy_bitmap (quit_clicked);
  
  destroy_bitmap (checkbox_normal);
  destroy_bitmap (checkbox_hover);
  destroy_bitmap (checkbox_clicked);
  destroy_bitmap (checkbox_selected);
  
  destroy_bitmap (radio_normal);
  destroy_bitmap (radio_hover);
  destroy_bitmap (radio_clicked);
  destroy_bitmap (radio_selected);
  
  destroy_bitmap (edit_small);
  destroy_bitmap (edit_large);
  
  back_normal = NULL;
  back_hover = NULL;
  back_clicked = NULL;
  
  next_normal = NULL;
  next_hover = NULL;
  next_clicked = NULL;
  
  sound_normal = NULL;
  sound_hover = NULL;
  sound_clicked = NULL;
  
  moreinfo_normal = NULL;
  moreinfo_hover = NULL;
  moreinfo_clicked = NULL;
  
  quit_normal = NULL;
  quit_hover = NULL;
  quit_clicked = NULL;
  
  checkbox_normal = NULL;
  checkbox_hover = NULL;
  checkbox_clicked = NULL;
  checkbox_selected = NULL;
  
  radio_normal = NULL;
  radio_hover = NULL;
  radio_clicked = NULL;
  radio_selected = NULL;
}

//----------------------------------------------------------------------------
// Autoinit
//----------------------------------------------------------------------------
void
autoinit_interface (void)
{
  using namespace interface_gui;
  
  if (atexit (deinit_interface) != 0)
    throw 0;
  
  back_normal = load_bitmap ("back_normal.tga", NULL);
  back_hover = load_bitmap ("back_hover.tga", NULL);
  back_clicked = load_bitmap ("back_clicked.tga", NULL);
  
  next_normal = load_bitmap ("next_normal.tga", NULL);
  next_hover = load_bitmap ("next_hover.tga", NULL);
  next_clicked = load_bitmap ("next_clicked.tga", NULL);
  
  sound_normal = load_bitmap ("sound_normal.tga", NULL);
  sound_hover = load_bitmap ("sound_hover.tga", NULL);
  sound_clicked = load_bitmap ("sound_clicked.tga", NULL);
  
  moreinfo_normal = load_bitmap ("moreinfo_normal.tga", NULL);
  moreinfo_hover = load_bitmap ("moreinfo_hover.tga", NULL);
  moreinfo_clicked = load_bitmap ("moreinfo_clicked.tga", NULL);
  
  quit_normal = load_bitmap ("quit_normal.tga", NULL);
  quit_hover = load_bitmap ("quit_hover.tga", NULL);
  quit_clicked = load_bitmap ("quit_clicked.tga", NULL);
  
  checkbox_normal = load_bitmap ("checkbox_normal.tga", NULL);
  checkbox_hover = load_bitmap ("checkbox_hover.tga", NULL);
  checkbox_clicked = load_bitmap ("checkbox_clicked.tga", NULL);
  checkbox_selected = load_bitmap ("checkbox_selected.tga", NULL);
  
  radio_normal = load_bitmap ("radio_normal.tga", NULL);
  radio_hover = load_bitmap ("radio_hover.tga", NULL);
  radio_clicked = load_bitmap ("radio_clicked.tga", NULL);
  radio_selected = load_bitmap ("radio_selected.tga", NULL);
  
  edit_small = load_bitmap ("edit_small.tga", NULL);
  edit_large = load_bitmap ("edit_large.tga", NULL);
  
  if (back_normal == NULL || back_hover == NULL || back_clicked == NULL
      || next_normal == NULL || next_hover == NULL || next_clicked == NULL
      || sound_normal == NULL || sound_hover == NULL || sound_clicked == NULL
      || moreinfo_normal == NULL || moreinfo_hover == NULL || moreinfo_clicked == NULL
      || quit_normal == NULL || quit_hover == NULL || quit_clicked == NULL)
    throw 1;

  back = new gfxgui_button (back_normal, back_hover, back_normal, back_clicked,
                            "", main_font, BUT_FNT_SIZE, BUT_V_OFFSET, BUT_H_OFFSET);
  next = new gfxgui_button (next_normal, next_hover, next_normal, next_clicked,
                            "", main_font, BUT_FNT_SIZE, BUT_V_OFFSET, BUT_H_OFFSET,
                            interface_next_but_click);
  sound = new gfxgui_button (sound_normal, sound_hover, sound_normal, sound_clicked,
                             "", main_font, BUT_FNT_SIZE, BUT_V_OFFSET, BUT_H_OFFSET,
                             interface_sound_but_click);
  moreinfo = new gfxgui_button (moreinfo_normal, moreinfo_hover, moreinfo_normal,
                                moreinfo_clicked, "", main_font, BUT_FNT_SIZE,
                                BUT_V_OFFSET, BUT_H_OFFSET);
  quit = new gfxgui_button (quit_normal, quit_hover, quit_normal, quit_clicked,
                            "", main_font, BUT_FNT_SIZE, BUT_V_OFFSET, BUT_H_OFFSET);
}

//----------------------------------------------------------------------------
// Schreibt Werte in ein DIALOG-Struct
// Eingabe: Entspricht den Werten des DIALOG-Structs, dlg ist das Struct, in
//          das geschrieben wird.
//----------------------------------------------------------------------------
void
write_dlg (DIALOG &dlg, int (*proc) (int, DIALOG *, int), int x, int y,
           int w, int h, int fg, int bg, int key, int flags, int d1, int d2,
           void *dp, void *dp2, void * dp3)
{
  dlg.proc = proc;
  dlg.x = x;
  dlg.y = y;
  dlg.w = w;
  dlg.h = h;
  dlg.fg = fg;
  dlg.bg = bg;
  dlg.key = key;
  dlg.flags = flags;
  dlg.d1 = d1;
  dlg.d2 = d2;
  dlg.dp = dp;
  dlg.dp2 = dp2;
  dlg.dp3 = dp3;
}

//----------------------------------------------------------------------------
// Kopiert ein DIALOG-Struct
// Eingabe: to ist das Struct, in das geschrieben wird und from aus dem
//          gelesen wird.
//----------------------------------------------------------------------------
void
copy_dlg (DIALOG &to, DIALOG &from)
{
  to.proc = from.proc;
  to.x = from.x;
  to.y = from.y;
  to.w = from.w;
  to.h = from.h;
  to.fg = from.fg;
  to.bg = from.bg;
  to.key = from.key;
  to.flags = from.flags;
  to.d1 = from.d1;
  to.d2 = from.d2;
  to.dp = from.dp;
  to.dp2 = from.dp2;
  to.dp3 = from.dp3;
}

//----------------------------------------------------------------------------
// Setzt alle wichtigen Interface-Variablen zurück
// Eingabe: entspricht den lokalen Variablen der load_content-Funktion
//----------------------------------------------------------------------------
void
interface_clean (TiXmlDocument *xml_tree, BITMAP *img)
{
  unsigned int i;

  using namespace interface_gui;

  for (i = 0; i < buttons.size (); i++)
    delete buttons[i];
  buttons.clear ();
  for (i = 0; i < edit_fields.size (); i++)
    delete edit_fields[i];
  edit_fields.clear ();
  for (i = 0; i < edit_input.size (); i++)
    delete edit_input[i];
  edit_input.clear ();
  for (i = 0; i < xma_buttons.size (); i++)
    delete xma_buttons[i];
  xma_buttons.clear ();
  buttons_dlg_pos.clear ();
  buttons_right.clear ();
  edit_fields.clear ();
  edit_input.clear ();
  edit_dlg_pos.clear ();
  edit_right.clear ();
  if (plot_area != NULL)
    { delete plot_area; plot_area = NULL; }
  if (plot_edt != NULL)
    { delete plot_edt; plot_edt = NULL; }
  if (plot_edt_buf != NULL)
    { delete plot_edt_buf; plot_edt_buf = NULL; }
  if (plot_but != NULL)
    { delete plot_but; plot_but = NULL; }
  if (xml_tree != NULL)
    {
      xml_tree->Clear ();
      delete xml_tree;
    }
  delete fin_dialog;
  destroy_bitmap (img);
  stop_xma ();
  destroy_xma (animation);
  animation = NULL;
}

//----------------------------------------------------------------------------
// Ordner-Inhalt wiedergeben sowie Interface generieren
// Eingabe:  dir = Ordner
//          page = Seite, mit der begonnen werden soll, -1 für auto
// Rückgabe: Seite, bei der beendet wurde; -1, wenn Ende erreicht
//----------------------------------------------------------------------------
int
load_content (const char *dir, int page = -1)
{
  /*** Variables ***/
  char img_filename[strlen (dir) + 32];
  char buffer[strlen (dir) + 32];
  int clicked_button;
  int i = page; // Zählvariable
  
  using namespace interface_gui;
  using namespace interface_sound;
  
  /*** Commands ***/
  if (page == -1)
    i = get_config_int ("interface", dir, 0);

  for (sprintf (img_filename, "%s%s%i%s", dir, DIR_SEP, i, ".jpg");
       file_exists (img_filename, -1, NULL) != 0;
       sprintf (img_filename, "%s%s%i%s", dir, DIR_SEP, i, ".jpg"))
    {
      /** Variables **/
      BITMAP *img = load_bitmap (img_filename, NULL); // Hauptbild
      
      TiXmlDocument *xml_tree = NULL; // Aufbau der Datei aus der geladen wird
      TiXmlElement *root, *act_edited; // Root-Element, aktuell und vorhergehend
                                       // editiertes Element

      DIALOG creation_dialog; // Zur Erstellung eines Dialogelementes
      vector <DIALOG> pre_dialog; // Der Dialog
      
      unsigned int j; // Zählvariable

      /** Commands **/
      /* Dialog erstellen*/
      write_dlg (creation_dialog, d_bitmap_proc, 0, 0, WIN_WIDTH, WIN_HEIGHT, 0,
                 0, 0, 0, 0, 0, img, NULL, NULL);
      pre_dialog.push_back (creation_dialog);
      write_dlg (creation_dialog, d_iter_proc, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, NULL, NULL, NULL);
      pre_dialog.push_back (creation_dialog);
      
      if (i > 0) // Back-Button
        {
          write_dlg (creation_dialog, d_gfxgui_button_proc, 0,
                     WIN_HEIGHT - back_normal->h, 0, 0, BUT_FNT_COL, BUT_FNT_COL_ACT,
                     0, D_EXIT, 0, INTERFACE_BACK, back, NULL, NULL);
          pre_dialog.push_back (creation_dialog);
        }

      sprintf (buffer, "%s%s%i%s", dir, DIR_SEP, i + 1, ".jpg");
      if (file_exists (buffer, -1, NULL) != 0) // Next- u. Quit- u. MoreInfo-Button
        {
          write_dlg (creation_dialog, d_gfxgui_button_proc, WIN_WIDTH - next_normal->w,
                     WIN_HEIGHT - next_normal->h, 0, 0, BUT_FNT_COL, BUT_FNT_COL_ACT,
                     0, 0, 0, INTERFACE_NEXT, next, NULL, NULL);
          pre_dialog.push_back (creation_dialog);
          write_dlg (creation_dialog, d_gfxgui_button_proc, WIN_WIDTH - quit_normal->w
                     - next_normal->w,  WIN_HEIGHT - quit_normal->h,
                     0, 0, BUT_FNT_COL, BUT_FNT_COL_ACT, scancode_to_ascii (KEY_ESC),
                     D_EXIT, 0, INTERFACE_QUIT, quit, NULL, NULL);
          pre_dialog.push_back (creation_dialog);
        }
      else
        {

          write_dlg (creation_dialog, d_gfxgui_button_proc, WIN_WIDTH - quit_normal->w,
                     WIN_HEIGHT - quit_normal->h, 0, 0, BUT_FNT_COL, BUT_FNT_COL_ACT,
                     scancode_to_ascii (KEY_ESC), D_EXIT, 0, INTERFACE_QUIT,
                     quit, NULL, NULL);
          pre_dialog.push_back (creation_dialog);

	  sprintf (buffer, "%s%smoreinfo", dir, DIR_SEP);
          if (file_exists (buffer, -1, NULL) != 0) // MoreInfo-Button
            {
              write_dlg (creation_dialog, d_gfxgui_button_proc, WIN_WIDTH
                         - moreinfo_normal->w - quit_normal->w, WIN_HEIGHT
                         - moreinfo_normal->h, 0, 0, BUT_FNT_COL, BUT_FNT_COL_ACT,
                         0, D_EXIT, 0, INTERFACE_MOREINFO, moreinfo, NULL, NULL);
              pre_dialog.push_back (creation_dialog);
            }
        }

      sprintf (sound_filename, "%s%s%i%s", dir, DIR_SEP, i, ".mp3");
      if (file_exists (sound_filename, -1, NULL) != 0) // Sound-Button
        {
          write_dlg (creation_dialog, d_gfxgui_button_proc,
                     (WIN_WIDTH - sound_normal->w) / 2,
                     WIN_HEIGHT - sound_normal->h, 0, 0, BUT_FNT_COL,
                     BUT_FNT_COL_ACT, 0, 0, 0, INTERFACE_SOUND, sound, NULL, NULL);
          pre_dialog.push_back (creation_dialog);
          write_dlg (creation_dialog, d_mp3poll_proc, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                     NULL, NULL, NULL);
          mp3poll_dlg_obj = pre_dialog.size ();
          pre_dialog.push_back (creation_dialog);
        }

      /* GUI-File einlesen */
      sprintf (buffer, "%s%s%i%s", dir, DIR_SEP, i, ".gui");
      if (file_exists (buffer, -1, NULL) != 0)
        {
          xml_tree = new TiXmlDocument (buffer);
          xml_tree->LoadFile ();
          root = xml_tree->RootElement ();
          if (root != NULL)
            {
              if (strcmp (root->Value (), "gui") == 0)
                {
                  for (act_edited = root->FirstChildElement (); act_edited != NULL;
                       act_edited = act_edited->NextSiblingElement ())
                    {
                      if (strcmp (act_edited->Value(), "iter") == 0)
                        {
			  double xmin, xmax, ymin, ymax;
			  int xres, yres;

			  if (act_edited->QueryDoubleAttribute ("xmin", &xmin)
                              != TIXML_SUCCESS)
                            xmin = 0;
			  if (act_edited->QueryDoubleAttribute ("xmax", &xmax)
                              != TIXML_SUCCESS)
                            xmax = 1;
                          if (act_edited->QueryDoubleAttribute ("xmin", &ymin)
                              != TIXML_SUCCESS)
                            xmin = 0;
			  if (act_edited->QueryDoubleAttribute ("xmax", &ymax)
                              != TIXML_SUCCESS)
                            xmax = 1;
			  if (act_edited->QueryIntAttribute ("xres", &xres)
                              != TIXML_SUCCESS)
                            xres = 0;
			  if (act_edited->QueryIntAttribute ("yres", &yres)
                              != TIXML_SUCCESS)
                            yres = 0;
			  if (act_edited->QueryDoubleAttribute ("my", &my)
                              != TIXML_SUCCESS)
                            my = 1;
			  if (act_edited->QueryIntAttribute ("n", &iter)
                              != TIXML_SUCCESS)
                            iter = 50;
			  if (act_edited->QueryDoubleAttribute ("lower", &lowert)
                              == TIXML_SUCCESS)
                            have_lower = true;
			  else
			    have_lower = false;
			  if (act_edited->QueryDoubleAttribute ("greater", &greatert)
                              == TIXML_SUCCESS)
                            have_greater = true;
			  else
			    have_greater = false;
                          if (act_edited->QueryIntAttribute ("x", &plot_x)
                              != TIXML_SUCCESS)
                            plot_x = 0;
			  if (act_edited->QueryIntAttribute ("y", &plot_y)
                              != TIXML_SUCCESS)
                            plot_y = 0;

			  plot_area = new plot(xmin, xmax, ymin, ymax, xres, yres, main_font);
                        }

		      if (strcmp (act_edited->Value(), "iteredt") == 0)
                        {
                          int max_size;

                          if (act_edited->Attribute ("label") == NULL)
                            act_edited->SetAttribute ("label", "");
                          if (act_edited->QueryIntAttribute ("maxchar", &max_size)
                              != TIXML_SUCCESS)
                            max_size = 256;
                          plot_edt_buf = (new char[(max_size + 1) * 6]);
                          plot_edt_buf[0] = '\0';

                          if ((act_edited->Attribute ("type") == NULL) ? false
                              : strcmp (act_edited->Attribute ("type"), "small") == 0)
                            plot_edt = new gfxgui_edit (edit_small,
                                                        (char *) act_edited->Attribute ("label"),
                                                         main_font, main_font, EDIT_FNT_SIZE,
                                                         plot_edt_buf, max_size,
                                                         EDIT_V_BORDER, EDIT_H_BORDER,
                                                         EDIT_H_BORDER, EDIT_V_BORDER);
                          else
                            plot_edt = new gfxgui_edit (edit_large,
                                                        (char *) act_edited->Attribute ("label"),
                                                        main_font, main_font, EDIT_FNT_SIZE,
                                                        plot_edt_buf, max_size,
                                                        EDIT_V_BORDER, EDIT_H_BORDER,
                                                        EDIT_H_BORDER, EDIT_V_BORDER);

                          write_dlg (creation_dialog, d_gfxgui_edit_proc, 0, 0, 0, 0,
                                     EDT_FNT_COL, EDT_FNT_COL_BG, 0, 0, 0, 0,
                                     plot_edt, NULL, NULL);
                          if (act_edited->QueryIntAttribute ("x", &creation_dialog.x)
                              != TIXML_SUCCESS)
                            creation_dialog.x = 0;
                          if (act_edited->QueryIntAttribute ("y", &creation_dialog.y)
                              != TIXML_SUCCESS)
                            creation_dialog.y = 0;
                          plot_edt_dlg_pos = pre_dialog.size ();
                          pre_dialog.push_back (creation_dialog);
			}

		      if (strcmp (act_edited->Value (), "iterbut") == 0)
                        {
                          if (act_edited->Attribute ("label") == NULL)
                            act_edited->SetAttribute ("label", "");
                          plot_but = new gfxgui_button (but_normal, but_hover, but_selected,
			                                but_clicked, (char *) act_edited->Attribute ("label"),
                                                        main_font, BUT_FNT_SIZE, BUT_V_OFFSET,
                                                        BUT_H_OFFSET, iterate);
                          write_dlg (creation_dialog, d_gfxgui_button_proc, 0, 0,
                                     0, 0, BUT_FNT_COL, BUT_FNT_COL_ACT, 0, 0,
                                     0, 0, plot_but, NULL, NULL);
                          if (act_edited->QueryIntAttribute ("x", &creation_dialog.x)
                              != TIXML_SUCCESS)
                            creation_dialog.x = 0;
                          if (act_edited->QueryIntAttribute ("y", &creation_dialog.y)
                              != TIXML_SUCCESS)
                            creation_dialog.y = 0;
                          pre_dialog.push_back (creation_dialog);
                        }

                      if (strcmp (act_edited->Value(), "qcheck") == 0)
                        {
                          if (act_edited->Attribute ("label") == NULL)
                            act_edited->SetAttribute ("label", "");
                          if ((act_edited->Attribute ("type") == NULL) ? false
                              : strcmp (act_edited->Attribute ("type"), "radio") == 0)
                            buttons.push_back (new gfxgui_button (radio_normal, radio_hover,
                                               radio_selected, radio_clicked,
                                               (char *) act_edited->Attribute ("label"),
                                               main_font, CHKBOX_FNT_SIZE, CHKBOX_V_OFFSET,
                                               CHKBOX_H_OFFSET, interface_radio_click));
                          else
                            buttons.push_back (new gfxgui_button (checkbox_normal,
                                               checkbox_hover, checkbox_selected,
                                               checkbox_clicked,
                                               (char *) act_edited->Attribute ("label"),
                                               main_font, CHKBOX_FNT_SIZE, CHKBOX_V_OFFSET,
                                               CHKBOX_H_OFFSET, interface_chkbox_click));
                          buttons[buttons.size () - 1]->lbl_align = GFXGUI_TXT_ALIGN_LEFT;
                          write_dlg (creation_dialog, d_gfxgui_button_proc, 0, 0,
                                     0, 0, CHKBOX_FNT_COL, CHKBOX_FNT_COL_ACT, 0, 0,
                                     0, 0, buttons[buttons.size () - 1], NULL, NULL);
                          if (act_edited->QueryIntAttribute ("x", &creation_dialog.x)
                              != TIXML_SUCCESS)
                            creation_dialog.x = 0;
                          if (act_edited->QueryIntAttribute ("y", &creation_dialog.y)
                              != TIXML_SUCCESS)
                            creation_dialog.y = 0;
                          if (act_edited->QueryIntAttribute ("group", &creation_dialog.d1)
                              != TIXML_SUCCESS)
                            creation_dialog.d1 = 0;
                          if ((act_edited->Attribute ("selected") == NULL) ? false
                              : strcmp (act_edited->Attribute ("selected"), "yes") == 0)
                            creation_dialog.flags |= D_SELECTED;
                          if ((act_edited->Attribute ("rightanswer") == NULL) ? false
                              : strcmp (act_edited->Attribute ("rightanswer"), "yes") == 0)
                            buttons_right.push_back (true);
                          else
                            buttons_right.push_back (false);
                          buttons_dlg_pos.push_back (pre_dialog.size ());
                          pre_dialog.push_back (creation_dialog);
                        }
                         
                      if (strcmp (act_edited->Value(), "qedit") == 0)
                        {
                          int max_size;

                          if (act_edited->Attribute ("label") == NULL)
                            act_edited->SetAttribute ("label", "");
                          if (act_edited->QueryIntAttribute ("maxchar", &max_size)
                              != TIXML_SUCCESS)
                            max_size = 256;
                          edit_input.push_back (new char[(max_size + 1) * 6]);
                          edit_input[edit_input.size () - 1][0] = '\0';

                          if ((act_edited->Attribute ("type") == NULL) ? false
                              : strcmp (act_edited->Attribute ("type"), "small") == 0)
                            edit_fields.push_back (new gfxgui_edit (edit_small,
                                                   (char *) act_edited->Attribute ("label"),
                                                   main_font, main_font, EDIT_FNT_SIZE,
                                                   edit_input[edit_input.size () - 1],
                                                   max_size,
                                                   EDIT_V_BORDER, EDIT_H_BORDER,
                                                   EDIT_H_BORDER, EDIT_V_BORDER));
                          else
                            edit_fields.push_back (new gfxgui_edit (edit_large,
                                                   (char *) act_edited->Attribute ("label"),
                                                   main_font, main_font, EDIT_FNT_SIZE,
                                                   edit_input[edit_input.size () - 1],
                                                   max_size,
                                                   EDIT_V_BORDER, EDIT_H_BORDER,
                                                   EDIT_H_BORDER, EDIT_V_BORDER));

                          write_dlg (creation_dialog, d_gfxgui_edit_proc, 0, 0, 0, 0,
                                     EDT_FNT_COL, EDT_FNT_COL_BG, 0, 0, 0, 0,
                                     edit_fields[edit_fields.size () - 1], NULL, NULL);
                          if (act_edited->QueryIntAttribute ("x", &creation_dialog.x)
                              != TIXML_SUCCESS)
                            creation_dialog.x = 0;
                          if (act_edited->QueryIntAttribute ("y", &creation_dialog.y)
                              != TIXML_SUCCESS)
                            creation_dialog.y = 0;
                          if (act_edited->Attribute ("rightanswer") == NULL)
                            act_edited->SetAttribute ("rightanswer", "");
                          edit_right.push_back (act_edited->Attribute ("rightanswer"));
                          edit_dlg_pos.push_back (pre_dialog.size ());
                          pre_dialog.push_back (creation_dialog);
                        }

                      if (strcmp (act_edited->Value(), "xma") == 0
                          && act_edited->Attribute ("src") != NULL)
                        {
                          destroy_xma (animation);
                          sprintf (buffer, "%s%s%s", dir, DIR_SEP,
                                   act_edited->Attribute ("src"));
                          animation = load_xma (buffer);
                          if (animation != NULL)
                            {
                              if ((act_edited->Attribute ("loop") == NULL) ? false
                                  : strcmp (act_edited->Attribute ("loop"), "yes") == 0)
                                xma::loop = true;
                              if (act_edited->QueryIntAttribute ("x", &animation_x)
                                  != TIXML_SUCCESS)
                                animation_x = 0;
                              if (act_edited->QueryIntAttribute ("y", &animation_y)
                                  != TIXML_SUCCESS)
                                animation_y = 0;

                              write_dlg (creation_dialog, d_renderxma_proc, 0, 0, 0, 0,
                                         0, 0, 0, 0, 0, 0, NULL, NULL, NULL);
                              pre_dialog.push_back (creation_dialog);
                              if ((act_edited->Attribute ("autostart") == NULL) ? false
                                  : strcmp (act_edited->Attribute ("autostart"), "yes") == 0)
                                play_xma (animation, gui_get_screen (), animation_x,
                                          animation_y);
                            }
                        }
                         
                      if (strcmp (act_edited->Value (), "xmacontrol") == 0)
                        {
                          if (act_edited->Attribute ("label") == NULL)
                            act_edited->SetAttribute ("label", "");
                          if ((act_edited->Attribute ("type") == NULL) ? true
                              : strcmp (act_edited->Attribute ("type"), "play") == 0)
                            xma_buttons.push_back (new gfxgui_button (but_normal,
                                                   but_hover, but_selected, but_clicked,
                                                   (char *) act_edited->Attribute ("label"),
                                                   main_font, BUT_FNT_SIZE, BUT_V_OFFSET,
                                                   BUT_H_OFFSET,
                                                   interface_playxma_but_click));
                          else if (strcmp (act_edited->Attribute ("type"), "pause") == 0)
                            xma_buttons.push_back (new gfxgui_button (but_normal,
                                                   but_hover, but_selected, but_clicked,
                                                   (char *) act_edited->Attribute ("label"),
                                                   main_font, BUT_FNT_SIZE, BUT_V_OFFSET,
                                                   BUT_H_OFFSET,
                                                   interface_pausexma_but_click));
                          else if (strcmp (act_edited->Attribute ("type"), "stop") == 0)
                            xma_buttons.push_back (new gfxgui_button (but_normal,
                                                   but_hover, but_selected, but_clicked,
                                                   (char *) act_edited->Attribute ("label"),
                                                   main_font, BUT_FNT_SIZE, BUT_V_OFFSET,
                                                   BUT_H_OFFSET,
                                                   interface_stopxma_but_click));
                          write_dlg (creation_dialog, d_gfxgui_button_proc, 0, 0,
                                     0, 0, BUT_FNT_COL, BUT_FNT_COL_ACT, 0, 0,
                                     0, 0, xma_buttons[xma_buttons.size () - 1],
                                     NULL, NULL);
                          if (act_edited->QueryIntAttribute ("x", &creation_dialog.x)
                              != TIXML_SUCCESS)
                            creation_dialog.x = 0;
                          if (act_edited->QueryIntAttribute ("y", &creation_dialog.y)
                              != TIXML_SUCCESS)
                            creation_dialog.y = 0;
                          pre_dialog.push_back (creation_dialog);
                        }
                    }
                }
            }
        }

      write_dlg (creation_dialog, NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, NULL, NULL, NULL);
      pre_dialog.push_back (creation_dialog);
      
      /* Dialog konvertieren */
      fin_dialog = new DIALOG[pre_dialog.size ()];
      for (j = 0; j < pre_dialog.size (); j++)
        copy_dlg (fin_dialog[j], pre_dialog[j]);

      clicked_button = fin_dialog[do_dialog (fin_dialog, -1)].d2;

      interface_clean (xml_tree, img);

      switch (clicked_button)
        {
          case INTERFACE_BACK:
            i--;
            break;
          case INTERFACE_NEXT:
            i++;
            break;
          case INTERFACE_MOREINFO:
            sprintf (buffer, "%s%smoreinfo", dir, DIR_SEP);
            load_content (buffer, 0);
          case INTERFACE_QUIT:
            sprintf (buffer, "%s%s%i%s", dir, DIR_SEP, i + 1, ".jpg");
            if (file_exists (buffer, -1, NULL) == 0)
              i = -1;
            if (page == -1)
              set_config_int ("interface", dir, (i >= 0) ? i : 0);
            return i;
            break;
        }
    }

  return -1;
}

#endif
