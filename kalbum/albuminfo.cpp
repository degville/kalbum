/***************************************************************************
                          albuminfo.cpp  -  description
                             -------------------
    begin                : Tue Feb 11 2003
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

#include <qlineedit.h>
#include "kdateedit.h"
#include <qtextedit.h>
#include <qradiobutton.h>

#include "albuminfo.h"

#define MAXLEN 250

AlbumInfo::AlbumInfo(QWidget *parent, const char *name ) : AlbumInfoDlg(parent,name) {
}
AlbumInfo::~AlbumInfo(){
}

void AlbumInfo::slotNewAlbum(){

  album_info.albumName 				= "";
  album_info.webAddress 			= "http://";
  album_info.authorName				=	"";
  album_info.authorEmail			=	"";
  album_info.startDate        = (QDate::currentDate());
  album_info.endDate          = (QDate::currentDate());
  album_info.albumDescription	= "";
  album_info.ignoredates      = false;
  album_info.exp_colour       = false;
  album_info.exp_original     = false;
  album_info.exp_war          = false;
  album_info.exp_url          ="$HOME";
  album_info.exp_saturation   = 0;
  album_info.exp_hue          = 0;

  updateAlbum();

}

void AlbumInfo::updateAlbum() {

/*if (!album_info.albumName.isEmpty())
      albumName->setText(album_info.albumName);
  if (album_info.webAddress.isValid())
      webAddress->setText(album_info.webAddress.url());
  if (!album_info.authorName.isEmpty())
      authorName->setText(album_info.authorName);
  if (!album_info.authorEmail.isEmpty())
      authorEmail->setText(album_info.authorEmail);
  if (album_info.startDate.isValid())
      startDate->setDate(album_info.startDate);
  if (album_info.endDate.isValid())
      endDate->setDate(album_info.endDate);
  if (!album_info.albumDescription.isEmpty())
      albumDescription->setText(album_info.albumDescription);
*/

  albumName->setText(album_info.albumName);
  webAddress->setText(album_info.webAddress.url());
  authorName->setText(album_info.authorName);
  authorEmail->setText(album_info.authorEmail);
  startDate->setDate(album_info.startDate);
  endDate->setDate(album_info.endDate);
  albumDescription->setText(album_info.albumDescription);

  RadioButton3->setChecked(album_info.ignoredates);

}


void AlbumInfo::slotAlbumChanged(KalbumCredits album){

  album_info = album;

  updateAlbum();

}

void AlbumInfo::slotCancel(){

  updateAlbum();

}

void AlbumInfo::slotAccept(){

  KalbumCredits album;

  album.albumName 				= albumName->text();
  album.webAddress 				= webAddress->text();
  album.authorName				=	authorName->text();
  album.authorEmail				=	authorEmail->text();
  album.startDate					= startDate->getDate();
  album.endDate						= endDate->getDate();
  album.albumDescription	= albumDescription->text();
  album.ignoredates       = RadioButton3->isChecked();

  emit modifiedAlbumInfo(album);

}


void AlbumInfo::slotTextChanged()
{

  int length;
  QString text;

  length = albumDescription->length();

  if (length > MAXLEN){
      albumDescription->moveCursor( QTextEdit::MoveBackward,false);
      albumDescription->del();

  }

}




