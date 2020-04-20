/***************************************************************************
 * Filename: gfx_gui.h                                                     *
 * Begin: Mo Apr 25 08:36:17 CET 2005                                      *
 * Description: GUI mit Bitmap-Objekten                                    *
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

#ifndef GFXGUI_H
#define GFXGUI_H 1

/***** Includes *****/
#include <allegro.h>
#include <alfont.h>

/***** Defines *****/
#define GFXGUI_TXT_ALIGN_LEFT 0
#define GFXGUI_TXT_ALIGN_CENTER 1
#define GFXGUI_TXT_ALIGN_RIGHT 2

//============================================================================
// Struct für ein grafisches Bereichs-Element
//============================================================================
struct gfxgui_area {
  BITMAP *left_top, *left_bottom, *right_top, *right_bottom, *top, *bottom,
         *left, *right, *middle; // Grafiken
  gfxgui_area (void);
   ~gfxgui_area (void);
};

//----------------------------------------------------------------------------
// Konstruktor, Destruktor
//----------------------------------------------------------------------------
gfxgui_area::gfxgui_area (void)
{
  left_top = NULL;
  left_bottom = NULL;
  right_top = NULL;
  right_bottom = NULL;
  top = NULL;
  bottom = NULL;
  left = NULL;
  right = NULL;
  middle = NULL;
}

gfxgui_area::~gfxgui_area (void)
{
  if (left != NULL);
    destroy_bitmap (left);
  if (right != NULL);
    destroy_bitmap (right);
  if (top != NULL);
    destroy_bitmap (top);
  if (bottom != NULL);
    destroy_bitmap (bottom);
  if (left_top != NULL);
    destroy_bitmap (left_top);
  if (right_top != NULL);
    destroy_bitmap (right_top);
  if (left_bottom != NULL);
    destroy_bitmap (left_bottom);
  if (right_bottom != NULL);
    destroy_bitmap (right_bottom);
  if (middle != NULL);
    destroy_bitmap (middle);
}

//============================================================================
// Struct für einen Button
//============================================================================
struct gfxgui_button {
  public:
    BITMAP *bmp[4]; // Grafiken, 0 = Normal, 1 = Hover, 2 = Selected, 3 = Clicked
    char *label; // Beschriftung
    int lbl_align; // Label-Ausrichtung, 0 = links, 1 = mittig, 2 = rechts
    int lbl_x, lbl_y; // Position des Labels
    ALFONT_FONT *lbl_fnt; // Verwendete Schrift
    unsigned int fnt_size; // Schriftgröße
    void (*onClick)(DIALOG *); // Funktion, die bei Klick aufgerufen wird
    void (*onDeselect)(DIALOG *); // Funktion, die bei Deselektierung aufgerufen wird
    gfxgui_button (BITMAP *normal, BITMAP *hover, BITMAP *selected,
                   BITMAP *clicked, char *_label, ALFONT_FONT *_lbl_fnt,
                   unsigned int _fnt_size, int label_y_offset = 0,
                   void (*_onClick)(DIALOG *) = NULL, void (*_onDeselect)(DIALOG *) = NULL);
    ~gfxgui_button (void);
    friend int d_gfxgui_button_proc (int msg, DIALOG *d, int c);

  protected:
    BITMAP *back; // Überzeichneter Bereich
    bool first_draw;
};

//----------------------------------------------------------------------------
// Konstruktor, Destruktor
//----------------------------------------------------------------------------
gfxgui_button::gfxgui_button (BITMAP *normal, BITMAP *hover, BITMAP *selected,
                              BITMAP *clicked, char *_label,
                              ALFONT_FONT *_lbl_fnt, unsigned int _fnt_size,
                              int label_y_offset, void (*_onClick)(DIALOG *),
                              void (*_onDeselect)(DIALOG *))
{
  bmp[0] = normal;
  bmp[1] = hover;
  bmp[2] = selected;
  bmp[3] = clicked;
  back = create_bitmap (bmp[0]->w, bmp[0]->h);
	if (back == NULL)
    throw bad_alloc ();
  label = _label;
  lbl_align = GFXGUI_TXT_ALIGN_CENTER;
  lbl_fnt = _lbl_fnt;
  fnt_size = _fnt_size;
  lbl_x = normal->w / 2;
  alfont_set_font_size(lbl_fnt, fnt_size);
  lbl_y = (bmp[0]->h + label_y_offset - alfont_text_height (lbl_fnt)) / 2;
  onClick = _onClick;
  onDeselect = _onDeselect;
  first_draw = true;
}

gfxgui_button::~gfxgui_button (void)
{
  destroy_bitmap (back);
}

//============================================================================
// Struct für ein Eingabefeld
//============================================================================
struct gfxgui_edit {
  public:
    BITMAP *bmp; // Grafik
    char *label; // Beschriftung
    int lbl_x, lbl_y; // Position des Labels
    ALFONT_FONT *lbl_fnt; // Verwendete Schrift (Label)
    ALFONT_FONT *edt_fnt; // Verwendete Schrift (Eingabe)
    unsigned int fnt_size; // Schriftgröße
    char *input; // Eingabe
    int max_c; // Maximale Zeichenanzahl
    int input_x, input_y; // Position der Eingabe
    int input_w, input_h; // Größe des Eingabebereichs
    gfxgui_edit (BITMAP *_bmp, char *_label, ALFONT_FONT *_lbl_fnt,
                 ALFONT_FONT *_edt_fnt, unsigned int _fnt_size, char *_input,
                 int _max_c, int _input_x = 0, int _input_y = 0,
                 int input_right_x = 0, int input_right_y = 0);
    ~gfxgui_edit (void);
    friend int d_gfxgui_edit_proc (int msg, DIALOG *d, int c);
    
  protected:
    BITMAP *back; // Überzeichneter Eingabebereich
    bool first_draw;
};

//----------------------------------------------------------------------------
// Konstruktor, Destruktor
//----------------------------------------------------------------------------
gfxgui_edit::gfxgui_edit (BITMAP *_bmp, char *_label, ALFONT_FONT *_lbl_fnt,
                          ALFONT_FONT *_edt_fnt, unsigned int _fnt_size,
                          char *_input, int _max_c, int _input_x, int _input_y,
			                    int input_right_x, int input_right_y)
{
  bmp = _bmp;
  back = create_bitmap (bmp->w, bmp->h);
	if (back == NULL)
    throw bad_alloc ();
  label = _label;
  lbl_fnt = _lbl_fnt;
  edt_fnt = _edt_fnt;
  fnt_size = _fnt_size;
  lbl_x = 0;
  alfont_set_font_size(lbl_fnt, fnt_size);
  lbl_y = (bmp->h - alfont_text_height (lbl_fnt)) / 2;
  input = _input;
  input_x = _input_x;
  input_y = _input_y;
  input_w = bmp->w - input_x - input_right_x;
  input_h = bmp->h - input_y - input_right_y;
  max_c = _max_c;
  first_draw = true;
}

gfxgui_edit::~gfxgui_edit (void)
{
  destroy_bitmap (back);
}

//============================================================================
// Allegro-Dialog-Handler
//============================================================================
//----------------------------------------------------------------------------
// Ausgabe eines Bereichs-Elements
//----------------------------------------------------------------------------
int
d_gfxgui_area_proc (int msg, DIALOG *d, int c)
{
  /*** Variables ***/
  gfxgui_area *obj = (gfxgui_area *) d->dp; // Grafiken
  unsigned int x_left = 0, y_top = 0, x_right = 0, y_bottom = 0; // Zeichen-Verschiebung
  
  /*** Commands ***/
  if (msg == MSG_START)
    d->d1 = 1;
  if (msg == MSG_DRAW && d->d1 == 1)
    {
      d->d1 = 0;
      set_alpha_blender ();
      if (obj->left_top != NULL)
        {
          draw_trans_sprite (gui_get_screen (), obj->left_top, d->x, d->y);
          x_left = obj->left_top->w;
          y_top = obj->left_top->h;
        }
      if (obj->right_top != NULL)
        {
          draw_trans_sprite (gui_get_screen (), obj->right_top,
                             d->x + d->w - obj->right_top->w, d->y);
          x_right = obj->right_top->w;
        }
      if (obj->top != NULL)
        stretch_sprite (gui_get_screen (), obj->top, d->x + x_left, d->y,
                        d->w - x_left - x_right, obj->top->h);

      if (obj->left_bottom != NULL)
        {
          draw_trans_sprite (gui_get_screen (), obj->left_bottom, d->x,
                             d->y + d->h - obj->left_bottom->h);
          y_bottom = obj->left_bottom->h;
        }
      if (obj->left != NULL)
        stretch_sprite (gui_get_screen (), obj->left, d->x, d->y + y_top,
                        obj->left->w, d->h - y_top - y_bottom);

      if (obj->right_bottom != NULL)
        draw_trans_sprite (gui_get_screen (), obj->right_bottom,
                           d->x + d->w - obj->right_bottom->w,
                           d->y + d->h - obj->left_bottom->h);
      if (obj->right != NULL)
        stretch_sprite (gui_get_screen (), obj->right,
                        d->x + d->w - obj->right->w, d->y + y_top,
                        obj->right->w, d->h - y_top - y_bottom);
      if (obj->bottom != NULL)
        stretch_sprite (gui_get_screen (), obj->bottom, d->x + x_left,
                        d->y + d->h - y_bottom, d->w - x_left - x_right,
                        obj->bottom->h);
                        
      if (obj->middle != NULL)
        stretch_sprite (gui_get_screen (), obj->middle, d->x + x_left,
                        d->y + y_top, d->w - x_left - x_right,
                        d->h - y_top - y_bottom);
    }

  return D_O_K;
}

//----------------------------------------------------------------------------
// Ein Button
//----------------------------------------------------------------------------
int
d_gfxgui_button_proc (int msg, DIALOG *d, int c)
{
  /*** Variables ***/
  gfxgui_button *obj = (gfxgui_button *) d->dp;
  const int draw_col = (d->flags & D_GOTMOUSE) ? d->bg : d->fg;
  int last_state = -1, act_state; // Letzter/aktueller Status (für Anti-Flicker)
  
  /*** Commands ***/
  switch (msg)
    {
      case MSG_START:
        d->w = obj->bmp[0]->w;
        d->h = obj->bmp[0]->h;
        break;

      case MSG_DRAW:
        if (obj->first_draw)
           blit (gui_get_screen (), obj->back, d->x, d->y, 0, 0, d->w, d->h);

        obj->first_draw = false;

        draw_sprite (gui_get_screen (), obj->back, d->x, d->y);
        set_alpha_blender ();
        if (gui_mouse_x () >= d->x && gui_mouse_x () <= d->x + d->w
            && gui_mouse_y () >= d->y && gui_mouse_y () <= d->y + d->h && c == 1)
          draw_trans_sprite (gui_get_screen (), obj->bmp[3], d->x, d->y);
        else if (d->flags & D_GOTMOUSE || c == 1)
          draw_trans_sprite (gui_get_screen (), obj->bmp[1], d->x, d->y);
	      else if (d->flags & D_SELECTED)
          draw_trans_sprite (gui_get_screen (), obj->bmp[2], d->x, d->y);
        else
          draw_trans_sprite (gui_get_screen (), obj->bmp[0], d->x, d->y);

        alfont_set_font_size(obj->lbl_fnt, obj->fnt_size);
        switch (obj->lbl_align)
          {
            case GFXGUI_TXT_ALIGN_LEFT:
              alfont_textout_aa_ex (gui_get_screen (), obj->lbl_fnt, obj->label,
                                    d->x + obj->lbl_x, d->y + obj->lbl_y, draw_col,
                                    makeacol (0, 0, 0, 255));
              break;
            case GFXGUI_TXT_ALIGN_CENTER:
                 
               alfont_textout_centre_aa_ex (gui_get_screen (), obj->lbl_fnt, obj->label,
                                           d->x + obj->lbl_x, d->y + obj->lbl_y, draw_col,
                                           makeacol (0, 0, 0, 255));
              break;
            case GFXGUI_TXT_ALIGN_RIGHT:
              alfont_textout_right_aa_ex (gui_get_screen (), obj->lbl_fnt, obj->label,
                                          d->x + obj->lbl_x, d->y + obj->lbl_y, draw_col,
                                          makeacol (0, 0, 0, 255));
              break;
          }
     	  break;
     	  
     case MSG_GOTMOUSE:
     case MSG_LOSTMOUSE:
       return D_REDRAWME;
       break;

     case MSG_LPRESS:
       while (gui_mouse_b () & 1)
         {
	   if (gui_mouse_x () >= d->x && gui_mouse_x () <= d->x + d->w
               && gui_mouse_y () >= d->y && gui_mouse_y () <= d->y + d->h)
             act_state = 0;
           else if (d->flags & D_GOTMOUSE)
             act_state = 1;
	   else if (d->flags & D_SELECTED)
             act_state = 2;
           else
             act_state = 3;

	   if (last_state != act_state)
	     {
	       object_message (d, MSG_DRAW, 1);
               last_state = act_state;
	     }
           broadcast_dialog_message (MSG_IDLE, 0);
         }
       if (gui_mouse_x () >= d->x && gui_mouse_x () <= d->x + d->w
           && gui_mouse_y () >= d->y && gui_mouse_y () <= d->y + d->h)
         {
           if (obj->onClick != NULL)
             obj->onClick (d);
           if (d->flags & D_EXIT)
             return D_CLOSE;
         }
       object_message (d, MSG_DRAW, 0);
       break;
       
     case MSG_KEY:
       if (obj->onClick != NULL)
         obj->onClick (d);
       if (d->flags & D_EXIT)
         return D_CLOSE;
       break;

     case MSG_RADIO:
       if (c == d->d1 && d->flags & D_SELECTED)
         {
           d->flags &= ~D_SELECTED;
           if (obj->onDeselect != NULL)
             obj->onDeselect (d);
           object_message (d, MSG_DRAW, 0);
         }
       break;
   }

   return D_O_K;
}

//----------------------------------------------------------------------------
// Ein Eingabefeld
// Code based on the original Allegro-code for an edit box by VolkerOth
//----------------------------------------------------------------------------
int
d_gfxgui_edit_proc(int msg, DIALOG *d, int c)
{
   static int ignore_next_uchar = FALSE;
   int last_was_space, new_pos, i, k;
   int f, l, p, w, x, fg, b, scroll;
   char buf[16];
   char *s, *t;
   gfxgui_edit *obj = (gfxgui_edit *) d->dp;
   void (*lostfocus_func)(DIALOG *) = (void (*)(DIALOG *)) d->dp2;

   s = obj->input;
   l = ustrlen (s);
   if (d->d1 > l)
     d->d1 = l;

   /* calculate maximal number of displayable characters */
   if (d->d1 == l)
     {
       usetc (buf + usetc (buf, ' '), 0);
       x = alfont_text_length (obj->edt_fnt, buf);
     }
   else
     x = 0;

   b = 0;

   for (p = d->d1; p>=0; p--)
     {
       usetc (buf + usetc (buf, ugetat (s, p)), 0);
       x += alfont_text_length (obj->edt_fnt, buf);
       b++;
       if (x > d->w)
	       break;
     }

   if (x <= obj->input_w)
     {
       b = l;
       scroll = FALSE;
     }
   else
     {
       b--;
       scroll = TRUE;
     }

   switch (msg)
     {
       case MSG_START:
	       d->d1 = l;
	       d->d2 = 1;
	       d->w = obj->bmp->w;
	       d->h = obj->bmp->h;
	       break;

       case MSG_DRAW:
         if (obj->first_draw)
           blit (gui_get_screen (), obj->back, d->x, d->y, 0, 0, d->w, d->h);

         obj->first_draw = false;
         set_alpha_blender ();
         draw_sprite (gui_get_screen (), obj->back, d->x, d->y);
         draw_trans_sprite (gui_get_screen (), obj->bmp, d->x, d->y);
         if (d->d2 == 1)
           {
             alfont_set_font_size (obj->lbl_fnt, obj->fnt_size);
             alfont_textout_right_aa_ex (gui_get_screen (), obj->lbl_fnt, obj->label,
                                         d->x + obj->lbl_x, d->y + obj->lbl_y, d->fg,
                                         makeacol (0, 0, 0, 255));
             d->d2 = 0;
           }
	       fg = (d->flags & D_DISABLED) ? gui_mg_color : d->fg;
	       x = 0;

	       if (scroll)
           {
	           p = d->d1-b+1;
	           b = d->d1;
	         }
	       else
	         p = 0;

	       for (; p <= b; p++)
           {
	           f = ugetat (s, p);
	           usetc (buf + usetc (buf, (f) ? f : ' '), 0);
	           w = alfont_text_length (obj->edt_fnt, buf);
	           if (x + w > obj->input_w)
	             break;

             f = ((p == d->d1) && (d->flags & D_GOTFOCUS));
             alfont_set_font_size (obj->edt_fnt, obj->fnt_size);
	           alfont_textout_aa_ex (gui_get_screen (), obj->edt_fnt, buf,
                                   d->x + x + obj->input_x, d->y + obj->input_y,
                                   (f) ? d->bg : fg, (f) ? fg : makeacol (0, 0, 0, 255));
	           x += w;
	         }
	       break;

       case MSG_CLICK:
	       x = d->x;

	       if (scroll)
           {
	           p = d->d1 - b + 1;
	           b = d->d1;
	         }
      	 else
	         p = 0;

      	 for (; p < b; p++)
           {
	           usetc (buf + usetc (buf, ugetat (s, p)), 0);
	           x += alfont_text_length (obj->edt_fnt, buf);
	           if (x > gui_mouse_x ())
	             break;
	         }

         d->d1 = MID (0, p, l);
      	 object_message(d, MSG_DRAW, 0);
      	 break;

       case MSG_LOSTFOCUS:
         if (lostfocus_func != NULL)
           lostfocus_func (d);
       case MSG_WANTFOCUS:
//         while (gui_mouse_b () != 1 && gui_mouse_x () >= d->x
//                && gui_mouse_x () <= d->x + d->w && gui_mouse_y () >= d->y
//                && gui_mouse_y () <= d->y + d->h)
//           broadcast_dialog_message (MSG_IDLE, 0);
//         while (gui_mouse_b () == 1)
//           broadcast_dialog_message (MSG_IDLE, 0);
//         if (gui_mouse_x () < d->x || gui_mouse_x () > d->x + d->w
//             || gui_mouse_y () < d->y || gui_mouse_y () > d->y + d->h)
//           break;
       case MSG_KEY:
	       return D_WANTFOCUS;
	       break;

       case MSG_CHAR:
	       ignore_next_uchar = FALSE;

	       if ((c >> 8) == KEY_LEFT)
           {
	           if (d->d1 > 0)
               {
	               if (key_shifts & KB_CTRL_FLAG)
                   {
	               	   last_was_space = TRUE;
	  	               new_pos = 0;
		                 t = s;
		                 for (i = 0; i < d->d1; i++)
                       {
		                     k = ugetx (&t);
		                     if (uisspace (k))
		                 	     last_was_space = TRUE;
		                     else if (last_was_space)
                           {
			                       last_was_space = FALSE;
			                       new_pos = i;
		                       }
		                   }
		                 d->d1 = new_pos;
	                 }
	               else
	              	 d->d1--;
	             }
	         }
	       else if ((c >> 8) == KEY_RIGHT)
           {
	           if (d->d1 < l)
               {
	               if (key_shifts & KB_CTRL_FLAG)
                   {
		                 t = s + uoffset (s, d->d1);
		                 for (k = ugetx (&t); uisspace (k); k = ugetx (&t))
		                   d->d1++;
		                 for (; k && !uisspace (k); k = ugetx (&t))
		                   d->d1++;
	                 }
	               else
		               d->d1++;
	             }
	         }
	       else if ((c >> 8) == KEY_HOME)
	         d->d1 = 0;
      	 else if ((c >> 8) == KEY_END)
	         d->d1 = l;
      	 else if ((c >> 8) == KEY_DEL)
           {
	           if (d->d1 < l)
	           uremove (s, d->d1);
	         }
       	 else if ((c >> 8) == KEY_BACKSPACE)
           {
	           if (d->d1 > 0)
               {
	               d->d1--;
	               uremove (s, d->d1);
	             }
	         }
	       else if ((c >> 8) == KEY_ENTER)
           {
	           if (d->flags & D_EXIT)
               {
	               object_message(d, MSG_DRAW, 0);
	               return D_CLOSE;
	             }
	           else
	             return D_O_K;
	         }
	       else if ((c >> 8) == KEY_TAB)
           {
	           ignore_next_uchar = TRUE;
             return D_O_K;
	         }
	       else /* don't process regular keys here: MSG_UCHAR will do that */
	         break;

      	 object_message (d, MSG_DRAW, 0);
	       return D_USED_CHAR;

       case MSG_UCHAR:
	       if ((c >= ' ') && (uisok (c)) && (!ignore_next_uchar))
           {
	           if (l < obj->max_c)
               {
	               uinsert (s, d->d1, c);
	               d->d1++;

	               object_message (d, MSG_DRAW, 0);
	             }
	           return D_USED_CHAR;
	         }
	       break;
     }

   return D_O_K;
}

//----------------------------------------------------------------------------
// Ausgabe von Text
//----------------------------------------------------------------------------
int
d_gfxgui_textout_proc (int msg, DIALOG *d, int c)
{
  alfont_set_font_size ((ALFONT_FONT *) d->dp2, d->d2);
  if (msg == MSG_DRAW)
    {
      switch (d->d1)
        {
          case GFXGUI_TXT_ALIGN_LEFT:
            alfont_textout_aa_ex (gui_get_screen (), (ALFONT_FONT *) d->dp2,
                                  (char *) d->dp, d->x, d->y, d->fg, d->bg);
            break;
          case GFXGUI_TXT_ALIGN_CENTER:
            alfont_textout_centre_aa_ex (gui_get_screen (), (ALFONT_FONT *) d->dp2,
                                         (char *) d->dp, d->x, d->y, d->fg, d->bg);
            break;
          case GFXGUI_TXT_ALIGN_RIGHT:
            alfont_textout_right_aa_ex (gui_get_screen (), (ALFONT_FONT *) d->dp2,
                                        (char *) d->dp, d->x, d->y, d->fg, d->bg);
            break;
        }
    }
  return D_O_K;
}

#endif // ifndef GFX_GUI_H
