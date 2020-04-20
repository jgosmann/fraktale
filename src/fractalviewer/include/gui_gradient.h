/***************************************************************************
 * Project: FractalViewer                                                  *
 * Filename: gui_gradient.h                                                *
 * Begin: Mo May 16 16:37:05 CET 2005                                      *
 * Description: Farbenverlaufswähler                                       *
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

#ifndef GUI_GRADIENT_H
#define GUI_GRADIENT_H 1

/*** Includes ***/
#include <allegro.h>
#include <alfont.h>
#include "gfxgui.h"
#include <palette.h>
#include <tinyxml.h>

#include "gui_colsel.h"
#include "gui_alert.h"
#include "xml_gradient.h"

//----------------------------------------------------------------------------
// Namespace mit Zeigern auf Funktionsinterne Variablen von gui_gradient
//----------------------------------------------------------------------------
namespace gui_gradient_vars {
  colpal *new_gradient;
  char (*new_disp_cols)[24];
}

//============================================================================
// Callbacks
//============================================================================
void
gui_gradient_addcol_but_click (DIALOG *d)
{
  int col = gui_colsel (makecol (0, 0, 0));
  gui_gradient_vars::new_gradient->insert_color (0, col);
  gui_gradient_vars::new_gradient->calc_pal ();
  broadcast_dialog_message (MSG_DRAW, 0);
}

void
gui_gradient_ok_but_click (DIALOG *d)
{
  if (atoi (*gui_gradient_vars::new_disp_cols) <= 0
      || atoi (*gui_gradient_vars::new_disp_cols) > 255)
    {
      gui_alert (_("Colors has to be a value greater than 0 and lower than 256!"),
                 _("OK"));
      return;
    }
    
  if (gui_gradient_vars::new_gradient->colors.size () <= 0)
    {
      gui_alert (_("There has to be at least one color in the gradient!"), _("OK"));
      return;
    }
    
  d->flags |= D_EXIT;
  return;
}

void
gui_gradient_changebg_but_click (DIALOG *d)
{
  gui_gradient_vars::new_gradient->bg_col
    = gui_colsel (gui_gradient_vars::new_gradient->bg_col);
  gui_gradient_vars::new_gradient->calc_pal ();
  broadcast_dialog_message (MSG_DRAW, 0);
}

void
gui_gradient_load_but_click (DIALOG *d)
{
  /*** Variables ***/
  vector <file_type> filter_list; // Filter für Dateitypen
  string load_from; // Dateiname der Datei, in die geladem wird
  TiXmlDocument *xml_tree; // Aufbau der Datei in die geladen wird

  /*** Commands ***/
  filter_list.push_back (file_type ());
  (filter_list.back ()).name = _("FractalViewer gradient (*.fvg)");
  (filter_list.back ()).endings.push_back (string ());
  (filter_list.back ()).endings.back () = "fvg";
  filter_list.push_back (file_type ());
  (filter_list.back ()).name = _("All files (*)");
  (filter_list.back ()).endings.push_back (string ());
  (filter_list.back ()).endings.back () = "";

  load_from = file_dialog (filter_list, 0);
  if (load_from == "")
    return;

  xml_tree = new TiXmlDocument (load_from);
  xml_tree->LoadFile ();
  if (xml_tree->RootElement () == NULL)
    {
      gui_alert (_("File could not be loaded!"), _("OK"));
      delete xml_tree;
      return;
    }

  if (!xml_to_gradient (xml_tree->RootElement (), *gui_gradient_vars::new_gradient))
    {
      xml_tree->Clear ();
      delete xml_tree;
      return;
    }
  gui_gradient_vars::new_gradient->calc_pal ();
  broadcast_dialog_message (MSG_DRAW, 0);
  xml_tree->Clear ();
  delete xml_tree;
  return;
}

void
gui_gradient_save_but_click (DIALOG *d)
{
  /*** Variables ***/
  vector <file_type> filter_list; // Filter für Dateitypen
  string save_to; // Dateiname der Datei, in die gespeichert wird
  TiXmlDocument *xml_tree; // Aufbau der Datei in die gespeichert wird

  /*** Commands ***/
  filter_list.push_back (file_type ());
  (filter_list.back ()).name = _("FractalViewer gradient (*.fvg)");
  (filter_list.back ()).endings.push_back (string ());
  (filter_list.back ()).endings.back () = "fvg";
  filter_list.push_back (file_type ());
  (filter_list.back ()).name = _("All files (*)");
  (filter_list.back ()).endings.push_back (string ());
  (filter_list.back ()).endings.back () = "";
  
  save_to = file_dialog (filter_list, 1);
  if (save_to == "")
    return;

  xml_tree = new TiXmlDocument (save_to);
  xml_tree->LinkEndChild (gradient_to_xml (*gui_gradient_vars::new_gradient));
  if (!xml_tree->SaveFile ())
    gui_alert (_("An error occured while saving!"), _("OK"));
  xml_tree->Clear ();
  delete xml_tree;
  return;
}

//----------------------------------------------------------------------------
// Anzeige eines Farbwählers
//----------------------------------------------------------------------------
void
gui_gradient (colpal &gradient, int &disp_cols)
{
  /*** Variables ***/
  char new_disp_cols[24]; // Für Farbdichte-Eingabefeld
  int exit_status; // Durch welchen Button wurde der Dialog beendet?

  /*** GUI-Elements ***/
  gfxgui_button ok_but (but_normal, but_hover, but_selected, but_clicked, _("OK"),
                        main_font, BUT_FNT_SIZE, BUT_H_OFFSET, gui_gradient_ok_but_click);
  gfxgui_button cancel_but (but_normal, but_hover, but_selected, but_clicked,
                            _("Cancel"), main_font, BUT_FNT_SIZE, BUT_H_OFFSET);
  gfxgui_button addcol_but (but_normal, but_hover, but_selected, but_clicked,
                            _("Add color"), main_font, BUT_FNT_SIZE, BUT_H_OFFSET,
                            gui_gradient_addcol_but_click);
  gfxgui_button changebg_but (but_normal, but_hover, but_selected, but_clicked,
                              _("Bg color"), main_font, BUT_FNT_SIZE, BUT_H_OFFSET,
                              gui_gradient_changebg_but_click);
  gfxgui_button load_but (but_normal, but_hover, but_selected, but_clicked,
                          _("Load"), main_font, BUT_FNT_SIZE, BUT_H_OFFSET,
                          gui_gradient_load_but_click);
  gfxgui_button save_but (but_normal, but_hover, but_selected, but_clicked,
                          _("Save"), main_font, BUT_FNT_SIZE, BUT_H_OFFSET,
                          gui_gradient_save_but_click);

  gfxgui_edit disp_cols_edit (small_edit, _("Colors: "), main_font, main_font,
                              EDIT_FNT_SIZE, new_disp_cols, 3, EDIT_V_BORDER,
                              EDIT_H_BORDER, EDIT_H_BORDER, EDIT_V_BORDER);

  colpal new_gradient (&gradient);
  new_gradient.calc_pal ();

  colslider_data gradient_data (&new_gradient);

  /* Namespace */
  gui_gradient_vars::new_gradient = &new_gradient;
   gui_gradient_vars::new_disp_cols = &new_disp_cols;

  /* Weiteres */
  sprintf (new_disp_cols, "%i", disp_cols);

  alfont_set_font_size (main_font, BUT_FNT_SIZE);
  DIALOG dlg[] = {
    { d_gfxgui_area_proc, 0, 0, 370, 225, 0, 0, 0, 0, 0, 0, popup_back, NULL, NULL },
    { d_colslider_proc, 50, 30, 270, 50, BUT_FNT_COL_ACT, BUT_FNT_COL,
      0, 0, COLSLIDER_CHANGEABLE, 0, &gradient_data, NULL, NULL },
    { d_gfxgui_edit_proc, 100, 90, 0, 0, EDT_FNT_COL, EDT_FNT_COL_BG,
      0, 0, 0, 0, &disp_cols_edit, NULL },
    { d_gfxgui_button_proc, 30, 130, 0, 0, BUT_FNT_COL, BUT_FNT_COL_ACT,
      0, 0, 0, 0, &addcol_but, NULL, NULL },
    { d_gfxgui_button_proc, 130, 130, 0, 0, BUT_FNT_COL, BUT_FNT_COL_ACT,
      0, 0, 0, 0, &changebg_but, NULL, NULL },
    { d_gfxgui_button_proc, 250, 130, 0, 0, BUT_FNT_COL, BUT_FNT_COL_ACT,
      0, 0, 0, 0, &load_but, NULL, NULL },
    { d_gfxgui_button_proc, 250, 170, 0, 0, BUT_FNT_COL, BUT_FNT_COL_ACT,
      0, 0, 0, 0, &save_but, NULL, NULL },
    { d_gfxgui_button_proc, 30, 170, 0, 0, BUT_FNT_COL, BUT_FNT_COL_ACT,
      scancode_to_ascii (KEY_ENTER), 0, 0, 0, &ok_but, NULL, NULL },
    { d_gfxgui_button_proc, 130, 170, 0, 0, BUT_FNT_COL, BUT_FNT_COL_ACT,
      0, D_EXIT, 0, 0, &cancel_but, NULL, NULL },
    { NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, NULL, NULL, NULL } };

  centre_dialog (dlg);
  exit_status = popup_dialog (dlg, -1);

  if (exit_status == 7)
    {
      disp_cols = atoi (new_disp_cols);
      gradient = new_gradient;
    }

  return;
}

#endif // GUI_GRADIENT_H
