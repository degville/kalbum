/***************************************************************************
                          kalbumview.cpp  -  description
                             -------------------
    begin                : Tue Jan 22 14:13:58 CET 2002
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

// include files for Qt
#include <qprinter.h>
#include <qpainter.h>

// Kalbum
#include <stdio.h>
#include <kmessagebox.h>
#include <kiconview.h>
#include <kio/job.h>
#include <kio/netaccess.h>
#include <qptrlist.h>
#include <kpixmap.h>
#include <qmultilineedit.h>
#include <qlineedit.h>
#include <qdatetime.h>
#include <kurl.h>
#include <kurldrag.h>
#include <kiconeffect.h>
#include <klocale.h>
#include <qiconview.h>
#include <qclipboard.h>
#include <qdatastream.h>
#include <qtextstream.h>
#include <qfile.h>
#include <qfileinfo.h>
#include <ksimpleconfig.h>
#include <qbuffer.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <kiconloader.h>
#include <qradiobutton.h>
#include <klineedit.h>
#include <qdir.h>
#include <kimageeffect.h>
#include <qslider.h>
#include <kurlrequester.h>
#include <qradiobutton.h>
#include <kpixmap.h>
#include <kstddirs.h>
#include <kprocess.h>


// application specific includes
#include "kalbumview.h"
#include "kalbumdoc.h"
#include "kalbum.h"
#include "photoalbum.h"
#include "kalmimesource.h"
#include "exportweb.h"
#include "kalbumviewitem.h"
#include "previewimage.h"

#define MAX(A,B) (((A)>(B)) ? (A) : (B))
#define ITEMPIXX	350
#define ITEMPIXY	350
#define PHOTOSZX	450
#define PHOTOSZY  320
#define THUMBSZX	142  // html thumbnail size -
#define THUMBSZY	126  // for the icon view, the size is set withint the photoalbum class
#define THUMBFMT	"PNG"
#define PHOTOFMT	"JPEG"
#define MAXPHOTO	4 // Number of zeros for inserting photo numbers into HTML

#define THUMBHTM	"thm_"
#define PHOTOHTM	"pho_"

#define KWDPHCNT	"<!-- {Kalbum Photos="
#define KWDTHCNT	"<!-- {Kalbum Thumbs="
#define KWDVERSI	"<!-- {Kalbum Vers="
#define KWDTHUMB	"<!-- {Kalbum Thumbnail="
#define KWDTITLE  "{Kalbum Title="

// Pop Up Menu Number defaults
#define POPUPCUT  1
#define POPUPCPY  2
#define POPUPPST  3
#define POPUPLFT  4
#define POPUPRGT  5
#define POPUPEDT  6

QPtrList<PhotoAlbum> *KalbumView::album = 0L;
KalbumCredits KalbumView::credits;

KalbumView::KalbumView(QWidget *parent, const char *name) :KalbumViewDlg(parent, name) {

    setBackgroundMode(PaletteBase);
    setAcceptDrops(true);

    if (!album) {
        album = new QPtrList<PhotoAlbum>;
    }

    album->setAutoDelete(true);

    popIconView = new KPopupMenu();
    popIconView->insertItem(SmallIconSet("editcut"),i18n("Cut"),this,SLOT(slotCut()), 1, POPUPCUT);
    popIconView->insertItem(SmallIconSet("editcopy"),i18n("Copy"),this,SLOT(slotCopy()), 2, POPUPCPY);
    popIconView->insertItem(SmallIconSet("editpaste"),i18n("Paste"),this,SLOT(slotPaste()), 3, POPUPPST);
    popIconView->insertSeparator(4);
    popIconView->insertItem(SmallIconSet("back"),i18n("Move Left"),this,SLOT(slotMoveLeft()), 5, POPUPLFT);
    popIconView->insertItem(SmallIconSet("forward"),i18n("Move Right"),this,SLOT(slotMoveRight()), 6, POPUPRGT);

    popIconView->setItemEnabled(POPUPCPY, false);
    popIconView->setItemEnabled(POPUPCUT, false);

    popIconView->setItemEnabled(POPUPLFT, false);
    popIconView->setItemEnabled(POPUPRGT, false);
    popIconView->setItemEnabled(POPUPEDT, false);

}

KalbumView::~KalbumView() {}


KalbumDoc *KalbumView::getDocument() const {
    KalbumApp *theApp=(KalbumApp *) parentWidget();

    return theApp->getDocument();
}

void KalbumView::updateProgress() {

    emit incProgress();
    KIconView1->arrangeItemsInGrid();

}

void KalbumView::updateProgress(int current) {

    emit setProgress(current);

}


void KalbumView::updateProgress(int current, int max) {

    emit resetProgress(current, max);

}




void KalbumView::print(QPrinter *pPrinter) {
    QPainter printpainter;
    printpainter.begin(pPrinter);

    // TODO: add your printing code here

    printpainter.end();
}

/** Returns Photo data for selected Icon */
PhotoAlbum *KalbumView::getPhoto(KalbumViewItem *chosen) {
    PhotoAlbum *photo;

    for (photo=album->first(); photo!=0; photo=album->next()) {
        if (chosen == photo->iconitem) {
            return photo;
        }
    }

    return photo;


}

PhotoAlbum *KalbumView::nextPhoto(PhotoAlbum *photo) {
    PhotoAlbum *tmp_photo;

    tmp_photo = getPhoto((KalbumViewItem *)photo->iconitem->nextItem());
    if (tmp_photo)
        return tmp_photo;
    else
        return getPhoto((KalbumViewItem *)photo->iconitem->iconView()->firstItem());

}

PhotoAlbum *KalbumView::prevPhoto(PhotoAlbum *photo) {
    PhotoAlbum *tmp_photo;

    tmp_photo = getPhoto((KalbumViewItem *)photo->iconitem->prevItem());
    if (tmp_photo)
        return tmp_photo;
    else
        return getPhoto((KalbumViewItem *)photo->iconitem->iconView()->lastItem());

}


/** Adds a new photo into the album with an URL as an argument */
bool KalbumView::addPhotoItem(const KURL& url) {
    PhotoAlbum *photo = new PhotoAlbum;
    photo->fileurl=url;

    photo=photo->dataPhoto();

    if (photo!=NULL) {
        photo->iconitem = new KalbumViewItem( KIconView1, photo->title, photo->thumbnail);
        album->append(photo);
    }

    return true;

}

/** Overloaded for PhotoAlbum - Inserts a photo */
bool KalbumView::addPhotoItem(const KURL& url, PhotoAlbum *before) {
    PhotoAlbum *photo = new PhotoAlbum;
    photo->fileurl=url;
    photo=photo->dataPhoto();

    if (photo!=NULL) {
        if (before == album->first()) {
            photo->iconitem = new KalbumViewItem( KIconView1, (KalbumViewItem *)KIconView1->firstItem(), photo->title, photo->thumbnail);
            album->insert(0, photo);

            PhotoAlbum *tmpphoto = new PhotoAlbum;
            tmpphoto = getPhoto((KalbumViewItem *)KIconView1->firstItem());

            delete KIconView1->firstItem();
            tmpphoto->iconitem = new KalbumViewItem( KIconView1, (KalbumViewItem *)KIconView1->firstItem(), tmpphoto->title, tmpphoto->thumbnail);

        } else {
            int position;
            position = album->find(before);
            photo->iconitem = new KalbumViewItem( KIconView1, (KalbumViewItem *)before->iconitem->prevItem(), photo->title, photo->thumbnail);
            album->insert(position, photo);
        }


    }

    slotUpdateTotal();

    return true;

}


/** Overloaded for PhotoAlbum */
bool KalbumView::addPhotoItem(PhotoAlbum *photo) {

    photo=photo->dataPhoto();

    if (photo!=NULL) {
        photo->iconitem = new KalbumViewItem( KIconView1, photo->title, photo->thumbnail);
        album->append(photo);
    }

    slotUpdateTotal();

    return true;
}

/** Overloaded for PhotoAlbum - Inserts a photo */
bool KalbumView::addPhotoItem(PhotoAlbum *photo, PhotoAlbum *before) {
    photo=photo->dataPhoto();

    if (photo!=NULL) {
        if (before == album->first()) {
            photo->iconitem = new KalbumViewItem( KIconView1, (KalbumViewItem *)KIconView1->firstItem(), photo->title, photo->thumbnail);
            album->insert(0, photo);

            PhotoAlbum *tmpphoto = new PhotoAlbum;
            tmpphoto = getPhoto((KalbumViewItem *)KIconView1->firstItem());

            delete KIconView1->firstItem();
            tmpphoto->iconitem = new KalbumViewItem( KIconView1, (KalbumViewItem *)KIconView1->firstItem(), tmpphoto->title, tmpphoto->thumbnail);

        } else {
            int position;
            position = album->find(before);
            photo->iconitem = new KalbumViewItem( KIconView1, (KalbumViewItem *)before->iconitem->prevItem(), photo->title, photo->thumbnail);
            album->insert(position, photo);
        }


    }

    slotUpdateTotal();

    return true;

}

void KalbumView::deletePhotoItem(PhotoAlbum *photo) {

    delete photo->iconitem;
    album->remove
    (photo);

}


void KalbumView::deleteAll() {
    PhotoAlbum *photo = new PhotoAlbum;

    if (album!=NULL) {
        for (photo=album->first(); photo!=0; photo=album->next()) {
            delete photo->iconitem;
            album->remove
            (photo);
            photo=album->first();
        }

        for (photo=album->first(); photo!=0; photo=album->next()) {
            delete photo->iconitem;
            album->remove
            (photo);

        }
        KIconView1->arrangeItemsInGrid();
    }

}


void KalbumView::slotPhotoProp(QIconViewItem *i) {


    PhotoAlbum *photo = new PhotoAlbum;
    QPtrList<PhotoAlbum> photolist;


    for (photo=album->first(); photo!=0; photo=album->next()) {
        if (photo->iconitem->isSelected()) {
            PhotoAlbum *phototmp = new PhotoAlbum;
            phototmp = photo;
            photolist.append(phototmp);

        }

    }

    emit executedIcon();
    emit selectionChanged(photolist);


}

void KalbumView::slotModifiedImage (PhotoAlbum *photo_tmp, int selected) {

    /*
      PhotoAlbum *photo = new PhotoAlbum;
      
      for (photo=album->first(); photo!=0; photo=album->next())
      {
        if (photo->iconitem->isSelected())
        {
          if (selected == 1)
          {
            photo->comment    = photo_tmp->comment;
            photo->title      = photo_tmp->title;
            photo->date       = photo_tmp->date;
            photo->ignoredate = photo_tmp->ignoredate;
            photo->iconitem->setText(photo_tmp->title);
            selected = 0;
          } else if (selected > 1)
          {
            if (photo_tmp->comment != "")
              photo->comment    = photo_tmp->comment;
            if (photo_tmp->title != ""){
              photo->title      = photo_tmp->title;
              photo->iconitem->setText(photo_tmp->title);
            }
    //        photo->date       = photo_tmp->date;
    //        photo->ignoredate = photo_tmp->ignoredate;
     
          }
        }
      }
     
      KIconView1->arrangeItemsInGrid();
      emit setModified();
     
    */

}


void KalbumView::slotAlbumChanged(KalbumCredits album) {

  credits.albumName 				= album.albumName;
  credits.webAddress 				= album.webAddress;
  credits.authorName				=	album.authorName;
  credits.authorEmail				=	album.authorEmail;
  credits.startDate					= album.startDate;
  credits.endDate						= album.endDate;
  credits.albumDescription	=	album.albumDescription;
  credits.ignoredates       = album.ignoredates;

  emit setModified();
  
}


QPtrList<PhotoAlbum> KalbumView::getPhotoData() {
    PhotoAlbum *photo = new PhotoAlbum;
    QPtrList<PhotoAlbum> photolist;


    for (photo=album->first(); photo!=0; photo=album->next()) {
        if (photo->iconitem->isSelected()) {
            PhotoAlbum *phototmp = new PhotoAlbum;
            phototmp = photo->copy();
            photolist.append(phototmp);

        }

    }


    return photolist;


}


/** Copy the icon data to the Clipboard */
void KalbumView::copyPhotoItem() {
    PhotoAlbum *photo = new PhotoAlbum;
    QPtrList<PhotoAlbum> photolist;


    for (photo=album->first(); photo!=0; photo=album->next()) {
        if (photo->iconitem->isSelected()) {
            PhotoAlbum *phototmp = new PhotoAlbum;
            phototmp = photo->copy();
            photolist.append(phototmp);

        }

    }

    KalMimeSource * mime_source = new KalMimeSource(photolist);
    KApplication::clipboard()->setData(mime_source);

}

void KalbumView::editPhotoItem() {
    PhotoAlbum *photo = new PhotoAlbum;


    for (photo=album->first(); photo!=0; photo=album->next()) {
        if (photo->iconitem->isSelected()) {

            slotPhotoProp(photo->iconitem);

        }

    }
}


void KalbumView::cutPhotoItem() {
    PhotoAlbum *photo = new PhotoAlbum;
    QPtrList<PhotoAlbum> photolist;

    for (photo=album->first(); photo!=0; photo=album->next()) {
        if (photo->iconitem->isSelected()) {

            PhotoAlbum *phototmp = new PhotoAlbum;
            phototmp = photo->copy();
            photolist.append(phototmp);
            delete photo->iconitem;
            album->remove
            (photo);
            photo=album->first();

        }
    }

    for (photo=album->first(); photo!=0; photo=album->next()) {
        if (photo->iconitem->isSelected()) {

            PhotoAlbum *phototmp = new PhotoAlbum;
            phototmp = photo->copy();
            photolist.append(phototmp);
            delete photo->iconitem;
            album->remove
            (photo);

        }
    }

    KalMimeSource * mime_source = new KalMimeSource(photolist);
    KApplication::clipboard()->setData(mime_source);

    emit setModified();
    slotUpdateTotal();

    KIconView1->arrangeItemsInGrid();

}


/** Pastes URL information into the App */
void KalbumView::pastePhotoItem() {
    QString title;
    QMimeSource * mime_source = KApplication::clipboard()->data();

    if (mime_source->provides("application/kalbum")) {

        QBuffer buffer(mime_source->encodedData("application/kalbum"));
        buffer.open(IO_ReadOnly);
        QDataStream stream(&buffer);

        int howmany;

        stream>>howmany;

        for (int i=0; i<howmany; i++) {
            PhotoAlbum *photo = new PhotoAlbum(stream, 0);

            addPhotoItem(photo);
        }
    }

    emit setModified();

}

int KalbumView::countPhoto() {

    return KIconView1->count();

}


bool KalbumView::savePhotoAlbum(const KURL& url) {

    QString tmpfile;
    QString checkFileEnd;
    QString stringCount;
    KalbumViewItem *positem;
    QDateTime temp_datetime;

    KSimpleConfig *config;
    PhotoAlbum *photo;
    int photo_count=0;

    checkFileEnd = url.fileName();
    checkFileEnd = checkFileEnd.right(7);

    KIO::NetAccess::download( url, tmpfile);
    if (!(checkFileEnd == FILEEX)) {
        tmpfile+=FILEEX;
    }

    QFile f(tmpfile);

    // if (KIO::NetAccess::exists(url))

    if (f.exists()) {
        int result = KMessageBox::warningContinueCancel(this, i18n("Do you want to overwrite "
                     "the existing file?"), i18n("Overwrite File"), i18n("OK"), "Overwrite " + tmpfile, false);

        if (result == KMessageBox::Cancel) {
            return false;
        }
    }




    if (!f.open(IO_WriteOnly)) {
        return false;
    }
    f.close();


    config = new KSimpleConfig(tmpfile);
    config->setGroup("Kalbum");
    config->writeEntry("Version", VERSION);

    config->writeEntry("Album Name", credits.albumName);
    config->writeEntry("Web Address", credits.webAddress.url());
    config->writeEntry("Authors Name", credits.authorName);
    config->writeEntry("Authors Email", credits.authorEmail);

    temp_datetime=credits.startDate;
    config->writeEntry("Start Date", temp_datetime);
    temp_datetime=credits.endDate;
    config->writeEntry("End Date", temp_datetime);

    config->writeEntry("Ignore Dates", (int)credits.ignoredates);

    config->writeEntry("Description", credits.albumDescription);

    config->writeEntry("Export URL", credits.exp_url.url());
    config->writeEntry("Export Colour", (int) credits.exp_colour);
    config->writeEntry("Export WAR", (int) credits.exp_war);
    config->writeEntry("Export Original", (int) credits.exp_original);
    config->writeEntry("Export Saturation", credits.exp_saturation);
    config->writeEntry("Export Hue", credits.exp_hue);

    for (positem=(KalbumViewItem *)KIconView1->firstItem(); positem!=0; positem=(KalbumViewItem *)positem->nextItem()) {

        photo = getPhoto(positem);
        stringCount.setNum(photo_count++);
        config->setGroup("Photo"+stringCount);
        config->writeEntry("Name", photo->title);
        config->writeEntry("URL", photo->fileurl.url());

        temp_datetime.setDate(photo->date);
	temp_datetime.setTime(photo->time);
        config->writeEntry("Date", temp_datetime);

        config->writeEntry("Rotate", (int) photo->rotate);
        config->writeEntry("Ignore Date", (int) photo->ignoredate);
	
	config->writeEntry("Ignore Time", (int) photo->ignoretime);

        config->writeEntry("Comment", photo->comment);
        config->writeEntry("Selected", photo->selected);

    }

    config->setGroup("Kalbum");
    config->writeEntry("HowMany", photo_count);

    config->sync();
    delete config;

    return true;

}

void KalbumView::colourHTML(const KURL& src, const KURL& des, int saturation, int hue) {
    /* XPM */
    static char * red_xpm[] = {
                                  "1 1 2 1",
                                  " 	c None",
                                  ".	c #FF0000",
                                  "."};

    QDir d;
    QImage image;
    QImage image_rgb = red_xpm;

    d = src.path();
    d.setFilter( QDir::Files | QDir::NoSymLinks );
    d.setNameFilter("*.png");
    const QFileInfoList *list = d.entryInfoList();
    QFileInfoListIterator it( *list );
    QFileInfo *fi;

    while ( (fi = it.current()) != 0 ) {
        image.load(fi->absFilePath());

        image = KImageEffect::modulate(image, image_rgb, false, KImageEffect::Saturation, saturation, KImageEffect::Red);
        image = KImageEffect::modulate(image, image_rgb, false, KImageEffect::HueShift, hue, KImageEffect::Red);

        image.save( des.path() + "/" + fi->fileName(),"PNG", -1);

        ++it;
    }



}



bool KalbumView::exportHTML() {

    KURL url, images, thumbs, photos, dir, imagepath, htmldes, htmlsrc, origin;
    QString htmldata;

    dir = KGlobal::dirs()->findResourceDir("kalbum_data", "kalbum_html/");
    dir.addPath("kalbum_html");
    dir.addPath("folder");
    imagepath = dir;
    htmlsrc		= dir;

    ExportWeb *exportweb = new ExportWeb;

    exportweb->KURLRequester1->setURL(credits.exp_url.path());
    exportweb->RadioButton2_2->setChecked(credits.exp_war);
    exportweb->RadioButton2->setChecked(credits.exp_original);
    exportweb->saturation->setValue(credits.exp_saturation);
    exportweb->hue->setValue(credits.exp_hue);
    exportweb->RadioButton1->setChecked(credits.exp_colour);
    exportweb->slotToggle(credits.exp_colour);

    if (exportweb->exec()) {
        // Export Web Dialog
        credits.exp_url         = exportweb->KURLRequester1->url();
        credits.exp_colour      = exportweb->RadioButton1->isChecked();
        credits.exp_war         = exportweb->RadioButton2_2->isChecked();
        credits.exp_original    = exportweb->RadioButton2->isChecked();
        credits.exp_saturation  = exportweb->saturation->value();
        credits.exp_hue         = exportweb->hue->value();

        emit setModified();

        url       = credits.exp_url;
 
        thumbs 		= url;
	      photos    = url;
	      images    = url;
        htmldes		= url;
	      origin	  = url;

        thumbs.addPath("thumbs");
        photos.addPath("photos");
        images.addPath("images");
	      origin.addPath("original");

        imagepath.addPath("images");

        KURL tar_url = url;

        tar_url.adjustPath(-1);
        tar_url.setFileName(tar_url.fileName()+".war");

        if (KIO::NetAccess::exists(tar_url)){
            int result = KMessageBox::warningContinueCancel(this, i18n("Do you want to overwrite "
                     "the existing .war file?"), i18n("Overwrite File"), i18n("OK"), "Overwrite " + tar_url.fileName(), false);

            if (result == KMessageBox::Cancel) {
              return false;
          }
        }


        if (KIO::NetAccess::mkdir( url)) {

            KIO::NetAccess::mkdir( thumbs );
            KIO::NetAccess::mkdir( photos );
            KIO::NetAccess::dircopy(imagepath, url);

            if (credits.exp_colour)
                colourHTML(imagepath, images, exportweb->saturation->value(), exportweb->hue->value());

            updateProgress(0, (countPhoto()*2));

   	        if (credits.exp_original){
              KIO::NetAccess::mkdir( origin );
              updateProgress(0, (countPhoto()*3));
		          exportOriginal(origin);
	          }

            ;
	    
            exportThumbs(thumbs);
            exportImages(photos);

            makeHTML(htmlsrc, htmldes);

            if (credits.exp_war){
              exportWAR(url);
            }

            updateProgress(0,0);
          
            return true;

        }
   }

    return false;
}

bool KalbumView::exportWAR(const KURL& url) {

    KURL tar_url = url;

    tar_url.adjustPath(-1);
    tar_url.setFileName(tar_url.fileName()+".war");

    KTar *tar = new KTar (tar_url.path(), "application/x-gzip");

    if (tar->open(IO_WriteOnly)){

        emit setStatusMsg(i18n("Creating WAR..."));

        tar = exportWARdir(tar, url, "");
        tar = exportWARdir(tar, url, "thumbs/");
        tar = exportWARdir(tar, url, "images/");
        tar = exportWARdir(tar, url, "photos/");

   	    if (credits.exp_original){
  	      tar = exportWARdir(tar, url, "original/");
    }

    tar->close();
    KIO::NetAccess::del( url );

    }

    return true;

}

KTar *KalbumView::exportWARdir(KTar *tar, const KURL& url, QString directory){

    KURL dir_url = url;
    QDir d;

    dir_url.addPath(directory);
    d = dir_url.path();
    d.setFilter( QDir::Files | QDir::NoSymLinks );
    d.setNameFilter("*");
    const QFileInfoList *list = d.entryInfoList();
    QFileInfoListIterator it( *list );
    QFileInfo *fi;

    updateProgress(0, it.count());
    emit setStatusMsg(i18n("Creating WAR...")+directory);

    while ( (fi = it.current()) != 0 ) {

      QFile file(fi->absFilePath());

      file.open(IO_ReadOnly);
      QByteArray b_array = file.readAll();

      tar->writeFile( directory + fi->fileName(), "user", "group", b_array.size(), b_array.data());
      file.close();

      ++it;

      updateProgress();

    }
    
    return tar;

}

bool KalbumView::exportOriginal(const KURL& url) {
    
    KalbumViewItem *positem;
    PhotoAlbum *photo;
    QString format = PHOTOFMT;
    QString thumbnum;
    QImage buffer;
    QString tmpfile;

    for (positem=(KalbumViewItem *)KIconView1->firstItem(); positem!=0; positem=(KalbumViewItem *)positem->nextItem()) {
        updateProgress();

        KURL photopath=url;

        photo = getPhoto(positem);
        photopath.addPath(photo->fileurl.fileName());

        KIO::NetAccess::copy( photo->fileurl, photopath);


    }

    return true;
    
}


bool KalbumView::exportHTML(const KURL& url) {
    KURL thumbs, photos, dir, imagepath, htmldes, htmlsrc;
    QString htmldata;
    thumbs 		= url;
    photos 		= url;
    htmldes		= url;

    thumbs.addPath("thumbs");
    photos.addPath("photos");

    dir = KGlobal::dirs()->findResourceDir("kalbum_data", "kalbum_html/");
    dir.addPath("kalbum_html");
    dir.addPath("folder");
    imagepath = dir;
    htmlsrc		= dir;
    imagepath.addPath("images");



    if (KIO::NetAccess::mkdir( url)) {
        KIO::NetAccess::mkdir( thumbs );
        KIO::NetAccess::mkdir( photos );
        //        KIO::NetAccess::mkdir( imagepath );
        //KIO::NetAccess::dircopy(imagepath, url);

        //        exportThumbs(thumbs);
        //        exportImages(photos);

        //        makeHTML(htmlsrc, htmldes);

        return true;
    }




    return false;

}

bool KalbumView::makeHTML(const KURL& src, const KURL& des) {
    KURL htmltmp, htmldes;

    QString gen_src = "gen_src.html";
    QString pho_src = "pho_src.html";
    QString thm_src = "thm_src.html";

    PhotoAlbum *photo;
    KalbumViewItem *positem;
    QString htmldata;
    QString htmltitle;
    QString htmlphoto;
    QString htmlphoto_src;
    QString temp;
    QString first_photo;
    int no_thm, no_pho, photo_count;
    int i=0;

    QString key_pho = KWDPHCNT;
    QString key_thm = KWDTHCNT;

    htmltmp = src;
    htmltmp.addPath(gen_src);
    htmltitle = readString(htmltmp);

    photo = getPhoto((KalbumViewItem *)KIconView1->firstItem());
    first_photo = photo->filename;

    // Read the capabilities of the Web Site from the general page
    if (temp = htmltitle.mid(htmltitle.find(key_pho)+key_pho.length(), MAXPHOTO)) {
        if (!(no_pho = temp.toInt()))
            return false;                   // false if no photos defined

        temp = htmltitle.mid(htmltitle.find(key_thm)+key_thm.length(), MAXPHOTO);
        if (!(no_thm = temp.toInt()))
            return false;

        photo_count = countPhoto();

        htmltmp = src;
        htmltmp.addPath(thm_src);
        htmldata = readString(htmltmp);

        htmltmp = src;
        htmltmp.addPath(pho_src);
        htmlphoto_src = readString(htmltmp);

        for (positem=(KalbumViewItem *)KIconView1->firstItem(); positem!=0; positem=(KalbumViewItem *)positem->nextItem()) {
            photo 		= getPhoto(positem);
            htmlphoto = htmlphoto_src;

            htmldata 	= makeHTMLthumb(htmldata,	 (i++ % no_thm), photo);

            htmlphoto = makeHTMLphoto(htmlphoto, photo);
            htmlphoto = linkPhoto(htmlphoto, positem);
            htmldes 	= des;
            temp 			= photo->filename;
            temp.append(".html");
            htmldes.addPath(temp);
            writeString(htmlphoto, htmldes);

            if ((i % no_thm)==0) {

                htmldes = des;
                temp =  temp.setNum(i/no_thm);
                temp =  getPrecision(temp);
                temp.prepend(THUMBHTM);
                temp.append(".html");
                htmldes.addPath(temp);

                if (photo_count % no_thm)
                    htmldata = linkThumb(htmldata, first_photo, (i/no_thm), (photo_count/no_thm) + 1);
                else
                    htmldata = linkThumb(htmldata, first_photo, (i/no_thm), (photo_count/no_thm));

                writeString(htmldata, htmldes);

                htmltmp = src;
                htmltmp.addPath(thm_src);
                htmldata = readString(htmltmp);
            }
        }

        if (i % no_thm) {
            while (i % no_thm)
                htmldata = makeHTMLthumb(htmldata, (i++ % no_thm));

            htmldes = des;
            temp =  temp.setNum(i/no_thm);
            temp =  getPrecision(temp);
            temp.prepend(THUMBHTM);
            temp.append(".html");
            htmldes.addPath(temp);

            htmldata = linkThumb(htmldata, first_photo, (i/no_thm), (photo_count/no_thm) + 1);

            writeString(htmldata, htmldes);

        }

        htmldes = des;
        htmldes.addPath("index.html");
        htmltitle = makeHTMLtitle(htmltitle);
        htmltitle = linkPhoto(htmltitle);
        htmltitle = linkThumb(htmltitle, first_photo);

        writeString(htmltitle, htmldes);

        return true;
    }

    return false;

}

QString KalbumView::linkThumb(QString htmldata, QString first_photo) {
    QString ins_string;
    int findstt;


    //Change the Quick tabs
    findstt	= htmldata.find("<!-- {Kalbum Link Browse} -->");

    if (findstt>0) {
        findstt		 = htmldata.find("<a href=", findstt) + 9;

        htmldata.insert(findstt, first_photo);
    }


    return htmldata;

}




QString KalbumView::linkThumb(QString htmldata, QString first_photo, int page_number, int max_page) {
    QString ins_string;
    int findstt,findend;

    // Deal with the previous link
    if (page_number == 1) {

        findstt	= htmldata.find("<!-- {Kalbum Thumbnail Prev} -->");

        if (findstt>0) {
            findend	= htmldata.find("</a>", findstt) + 4;
            htmldata.remove(findstt, findend - findstt);
        }
    } else {

        findstt	= htmldata.find("<!-- {Kalbum Thumbnail Prev} -->");

        if (findstt>0) {
            findstt	= htmldata.find("href=", findstt) + 6;  		// Six including double quotes
            ins_string =  ins_string.setNum(page_number - 1);
            ins_string =  getPrecision(ins_string);
            ins_string.prepend(THUMBHTM);

            htmldata.insert(findstt, ins_string);
        }
    }

    // Deal with the next link
    if (page_number == max_page) {

        findstt	= htmldata.find("<!-- {Kalbum Thumbnail Next} -->");

        if (findstt>0) {
            findend	= htmldata.find("</a>", findstt) + 4;
            htmldata.remove(findstt, findend - findstt);
        }
    } else {

        findstt	= htmldata.find("<!-- {Kalbum Thumbnail Next} -->");

        if (findstt>0) {
            findstt	= htmldata.find("href=", findstt) + 6;  		// Six including double quotes
            ins_string =  ins_string.setNum(page_number + 1);
            ins_string =  getPrecision(ins_string);
            ins_string.prepend(THUMBHTM);

            htmldata.insert(findstt, ins_string);
        }
    }

    //Change the Quick tabs
    findstt	= htmldata.find("<!-- {Kalbum Link Browse} -->");

    if (findstt>0) {
        findstt		 = htmldata.find("<a href=", findstt) + 9;

        htmldata.insert(findstt, first_photo);
    }

    findstt	= htmldata.find("<!-- {Kalbum Link General} -->");

    if (findstt>0) {
        findstt		 = htmldata.find("<a href=", findstt) + 9;

        htmldata.insert(findstt, "index");
    }

    return htmldata;

}

QString KalbumView::linkPhoto(QString htmldata, KalbumViewItem *positem) {
    QString ins_string;
    KalbumViewItem * temp_positem;
    int findstt,findend;
    PhotoAlbum *photo;


    // Deal with the previous link
    if (!(temp_positem = (KalbumViewItem *)positem->prevItem())) {

        findstt	= htmldata.find("<!-- {Kalbum Photo Prev} -->");

        if (findstt>0) {
            findend	= htmldata.find("</a>", findstt) + 4;
            htmldata.remove(findstt, findend - findstt);
        }
    } else {

        findstt	= htmldata.find("<!-- {Kalbum Photo Prev} -->");

        if (findstt>0) {
            findstt	= htmldata.find("href=", findstt) + 6;  		// Six including double quotes
            photo 		 = getPhoto(temp_positem);
            ins_string = photo->filename;
            htmldata.insert(findstt, ins_string);
        }
    }

    // Deal with the next link
    if (!(temp_positem = (KalbumViewItem *)positem->nextItem())) {

        findstt	= htmldata.find("<!-- {Kalbum Photo Next} -->");

        if (findstt>0) {
            findend	= htmldata.find("</a>", findstt) + 4;
            htmldata.remove(findstt, findend - findstt);
        }
    } else {

        findstt	= htmldata.find("<!-- {Kalbum Photo Next} -->");

        if (findstt>0) {
            findstt	= htmldata.find("href=", findstt) + 6;  		// Six including double quotes
            photo 		 = getPhoto(temp_positem);
            ins_string = photo->filename;

            htmldata.insert(findstt, ins_string);
        }
    }

    //Change the Quick tabs
    findstt	= htmldata.find("<!-- {Kalbum Link Thumbnails} -->");

    if (findstt>0) {
        findstt		 = htmldata.find("<a href=", findstt) + 9;
        ins_string = ins_string.setNum(1);
        ins_string = getPrecision(ins_string);
        ins_string.prepend(THUMBHTM);

        htmldata.insert(findstt, ins_string);
    }

    findstt	= htmldata.find("<!-- {Kalbum Link General} -->");

    if (findstt>0) {
        findstt		 = htmldata.find("<a href=", findstt) + 9;

        htmldata.insert(findstt, "index");
    }

    return htmldata;

}

QString KalbumView::linkPhoto(QString htmldata) {
    QString ins_string;
    int findstt;


    //Change the Quick tabs
    findstt	= htmldata.find("<!-- {Kalbum Link Thumbnails} -->");

    if (findstt>0) {
        findstt		 = htmldata.find("<a href=", findstt) + 9;
        ins_string = ins_string.setNum(1);
        ins_string = getPrecision(ins_string);
        ins_string.prepend(THUMBHTM);

        htmldata.insert(findstt, ins_string);
    }

    return htmldata;

}

QString KalbumView::makeHTMLtitle(QString htmldata) {
    QString ins_string;
    QDateTime generation_time;
    int findstt,findend;

    findstt			= htmldata.find("{Kalbum Document Title}");

    if (findstt>0) {
        findend			= htmldata.find("}", findstt) + 1;
        htmldata.remove(findstt, findend - findstt);
        htmldata.insert(findstt, credits.albumName);
    }

    findstt			= htmldata.find("{Kalbum Page Title}");

    if (findstt>0) {
        findend			= htmldata.find("}", findstt) + 1;
        htmldata.remove(findstt, findend - findstt);
        htmldata.insert(findstt, credits.albumName);
    }

    findstt			= htmldata.find("{Kalbum Album Title}");

    if (findstt>0) {
        findend			= htmldata.find("}", findstt) + 1;
        htmldata.remove(findstt, findend - findstt);
        htmldata.insert(findstt, credits.albumName);
    }

    findstt			= htmldata.find("{Kalbum Album Start Date}");

    if (findstt>0) {
        findend			= htmldata.find("}", findstt) + 1;
        htmldata.remove(findstt, findend - findstt);
        if (!credits.ignoredates)
            htmldata.insert(findstt, credits.startDate.toString() + " - ");
    }

    findstt			= htmldata.find("{Kalbum Album End Date}");

    if (findstt>0) {
        findend			= htmldata.find("}", findstt) + 1;
        htmldata.remove(findstt, findend - findstt);
        if (!credits.ignoredates)
            htmldata.insert(findstt, credits.endDate.toString());
    }

    findstt			= htmldata.find("{Kalbum Author Name}");

    if (findstt>0) {
        findend			= htmldata.find("}", findstt) + 1;
        htmldata.remove(findstt, findend - findstt);
        if (!credits.authorName.isEmpty())
            htmldata.insert(findstt, "Author: " + credits.authorName);
    }

    findstt			= htmldata.find("{Kalbum Author email Link}");

    if (findstt>0) {
        findend			= htmldata.find("}", findstt) + 1;
        htmldata.remove(findstt, findend - findstt);
        if (!credits.authorEmail.isEmpty())
            htmldata.insert(findstt, "<a HREF=\"mailto:" + credits.authorEmail + "\">");
    }

    findstt			= htmldata.find("{Kalbum Author email}");

    if (findstt>0) {
        findend			= htmldata.find("}", findstt) + 1;
        htmldata.remove(findstt, findend - findstt);
        if (!credits.authorEmail.isEmpty())
            htmldata.insert(findstt, credits.authorEmail);
    }

    findstt			= htmldata.find("{Kalbum Album Description}");

    if (findstt>0) {
        findend			= htmldata.find("}", findstt) + 1;
        htmldata.remove(findstt, findend - findstt);
        htmldata.insert(findstt, credits.albumDescription);
    }

    findstt			= htmldata.find("{Kalbum Generation Details}");

    if (findstt>0) {
        findend			= htmldata.find("}", findstt) + 1;
        htmldata.remove(findstt, findend - findstt);

        ins_string = "Generated by Kalbum v.";
        ins_string.append(VERSION);
        ins_string.append(" on ");
        generation_time = generation_time.currentDateTime();
        ins_string.append(generation_time.toString());

        htmldata.insert(findstt, ins_string);
    }


    return htmldata;

}

QString KalbumView::makeHTMLphoto(QString htmldata, PhotoAlbum *photo) {
    QString ins_string;
    QString temp;
    int findstt,findend;
    //	int photoszx = PHOTOSZX;
    //	int photoszy = PHOTOSZY;
    QString format = PHOTOFMT;

    // Inserts the general album credits
    htmldata = makeHTMLtitle(htmldata);

    findstt		= htmldata.find("<!-- {Kalbum Photo Image} -->");

    // Inserts Photo Image
    if (findstt>0) {

        if (credits.exp_original){  // Insert anchor to the original image
          findstt		        = htmldata.find("<img src=", findstt);
          ins_string = "<a href = \"";
          ins_string.append( "original/");
          ins_string.append( photo->fileurl.fileName());
          ins_string.append( "\"");
          ins_string.append( ">");

          htmldata.insert(findstt, ins_string);
        }

        findstt		= htmldata.find("img src=", findstt) + 9;
        findend		= htmldata.find(">", findstt);

        htmldata.remove(findstt, findend - findstt);


        ins_string = "photos/";
        ins_string.append (photo->filename);
        ins_string.append ("."+format.lower());
        //		temp = temp.setNum(photoszx);
        //		ins_string.append ("\" width=\"");
        //		ins_string.append (temp);
        //		temp = temp.setNum(photoszy);
        //		ins_string.append ("\" height=\"");
        //		ins_string.append (temp);
        ins_string.append ("\" border=\"0\"");

        htmldata.insert(findstt, ins_string);

        if (credits.exp_original){  // Finish anchor link to the original image
          findstt		        = htmldata.find(">", findstt) +1;
          ins_string = "</a>";

          htmldata.insert(findstt, ins_string);
        }


    }

    // Inserts Photo name
    temp 				= "{Kalbum Photo Name}";
    findstt			= htmldata.find(temp);
    findend			= htmldata.find("}", findstt) + 1;
    if (findstt>0) {
        htmldata.remove(findstt, findend - findstt);
        if (!photo->title.isEmpty())
            htmldata.insert(findstt, photo->title);
    }

    // Inserts Photo Date
    temp 				= "{Kalbum Photo Date}";
    findstt			= htmldata.find(temp);
    findend			= htmldata.find("}", findstt) + 1;
    if (findstt>0) {
        htmldata.remove(findstt, findend - findstt);
        if (!photo->ignoredate)
            htmldata.insert(findstt, photo->date.toString());
    }

    // Inserts Photo Time
    temp 				= "{Kalbum Photo Time}";
    findstt			= htmldata.find(temp);
    findend			= htmldata.find("}", findstt) + 1;
    if (findstt>0) {
        htmldata.remove(findstt, findend - findstt);
        if (!photo->ignoretime)
            htmldata.insert(findstt, photo->time.toString());
    }

    // Inserts Photo Description
    temp 				= "{Kalbum Photo Description}";
    findstt			= htmldata.find(temp);
    findend			= htmldata.find("}", findstt) + 1;
    if (findstt>0) {
        htmldata.remove(findstt, findend - findstt);
        if (!photo->comment.isEmpty())
            htmldata.insert(findstt, photo->comment);
    }

    return htmldata;

}


QString KalbumView::makeHTMLthumb(QString htmldata, int photo_number, PhotoAlbum *photo) {
    QString key_thm_thumb = KWDTHUMB;
    QString key_thm_title = KWDTITLE;
    QString format = THUMBFMT;
    QString ins_string;

    QString temp;
    int findstt,findend;
    int thumbszx = THUMBSZX;
    int thumbszy = THUMBSZY;

    photo_number++;

    temp = temp.setNum(photo_number);
    key_thm_thumb.append(getPrecision(temp));

    // Inserts the general album credits
    htmldata = makeHTMLtitle(htmldata);

    // Find the Kalbum thumbnail entry
    findstt 		= htmldata.find(key_thm_thumb);

    // Edits the link to the photo's HTML
    if (findstt>0) {
        findstt			= htmldata.find("<a href=", findstt) + 9;
        ins_string 	= photo->filename;
        ins_string.append (".");
        ins_string.append ("html\"");

        htmldata.insert(findstt, ins_string);

        // Inserts Photo's thumbnail
        findstt			=	htmldata.find("<img src=", findstt) + 9;
        findend 		= htmldata.find(">", findstt);

        if (findstt>0) {
            htmldata.remove(findstt, findend - findstt);

            ins_string = "\"thumbs/";
            ins_string.append (photo->filename);
            ins_string.append ("."+format.lower());
            temp = temp.setNum(thumbszx);
            ins_string.append ("\" width=\"");
            ins_string.append (temp);
            temp = temp.setNum(thumbszy);
            ins_string.append ("\" height=\"");
            ins_string.append (temp);
            ins_string.append ("\" border=\"0\"");

            htmldata.insert(findstt, ins_string);
        }
    }

    // Inserts Photo name
    temp = temp.setNum(photo_number);
    temp = getPrecision(temp);
    temp.prepend("{Kalbum Title=");
    findstt			= htmldata.find(temp);
    findend			= htmldata.find("}", findstt) + 1;
    if (findstt>0) {
        htmldata.remove(findstt, findend - findstt);
        temp = temp.setNum(findstt);

        if (!photo->title.isEmpty())
            htmldata.insert(findstt, photo->title);
    }

    return htmldata;

}


QString KalbumView::makeHTMLthumb(QString htmldata, int photo_number) {
    QString key_thm_thumb = KWDTHUMB;
    QString key_thm_title = KWDTITLE;
    QString format = THUMBFMT;
    QString ins_string;

    photo_number++;

    QString temp;
    int findstt,findend;
    //	int thumbszx = THUMBSZX;
    //	int thumbszy = THUMBSZY;

    temp = temp.setNum(photo_number);
    key_thm_thumb.append(getPrecision(temp));

    // Inserts the general album credits
    htmldata = makeHTMLtitle(htmldata);

    // Find the Kalbum thumbnail entry
    findstt 		= htmldata.find(key_thm_thumb);

    // Removes the link to the photo's HTML
    if (findstt>0) {
        findstt			= htmldata.find("<a href=", findstt);
        findend 		= htmldata.find("</a>", findstt) + 4;

        htmldata.remove(findstt, findend - findstt);
    }

    // Removes Photo name
    temp = temp.setNum(photo_number);
    temp = getPrecision(temp);
    temp.prepend("{Kalbum Title=");
    findstt			= htmldata.find(temp);
    findend			= htmldata.find("}", findstt) + 1;
    if (findstt>0)
        htmldata.remove(findstt, findend - findstt);


    return htmldata;

}


QString KalbumView::getPrecision(QString str_number) {
    int str_length;

    str_length = str_number.length();
    if (str_length < MAXPHOTO)
        for (int i=str_length; i < MAXPHOTO; i++)
            str_number.prepend("0");

    return str_number;

}




bool KalbumView::writeString(QString stringHTML, const KURL& url) {

    QFile f(url.path());

    if(f.open(IO_WriteOnly)) {
        QTextStream t(&f);

        t << stringHTML;

    }

    return true;

}

QString KalbumView::readString(const KURL& url) {
    QString string = "";
    QString tmpfile;

    KIO::NetAccess::download( url, tmpfile);
    QFile f(tmpfile);

    if(f.open(IO_ReadOnly)) {
        QTextStream t(&f);

        while (!t.eof()) {
            string = string + t.readLine();
            string.append("\n");
        }
    }

    return string;

}



int KalbumView::exportThumbs(const KURL& url) {
    KalbumViewItem *positem;
    PhotoAlbum *photo;
    int photo_count=0;
    QString format = THUMBFMT;
    QString tmpfile;
    QString thumbnum;
    QImage buffer;
    KPixmap background, thumbnail;
    KURL dir;

    for (positem=(KalbumViewItem *)KIconView1->firstItem(); positem!=0; positem=(KalbumViewItem *)positem->nextItem()) {

        KURL thumbpath=url;
        photo = getPhoto(positem);
        thumbpath.addPath(photo->filename+"."+format.lower());

        KIO::NetAccess::download( photo->fileurl, tmpfile);

        updateProgress();

        if (buffer.load(tmpfile)) {
            int thumbSizeX=110;
            int thumbSizeY=90;

            dir = url;
            dir = dir.upURL();
            dir.addPath("images/slide01.png");




            background.load(dir.path());

            buffer = effectImage(buffer, photo->rotate);
            buffer = scaleImage(buffer, thumbSizeX, thumbSizeY);

            thumbnail.convertFromImage(buffer, QPixmap::Color);

            bitBlt(&background, (background.width()-thumbnail.width())/2,
                   (background.height()-thumbnail.height())/2, &thumbnail, 0, 0,
                   thumbnail.width(), thumbnail.height(), Qt::CopyROP, false);

            background.save(thumbpath.path(), format);

            photo_count++;

        } else {    // else just save the thumbnail(icon) used in the view

            photo->thumbnail.save(thumbpath.path(), format);
            photo_count++;

        }


    }

    return photo_count;
}


int KalbumView::exportImages(const KURL& url) {

    KalbumViewItem *positem;
    PhotoAlbum *photo;
    int photo_count=0;
    QString format = PHOTOFMT;
    QString thumbnum;
    QImage buffer;
    QString tmpfile;

    for (positem=(KalbumViewItem *)KIconView1->firstItem(); positem!=0; positem=(KalbumViewItem *)positem->nextItem()) {
        updateProgress();

        KURL photopath=url;

        photo = getPhoto(positem);
        photopath.addPath(photo->filename+"."+format.lower());

        KIO::NetAccess::download( photo->fileurl, tmpfile);

        if (buffer.load(tmpfile)) {
            buffer = effectImage(buffer, photo->rotate);
            buffer = scaleImage (buffer, PHOTOSZX, PHOTOSZY);

            buffer.save(photopath.path(), format);
            photo_count++;
        }
    }

    return photo_count;

}



void KalbumView::editMoveUp() {

    PhotoAlbum *photo, *tempphoto;
    KalbumViewItem *newitem, *positem;
    bool lastitem=false;
    QPtrList <PhotoAlbum> photolist;

    if (!allSelected()) {

        photolist.setAutoDelete(false);

        for (photo=album->last(); photo!=0; photo=album->prev()) {
            if (photo->iconitem->isSelected()) {

                photolist.append(photo);

                // Move the insert position along until there's an Unselected icon
                positem=photo->iconitem;
                while (positem->isSelected()) {
                    if (positem == (KalbumViewItem *)KIconView1->lastItem()) {
                        lastitem =true;
                        positem = (KalbumViewItem *)KIconView1->firstItem();
                    } else {
                        positem = (KalbumViewItem *)positem->nextItem();
                    }
                }

                newitem = photo->iconitem;
                photo->iconitem = new KalbumViewItem( KIconView1, positem, photo->title, photo->thumbnail);

                if (lastitem) {
                    tempphoto = getPhoto((KalbumViewItem *)KIconView1->firstItem());
                    delete KIconView1->firstItem();
                    tempphoto->iconitem = new KalbumViewItem( KIconView1, (KalbumViewItem *)KIconView1->firstItem(), tempphoto->title, tempphoto->thumbnail);
                    lastitem = false;
                }

                delete newitem;
                photo=album->last();

            }
        }

        for (photo=photolist.first(); photo !=0; photo=photolist.next()) {
            photo->iconitem->setSelected(true, true);
        }

        emit setModified();

    }
}

void KalbumView::editMoveDown() {

    PhotoAlbum *photo, *tempphoto;
    KalbumViewItem *newitem, *positem;
    bool firstitem=false, seconditem=false;
    QPtrList <PhotoAlbum> photolist;

    if (!allSelected()) {

        photolist.setAutoDelete(false);

        for (photo=album->first(); photo!=0; photo=album->next()) {
            if (photo->iconitem->isSelected()) {

                photolist.append(photo);

                // Move the insert position back until there's an Unselected icon
                positem=photo->iconitem;
                while (positem->isSelected()) {
                    if (positem == (KalbumViewItem *)KIconView1->firstItem()) {   												// Special case: First Item
                        firstitem = true;
                        positem = (KalbumViewItem *)KIconView1->lastItem();
                    } else if (positem == (KalbumViewItem *)KIconView1->firstItem()->nextItem()) {     	// Special case: Second Item
                        seconditem = true;
                        positem = (KalbumViewItem *)positem->prevItem();
                    } else {
                        positem = (KalbumViewItem *)positem->prevItem();
                    }
                }


                if (firstitem) {

                    newitem = photo->iconitem;
                    photo->iconitem = new KalbumViewItem( KIconView1, (KalbumViewItem *)positem->prevItem(), photo->title, photo->thumbnail);

                    tempphoto = getPhoto((KalbumViewItem *)KIconView1->lastItem());
                    delete KIconView1->lastItem();
                    tempphoto->iconitem = new KalbumViewItem( KIconView1, (KalbumViewItem *)KIconView1->lastItem()->prevItem(), tempphoto->title, tempphoto->thumbnail);
                    firstitem = false;

                } else if (seconditem) {

                    newitem = photo->iconitem;
                    photo->iconitem = new KalbumViewItem( KIconView1, (KalbumViewItem *)KIconView1->firstItem(), photo->title, photo->thumbnail);
                    tempphoto = getPhoto((KalbumViewItem *)KIconView1->firstItem());
                    delete KIconView1->firstItem();

                    tempphoto->iconitem = new KalbumViewItem( KIconView1, (KalbumViewItem *)KIconView1->firstItem(), tempphoto->title, tempphoto->thumbnail);
                    seconditem=false;

                } else {

                    newitem = photo->iconitem;
                    photo->iconitem = new KalbumViewItem( KIconView1, (KalbumViewItem *)positem->prevItem(), photo->title, photo->thumbnail);
                }


                delete newitem;
                photo=album->first();

            }
        }

        for (photo=photolist.first(); photo !=0; photo=photolist.next()) {
            photo->iconitem->setSelected(true, true);
        }

        emit setModified();

    }
}

bool KalbumView::loadPhotoAlbum(const KURL& url) {
    QString tmpfile;
    QString stringCount;
    KSimpleConfig *config;
    QDateTime tmp_date;

    int photo_count=0;

    if (!(url.isValid()))
        return false;

    KIO::NetAccess::download( url, tmpfile);

    QFile f(tmpfile);

    if ((!f.open(IO_ReadOnly)) || (!f.exists())) {
        return false;
    }

    f.close();

    config = new KSimpleConfig(tmpfile);
    config->setGroup("Kalbum");

    credits.albumName         = config->readEntry("Album Name");
    credits.webAddress	      = config->readEntry("Web Address");
    credits.authorName	      = config->readEntry("Authors Name");
    credits.authorEmail	      = config->readEntry("Authors Email");
    credits.albumDescription  =	config->readEntry("Description");
    tmp_date = config->readDateTimeEntry("Start Date");
    credits.startDate					= tmp_date.date();
    tmp_date = config->readDateTimeEntry("End Date");
    credits.endDate						= tmp_date.date();

    credits.ignoredates       = config->readBoolEntry("Ignore Dates");

    credits.exp_url           = config->readEntry("Export URL");
    credits.exp_colour        = config->readBoolEntry("Export Colour");
    credits.exp_war           = config->readBoolEntry("Export WAR");
    credits.exp_original      = config->readBoolEntry("Export Original");
    credits.exp_saturation    = config->readNumEntry("Export Saturation");
    credits.exp_hue           = config->readNumEntry("Export Hue");

    photo_count = config->readUnsignedNumEntry("HowMany");

    updateProgress(0, photo_count);

    for (int i=0; i<photo_count; i++) {
        PhotoAlbum *photo = new PhotoAlbum;
        stringCount.setNum(i);
        config->setGroup("Photo"+stringCount);
        photo->title = config->readEntry("Name");
        photo->fileurl = config->readEntry("URL");
        tmp_date = config->readDateTimeEntry("Date");
        photo->date =  tmp_date.date();
	      photo->time = tmp_date.time();
        photo->rotate = config->readNumEntry("Rotate");
        photo->ignoredate = config->readBoolEntry("Ignore Date");
	      photo->ignoretime = config->readBoolEntry("Ignore Time", true);
        photo->comment = config->readEntry("Comment");
        addPhotoItem(photo);
        updateProgress();
    }

    emit albumChanged(credits);

    updateProgress(0,0);

    //    Shows album information whenever a new one is loaded
    //    setDocInfo();

    delete config;
    return true;
}

void KalbumView::slotNextImage() {

    KalbumViewItem *item;

    for ( item  = (KalbumViewItem *) KIconView1->firstItem(); item != 0; item  = (KalbumViewItem *) item->nextItem() ) {
        if (item->isSelected()) {
            if (item == (KalbumViewItem *) KIconView1->lastItem()) {
                KIconView1->clearSelection();
                KIconView1->firstItem()->setSelected(true);
            } else {
                KIconView1->clearSelection();
                item->nextItem()->setSelected(true);
            }
            break;
        }
    }

}

void KalbumView::slotPrevImage() {

    KalbumViewItem *item;

    for ( item  = (KalbumViewItem *) KIconView1->lastItem(); item != 0; item  = (KalbumViewItem *) item->prevItem() ) {
        if (item->isSelected()) {
            if (item == (KalbumViewItem *) KIconView1->firstItem()) {
                KIconView1->clearSelection();
                KIconView1->lastItem()->setSelected(true);
            } else {
                KIconView1->clearSelection();
                item->prevItem()->setSelected(true);
            }
            break;
        }
    }

}


QImage KalbumView::scaleImage(QImage buffer, int x, int y) {
    float s=MAX((float(buffer.width())/x), (float(buffer.height())/y));

    buffer=buffer.smoothScale(MAX(1,int(buffer.width()/s)), MAX(1,int(buffer.height()/s)));

    return buffer;

}

QImage KalbumView::effectImage(QImage buffer, int rotate) {

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


/** Standard dnd from the QT Docs */
void KalbumView::dragEnterEvent(QDragEnterEvent *event) {
    event->accept(
        QTextDrag::canDecode(event) ||
        QImageDrag::canDecode(event)
    );

}


bool KalbumView::allSelected() {
    PhotoAlbum *photo;
    for (photo=album->first(); photo!=0; photo=album->next())
        if (!(photo->iconitem->isSelected()))
            return false;

    return true;

}

void KalbumView::slotRHClick() {

    popIconView->exec(QCursor::pos());

}

void KalbumView::slotCut() {

    cutPhotoItem();

}

void KalbumView::slotCopy() {

    copyPhotoItem();

}

void KalbumView::slotPaste() {

    pastePhotoItem();

}

void KalbumView::slotMoveLeft() {

    editMoveDown();

}

void KalbumView::slotMoveRight() {

    editMoveUp();

}

void KalbumView::slotEditPhoto() {

    editPhotoItem();

}



void KalbumView::slotDropEvent( QDropEvent *event, int drop_index ) {

    QString title;
    int found=0;
    int count;

    KURL::List::ConstIterator it;
    KURL::List url;

    QIconViewItem *positem;

    //		if (QIconDrag::canDecode(event)) {
    //		KMessageBox::information(this, "Dnd Info", "Dropped a KIcon.");
    //		}

    /*  if (event->action() == QDropEvent::Copy)
        KMessageBox::information(this, "Dnd Copy", "DnD Copy!");
      else
        KMessageBox::information(this, "Dnd Move", "DnD Move!");
    */
    event->acceptAction();

    emit setStatusMsg(i18n("Inserting images..."));

    if (event->provides("application/kalbum")) {

        QBuffer buffer(event->encodedData("application/kalbum"));
        buffer.open(IO_ReadOnly);
        QDataStream stream(&buffer);

        int howmany;
        stream>>howmany;

        if ((KIconView1->count()) == 0) {          // If there are no icons in the view
            for (int i=0; i<howmany; i++) {
                PhotoAlbum *photo = new PhotoAlbum(stream, 0);
                addPhotoItem(photo, album->first());
            }
        } else {                                  // Insert at the attached index
            positem=KIconView1->firstItem();        // Find the position to insert drop
            while ((!(found)) && (positem!=0)) {
                if (positem->index() == drop_index) {
                    found = true;
                } else {
                    positem=positem->nextItem();
                }
            }

            for (int i=0; i<howmany; i++) {
                PhotoAlbum *photo = new PhotoAlbum(stream, 0);
                if (positem) {
                    addPhotoItem(photo, getPhoto((KalbumViewItem *)positem));
                } else {
                    addPhotoItem(photo);
                }
            }

            if (event->source()) {
                deleteSelected();
            }

        }
    } else if (KURLDrag::decode(event, url)) {                 // Dragging URLs



        count = 0;
        for (it = url.begin(); it != url.end(); ++it)
            count++;

        updateProgress(0, count);


        if ((KIconView1->count()) == 0) {

            for (it = url.begin(); it != url.end(); ++it) {
                updateProgress();
                if((*it).isValid()) {
                    addPhotoItem(*it, album->first());
                }
            }
        } else {
            positem=KIconView1->firstItem();
            while ((!(found)) && (positem!=0)) {
                if (positem->index() == drop_index) {
                    found = true;
                } else {
                    positem=positem->nextItem();
                }
            }
            for (it = url.begin(); it != url.end(); ++it) {
                updateProgress();
                if((*it).isValid()) {
                    if (positem)
                        addPhotoItem(*it, getPhoto((KalbumViewItem *)positem));
                    else
                        addPhotoItem(*it);
                }
            }
        }
        updateProgress(0,0);

    }

    KIconView1->arrangeItemsInGrid();
    emit setStatusMsg(i18n("Ready."));
    emit setModified();

}

void KalbumView::deleteSelected() {
    // Duplicated delete loops to catch every selection up/down
    PhotoAlbum *photo = new PhotoAlbum;
    for (photo=album->first(); photo!=0; photo=album->next()) {
        if (photo->iconitem->isSelected()) {
            deletePhotoItem(photo);
            photo=album->first();
        }
    }

    for (photo=album->last(); photo!=0; photo=album->prev()) {
        if (photo->iconitem->isSelected()) {
            deletePhotoItem(photo);
            photo=album->last();
        }
    }

}

void KalbumView::slotSelection() {


    QPtrList<PhotoAlbum> photolist;

    PhotoAlbum *photo = new PhotoAlbum;
    int i=0, total=0;

    for (photo=album->first(); photo!=0; photo=album->next()) {
        if (photo->iconitem->isSelected()) {
            i++;
            total++;
            PhotoAlbum *phototmp = new PhotoAlbum;
            phototmp = photo;
            photolist.append(phototmp);

        } else {

            total++;
        }
    }

    emit updateSelected(i);
    emit selectionChanged(photolist);


    if (i) {

        emit setCopyEnabled(true);
        emit setCutEnabled(true);
        emit setMoveUpEnabled(true);
        emit setMoveDownEnabled(true);
        emit setEditPhotoEnabled(true);
        popIconView->setItemEnabled(POPUPCPY, true);
        popIconView->setItemEnabled(POPUPCUT, true);

        popIconView->setItemEnabled(POPUPLFT, true);
        popIconView->setItemEnabled(POPUPRGT, true);
        popIconView->setItemEnabled(POPUPEDT, true);

        if (i==total) {
            emit setMoveUpEnabled(false);
            emit setMoveDownEnabled(false);

            popIconView->setItemEnabled(POPUPLFT, false);
            popIconView->setItemEnabled(POPUPRGT, false);
        }

        if (i>1) {
            emit setEditPhotoEnabled(false);
        }
    } else {
        emit setCopyEnabled(false);
        emit setCutEnabled(false);
        emit setMoveUpEnabled(false);
        emit setMoveDownEnabled(false);
        emit setEditPhotoEnabled(false);

        popIconView->setItemEnabled(POPUPCPY, false);
        popIconView->setItemEnabled(POPUPCUT, false);

        popIconView->setItemEnabled(POPUPLFT, false);
        popIconView->setItemEnabled(POPUPRGT, false);
        popIconView->setItemEnabled(POPUPEDT, false);
    }

}

void KalbumView::sortDateInc() {

    KIconView1->setSortMode("Date Inc");
    KIconView1->sort();

}

void KalbumView::sortDateDec() {

    KIconView1->setSortMode("Date Dec");
    KIconView1->sort();

}

void KalbumView::sortTitleInc() {

    KIconView1->setSortMode("Title Inc");
    KIconView1->sort();

}

void KalbumView::sortTitleDec() {

    KIconView1->setSortMode("Title Dec");
    KIconView1->sort();

}

void KalbumView::statusMsg(const QString &text) {
    KalbumApp *theApp=(KalbumApp *) parentWidget();

    theApp->slotStatusMsg(text);

}

void KalbumView::slotUpdateTotal(){

  emit updateTotal(album->count());

}

