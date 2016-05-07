/***************************************************************************
                          kalbumviewicon.h  -  description
                             -------------------
    begin                : Wed Sep 11 2002
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

#ifndef KALBUMVIEWICON_H
#define KALBUMVIEWICON_H

#include <kiconview.h>

#include "kalbumiconview.h"

/**
  *@author Graham Morrison
  */


class QDragEnterEvent;
class QDragDropEvent;

class KalbumViewItem : public KIconViewItem  {
public:
	KalbumViewItem ( KIconView *parent) :
    KIconViewItem (parent) {}

  KalbumViewItem ( KIconView *parent, KalbumViewItem *after) :
    KIconViewItem (parent, after) {}
 
	KalbumViewItem ( KIconView *parent, const QString &text)
    : KIconViewItem (parent, text) {}

	KalbumViewItem ( KIconView *parent, KalbumViewItem *after, const QString &text) :
    KIconViewItem (parent, after, text) {}

	KalbumViewItem ( KIconView *parent, const QString &text, const QPixmap& icon) :
    KIconViewItem (parent, text, icon) {}

	KalbumViewItem ( KIconView *parent, KalbumViewItem *after, const QString &text, const QPixmap& icon) :
    KIconViewItem (parent, after, text, icon) {}

	KalbumViewItem ( KIconView *parent, const QString &text, const QPicture& picture) :
    KIconViewItem (parent, text, picture) {}

	KalbumViewItem ( KIconView *parent, KalbumViewItem *after, const QString &text, const QPicture& picture) :
    KIconViewItem (parent, after, text, picture) {}

  virtual int compare(QIconViewItem *i) const;


};

#endif
