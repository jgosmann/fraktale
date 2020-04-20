/***************************************************************************
 * Filename: alFileDlg.h                                                   *
 * Begin: Mo May 23 18:47:54 CET 2005                                      *
 * Description: Bereitstellung des Systemeigenen Öffnen/Speicher-Dialogs   *
 *              für Linux und Windows                                      *
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

#ifndef ALFILEDLG_H
#define ALFILEDLG_H 1

/***** Includes *****/
#include <iostream>
#include <string>
#include <cstring>
#include <vector>

#ifdef __WIN32__
 #include <allegro.h> // Hier muss Allegro zu erst inkludiert werden...
 #include <winalleg.h>
 #include <windows.h>
 #include <commdlg.h>
 #include <winuser.h>
 #include <winbase.h>
#elif __unix__
 #include <qapplication.h>
 #include <qfiledialog.h>
 #include <qfileinfo.h>
 #include <qmessagebox.h>
 #include <allegro.h> // ...und hier zu letzt
#endif

/***** Defines *****/
#ifdef __WIN32__
 #ifndef MAX_PATH
  #define MAX_PATH 512
 #endif
#endif

#ifndef EXIT_FAILURE
 #define EXIT_FAILURE 1
#endif

/***** Misc *****/
using namespace std;

/***** Structs *****/
//----------------------------------------------------------------------------
// Spezifikation eines Dateityps für die Dateitypenliste des Dialogs
//----------------------------------------------------------------------------
struct file_type {
  string name; // Name
  vector <string> endings; // Mögliche Endungen
};

//----------------------------------------------------------------------------
// Automatisches Anhängen der Dateiendung
// Eingabe:   file = Dateiname
//          filter = Filter für die Dateiendung
//----------------------------------------------------------------------------
void
auto_append_ending (string &file, file_type &filter)
{
  /*** Variables ***/
  unsigned int i; // Zählvariable

  /*** Commands ***/
  if (filter.endings[0] == "") // Wenn Filter *.*
    return;

  for (i = 0; i < filter.endings.size (); i++)
    {
      if (filter.endings[i].size () + 1 >= file.size ())
        continue;
      if (file.compare (file.size () - filter.endings[i].size () - 1,
          filter.endings[i].size () + 1, "." + filter.endings[i]) == 0)
        return;
    }

  file += "." + filter.endings[0];
}

/***** Functions *****/
//----------------------------------------------------------------------------
// file_dialog (vector <file_type> &file_type_list, bool save);
// Der eigentliche Dialog
// Eingabe: file_type_list = Dateitypen
//                    save = Wenn true wird ein Speicherndialog gezeigt
// Rückgabe: Die gewählte Datei oder ""
//----------------------------------------------------------------------------

#ifdef __WIN32__

string
file_dialog (vector <file_type> &file_type_list, bool save)
{
  /*** Variables ***/
  OPENFILENAME dlg_data; // Daten des Dialogs
  char file[MAX_PATH], path[MAX_PATH]; // Datei und Pfad
  string stlstr_file; // Dateiname als STL-String
  TCHAR filter[512]; // Filter (konvertiert)
  const unsigned int W = SCREEN_W, H = SCREEN_H; // Bildschirmgröße
  const int WINDOWED = is_windowed_mode (); // Fenster-/Vollbildmodus?
  BITMAP *screen_bck; // Inhalt des Bildschirms
  unsigned int i, j, k, l; // Zählvariablen

  /*** Commands ***/
  file[0] = '\0';
  path[0] = '\0';

  for (i = 0, k = 0; i < file_type_list.size (); i++)
    {
      for (l = 0; l < file_type_list[i].name.size () && k < 507; k++, l++)
        filter[k] = file_type_list[i].name[l];
      filter[k++] = '\0';

      for (j = 0; j < file_type_list[i].endings.size (); j++)
        {
          if (file_type_list[i].endings[j] != "")
            {
              for (l = 0; l < file_type_list[i].endings[j].size () + 2 && k < 507;
                   k++, l++)
                {
                  if (l == 0)
                    filter[k] = '*';
                  else if (l == 1)
                    filter[k] = '.';
                  else
                    filter[k] = file_type_list[i].endings[j][l - 2];
                }
            }
          else
            filter[k++] = '*';

          if (j != file_type_list[i].endings.size () - 1)
            filter[k++] = ';';
          else
            filter[k++] = '\0';
        }
    }
  filter[k++] = '\0';
  filter[k] = '\0'; // Zur Sicherheit, falls Konvertierungsschleife vorzeitig abbrach.

  ZeroMemory (&dlg_data, sizeof (dlg_data));
  dlg_data.lStructSize = sizeof (dlg_data);
  dlg_data.hwndOwner = win_get_window ();
  dlg_data.nMaxFile =  MAX_PATH;
  dlg_data.lpstrFile = path;
  dlg_data.lpstrFilter = filter;
  dlg_data.nFilterIndex = 0;
  dlg_data.nMaxFileTitle = MAX_PATH;
  dlg_data.lpstrFileTitle = file;
  dlg_data.lpstrInitialDir = NULL;
  //dlg_data.lpstrDefExt = file_type_list[0].endings[0].c_str ();

  if (WINDOWED == 0)
    {
      screen_bck = create_bitmap (W, H);
      if (screen_bck == NULL)
        {
          cout << "alFileDlg: Error while backuping the screen!" << endl;
          exit (EXIT_FAILURE);
        }
      scare_mouse ();
      draw_sprite (screen_bck, screen, 0, 0);
        
      if (set_gfx_mode (GFX_TEXT, 0, 0, 0, 0) < 0)
        {
          destroy_bitmap (screen_bck);
          cerr << "alFileDlg: The graphics mode could not be changed!" << endl;
          cerr << "Following error occured: " << allegro_error << endl;
          exit (EXIT_FAILURE);
        }
    }
  remove_keyboard();

  do
    {
      if(!save)
        {
          dlg_data.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY
                           | OFN_NOCHANGEDIR;

          if (GetOpenFileName (&dlg_data) == 0)
            {
              dlg_data.lpstrFile = "";
              break;
            }
        }
      else
        {
          dlg_data.Flags = OFN_HIDEREADONLY | OFN_NOCHANGEDIR;

          if (GetSaveFileName (&dlg_data) == 0)
            {
              dlg_data.lpstrFile = "";
              break;
            }
        }

      stlstr_file = dlg_data.lpstrFile;
      auto_append_ending (stlstr_file, file_type_list[dlg_data.nFilterIndex - 1]);
      strcpy (dlg_data.lpstrFile, stlstr_file.c_str ());

      if (save && GetFileAttributes (dlg_data.lpstrFile) != 0xFFFFFFFF)
        {
          if (MessageBox (win_get_window (), "The file exists already. Overwrite it?",
              "Overwrite?", MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2) == IDYES)
	          break;
	      }
    }
  while (save && GetFileAttributes (dlg_data.lpstrFile) != 0xFFFFFFFF);

  if (install_keyboard () < 0)
    cerr << "Reinitialisation of keyboard failed! Anyway you will be able to"
         << " use the mouse." << endl;
  if (WINDOWED == 0)
    {
      if (set_gfx_mode (GFX_AUTODETECT_FULLSCREEN, W, H, 0, 0) < 0)
        {
          destroy_bitmap (screen_bck);
          cerr << "alFileDlg: The graphics mode could not be changed!" << endl;
          cerr << "Following error occured: " << allegro_error << endl;
          exit (EXIT_FAILURE);
        }
      draw_sprite (screen, screen_bck, 0, 0);
      unscare_mouse ();
      destroy_bitmap (screen_bck);
    }

  return dlg_data.lpstrFile;
}

#elif __unix__

QApplication *qt_app = NULL;

string
file_dialog (vector <file_type> &file_type_list, bool save)
{
  /*** Variables ***/
  QFileDialog *file_dlg; // Der File Dialog
  QFileInfo *file_info; // Zum Prüfen, ob Datei existiert
  string file; // Ausgewählte Datei
  const unsigned int W = SCREEN_W, H = SCREEN_H; // Bildschirmgröße
  const int WINDOWED = is_windowed_mode (); // Fenster-/Vollbildmodus?
  BITMAP *screen_bck; // Inhalt des Bildschirms
  unsigned int i; // Zählvariablen

  /*** Commands ***/
  if (qt_app == NULL)
    {
      int qt_argc = 0;
      char **qt_argv;
      qt_app = new QApplication (qt_argc, qt_argv); // Damit getOpenFileName
                                                    // aufgerufen werden kann.
    }

  file_dlg = new QFileDialog ();

  file_dlg->setFilter (file_type_list[0].name.c_str ());

  for (i = 1; i < file_type_list.size (); i++)
    file_dlg->addFilter (file_type_list[i].name.c_str ());

  file_dlg->setSelectedFilter (0);

  if (WINDOWED == 0)
    {
      screen_bck = create_bitmap (W, H);
      if (screen_bck == NULL)
        {
          cout << "alFileDlg: Error while backuping the screen!" << endl;
          exit (EXIT_FAILURE);
        }
      show_mouse (NULL);
      blit (screen, screen_bck, 0, 0, 0, 0, W, H);

      if (set_gfx_mode (GFX_TEXT, 0, 0, 0, 0) < 0)
        {
          destroy_bitmap (screen_bck);
          cerr << "alFileDlg: The graphics mode could not be changed!" << endl;
          cerr << "Following error occured: " << allegro_error << endl;
          exit (EXIT_FAILURE);
        }
    }

  if (!save)
    file_dlg->setMode (QFileDialog::ExistingFile);
  else
    file_dlg->setMode (QFileDialog::AnyFile);

  file_info = new QFileInfo ();

  do
    {
      if (file_dlg->exec () == QDialog::Accepted)
        file = (file_dlg->selectedFile ()).ascii ();
      else
        {
          file = "";
	        break;
	      }

      for (i = 0; i < file_type_list.size ()
           && file_type_list[i].name.c_str () != file_dlg->selectedFilter ();
	   i++) { }

      auto_append_ending (file, file_type_list[i]);

      file_info->setFile (file.c_str ());

      if (save && file_info->exists ())
        {
          if (QMessageBox::question (0, "Overwrite?", "The file exists"
	            " already. Overwrite it?", QMessageBox::Yes, QMessageBox::No)
	            == QMessageBox::Yes)
	          break;
	      }
    }
  while (save && file_info->exists ());

  delete file_info;
  delete file_dlg;

  if (WINDOWED == 0)
    {
      if (set_gfx_mode (GFX_AUTODETECT_FULLSCREEN, W, H, 0, 0) < 0)
        {
          destroy_bitmap (screen_bck);
          cerr << "alFileDlg: The graphics mode could not be changed!" << endl;
          cerr << "Following error occured: " << allegro_error << endl;
          exit (EXIT_FAILURE);
        }
      draw_sprite (screen, screen_bck, 0, 0);
      show_mouse (screen);
      destroy_bitmap (screen_bck);
    }

  clear_keybuf ();
  return file;
}

#endif

#endif // ALFILEDLG_H
