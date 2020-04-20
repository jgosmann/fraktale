/***************************************************************************
 * Project: FractalViewer                                                  *
 * Project: FractalViewer                                                  *
 * Filename: settings.h                                                    *
 * Begin: Su Apr 17 09:11:29 CET 2005                                      *
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
#define PRGM_NAME "FractalViewer"
#define LOCALE_NAME "fractalviewer"
#define PRGM_VERSION "2.1"

/* Fenstergröße */
#define WIN_WIDTH 800
#define WIN_HEIGHT 600

/* Fraktalgrößen */
#define FRAC_WIDTH 640
#define FRAC_HEIGHT 480
#define PREV_FRAC_WIDTH 320
#define PREV_FRAC_HEIGHT 240
#define FRAC_XMIN -3.0
#define FRAC_XMAX 3.0
#define FRAC_YMIN -2.25
#define FRAC_STD_ITER 160
#define PREV_FRAC_ITER 40

/* Schrift */
#define FNT_FILE "vera.ttf"

#define BUT_FNT_SIZE 12
#define EDIT_FNT_SIZE 16
#define ALERT_FNT_SIZE 18

/* Farben */
#define BG_COL makecol (236, 236, 236)

#define BUT_FNT_COL makecol (0, 0, 0)
#define BUT_FNT_COL_ACT makecol (255, 255, 255)

#define EDT_FNT_COL makecol (0, 0, 0)
#define EDT_FNT_COL_BG makecol (255, 255, 255)

/* Grafiken */
#define BUT_NORMAL_PATH "button_normal.tga"
#define BUT_HOVER_PATH "button_hover.tga"
#define BUT_SELECTED_PATH "button_selected.tga"
#define BUT_CLICKED_PATH "button_click.tga"

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

/* Config-File */
#define CONFIG_FILE "FractalViewer.cfg"

#define BUT_H_OFFSET -3
