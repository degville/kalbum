/***************************************************************************
                          exportweb.h  -  description
                             -------------------
    begin                : Mon Oct 21 2002
    copyright            : (C) 2002 by Graham Morrison
    email                : kalbum@paldandy.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef EXPORTWEB_H
#define EXPORTWEB_H

#include <qwidget.h>
#include <exportwebdlg.h>
#include <qimage.h>

/**
  *@author Graham Morrison
  */

class ExportWeb : public exportWebDlg  {
   Q_OBJECT
public: 
	ExportWeb(QWidget *parent=0, const char *name=0);
	~ExportWeb();

  QImage image;
public slots:
  void slotChangeHue(int amount);
  void slotChangeSat(int amount);
  void slotToggle(bool on);

private:
  QImage image_rgb;
  static char * red_xpm[];



};

#endif
