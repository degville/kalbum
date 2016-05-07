/***************************************************************************
                          albuminfo.h  -  description
                             -------------------
    begin                : Tue Feb 11 2003
    copyright            : (C) 2003 by Graham Morrison
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

#ifndef ALBUMINFO_H
#define ALBUMINFO_H

#include <qwidget.h>
#include "albuminfodlg.h"

#include "kalbumcredits.h"

/**
  *@author Graham Morrison
  */

class AlbumInfo : public AlbumInfoDlg  {
   Q_OBJECT
public: 
	AlbumInfo(QWidget *parent=0, const char *name=0);
	~AlbumInfo();

public slots:
  void slotNewAlbum();
  void slotAlbumChanged(KalbumCredits album);
  void slotCancel();
  void slotAccept();
  void slotTextChanged();

signals:
  void modifiedAlbumInfo(KalbumCredits album);

private:
  void updateAlbum();
  KalbumCredits album_info;

};

#endif
