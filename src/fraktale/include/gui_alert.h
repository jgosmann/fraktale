/***************************************************************************
 * Project: FractalViewer                                                  *
 * Filename: gui_alert.h                                                   *
 * Begin: Fr May 06 09:46:57 CET 2005                                      *
 * Description: MsgBox                                                     *
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

#ifndef GUI_ALERT_H
#define GUI_ALERT_H 1

/***** Includes *****/
#include <allegro.h>
#include <alfont.h>
#include "gfxgui.h"

/***** Functions *****/
//----------------------------------------------------------------------------
// Anzeige eines Alerts
//----------------------------------------------------------------------------
void
gui_alert (char *msg, char *but1)
{
  gfxgui_button ok_but (but_normal, but_hover, but_selected, but_clicked, but1,
                        main_font, BUT_FNT_SIZE, BUT_V_OFFSET, BUT_H_OFFSET);
  alfont_set_font_size (main_font, 20);
  unsigned int w = alfont_text_length (main_font, msg) + 100;
  unsigned int h = alfont_text_height (main_font) + 100;
  DIALOG alert_box[] = {
    { d_gfxgui_area_proc, 0, 0, w, h, 0, 0, 0, 0, 0, 0, popup_back, NULL, NULL },
    { d_gfxgui_textout_proc, w / 2, 35, 0, 0, EDT_FNT_COL, makeacol (0, 0, 0, 255),
      0, 0, GFXGUI_TXT_ALIGN_CENTER, 20, msg, main_font, NULL},
    { d_gfxgui_button_proc, (w - ok_but.bmp[0]->w) / 2, h - ok_but.bmp[0]->h - 25,
      0, 0, BUT_FNT_COL, BUT_FNT_COL_ACT, scancode_to_ascii (KEY_ENTER), D_EXIT,
      0, 0, &ok_but, NULL, NULL },
    { NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, NULL, NULL, NULL } };
    
  centre_dialog (alert_box);
  popup_dialog (alert_box, -1);
}

#endif // GUI_ALERT_H
