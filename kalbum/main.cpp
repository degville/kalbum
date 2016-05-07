/***************************************************************************
                          main.cpp  -  description
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

#include <kcmdlineargs.h>
#include <kaboutdata.h>
#include <klocale.h>

#include "kalbum.h"

static const char *description =
	I18N_NOOP("Kalbum is an application that allows you to\ncreate albums from a collection of images");
// INSERT A DESCRIPTION FOR YOUR APPLICATION HERE
	
	
static KCmdLineOptions options[] =
{
  { "+[File]", I18N_NOOP("file to open"), 0 },
  { "images", I18N_NOOP("Optional image filenames to add to a new album"), 0 },
  { 0, 0, 0 }
  // INSERT YOUR COMMANDLINE OPTIONS HERE
};

int main(int argc, char *argv[])
{

	KAboutData aboutData( "kalbum", I18N_NOOP("Kalbum"),
		VERSION, description, KAboutData::License_GPL,
		"(c) 2002-2003, Graham Morrison", 0, "http://www.paldandy.com/kalbum", "kalbum@paldandy.com");
	aboutData.addAuthor("Graham Morrison",0, "kalbum@paldandy.com");
	KCmdLineArgs::init( argc, argv, &aboutData );
	KCmdLineArgs::addCmdLineOptions( options ); // Add our own options.

  KApplication app;
 
  if (app.isRestored())
  {
    RESTORE(KalbumApp);
  }
  else 
  {
    KalbumApp *kalbum = new KalbumApp();

    kalbum->show();

  }

  return app.exec();
}  
