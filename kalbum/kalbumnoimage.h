/***************************************************************************
                          kalbumnoimage.h  -  description
                             -------------------
    begin                : Thu Oct 17 2002
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

#ifndef KALBUMNOIMAGE_H
#define KALBUMNOIMAGE_H

#include <qwidget.h>
#include "kalbumnoimagedlg.h"

/**
  *@author Graham Morrison
  */

class KalbumNoImage : public KalbumNoImageDlg  {
   Q_OBJECT
public:

  KalbumNoImage(QWidget *parent=0, const char *name=0, bool modal = TRUE, WFlags f=0) :
    KalbumNoImageDlg(parent,name, modal, f) {}

	~KalbumNoImage();
};

#endif
