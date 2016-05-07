/***************************************************************************
                          kalbumdoc.cpp  -  description
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
#include <qdir.h>
#include <qwidget.h>

// include files for KDE
#include <klocale.h>
#include <kmessagebox.h>
#include <kio/job.h>
#include <kio/netaccess.h>

//Includes added by GMM
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
#include "photoalbum.h"
// application specific includes
#include "kalbumdoc.h"
#include "kalbum.h"
#include "kalbumview.h"

QPtrList<KalbumView> *KalbumDoc::pViewList = 0L;

KalbumDoc::KalbumDoc(QWidget *parent, const char *name) : QObject(parent, name)
{
  if(!pViewList)
  {
    pViewList = new QPtrList<KalbumView>();
  }	

	pViewList->setAutoDelete(true);



	
}

KalbumDoc::~KalbumDoc()
{
}

void KalbumDoc::addView(KalbumView *view)
{
  pViewList->append(view);
}

void KalbumDoc::removeView(KalbumView *view)
{
  pViewList->remove(view);
}
void KalbumDoc::setURL(const KURL &url)
{

  doc_url=url;
}

const KURL& KalbumDoc::URL() const
{
  return doc_url;
}

void KalbumDoc::slotUpdateAllViews(KalbumView *sender)
{
  KalbumView *w;
  if(pViewList)
  {
    for(w=pViewList->first(); w!=0; w=pViewList->next())
    {
      if(w!=sender)
        w->repaint();
    }
  }

}

void KalbumDoc::slotSetModified()
{

  modified = true;

}


bool KalbumDoc::saveModified()
{
  bool completed=true;

  if(modified)
  {
    KalbumApp *win=(KalbumApp *) parent();
    int want_save = KMessageBox::warningYesNoCancel(win,
                                         i18n("The current file has been modified.\n"
                                              "Do you want to save it?"),
                                         i18n("Warning"));
    switch(want_save)
    {
      case KMessageBox::Yes:
           if (doc_url.fileName() == i18n("Untitled"))
           {
             win->slotFileSaveAs();
           }
           else
           {
             saveDocument(URL());
       	   };

       	   deleteContents();
           completed=true;
           break;

      case KMessageBox::No:
           setModified(false);
           deleteContents();
           completed=true;
           break;

      case KMessageBox::Cancel:
           completed=false;
           break;

      default:
           completed=false;
           break;
    }
  }

  return completed;
}

void KalbumDoc::closeDocument()
{
  deleteContents();
}

bool KalbumDoc::newDocument()
{
  /////////////////////////////////////////////////
  // TODO: Add your document initialization code here
  /////////////////////////////////////////////////
  modified=false;
  doc_url.setFileName(i18n("Untitled"));

	pViewList->current()->deleteAll();
  emit albumNew();

  return true;
}

bool KalbumDoc::openDocument(const KURL& url, const char *format /*=0*/)
{
//  QString tmpfile;
//  KIO::NetAccess::download( url, tmpfile );
  /////////////////////////////////////////////////
  // TODO: Add your document opening code here
  /////////////////////////////////////////////////

//  KIO::NetAccess::removeTempFile( tmpfile );

	setURL(url);
  pViewList->current()->loadPhotoAlbum(url);

	modified=false;
  return true;
}


bool KalbumDoc::openPhoto(const KURL& url, const char *format /*=0*/)
{

	
//  view->addPhotoItem(url);

	/* QString tmpfile;
	KPixmap buffer;	

	KIO::NetAccess::download( url, tmpfile );


	if (!buffer.load(tmpfile))
		return false;

	view->addPhotoItem(buffer, "This is the text");


  KIO::NetAccess::removeTempFile( tmpfile );  */

	pViewList->current()->addPhotoItem(url);
	
	modified=true;
	return true;
}


bool KalbumDoc::saveDocument(const KURL& url, const char *format /*=0*/)
{
  /////////////////////////////////////////////////
  // TODO: Add your document saving code here
  /////////////////////////////////////////////////

	setURL(url);

  if (pViewList->current()->savePhotoAlbum(url)){
  	modified=false;
  	return true;
	}

  return false;
}


bool KalbumDoc::exportHTML(const KURL& url, const char *format /*=0*/)
{

	if (pViewList->current()->exportHTML(url)){
  	return true;
	}

  return false;

}

bool KalbumDoc::exportHTML()
{

	pViewList->current()->exportHTML();

  return true;

}


void KalbumDoc::deleteContents()
{
  /////////////////////////////////////////////////
  // TODO: Add implementation to delete the document contents
  /////////////////////////////////////////////////

}



