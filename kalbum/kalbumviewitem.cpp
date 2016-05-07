/***************************************************************************
                          kalbumviewicon.cpp  -  description
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

#include <qdatetime.h>

#include "kalbumviewitem.h"
#include "kalbumview.h"
#include "kalbum.h"
#include "photoalbum.h"

int KalbumViewItem::compare(QIconViewItem *i) const
{

    PhotoAlbum *s1;
    PhotoAlbum *s2;
    QString sortmode;
    QDateTime s1_dt, s2_dt;

    KalbumApp *win=(KalbumApp *) iconView()->parent();
    KalbumIconView *iconview=(KalbumIconView *) iconView();

    sortmode = iconview->getSortMode();

    s1 = win->view->getPhoto((KalbumViewItem *)this);
    s2 = win->view->getPhoto((KalbumViewItem *)i);

    s1_dt.setDate(s1->date);
    s1_dt.setTime(s1->time);

    s2_dt.setDate(s2->date);
    s2_dt.setTime(s2->time);

    if (sortmode=="Date Inc") {                 // By Date - Earliest first
   
        if ((s1_dt)<=(s2_dt) || (s1->ignoredate))
          return -1;
        else if (((s1_dt)>=(s2_dt)) && (!s1->ignoredate))
          return 1;
        else
          return 0;

    } else if (sortmode=="Date Dec") {          // By Date - Latest first

        if (((s1_dt)<=(s2_dt)) || (s1->ignoredate))
          return 1;
        else if (((s1_dt)>=(s2_dt)) && (!s1->ignoredate))
          return -1;
        else
          return 0;

    } else if (sortmode=="Title Inc") {         // By Title - Ascending from A

        if ((s1->title)<=(s2->title))
          return -1;
        else if (((s1->title)>=(s2->title)))
          return 1;
        else
          return 0;

    } else if (sortmode=="Title Dec") {         // By Title - Descending from Z

        if ((s1->title)<=(s2->title))
          return 1;
        else if (((s1->title)>=(s2->title)))
          return -1;
        else
          return 0;

    }

  return 0;


}

