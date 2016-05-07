/***************************************************************************
                          exportweb.cpp  -  description
                             -------------------
    begin                : Mon Oct 21 2002
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

#include <qpixmap.h>
#include <qlabel.h>
#include <qslider.h>
#include <kimageeffect.h>
#include <kglobal.h>
#include <kstddirs.h>

#include "exportweb.h"

ExportWeb::ExportWeb(QWidget *parent, const char *name ) : exportWebDlg(parent,name) {

  QString dir;
  QPixmap image_pix;

  KGlobal::dirs()->addResourceType("kalbum_data", KGlobal::dirs()->kde_default("data") + "kalbum/data/");
  dir = KGlobal::dirs()->findResourceDir("kalbum_data", "kalbum_html/folder/screenshot.png");
  dir = dir + "kalbum_html/folder/screenshot.png";

  image.load(dir);
  image_pix.convertFromImage(image, QPixmap::Color);
  PixmapLabel1->setPixmap(image_pix);
  PixmapLabel1->setScaledContents(true);

  static char * red_xpm[] = {"1 1 2 1"," 	c None",".	c #FF0000","."};

  image_rgb = red_xpm;

}
ExportWeb::~ExportWeb(){
}

void ExportWeb::slotChangeHue(int amount)
{

  QImage image_tmp;
  QPixmap image_pix;
  int amount_other;

  image_tmp = image.copy();

  amount_other = saturation->value();

  image_tmp = KImageEffect::modulate(image_tmp, image_rgb, false, KImageEffect::Saturation, amount_other, KImageEffect::Red);
  image_tmp = KImageEffect::modulate(image_tmp, image_rgb, false, KImageEffect::HueShift, amount, KImageEffect::Red);

  image_pix.convertFromImage(image_tmp, QPixmap::Color);
  PixmapLabel1->setPixmap(image_pix);


}

void ExportWeb::slotChangeSat(int amount)
{

  QImage image_tmp;
  QPixmap image_pix;
  int amount_other;

  image_tmp = image.copy();

  amount_other = hue->value();

  image_tmp = KImageEffect::modulate(image_tmp, image_rgb, false, KImageEffect::HueShift, amount_other, KImageEffect::Red);
  image_tmp = KImageEffect::modulate(image_tmp, image_rgb, false, KImageEffect::Saturation, amount, KImageEffect::Red);

  image_pix.convertFromImage(image_tmp, QPixmap::Color);
  PixmapLabel1->setPixmap(image_pix);

}


void ExportWeb::slotToggle(bool on)
{

  QImage image_tmp;
  QPixmap image_pix;

  if (on) {
    image_tmp = image.copy();
    image_tmp = KImageEffect::modulate(image_tmp, image_rgb, false, KImageEffect::HueShift, hue->value(), KImageEffect::Red);
    image_tmp = KImageEffect::modulate(image_tmp, image_rgb, false, KImageEffect::Saturation, saturation->value(), KImageEffect::Red);

    image_pix.convertFromImage(image_tmp, QPixmap::Color);
    PixmapLabel1->setPixmap(image_pix);
  } else {
    image_tmp = image.copy();

    image_pix.convertFromImage(image_tmp, QPixmap::Color);
    PixmapLabel1->setPixmap(image_pix);
  }

}

