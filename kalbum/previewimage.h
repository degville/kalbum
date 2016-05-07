/***************************************************************************
                          previewimage.h  -  description
                             -------------------
    begin                : Mon Jan 20 2003
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

#ifndef PREVIEWIMAGE_H
#define PREVIEWIMAGE_H

#include <qwidget.h>
#include <previewimagedlg.h>
#include <qptrlist.h>
#include <kurl.h>

#include "photoalbum.h"

/**
  *@author Graham Morrison
  */

class PreviewImage : public PreviewImageDlg  {
   Q_OBJECT
public: 

	PreviewImage(QWidget *parent=0, const char *name=0);
	~PreviewImage();

public slots: // Public slots
  /** Emits the selected photo here when there's a selection change */
  void slotSelectionChanged(QPtrList <PhotoAlbum> photolist);
  void slotUpdatePreview(PhotoAlbum *photo);
  void slotRotateImage(int rotate);
  void slotVisiblePreview(bool visible);

private:
  void showPixmap(QImage buffer);
  bool is_visible;
  QPtrList<PhotoAlbum> m_photo;
  KURL imageurl;
  int image_rotate;
  QImage image;
  QImage effectImage(QImage buffer, int rotate);
  QImage scaleImage(QImage buffer, QSize size);
  void resizeEvent(QResizeEvent *resize);


};

#endif
