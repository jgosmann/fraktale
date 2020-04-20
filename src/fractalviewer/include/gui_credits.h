/***************************************************************************
 * Project: FractalViewer                                                  *
 * Filename: gui_credits.h                                                 *
 * Begin: Fr Jun 03 14:37:23 CET 2005                                      *
 * Description: Anzeige der Credits                                        *
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

#ifndef GUI_CREDITS_H
#define GUI_CREDITS_H 1

/***** Includes *****/
#include <allegro.h>
#include <alfont.h>
#include "gfxgui.h"

/***** Functions *****/
//----------------------------------------------------------------------------
// Anzeige der Credits
//----------------------------------------------------------------------------
void
gui_credits (void)
{
  gfxgui_button back_but (but_normal, but_hover, but_selected, but_clicked, _("Back"),
                          main_font, BUT_FNT_SIZE, BUT_H_OFFSET);
  DIALOG credits[] = {
    { d_clear_proc, 0, 0, WIN_WIDTH, WIN_HEIGHT, 0, 0, 0, 0, 0, 0, NULL, NULL, NULL },

    { d_gfxgui_textout_proc, WIN_WIDTH / 2, 10, 0, 0, makecol (255, 255, 255),
      makecol (0, 0, 0), 0, 0, GFXGUI_TXT_ALIGN_CENTER, 36, (void *) _("FractalViewer"),
      main_font, NULL},
    { d_gfxgui_textout_proc, WIN_WIDTH / 2, 45, 0, 0, makecol (255, 255, 255),
      makecol (0, 0, 0), 0, 0, GFXGUI_TXT_ALIGN_CENTER, 16,
      (void *) _("Copyright \251 2005 Jan Gosmann"), main_font, NULL},
    { d_gfxgui_textout_proc, WIN_WIDTH / 2, 75, 0, 0, makecol (255, 255, 255),
      makecol (0, 0, 0), 0, 0, GFXGUI_TXT_ALIGN_CENTER, 26,
      (void *) _("Developed by Jan Gosmann"), main_font, NULL},
    { d_gfxgui_textout_proc, WIN_WIDTH / 2, 100, 0, 0, makecol (255, 255, 255),
      makecol (0, 0, 0), 0, 0, GFXGUI_TXT_ALIGN_CENTER, 26,
      (void *) _("Graphics by Yujiang Wang"), main_font, NULL},
    { d_gfxgui_textout_proc, WIN_WIDTH / 2, 145, 0, 0, makecol (255, 255, 255),
      makecol (0, 0, 0), 0, 0, GFXGUI_TXT_ALIGN_CENTER, 26,
      (void *) _("Visit:"), main_font, NULL},
    { d_gfxgui_textout_proc, WIN_WIDTH / 2, 170, 0, 0, makecol (255, 255, 255),
      makecol (0, 0, 0), 0, 0, GFXGUI_TXT_ALIGN_CENTER, 26,
      (void *) _("http://www.hyper-world.de.vu"), main_font, NULL},
    { d_gfxgui_textout_proc, WIN_WIDTH / 2, 195, 0, 0, makecol (255, 255, 255),
      makecol (0, 0, 0), 0, 0, GFXGUI_TXT_ALIGN_CENTER, 26,
      (void *) _("http://www.simple-graphix.de"), main_font, NULL},
    { d_gfxgui_textout_proc, WIN_WIDTH / 2, 240, 0, 0, makecol (255, 255, 255),
      makecol (0, 0, 0), 0, 0, GFXGUI_TXT_ALIGN_CENTER, 22,
      (void *) _("Following (great) libraries has been used in this program:"),
      main_font, NULL},
    { d_gfxgui_textout_proc, WIN_WIDTH / 2, 265, 0, 0, makecol (255, 255, 255),
      makecol (0, 0, 0), 0, 0, GFXGUI_TXT_ALIGN_CENTER, 22,
      (void *) _("Allegro 4, http://alleg.sourceforg.net"), main_font, NULL},
    { d_gfxgui_textout_proc, WIN_WIDTH / 2, 290, 0, 0, makecol (255, 255, 255),
      makecol (0, 0, 0), 0, 0, GFXGUI_TXT_ALIGN_CENTER, 22,
      (void *) _("AllegroFont, http://nekros.freeshell.org/delirium/alfont.php"),
      main_font, NULL},
    { d_gfxgui_textout_proc, WIN_WIDTH / 2, 315, 0, 0, makecol (255, 255, 255),
      makecol (0, 0, 0), 0, 0, GFXGUI_TXT_ALIGN_CENTER, 22,
      (void *) _("Freetype (as part of AllegroFont), http://www.freetype.org"),
      main_font, NULL},
    { d_gfxgui_textout_proc, WIN_WIDTH / 2, 340, 0, 0, makecol (255, 255, 255),
      makecol (0, 0, 0), 0, 0, GFXGUI_TXT_ALIGN_CENTER, 22,
      (void *) _("JPGalleg, http://www.ecplusplus.com/index.php?page=projects&pid=1"),
      main_font, NULL},
    { d_gfxgui_textout_proc, WIN_WIDTH / 2, 365, 0, 0, makecol (255, 255, 255),
      makecol (0, 0, 0), 0, 0, GFXGUI_TXT_ALIGN_CENTER, 22,
      (void *) _("TinyXML, http://www.grinninglizard.com/tinyxml/"), main_font, NULL},
    { d_gfxgui_textout_proc, WIN_WIDTH / 2, 390, 0, 0, makecol (255, 255, 255),
      makecol (0, 0, 0), 0, 0, GFXGUI_TXT_ALIGN_CENTER, 22,
      (void *) _("(with a little change in tinyxml.cpp in line 589)"), main_font, NULL},
    { d_gfxgui_textout_proc, WIN_WIDTH / 2, 415, 0, 0, makecol (255, 255, 255),
      makecol (0, 0, 0), 0, 0, GFXGUI_TXT_ALIGN_CENTER, 22,
      (void *) _("Gettext, http://www.gnu.org/software/gettext/"), main_font, NULL},
    { d_gfxgui_textout_proc, WIN_WIDTH / 2, 440, 0, 0, makecol (255, 255, 255),
      makecol (0, 0, 0), 0, 0, GFXGUI_TXT_ALIGN_CENTER, 22,
      (void *) _("QT, http://www.trolltech.com (only in the Linux version)"), main_font, NULL},
    { d_gfxgui_textout_proc, WIN_WIDTH / 2, 500, 0, 0, makecol (255, 255, 255),
      makecol (0, 0, 0), 0, 0, GFXGUI_TXT_ALIGN_CENTER, 14,
      (void *) _("This program has been published under the terms"), main_font, NULL},
    { d_gfxgui_textout_proc, WIN_WIDTH / 2, 515, 0, 0, makecol (255, 255, 255),
      makecol (0, 0, 0), 0, 0, GFXGUI_TXT_ALIGN_CENTER, 14,
      (void *) _("of the GNU General Public License."), main_font, NULL},

    { d_gfxgui_button_proc, WIN_WIDTH - back_but.bmp[0]->w - 25,
      WIN_HEIGHT - back_but.bmp[0]->h - 15, 0, 0, BUT_FNT_COL, BUT_FNT_COL_ACT,
      scancode_to_ascii (KEY_ENTER), D_EXIT, 0, 0, &back_but, NULL, NULL },
    { NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, NULL, NULL, NULL } };

  popup_dialog (credits, -1);
}

#endif // GUI_CREDITS_H
