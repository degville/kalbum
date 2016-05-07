/***************************************************************************
                          kalbumiconview.cpp  -  description
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

#include <qptrlist.h>
#include <kurldrag.h>

#include "kalbumiconview.h"
#include "kalbumview.h"
#include "kalbum.h"
#include "kalmimesource.h"
#include "photoalbum.h"
#include "kalbumviewitem.h"

KalbumIconView::KalbumIconView(QWidget *parent, const char *name ) : KIconView(parent,name) {

  sortmode = "Date Inc";
  visible_cursor = false;
  first = false;
  drop_index = -1;
  
  static char * cursor_xpm[] = {
    "2 120 2 1"," 	c None", ".	c #000000","..","..","..","..","..","..","..","..","..","..","..","..","..","..",
    "..","..","..","..","..","..","..","..","..","..","..","..","..","..","..","..","..","..","..","..","..","..",
    "..","..","..","..","..","..","..","..","..","..","..","..","..","..","..","..","..","..","..","..","..","..",
    "..","..","..","..","..","..","..","..","..","..","..","..","..","..","..","..","..","..","..","..","..","..",
    "..","..","..","..","..","..","..","..","..","..","..","..","..","..","..","..","..","..","..","..","..","..",
    "..","..","..","..","..","..","..","..","..","..","..","..","..","..","..","..","..",".."};
   

  cursor_image = cursor_xpm;

}
KalbumIconView::~KalbumIconView(){
}

QDragObject* KalbumIconView::dragObject()
{
  QPtrList<PhotoAlbum> m_photo;

  KalbumApp *win=(KalbumApp *) parent();
  m_photo = win->view->getPhotoData();


  KalMimeSource * mime_source = new KalMimeSource(m_photo);

  QStoredDrag *drag = new QStoredDrag("application/kalbum",this);
  drag->setEncodedData(mime_source->encodedData("application/kalbum"));

  return drag;

      
}

KalbumApp *KalbumIconView::getApp()
{

  KalbumApp *win=(KalbumApp *) parent();

  return win;


}

QString KalbumIconView::getSortMode()
{

  return sortmode;

}

void KalbumIconView::setSortMode(QString mode)
{

  sortmode = mode;

}

void KalbumIconView::contentsDragMoveEvent(QDragMoveEvent * event)
{


  if ((event->provides("application/kalbum")) ||(KURLDrag::canDecode(event)))
  {
    if (this->count() == 0) {
      cursor = new QIconViewItem (this,"", cursor_image);
      visible_cursor = true;
    } else if (this->count()>=1) {                                               // As long as there are more icons than the cursor
      if (visible_cursor) {
        if (getPosition(event->pos())){                                         // If a destination can be found
          if ( (getPosition(event->pos())) != (cursor) ){                       // If we're not just overwriting the cursor
            deleteCursor();
                                                                                
            if (first){                                                         // If the cursor needs to be inserted at the beginning
              PhotoAlbum *photo = new PhotoAlbum;
              KalbumApp *win=(KalbumApp *) parent();
              photo = win->view->getPhoto((KalbumViewItem *)this->firstItem());
              bool isselected;

              isselected = this->firstItem()->isSelected();
              cursor = new QIconViewItem (this, this->firstItem(), "", cursor_image);

              delete this->firstItem();
              photo->iconitem = new KalbumViewItem( this, (KalbumViewItem *) this->firstItem(), photo->title, photo->thumbnail);
              photo->iconitem->setSelected(isselected);
            } else {                                           
              cursor = new QIconViewItem (this, getPosition(event->pos()), "", cursor_image);
            }

            visible_cursor = true;
             
          }
        }
      } else if (getPosition(event->pos())){                                    // ...else when there's no cursor, and a position can be found

        if (first){                                                             // If the cursor needs to be inserted at the beginning
          PhotoAlbum *photo = new PhotoAlbum;
          KalbumApp *win=(KalbumApp *) parent();
          photo = win->view->getPhoto((KalbumViewItem *)this->firstItem());
          bool isselected;

          isselected = this->firstItem()->isSelected();
          cursor = new QIconViewItem (this, this->firstItem(), "", cursor_image);

          delete this->firstItem();
          photo->iconitem = new KalbumViewItem( this, (KalbumViewItem *) this->firstItem(), photo->title, photo->thumbnail);
          photo->iconitem->setSelected(isselected);
        } else {
           cursor = new QIconViewItem (this, getPosition(event->pos()), "", cursor_image);
        }
                                                                                                                                                   
        visible_cursor = true;
      }
    }
    event->acceptAction();
  }
  //   event->ignore();      
  
}

void KalbumIconView::deleteCursor(){

  if (visible_cursor){
    delete cursor;
    visible_cursor = false;
    arrangeItemsInGrid();
  }

}

QIconViewItem *KalbumIconView::getPosition(QPoint pos)
{
  int width = this->firstItem()->width();
  QPoint destl = pos;
  QPoint destr = pos;

  destl.setX((destl.x()) - (width));
  destr.setX((destr.x()) + (width));

  if ((this->findItem(pos)) == cursor){                     // Just ignore if pointer is over the cursor

    return 0;

  } else if ((this->findItem(pos)) == (this->firstItem())){ // If the pointer is over the first item

    first = true;
    return this->firstItem();

  } else if (this->findItem(pos)){              // If the pointer is over a thumbnail, return the previous thumbnail

    first = false; 
    return this->findItem(pos)->prevItem();

  } else if (this->findItem(destl)){          // If there is a thumbnail before pointer, return the thumbnail

    first = false;
    return this->findItem(pos);

  } else if (this->findItem(destr)){          // If there is a thumbnail after pointer, return the previous one

    first = false;
    return this->findItem(destr)->prevItem();

  } else {

    return this->lastItem();

  }
}
    

QIconViewItem *KalbumIconView::getPosition()
{
  if (visible_cursor)
    return cursor->prevItem();
  else
    return 0;

}

void KalbumIconView::contentsDragLeaveEvent(QDragLeaveEvent * event)
{

  deleteCursor();

}

void KalbumIconView::contentsDropEvent(QDropEvent * event)
{

  drop_index = cursor->index();
  deleteCursor();
  emit dropped(event, drop_index);



}

/* XPM */
