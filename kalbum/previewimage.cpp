/***************************************************************************
                          previewimage.cpp  -  description
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

#include <qimage.h>
#include <kpixmap.h>
#include <kimageeffect.h>
#include <kio/job.h>
#include <kio/netaccess.h>
#include <qstring.h>
#include <qlabel.h>
#include <qsize.h>

#include "previewimage.h"

#define MAX(A,B) (((A)>(B)) ? (A) : (B))

PreviewImage::PreviewImage(QWidget *parent, const char *name ) : PreviewImageDlg(parent,name) {}
PreviewImage::~PreviewImage() {}
/** No descriptions */
void PreviewImage::slotSelectionChanged(QPtrList <PhotoAlbum> photolist) {

    m_photo = photolist;

    QImage buffer;
    QString tmpfile;

    if ((m_photo.count() == 0) || (m_photo.count() > 1)) {

        //    PixmapLabel2->setEnabled(false);
        PixmapLabel2->setPixmap(NULL);
	      imageurl = "";
	      image.reset();

    } else if (m_photo.first()->fileurl != imageurl){

	    imageurl      = m_photo.first()->fileurl;
	    image_rotate  = m_photo.first()->rotate;
            
	    slotUpdatePreview(m_photo.first());
	     
	} else if (m_photo.first()->rotate != image_rotate){
	    
	    imageurl      = m_photo.first()->fileurl;
	    image_rotate  = m_photo.first()->rotate;
         
      slotUpdatePreview(m_photo.first());
	 	
	}
    
}

void PreviewImage::showPixmap(QImage buffer) {

    QPixmap buffer_pix;
    
    buffer = effectImage(buffer, image_rotate);
    buffer = scaleImage(buffer,PixmapLabel2->size());
    buffer_pix.convertFromImage(buffer, QPixmap::Color);
    PixmapLabel2->setPixmap(buffer_pix);
        

}



void PreviewImage::slotRotateImage(int rotate) {

    QImage buffer;

    image_rotate = rotate;

    if (!image.isNull()) {

        buffer = image;
        showPixmap(buffer);

    }

}

QImage PreviewImage::effectImage(QImage buffer, int rotate) {

    switch (rotate) {
    case  90:
        buffer = KImageEffect::rotate(buffer, KImageEffect::Rotate90);
        break;
    case 180:
        buffer = KImageEffect::rotate(buffer, KImageEffect::Rotate180);
        break;
    case 270:
        buffer = KImageEffect::rotate(buffer, KImageEffect::Rotate270);
        break;
    }

    return buffer;

}

QImage PreviewImage::scaleImage(QImage buffer, QSize size) {

    buffer = buffer.scale(size.width(), size.height(), QImage::ScaleMin);

    return buffer;

}

void PreviewImage::resizeEvent( QResizeEvent *resize) {

    QImage buffer;
    QPixmap buffer_pix;

    if (!image.isNull()) {

        buffer = image;
        showPixmap(buffer);

    }

}

void PreviewImage::slotUpdatePreview( PhotoAlbum *photo) {

    QImage buffer;
    QPixmap buffer_pix;
    QString tmpfile;
    
    if (is_visible){

	KIO::NetAccess::download( photo->fileurl, tmpfile);

	if (image.load(tmpfile)) {

	    buffer = image;
	    showPixmap(buffer);

	} else {

	    image = m_photo.first()->iconitem->pixmap()->convertToImage();
	    buffer = image;
	    showPixmap(buffer);

	}
    }

}

void PreviewImage::slotVisiblePreview(bool visible){
    
    QImage buffer;
    QPixmap buffer_pix;
    QString tmpfile;
    
    is_visible = visible;
    
    if (!m_photo.isEmpty())
	slotUpdatePreview(m_photo.first());
    
}    

