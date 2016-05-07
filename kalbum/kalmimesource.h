/***************************************************************************
                          kalmimesource.h  -  description
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

#ifndef KALMIMESOURCE_H
#define KALMIMESOURCE_H

#include <qmime.h>
#include "photoalbum.h"

/**
  *@author Graham Morrison
  */

class KalMimeSource : public QMimeSource {
public: 
	KalMimeSource(QPtrList<PhotoAlbum> & photolist);

  // Returns the formates supported
	virtual const char * format(int number) const;

	// Returns the encoded data
	virtual QByteArray encodedData(const char * format) const;

private:
	QPtrList<PhotoAlbum> m_photo;

};

#endif
