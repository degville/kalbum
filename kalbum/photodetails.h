/***************************************************************************
                          photodetails.h  -  description
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

#ifndef PHOTODETAILS_H
#define PHOTODETAILS_H

#include <qwidget.h>
#include <photodetailsdlg.h>
#include <kpixmap.h>
#include <qptrlist.h>
#include <kurl.h>

#include "photoalbum.h"

/**
  *@author Graham Morrison
  */

class PhotoDetails : public PhotoDetailsDlg  {
   Q_OBJECT
public: 
	PhotoDetails(QWidget *parent=0, const char *name=0);
	~PhotoDetails();

public slots:
  void slotSelectionChanged(QPtrList <PhotoAlbum> photolist);
	void slotNextProp();
  void slotPrevProp();
  void slotTextChanged();
  void slotRotate();
  void slotCancel();
  void slotAccept();
  void slotRotate90();
  void slotRotate180();
  void slotRotate270();
  void slotChangeURL();
  void slotModifiedTitle();
  void slotModifiedDate();
  void slotModifiedDateBool();
  void slotModifiedTime();
  void slotModifiedTimeBool();
  void slotModifiedRotate();
  void slotModifiedURL();
  void slotModifiedComment();

signals:
  void nextImage();
  void prevImage();
  void setModified();
  void rotateImage(int rotate);
  void updatePreview(PhotoAlbum *photo);
  void modifiedImage(PhotoAlbum *photo, int selected);


private:

	QPtrList<PhotoAlbum> m_photo;
  void updateImageInfo();
  void updateSingleImage();
  void updateMultipleImage();
  void modifyImage();
  void modifySingleImage();
  void modifyMultipleImage();
  void disableFields();
  void enableFields();
  KPixmap blendIcon(QPixmap *icon);
  KPixmap slide;
  QImage slides;
  KURL oldfileurl;
  bool modifiedtitle;
  bool modifieddate;
  bool modifieddatebool;
  bool modifiedtime;
  bool modifiedtimebool;
  bool modifiedrotate;
  bool modifiedurl;
  bool modifiedcomment;


};

#endif
