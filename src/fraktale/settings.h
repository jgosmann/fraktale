/***************************************************************************
 * Project: Fraktale                                                       *
 * Filename: settings.h                                                    *
 * Begin: We Jul 06 18:15:00 CET 2005                                      *
 * Description: Einstellungen für die Benutzeroberfläche etc.              *
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

/***** Defines *****/
#define PRGM_NAME "Fraktale"
#define LOCALE_NAME "Fraktale"
#define PRGM_VERSION "1.0"

/* Fenstergröße */
#define WIN_WIDTH 800
#define WIN_HEIGHT 600

/* Schrift */
#define FNT_FILE "vera.ttf"

#define BUT_FNT_SIZE 12
#define LONGBUT_FNT_SIZE 16
#define CHKBOX_FNT_SIZE 14
#define EDIT_FNT_SIZE 16
#define ALERT_FNT_SIZE 18

/* Farben */
#define BG_COL makecol (236, 236, 236)

#define BUT_FNT_COL makecol (0, 0, 0)
#define BUT_FNT_COL_ACT makecol (255, 255, 255)

#define CHKBOX_FNT_COL makecol (0, 0, 0)
#define CHKBOX_FNT_COL_ACT makecol (255, 0, 0)

#define EDT_FNT_COL makecol (0, 0, 0)
#define EDT_FNT_COL_BG makecol (255, 255, 255)

/* Grafiken */
#define LOADING_SCREEN "load2.tga"
#define BG_PATH "bg.jpg"

#define BUT_NORMAL_PATH "button_normal.tga"
#define BUT_HOVER_PATH "button_hover.tga"
#define BUT_SELECTED_PATH "button_selected.tga"
#define BUT_CLICKED_PATH "button_click.tga"

#define LONGBUT_NORMAL_PATH "longbutton_normal.tga"
#define LONGBUT_HOVER_PATH "longbutton_hover.tga"
#define LONGBUT_SELECTED_PATH "longbutton_selected.tga"
#define LONGBUT_CLICKED_PATH "longbutton_clicked.tga"

#define EDIT_PATH "edit_large.tga"
#define SMALL_EDIT_PATH "edit_small.tga"

#define POPUPBACK_LEFT_PATH "left.tga"
#define POPUPBACK_RIGHT_PATH "right.tga"
#define POPUPBACK_TOP_PATH "top.tga"
#define POPUPBACK_BOTTOM_PATH "bottom.tga"
#define POPUPBACK_LEFT_TOP_PATH "left_top.tga"
#define POPUPBACK_RIGHT_TOP_PATH "right_top.tga"
#define POPUPBACK_LEFT_BOTTOM_PATH "left_bottom.tga"
#define POPUPBACK_RIGHT_BOTTOM_PATH "right_bottom.tga"
#define POPUPBACK_MIDDLE_PATH "middle.tga"

/* Grafik-Offsets */
#define EDIT_H_BORDER 8
#define EDIT_V_BORDER 12

#define BUT_H_OFFSET -3
#define BUT_V_OFFSET 0

#define CHKBOX_H_OFFSET 0
#define CHKBOX_V_OFFSET 15

/* Config-File */
#define CONFIG_FILE "fraktale.cfg"
