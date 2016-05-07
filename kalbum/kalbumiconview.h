/***************************************************************************
                          kalbumiconview.h  -  description
                             -------------------
    begin                : Fri Oct 11 2002
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

#ifndef KALBUMICONVIEW_H
#define KALBUMICONVIEW_H

#include <qwidget.h>
#include <kiconview.h>

#include "kalbumviewitem.h"
#include "kalbum.h"

/**
  *@author Graham Morrison
  */

class QDragEnterEvent;
class QDragDropEvent;
class QDragMoveEvent;

class KalbumIconView : public KIconView  {
   friend class KalbumViewItem;
   Q_OBJECT
public: 
	KalbumIconView(QWidget *parent=0, const char *name=0);
	~KalbumIconView();

  void setSortMode(QString mode);

  // Returns the Icon before the current cursor position
  QIconViewItem *getPosition();
  QIconViewItem *getPosition(QPoint pos);

signals:
  void dropped(QDropEvent *, int);

protected:
  QDragObject* dragObject();

  KalbumApp *getApp();
  QString getSortMode();
  void deleteCursor();
  virtual void contentsDropEvent(QDropEvent *event);
  virtual void contentsDragLeaveEvent(QDragLeaveEvent *event);
  virtual void contentsDragMoveEvent(QDragMoveEvent *event);
public slots:

private:
  int drop_index;
  QString sortmode;
  QPixmap cursor_image;
  QIconViewItem *cursor;
  bool first;
  bool visible_cursor;
  static char * cursor_xpm[];
   
};

#endif
