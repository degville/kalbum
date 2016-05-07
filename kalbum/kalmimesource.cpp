/***************************************************************************
                          kalmimesource.cpp  -  description
                             -------------------
    begin                : Tue Mar 12 2002
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

#include "kalmimesource.h"

#include <qbuffer.h>
#include <qdragobject.h>
#include <qdatastream.h>


KalMimeSource::KalMimeSource(QPtrList<PhotoAlbum> & photolist): m_photo(photolist)
{
}


const char * KalMimeSource::format(int number) const {
	
	const char *format = 0;
	
	switch (number)
	{
	case 0:
		format = "application/kalbum";
		break;
	case 1:
		format = "text/plain";
		break;
	}
	
	return format;
}

QByteArray KalMimeSource::encodedData(const char * format) const {
	
	QBuffer buffer;
	buffer.open(IO_WriteOnly);
	QDataStream stream(&buffer);
	PhotoAlbum *photo;
	QPtrList <PhotoAlbum> photolist;
	int howmany=0;
	
	photolist = m_photo;

//	photo=photolist.first();	
//	photo->writeToStream(stream);


//	photot=photolist.last();
// photot->writeToStream(stream);
	
	for (photo=photolist.first(); photo!=0; photo=photolist.next()){

			howmany++;

	}

	stream<<howmany;

	for (photo=photolist.first(); photo!=0; photo=photolist.next()){

			photo->writeToStream(stream);

	}

	return buffer.buffer();

}

