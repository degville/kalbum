/***************************************************************************
                          photodetails.cpp  -  description
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
#include <qtextedit.h>
#include <qlineedit.h>
#include <kpixmap.h>
#include <qradiobutton.h>
#include <qlabel.h>
#include <kurllabel.h>
#include <kglobal.h>
#include <kstddirs.h>
#include <klocale.h>
#include <kimageeffect.h>
#include <qptrlist.h>
#include <qtoolbutton.h>
#include <kfiledialog.h>
#include <qdatetimeedit.h>
#include <kpushbutton.h>

#include "kdateedit.h"
#include "photodetails.h"

#define MAXLEN 250

PhotoDetails::PhotoDetails(QWidget *parent, const char *name ) : PhotoDetailsDlg(parent,name) {

    QString dir;

    KGlobal::dirs()->addResourceType("kalbum_data", KGlobal::dirs()->kde_default("data")
                                     + "kalbum/data/");

    dir = KGlobal::dirs()->findResourceDir("kalbum_data", "slides/slides01.png");
    dir = dir + "slides/slides01.png";

    slides.load(dir);

    KGlobal::dirs()->addResourceType("kalbum_data", KGlobal::dirs()->kde_default("data")
                                     + "kalbum/data/");

    dir = KGlobal::dirs()->findResourceDir("kalbum_data", "slides/slide01.png");
    dir = dir + "slides/slide01.png";

    slide.load(dir);

    PixmapLabel2->setPixmap(slide);

    oldfileurl = "";

    disableFields();

}
PhotoDetails::~PhotoDetails() {}

/** No descriptions */
void PhotoDetails::slotSelectionChanged(QPtrList <PhotoAlbum> photolist) {

    m_photo = photolist;

    modifiedtitle    =false;
    modifieddate     =false;
    modifieddatebool =false;
    modifiedtime     =false;
    modifiedtimebool =false;
    modifiedrotate   =false;
    modifiedurl      =false;
    modifiedcomment  =false;

    updateImageInfo();

}

void PhotoDetails::slotNextProp() {

    modifyImage();
    emit nextImage();

}

void PhotoDetails::updateImageInfo() {

    if (m_photo.count() == 0) {
        disableFields();
    } else if (m_photo.count() == 1) {
        enableFields();
        updateSingleImage();
    } else if (m_photo.count() > 1) {
        enableFields();
        updateMultipleImage();
    }

}

void PhotoDetails::modifyImage() {

    emit setModified();

    if (m_photo.count() == 1)
        modifySingleImage();
    else if (m_photo.count() > 1)
        modifyMultipleImage();

}

void PhotoDetails::modifySingleImage() {

    if (modifiedtitle) {
        m_photo.first()->title       =LineEditTitle->text();
        m_photo.first()->iconitem->setText(LineEditTitle->text());
    }

    if (modifieddate)
        m_photo.first()->date       	= photoDate->getDate();

    if (modifieddatebool)
        m_photo.first()->ignoredate 	= RadioButton1->isChecked();

    if (modifiedtime)
        m_photo.first()->time		= photoTime->time();

    if (modifiedtimebool)
        m_photo.first()->ignoretime	= RadioButton2->isChecked();

    if (modifiedrotate) {

        if (RadioButton_90->isOn())
            m_photo.first()->rotate = 90;
        else if (RadioButton_180->isOn())
            m_photo.first()->rotate = 180;
        else if (RadioButton_270->isOn())
            m_photo.first()->rotate = 270;
        else
            m_photo.first()->rotate = 0;

    }

    if (modifiedurl || modifiedrotate) {

        m_photo.first()->updateThumbnail();    // Update the thumbnail image for the iconview
        m_photo.first()->iconitem->setPixmap(m_photo.first()->thumbnail, true, true);
        PixmapLabel2->setPixmap(*(m_photo.first()->iconitem->pixmap()));
    }

    if (modifiedcomment)
        m_photo.first()->comment     =EditComment->text();

}

void PhotoDetails::modifyMultipleImage() {

    PhotoAlbum *photo;

    for (photo=m_photo.first(); photo!=0; photo=m_photo.next()) {

        if (modifiedtitle) {
            photo->title       = LineEditTitle->text();
            photo->iconitem->setText(photo->title);
        }

        if (modifieddate)
            photo->date        = photoDate->getDate();

        if (modifieddatebool)
            photo->ignoredate  = RadioButton1->isChecked();

        if (modifiedtime)
            photo->time  = photoTime->time();

        if (modifiedtimebool)
            photo->ignoretime  = RadioButton2->isChecked();

        if (modifiedrotate) {
            if (RadioButton_90->isOn())
                photo->rotate = 90;
            else if (RadioButton_180->isOn())
                photo->rotate = 180;
            else if (RadioButton_270->isOn())
                photo->rotate = 270;
            else
                photo->rotate = 0;

        }

        if (modifiedcomment)
            photo->comment     =EditComment->text();

        if (modifiedurl) {
            photo->fileurl.setPath(KURLLabel1->url()+photo->fileurl.fileName());
        }

        if (modifiedurl || modifiedrotate) {

            photo->updateThumbnail();    // Update the thumbnail image for the iconview
            photo->iconitem->setPixmap(photo->thumbnail, true, true);
            PixmapLabel2->setPixmap(*(photo->iconitem->pixmap()));

        }

        //    if (modifiedtime)
        //    if (modifiedtimebool)

    }
}

void PhotoDetails::updateSingleImage() {

    setCaption(m_photo.first()->fileurl.fileName());
    EditComment->setText(m_photo.first()->comment);
    LineEditTitle->setText(m_photo.first()->title);
    photoDate->setDate(m_photo.first()->date);
    photoTime->setTime(m_photo.first()->time);

    if (m_photo.first()->ignoretime) {
        RadioButton2->setChecked(true);
        photoTime->setEnabled(false);
    } else {
        RadioButton2->setChecked(false);
        photoTime->setEnabled(true);
    }
    
    if (m_photo.first()->ignoredate) {
        RadioButton1->setChecked(true);
        photoDate->setEnabled(false);
	
    } else {
        RadioButton1->setChecked(false);
        photoDate->setEnabled(true);
    }

    PixmapLabel2->setPixmap(*(m_photo.first()->iconitem->pixmap()));
    KURLLabel1->setURL(m_photo.first()->fileurl.path());
    KURLLabel1->setText(m_photo.first()->fileurl.path());
    oldfileurl = m_photo.first()->fileurl;

    switch (m_photo.first()->rotate) {
    case  90:
        RadioButton_90->setChecked(true);
        RadioButton_180->setChecked(false);
        RadioButton_270->setChecked(false);
        break;
    case 180:
        RadioButton_90->setChecked(false);
        RadioButton_180->setChecked(true);
        RadioButton_270->setChecked(false);
        break;
    case 270:
        RadioButton_90->setChecked(false);
        RadioButton_180->setChecked(false);
        RadioButton_270->setChecked(true);
        break;
    default:
        RadioButton_90->setChecked(false);
        RadioButton_180->setChecked(false);
        RadioButton_270->setChecked(false);
        break;
    }

    ButtonForward->setEnabled(true);
    ButtonBack->setEnabled(true);

    modifiedtitle    =false;
    modifieddate     =false;
    modifieddatebool =false;
    modifiedtime     =false;
    modifiedtimebool =false;
    modifiedrotate   =false;
    modifiedurl      =false;
    modifiedcomment  =false;

}

void PhotoDetails::updateMultipleImage() {

    setCaption("");
    EditComment->setText("");
    LineEditTitle->setText("");
    photoDate->setDate(m_photo.first()->date);
    RadioButton1->setChecked(true);
    photoTime->setTime(m_photo.first()->time);
    RadioButton2->setChecked(true);
    PixmapLabel2->setPixmap(slides);
    KURLLabel1->setURL("");
    KURLLabel1->setText(i18n("Change destination directory"));
    PixmapLabel2->setPixmap(blendIcon(m_photo.first()->iconitem->pixmap()));
    RadioButton_90->setChecked(false);
    RadioButton_180->setChecked(false);
    RadioButton_270->setChecked(false);
    oldfileurl = m_photo.first()->fileurl;

    ButtonForward->setEnabled(false);
    ButtonBack->setEnabled(false);

    modifiedtitle    =false;
    modifieddate     =false;
    modifieddatebool =false;
    modifiedtime     =false;
    modifiedtimebool =false;
    modifiedrotate   =false;
    modifiedurl      =false;
    modifiedcomment  =false;
}

void PhotoDetails::slotPrevProp() {

    modifyImage();
    emit prevImage();

}

void PhotoDetails::slotTextChanged() {

    int length;
    QString text;

    length = EditComment->length();

    if (length > MAXLEN) {
        EditComment->moveCursor( QTextEdit::MoveBackward,false);
        EditComment->del();

    }

}

void PhotoDetails::slotRotate() {}

void PhotoDetails::slotChangeURL() {

    if (m_photo.count() == 1) {

        KURL url=KFileDialog::getOpenURL(m_photo.first()->fileurl.path(),
                                         i18n("*.jpeg *.jpg *.gif *.xpm *.ppm *.pgm *.pnm *.png *.bmp *.tiff|Pictures\n*|All files"), this, i18n("Locate Photo..."));

        if (url.isValid()) {

          m_photo.first()->fileurl = url;
          KURLLabel1->setURL(m_photo.first()->fileurl.path());
          KURLLabel1->setText(m_photo.first()->fileurl.path());

          emit updatePreview(m_photo.first());

        }



    } else if (m_photo.count() > 1) {

        KURL url=KFileDialog::getExistingDirectory(m_photo.first()->fileurl.path(),
                 this, i18n("Locate Directory For Photos..."));

        if (url.isValid()) {

          KURLLabel1->setURL(url.path());
          KURLLabel1->setText(url.path());

        }

    }


}

void PhotoDetails::slotRotate90() {

    if (RadioButton_90->isChecked()) {
        RadioButton_180->setChecked(false);
        RadioButton_270->setChecked(false);
        emit rotateImage(90);
    } else {
        RadioButton_90->setChecked(false);
        emit rotateImage(0);
    }



}

void PhotoDetails::slotRotate180() {

    if (RadioButton_180->isChecked()) {
        RadioButton_90->setChecked(false);
        RadioButton_270->setChecked(false);
        emit rotateImage(180);
    } else {
        RadioButton_180->setChecked(false);
        emit rotateImage(0);
    }

}

void PhotoDetails::slotRotate270() {

    if (RadioButton_270->isChecked()) {
        RadioButton_90->setChecked(false);
        RadioButton_180->setChecked(false);
        emit rotateImage(270);
    } else {
        RadioButton_270->setChecked(false);
        emit rotateImage(0);
    }

}

void PhotoDetails::slotCancel() {

    if (modifiedurl) {
        m_photo.first()->fileurl=oldfileurl;
        emit updatePreview(m_photo.first());
    }

    emit rotateImage(m_photo.first()->rotate);
    updateImageInfo();


}

void PhotoDetails::slotAccept() {

    modifyImage();

}


KPixmap PhotoDetails::blendIcon(QPixmap *icon) {
    QImage buffer;
    QImage buffer_icon;

    buffer_icon = slides.copy();

    KPixmap buffer_pix;

    buffer = icon->convertToImage();

    buffer = KImageEffect::blend(buffer_icon, buffer, buffer, KImageEffect::All);

    buffer_pix.convertFromImage(buffer, QPixmap::Color);
    return buffer_pix;

}

void PhotoDetails::disableFields() {

    setCaption("");
    EditComment->setText("");
    LineEditTitle->setText("");
    RadioButton1->setChecked(true);
    RadioButton2->setChecked(true);
    PixmapLabel2->setPixmap(slide);
    KURLLabel1->setURL("");
    KURLLabel1->setText(i18n("No Images Selected"));
    RadioButton_90->setChecked(false);
    RadioButton_180->setChecked(false);
    RadioButton_270->setChecked(false);

    ButtonForward->setEnabled(false);
    ButtonBack->setEnabled(false);

    EditComment->setEnabled(false);
    PixmapLabel2->setEnabled(false);
    LineEditTitle->setEnabled(false);
    KURLLabel1->setEnabled(false);

    RadioButton1->setEnabled(false);
    RadioButton2->setEnabled(false);
    RadioButton_90->setEnabled(false);
    RadioButton_180->setEnabled(false);
    RadioButton_270->setEnabled(false);

    TextLabel1->setEnabled(false);
    TextLabel2->setEnabled(false);
    TextLabel1_2->setEnabled(false);
    TextLabel1_3->setEnabled(false);
    TextLabel1_4->setEnabled(false);
    TextLabel1_5->setEnabled(false);
    TextLabel1_2_2->setEnabled(false);
    TextLabel1_5_2->setEnabled(false);

    photoDate->setEnabled(false);
    photoTime->setEnabled(false);

    KPushButton1->setEnabled(false);
    KPushButton1_2->setEnabled(false);

}

void PhotoDetails::enableFields() {

    ButtonForward->setEnabled(true);
    ButtonBack->setEnabled(true);

    EditComment->setEnabled(true);
    PixmapLabel2->setEnabled(true);
    LineEditTitle->setEnabled(true);
    KURLLabel1->setEnabled(true);

    RadioButton1->setEnabled(true);
    RadioButton2->setEnabled(true);
    RadioButton_90->setEnabled(true);
    RadioButton_180->setEnabled(true);
    RadioButton_270->setEnabled(true);

    TextLabel1->setEnabled(true);
    TextLabel2->setEnabled(true);
    TextLabel1_2->setEnabled(true);
    TextLabel1_3->setEnabled(true);
    TextLabel1_4->setEnabled(true);
    TextLabel1_5->setEnabled(true);
    TextLabel1_2_2->setEnabled(true);
    TextLabel1_5_2->setEnabled(true);

    photoDate->setEnabled(true);
    photoTime->setEnabled(true);

    KPushButton1->setEnabled(true);
    KPushButton1_2->setEnabled(true);

}

void PhotoDetails::slotModifiedTitle() {

    modifiedtitle=true;

}

void PhotoDetails::slotModifiedDate() {

    modifieddate=true;

}

void PhotoDetails::slotModifiedDateBool() {

    modifieddatebool=true;

}

void PhotoDetails::slotModifiedTime() {

    modifiedtime=true;

}

void PhotoDetails::slotModifiedTimeBool() {

    modifiedtimebool=true;

}

void PhotoDetails::slotModifiedRotate() {

    modifiedrotate=true;

}

void PhotoDetails::slotModifiedURL() {

    modifiedurl=true;

}

void PhotoDetails::slotModifiedComment() {

    modifiedcomment=true;

}
