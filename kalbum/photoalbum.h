/***************************************************************************
                          photoalbum.h  -  description
                             -------------------
    begin                : Mon Jan 28 2002
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

#ifndef PHOTOALBUM_H
#define PHOTOALBUM_H

#include <kpixmap.h>
#include <kiconview.h>
#include <qdatetime.h>
#include <qpoint.h>
#include <kio/job.h>
#include <kio/netaccess.h>
#include <qptrlist.h>
#include <kiconeffect.h>

#include "kalbumviewitem.h"

class QDataStream;

/**
  *@author Graham Morrison
  */

class PhotoAlbum {
public:
    PhotoAlbum();
    PhotoAlbum(QDataStream & stream, int version);
    ~PhotoAlbum();

    PhotoAlbum *dataPhoto();
    void removePhoto(const KURL& url);
    QString textEncode();
    PhotoAlbum *textDecode(QString &buffer, int item);
    //	void readFromStream(QDataStream & stream) const;
    void writeToStream(QDataStream & stream) const;
    PhotoAlbum *copy();
    void updateThumbnail();

    QString filename;
    KURL fileurl;
    int filesize, xsize, ysize, rotate;
    QDate date;
    QTime time;
    KPixmap thumbnail;
    bool selected;
    bool ignoredate, ignoretime;
    QString comment,title, key;
    QPoint pos;
    KalbumViewItem *iconitem;

};

#endif
