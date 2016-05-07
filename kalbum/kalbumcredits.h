/***************************************************************************
                          kalbumcredits.h  -  description
                             -------------------
    begin                : Thu Mar 28 2002
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

#ifndef KALBUMCREDITS_H
#define KALBUMCREDITS_H

#include <qstring.h>
#include <kurl.h>
#include <qdatetime.h>

/**
  *@author Graham Morrison
  */

class KalbumCredits {
public: 
	KalbumCredits();
	~KalbumCredits();

	QString albumName;
	QString authorName;
	QString authorEmail;
	QString albumDescription;
	QDate startDate, endDate;
  bool ignoredates, exp_original, exp_war, exp_colour;
  KURL exp_url;
  int exp_saturation, exp_hue;

	KURL webAddress;
	
};

#endif
