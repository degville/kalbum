/***************************************************************************
                          photoalbum.cpp  -  description
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
#include <kpixmap.h>
#include <qimage.h>
#include <qfileinfo.h>

#include <kglobal.h>
#include <kstddirs.h>
#include <kmessagebox.h>
#include <kiconloader.h>
#include <kurllabel.h>
#include <kurlrequester.h>
#include <kimageeffect.h>
#include <klocale.h>
#include <qpixmap.h>
#include <qdatastream.h>

#include "photoalbum.h"
#include "kalbumnoimage.h"

#define PHTHMSZX	110
#define PHTHMSZY	 90

#define MAX(A,B) (((A)>(B)) ? (A) : (B))

PhotoAlbum::PhotoAlbum() {

    ignoredate = false;
    ignoretime = false;
    
}



PhotoAlbum::PhotoAlbum(QDataStream & stream, int version) {

    QString str_title, str_comment, str_fileurl, str_date;
    int year, month, day, hour, minute, second;
    Q_INT32 pix_xsize, pix_ysize;
    Q_INT32 pix_ignoredate, pix_rotate, pix_ignoretime;
    KPixmap pix_thumbnail;

    stream>>str_title;
    stream>>str_comment;
    stream>>str_fileurl;
    stream>>year;
    stream>>month;
    stream>>day;
    stream>>hour;
    stream>>minute;
    stream>>second;
    stream>>pix_rotate;
    stream>>pix_xsize;
    stream>>pix_ysize;
    stream>>pix_ignoredate;
    stream>>pix_ignoretime;
    stream>>pix_thumbnail;

    title 	= str_title;
    comment = str_comment;
    fileurl = str_fileurl;
    date.setYMD(year, month, day);
    time.setHMS(hour, minute, second);
    rotate = pix_rotate;
    xsize = pix_xsize;
    ysize = pix_ysize;
    ignoredate = pix_ignoredate;
    ignoretime = pix_ignoretime;

    thumbnail = pix_thumbnail;

}


void PhotoAlbum::writeToStream(QDataStream & stream) const {
    // Don't forget to add written elements to the copy() function

    stream<<title;
    stream<<comment;
    stream<<fileurl.url();
    stream<<date.year();
    stream<<date.month();
    stream<<date.day();
    stream<<time.hour();
    stream<<time.minute();
    stream<<time.second();
    stream<<(Q_INT32)rotate;
    stream<<(Q_INT32)xsize;
    stream<<(Q_INT32)ysize;
    stream<<(Q_INT32)ignoredate;
    stream<<(Q_INT32)ignoretime;
    stream<<thumbnail;
}

//PhotoAlbum::readFromStream(QDataStream & stream) const;


PhotoAlbum *PhotoAlbum::copy() {

    PhotoAlbum *photo = new PhotoAlbum;
    ;

    photo->fileurl 	= this->fileurl;
    photo->filesize 	= this->filesize;
    photo->date		= this->date;
    photo->time		= this->time;
    photo->thumbnail    = this->thumbnail;
    photo->selected	= this->selected;
    photo->comment	= this->comment;
    photo->title	= this->title;
    photo->iconitem	= NULL;
    photo->rotate       = this->rotate;
    photo->xsize        = this->xsize;
    photo->ysize        = this->ysize;
    photo->ignoredate   = this->ignoredate;
    photo->ignoretime   = this->ignoretime;

    return photo;

}


PhotoAlbum::~PhotoAlbum() {}



PhotoAlbum *PhotoAlbum::dataPhoto() {
    QImage buffer;
    KPixmap background;
    QString tmpfile;
    QString dir;
    int extpos;

    QString test;

    selected = true;

    KIO::NetAccess::download( fileurl, tmpfile);


    if (fileurl.isValid()) {
        if (thumbnail.isNull())
            updateThumbnail();

        QFileInfo finfo(tmpfile);

        filesize = finfo.size();
        if (!date.isValid())
            date = finfo.lastModified().date();
	
        if (time.isNull())
	        time = finfo.lastModified().time();

        filename = fileurl.fileName();
        extpos=filename.find(".");
        if (extpos)
            filename=filename.remove(extpos, (filename.length()-extpos));

        if (title.isEmpty())
            title=filename;

        if (comment.isEmpty())
            comment=filename;


        KIO::NetAccess::removeTempFile( tmpfile );
        return this;
    }

    return NULL;

}

QString PhotoAlbum::textEncode() {
    QString buffer;

    buffer = "Title:" + this->title + ":Title\n";
    buffer = buffer + "Date:" + this->date.toString() + ":Date\n";
    buffer = buffer + "URL:" + this->fileurl.path() + ":URL\n";
    buffer = buffer + "Comment:" + this->comment + ":Comment\n";

    return buffer;

}


PhotoAlbum *PhotoAlbum::textDecode(QString &buffer, int item) {
    int start, end;

    start=buffer.find("URL:", item);

    if (start) {
        end=buffer.find(":URL", item);
        this->title=buffer.mid(start, (end-start));
    } else
        return NULL;

    return this;
}

void PhotoAlbum::removePhoto(const KURL& url) {}

void PhotoAlbum::updateThumbnail() {
    QImage buffer;
    KPixmap background;
    QString dir;
    QString tmpfile;

    int thumbSizeX=PHTHMSZX;
    int thumbSizeY=PHTHMSZY;

    KIO::NetAccess::download( fileurl, tmpfile);


    if (fileurl.isValid()) {
        while (!buffer.load(tmpfile)) {
            //          KalbumNoImage *noimage = new KalbumNoImage;
            //          noimage->KURLLabel1->setText(fileurl.url());
            //          noimage->KURLRequester1->setURL(fileurl.url());
            //          noimage->KURLRequester1->setFilter(i18n("*.jpeg *.jpg *.gif *.xpm *.ppm *.pgm *.pnm *.png *.bmp *.tiff |Pictures\n*|All files"));
            //          if (noimage->exec()){
            //            fileurl = noimage->KURLRequester1->url();
            //            KIO::NetAccess::download( fileurl, tmpfile);
            //          } else {

            KGlobal::dirs()->addResourceType("kalbum_data", KGlobal::dirs()->kde_default("data")
                                             + "kalbum/data/");
            dir = KGlobal::dirs()->findResourceDir("kalbum_data", "icons/file_broken.png");
            dir = dir + "icons/file_broken.png";

            tmpfile = dir;
            //          }
        }


        KGlobal::dirs()->addResourceType("kalbum_data", KGlobal::dirs()->kde_default("data")
                                         + "kalbum/data/");

        dir = KGlobal::dirs()->findResourceDir("kalbum_data", "slides/slide01.png");
        dir = dir + "slides/slide01.png";

        background.load(dir);

        xsize = buffer.size().width();
        ysize = buffer.size().height();

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
        default:
            rotate = 0;
            break;
        }

        float s=MAX((float(buffer.size().width())/thumbSizeX), (float(buffer.size().height())/thumbSizeY));
        buffer=buffer.smoothScale(MAX(1,int(buffer.width()/s)),
                                  MAX(1,int(buffer.height()/s)));

        thumbnail.convertFromImage(buffer, QPixmap::Color);

        bitBlt(&background, (background.width()-thumbnail.width())/2,
               (background.height()-thumbnail.height())/2, &thumbnail, 0, 0,
               thumbnail.width(), thumbnail.height(), Qt::CopyROP, false);

        thumbnail=background;

        KIO::NetAccess::removeTempFile( tmpfile );

    }

}


