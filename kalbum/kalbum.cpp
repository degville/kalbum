/***************************************************************************
                          kalbum.cpp  -  description
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

// include files for QT
#include <qdir.h>
#include <qprinter.h>
#include <qpainter.h>
#include <qlayout.h>

// include files for KDE
#include <kiconloader.h>
#include <kmessagebox.h>
#include <kfiledialog.h>
#include <kmenubar.h>
#include <klocale.h>
#include <kconfig.h>
#include <kaction.h>
#include <kstdaction.h>
#include <kiconloader.h>
#include <kcmdlineargs.h>
#include <kfileitem.h>

#include <qptrlist.h>
#include <qprogressbar.h>
#include <kstatusbar.h>

// application specific includes
#include "kalbum.h"
#include "kalbumview.h"
#include "kalbumdoc.h"
#include "photodetails.h"
#include "previewimage.h"
#include "albuminfo.h"
#include "photoalbum.h"

#define ID_STATUS_MSG 1

KalbumApp::KalbumApp(QWidget* , const char* name): KDockMainWindow(0,name) {

    config=kapp->config();

    ///////////////////////////////////////////////////////////////////
    // call inits to invoke all other construction parts
    initStatusBar();
    initActions();
    initDocument();
    initView();

    readOptions();

    ///////////////////////////////////////////////////////////////////
    // disable actions at startup
    fileSave->setEnabled(false);
    fileSaveAs->setEnabled(true);
    filePrint->setEnabled(false);
    fileOpenRecent->setEnabled(true);
    editCut->setEnabled(false);
    editCopy->setEnabled(false);
    editMoveUp->setEnabled(false);
    editMoveDown->setEnabled(false);

    KCmdLineArgs *args = KCmdLineArgs::parsedArgs();

    if (args->count()){

      KURL base;
      base.setPath( QDir::currentDirPath() + '/' );
      KURL startDir = base;


      if ( args->isSet( "images" ) ){
        for ( int i = 0; i < args->count(); i++ ) {
          KURL url( base, args->arg( i ) );
          slotAddPhoto(url);      
        }
      } else {
        KURL url( base, args->arg( 0 ) );
        openDocumentFile ( url );
      } 

    }  

}

KalbumApp::~KalbumApp() {}

void KalbumApp::initActions() {
    fileNewWindow = new KAction(i18n("New &Window"), "window_new", 0, this, SLOT(slotFileNewWindow()), actionCollection(),"file_new_window");
    fileNew = KStdAction::openNew(this, SLOT(slotFileNew()), actionCollection());
    fileOpen = KStdAction::open(this, SLOT(slotFileOpen()), actionCollection());
    fileOpenRecent = KStdAction::openRecent(this, SLOT(slotFileOpenRecent(const KURL&)), actionCollection());
    fileSave = KStdAction::save(this, SLOT(slotFileSave()), actionCollection());
    fileSaveAs = KStdAction::saveAs(this, SLOT(slotFileSaveAs()), actionCollection());
    //  fileClose = KStdAction::close(this, SLOT(slotFileClose()), actionCollection());
    filePrint = KStdAction::print(this, SLOT(slotFilePrint()), actionCollection());
    fileQuit = KStdAction::quit(this, SLOT(slotFileQuit()), actionCollection());
    editCut = KStdAction::cut(this, SLOT(slotEditCut()), actionCollection());
    editCopy = KStdAction::copy(this, SLOT(slotEditCopy()), actionCollection());
    editPaste = KStdAction::paste(this, SLOT(slotEditPaste()), actionCollection());
    viewToolBar = KStdAction::showToolbar(this, SLOT(slotViewToolBar()), actionCollection());
    viewStatusBar = KStdAction::showStatusbar(this, SLOT(slotViewStatusBar()), actionCollection());

    //My Actions------------------

    fileOpenPhoto = new KAction(i18n("Insert &Photo(s)"), "addphoto", 0, this, SLOT(slotFileOpenPhoto()),
                                actionCollection(), "file_open_photo");
    fileExportHTML = new KAction(i18n("&Web Page Album"), "exphtml", 0, this, SLOT(slotFileExportHTML()),
                                 actionCollection(), "file_export_html");
    editMoveUp = new KAction(i18n("Move Photo(s) &Right"), "forward", 0, this, SLOT(slotEditMoveUp()),
                             actionCollection(), "edit_move_up");
    editMoveDown = new KAction(i18n("Move Photo(s) &Left"), "back", 0, this, SLOT(slotEditMoveDown()),
                               actionCollection(), "edit_move_down");
    sortDateInc = new KAction(i18n("Ascending by &Date"), "sort_incr", 0, this, SLOT(slotSortDateInc()),
                              actionCollection(), "sort_date_inc");
    sortDateDec = new KAction(i18n("Descending by Date"), "sort_decrease", 0, this, SLOT(slotSortDateDec()),
                              actionCollection(), "sort_date_dec");
    sortTitleInc = new KAction(i18n("Ascending by &Title"), "sort_incr", 0, this, SLOT(slotSortTitleInc()),
                               actionCollection(), "sort_title_inc");
    sortTitleDec = new KAction(i18n("&Descending by Title"), "sort_decrease", 0, this, SLOT(slotSortTitleDec()),
                               actionCollection(), "sort_title_dec");
    viewAlbumInfo = new KToggleAction(i18n("Show Album Info"), CTRL+Key_L, this, SLOT(slotViewAlbumInfo()),
				      actionCollection(), "view_album_info");
    viewPhotoDetails = new KToggleAction(i18n("Show Image Details"), CTRL+Key_D, this, SLOT(slotViewPhotoDetails()),
					 actionCollection(), "view_photo_details");
    viewPreviewImage = new KToggleAction(i18n("Show Preview Image"), CTRL+Key_R, this, SLOT(slotViewPreviewImage()),
                                         actionCollection(), "view_preview_image");
    viewThumbnails = new KToggleAction(i18n("Show &Thumbnails"), CTRL+Key_T, this, SLOT(slotViewThumbnails()),
                                       actionCollection(), "view_thumbnails");
    viewLayoutDefault = new KAction(i18n("&Default"), "", 0, this, SLOT(slotViewLayoutDefault()),
                                    actionCollection(), "view_layout_default");

    //  insertAlbum = new KAction(i18n("Insert &Album"), "thumbnail", 0, this, SLOT(slotInsertAlbum()),
    //			actionCollection(), "insert_album");

    //-----------------------------

    fileNewWindow->setStatusText(i18n("Opens a new application window"));
    fileNew->setStatusText(i18n("Creates a new document"));
    fileOpen->setStatusText(i18n("Opens an existing document"));
    fileOpenRecent->setStatusText(i18n("Opens a recently used file"));
    fileSave->setStatusText(i18n("Saves the actual document"));
    fileSaveAs->setStatusText(i18n("Saves the actual document as..."));
    //  fileClose->setStatusText(i18n("Closes the actual document"));
    filePrint ->setStatusText(i18n("Prints out the actual document"));
    fileQuit->setStatusText(i18n("Quits the application"));
    editCut->setStatusText(i18n("Cuts the selected section and puts it to the clipboard"));
    editCopy->setStatusText(i18n("Copies the selected section to the clipboard"));
    editPaste->setStatusText(i18n("Pastes the clipboard contents to actual position"));
    viewToolBar->setStatusText(i18n("Enables/disables the toolbar"));
    viewStatusBar->setStatusText(i18n("Enables/disables the statusbar"));

    viewAlbumInfo->setStatusText(i18n("Shows/hides the Album Info view"));
    viewPhotoDetails->setStatusText(i18n("Shows/hides the Image Details view"));
    viewPreviewImage->setStatusText(i18n("Shows/hides the Preview Image"));
    viewThumbnails->setStatusText(i18n("Shows/hides the Thumbnail view"));
    viewLayoutDefault->setStatusText(i18n("Restores the Windows to their default layout"));

    fileOpenPhoto->setStatusText(i18n("Inserts photo(s) into the current album"));
    editMoveUp->setStatusText(i18n("Moves selected photo(s) right"));
    editMoveDown->setStatusText(i18n("Moves selected photo(s) left"));

    sortTitleInc->setStatusText(i18n("Sorts selected images by their Title"));
    sortTitleDec->setStatusText(i18n("Sorts selected images by their Title"));
    sortDateInc->setStatusText(i18n("Sorts selected images by their Date/Time"));
    sortDateDec->setStatusText(i18n("Sorts selected images by their Date/Time"));

    //	insertAlbum->setStatusText(i18n("Inserts another album into the current one"));

    // use the absolute path to your kalbumui.rc file for testing purpose in createGUI();
    createGUI();

}


void KalbumApp::initStatusBar() {
    ///////////////////////////////////////////////////////////////////
    // STATUSBAR
    // TODO: add your own items you need for displaying current application status.

    statProg = new QProgressBar(statusBar(), "Progressbar");
    statProg->setFixedWidth( 100 );             // arbitrary width
    statProg->setCenterIndicator(true);
    //  statProg->setFrameStyle( QFrame::NoFrame );
//    statProg->setMargin( 0 );
//    statProg->setLineWidth(0);
    statProg->setBackgroundMode( QWidget::PaletteBackground );
//    statProg->setFrameStyle(QFrame::Panel | QFrame::Raised);

    statusBar()->addWidget(statProg, 0, true);
    statProg->setFixedHeight( statProg->sizeHint().height() - 8 );
    statusBar()->insertItem(i18n("Ready."), ID_STATUS_MSG);

    statLabelSelect = new QLabel(statusBar());
    statLabelSelect->setFrameStyle(QFrame::Panel | QFrame::Raised);
    statLabelSelect->setAlignment(AlignRight);
    statLabelSelect->setText(i18n("0 Images Selected"));

    statLabelTotal = new QLabel(statusBar());
    statLabelTotal->setFrameStyle(QFrame::Panel | QFrame::Raised);
    statLabelTotal->setAlignment(AlignRight);
    statLabelTotal->setText(i18n("Total: 0 Images"));

    statusBar()->addWidget(statLabelSelect, 3000);
    statusBar()->addWidget(statLabelTotal, 3000);


}

void KalbumApp::initDocument() {
    doc = new KalbumDoc(this);
    doc->newDocument();
}

void KalbumApp::initView() {
    ////////////////////////////////////////////////////////////////////
    // create the main widget here that is managed by KTMainWindow's view-region and
    // connect the widget to your document to display document contents.

    view = new KalbumView(this);
    doc->addView(view);
    dock_thumbnails = createDockWidget(i18n("Thumbnails"), SmallIcon("view_icon"), this, i18n("Thumbnails"));
    dock_thumbnails->setWidget(view);
    dock_thumbnails->setDockSite(KDockWidget::DockFullSite);
    setCentralWidget(dock_thumbnails);
    setMainDockWidget(dock_thumbnails);
    setCaption(doc->URL().fileName(),false);

    // Signals to allow the changing of the progress bar
    connect(view, SIGNAL( incProgress()), this, SLOT( slotIncProgress()));
    connect(view, SIGNAL( setProgress(int )), this, SLOT( slotSetProgress(int )));
    connect(view, SIGNAL( resetProgress(int , int)), this, SLOT( slotResetProgress(int , int )));
    connect(view, SIGNAL( setModified()), doc, SLOT( slotSetModified() ));
    connect(view, SIGNAL( setStatusMsg(const QString &)), this, SLOT( slotStatusMsg(const QString &)));
    connect(view, SIGNAL( updateSelected(int )), this, SLOT (slotUpdateSelected(int )));
    connect(view, SIGNAL( updateTotal(int )), this, SLOT (slotUpdateTotal(int )));
    connect(view, SIGNAL( executedIcon()), this, SLOT( slotViewPhotoDetails()));
    
    // Signals for dis/enabling the selection dependant item
    connect(view, SIGNAL( setCopyEnabled( bool )), this->editCopy, SLOT( setEnabled( bool )));
    connect(view, SIGNAL( setCutEnabled( bool )), this->editCut, SLOT( setEnabled( bool )));
    connect(view, SIGNAL( setMoveUpEnabled( bool )), this->editMoveUp, SLOT( setEnabled( bool )));
    connect(view, SIGNAL( setMoveDownEnabled( bool )), this->editMoveDown, SLOT( setEnabled( bool )));

    connect(dock_thumbnails, SIGNAL(iMBeingClosed()), SLOT(slotDockThumbnailsHidden()));
    connect(dock_thumbnails, SIGNAL(hasUndocked()), SLOT(slotDockThumbnailsHidden()));

    dock_preview_image = createDockWidget(i18n("Preview Image"), SmallIcon("frame_image"), 0, i18n("Preview Image"));
    preview_image = new PreviewImage(dock_preview_image);
    dock_preview_image->setWidget(preview_image);
    dock_preview_image->manualDock(dock_thumbnails, KDockWidget::DockRight, 80);
    dock_preview_image->setDockSite(KDockWidget::DockFullSite);

    connect(this, SIGNAL( togglePreview( bool )), preview_image, SLOT( slotVisiblePreview( bool )));

    connect(dock_preview_image, SIGNAL(iMBeingClosed()), SLOT(slotDockPreviewHidden()));
    connect(dock_preview_image, SIGNAL(hasUndocked()), SLOT(slotDockPreviewHidden()));

    dock_photo_details = createDockWidget(i18n("Photo Details"), SmallIcon("photoedit"), 0, i18n("Image Details"));
    photo_details = new PhotoDetails(dock_photo_details);
    dock_photo_details->setWidget(photo_details);
    dock_photo_details->manualDock(dock_thumbnails, KDockWidget::DockTop, 80);
    dock_photo_details->setDockSite(KDockWidget::DockFullSite);

    connect(dock_photo_details, SIGNAL(iMBeingClosed()), SLOT(slotDockDetailsHidden()));
    connect(dock_photo_details, SIGNAL(hasUndocked()), SLOT(slotDockDetailsHidden()));

    dock_album_info = createDockWidget(i18n("Album Info"), SmallIcon("info"), 0, i18n("Album Info"));
    album_info = new AlbumInfo(dock_album_info);
    dock_album_info->setWidget(album_info);
    dock_album_info->manualDock(dock_thumbnails, KDockWidget::DockCenter, 50);
    dock_album_info->setDockSite(KDockWidget::DockFullSite);

    connect(dock_album_info, SIGNAL(iMBeingClosed()), SLOT(slotDockAlbumHidden()));
    connect(dock_album_info, SIGNAL(hasUndocked()), SLOT(slotDockAlbumHidden()));

    // Update the preview image when the selection changes
    connect(view, SIGNAL( selectionChanged( QPtrList <PhotoAlbum> )), photo_details, SLOT( slotSelectionChanged( QPtrList <PhotoAlbum> )));
    connect(view, SIGNAL( selectionChanged( QPtrList <PhotoAlbum> )), preview_image, SLOT( slotSelectionChanged( QPtrList <PhotoAlbum> )));

    // connections from the image details window
    connect(photo_details, SIGNAL( updatePreview( PhotoAlbum * )), preview_image, SLOT( slotUpdatePreview( PhotoAlbum * )));
    connect(photo_details, SIGNAL( modifiedImage( PhotoAlbum *, int  )), view, SLOT( slotModifiedImage( PhotoAlbum *, int )));
    connect(photo_details, SIGNAL( nextImage()), view, SLOT( slotNextImage()));
    connect(photo_details, SIGNAL( prevImage()), view, SLOT( slotPrevImage()));
    connect(photo_details, SIGNAL( rotateImage(int )), preview_image, SLOT( slotRotateImage(int )));
    connect(photo_details, SIGNAL( setModified()), doc, SLOT( slotSetModified() ));

    // connections to the album information window
    connect(view, SIGNAL( albumNew()), album_info, SLOT( slotNewAlbum()));
    connect(doc, SIGNAL( albumNew()), album_info, SLOT( slotNewAlbum()));
    connect(view, SIGNAL( albumChanged( KalbumCredits )), album_info, SLOT( slotAlbumChanged( KalbumCredits)));
    connect(album_info, SIGNAL( modifiedAlbumInfo( KalbumCredits )), view, SLOT ( slotAlbumChanged( KalbumCredits)));


}



void KalbumApp::openDocumentFile(const KURL& url) {
    slotStatusMsg(i18n("Opening file..."));

    doc->openDocument( url);
    if (url.isValid())
        fileOpenRecent->addURL( url );
    setCaption(url.fileName(), false);
    fileSave->setEnabled(true);
    slotStatusMsg(i18n("Ready."));
}


KalbumDoc *KalbumApp::getDocument() const {
    return doc;
}

void KalbumApp::saveOptions() {
    config->setGroup("General Options");
    config->writeEntry("Geometry", size());
    config->writeEntry("Show Toolbar", viewToolBar->isChecked());
    config->writeEntry("Show Statusbar",viewStatusBar->isChecked());
    config->writeEntry("ToolBarPos", (int) toolBar("mainToolBar")->barPos());
    fileOpenRecent->saveEntries(config);

    // store dock configuration
    manager()->writeConfig(KGlobal::config(),"dock_configuration");
}


void KalbumApp::readOptions() {

    config->setGroup("General Options");

    // bar status settings
    bool bViewToolbar = config->readBoolEntry("Show Toolbar", true);
    viewToolBar->setChecked(bViewToolbar);
    slotViewToolBar();

    bool bViewStatusbar = config->readBoolEntry("Show Statusbar", true);
    viewStatusBar->setChecked(bViewStatusbar);
    slotViewStatusBar();

    // bar position settings
    KToolBar::BarPosition toolBarPos;
    toolBarPos=(KToolBar::BarPosition) config->readNumEntry("ToolBarPos", KToolBar::Top);
    toolBar("mainToolBar")->setBarPos(toolBarPos);

    fileOpenRecent->loadEntries(config);

    // restore dock configuration
    manager()->readConfig(KGlobal::config(),"dock_configuration");

    QSize size=config->readSizeEntry("Geometry");
    if(!size.isEmpty()) {
        resize(size);
    }
}

void KalbumApp::saveProperties(KConfig *_cfg) {
    if(doc->URL().fileName()!=i18n("Untitled") && !doc->isModified()) {
        // saving to tempfile not necessary

    } else {
        KURL url=doc->URL();
        _cfg->writeEntry("filename", url.url());
        _cfg->writeEntry("modified", doc->isModified());

        QString tempname = kapp->tempSaveName(url.url());
        QString tempurl= KURL::encode_string(tempname);
        KURL _url(tempurl);
        doc->saveDocument(_url);
    }
}


void KalbumApp::readProperties(KConfig* _cfg) {
    QString filename = _cfg->readEntry("filename", "");
    KURL url(filename);
    bool modified = _cfg->readBoolEntry("modified", false);
    if(modified) {
        bool canRecover;
        QString tempname = kapp->checkRecoverFile(filename, canRecover);
        KURL _url(tempname);

        if(canRecover) {
            doc->openDocument(_url);
            doc->setModified();
            setCaption(_url.fileName(),true);
            QFile::remove
                (tempname);
        }
    } else {
        if(!filename.isEmpty()) {
            doc->openDocument(url);
            setCaption(url.fileName(),false);
        }
    }
}

bool KalbumApp::queryClose() {
    return doc->saveModified();
}

bool KalbumApp::queryExit() {
    saveOptions();
    return true;
}

/////////////////////////////////////////////////////////////////////
// SLOT IMPLEMENTATION
/////////////////////////////////////////////////////////////////////

void KalbumApp::slotFileNewWindow() {
    slotStatusMsg(i18n("Opening a new application window..."));

    KalbumApp *new_window= new KalbumApp();
    new_window->show();

    slotStatusMsg(i18n("Ready."));
}

void KalbumApp::slotFileNew() {
    slotStatusMsg(i18n("Creating new document..."));

    if(!doc->saveModified()) {
        // here saving wasn't successful

    } else {
        doc->newDocument();
        setCaption(doc->URL().fileName(), false);
        fileSave->setEnabled(false);
    }

    slotStatusMsg(i18n("Ready."));
}

void KalbumApp::slotFileOpen() {
    slotStatusMsg(i18n("Opening file..."));

    if(!doc->saveModified()) {
        // here saving wasn't successful

    } else {
        KURL url=KFileDialog::getOpenURL(QString::null,
                                         i18n("*.kalbum |Albums\n*|All files"), this, i18n("Open File..."));
        if(!url.isEmpty()) {
            doc->newDocument();
            doc->openDocument(url);
            //			view->loadPhotoAlbum(url);
            setCaption(url.fileName(), false);
            fileOpenRecent->addURL( url );
        }
    }
    fileSave->setEnabled(true);
    slotStatusMsg(i18n("Ready."));
}


void KalbumApp::slotInsertAlbum() {
    slotStatusMsg(i18n("Inserting album..."));

    KURL url=KFileDialog::getOpenURL(QString::null,
                                     i18n("*.kalbum |Albums\n*|All files"), this, i18n("Open File..."));
    if(!url.isEmpty()) {
        doc->openDocument(url);
        //	view->loadPhotoAlbum(url);
        fileOpenRecent->addURL( url );
    }
    doc->setModified(true);
    slotStatusMsg(i18n("Ready."));
}

void KalbumApp::slotFileOpenPhoto() {
    KURL::List::ConstIterator it;
    int count=0;

    KURL::List url=KFileDialog::getOpenURLs(QString::null,
                                            i18n("*.jpeg *.jpg *.gif *.xpm *.ppm *.pgm *.pnm *.png *.bmp *.tiff|Pictures\n*|All files"), this, i18n("Open Photo..."));

    slotStatusMsg(i18n("Adding Image(s)..."));
    for (it = url.begin(); it != url.end(); ++it)
        count++;

    view->updateProgress(0, count);

    for (it = url.begin(); it != url.end(); ++it) {
        view->updateProgress();

        if((*it).isValid()) {

            doc->openPhoto(*it);
            //    	view->addPhotoItem(*it);
            //    	setCaption(*it.fileName(), false);
        }
    }
    view->updateProgress(0,0);
    slotStatusMsg(i18n("Ready."));
}

void KalbumApp::slotAddPhoto(const KURL& url) {

    slotStatusMsg(i18n("Adding Image(s)..."));

    doc->openPhoto(url);
    doc->setModified(true);
    slotStatusMsg(i18n("Ready."));
}



void KalbumApp::slotFileOpenRecent(const KURL& url) {
    slotStatusMsg(i18n("Opening file..."));

    if(!doc->saveModified()) {
        // here saving wasn't successful
    } else {

        //    fileOpenRecent->removeURL(url);
        doc->newDocument();
        doc->openDocument(url);
        fileSave->setEnabled(true);

        setCaption(url.fileName(), false);
    }

    slotStatusMsg(i18n("Ready."));
}

void KalbumApp::slotFileSave() {
    slotStatusMsg(i18n("Saving file..."));

    doc->saveDocument(doc->URL());

    slotStatusMsg(i18n("Ready."));

}

void KalbumApp::slotFileSaveAs() {
    QString checkFileEnd;
    slotStatusMsg(i18n("Saving file with a new filename..."));

    KURL url=KFileDialog::getSaveURL(QDir::currentDirPath(),
                                     i18n("*|All files"), this, i18n("Save as..."));


    if(!url.isEmpty()) {

        //   	view->savePhotoAlbum(url);

        doc->saveDocument(url);

        checkFileEnd = url.fileName();
        checkFileEnd = checkFileEnd.right(7);

        if ( checkFileEnd != FILEEX )
            url.setFileName(url.fileName() + FILEEX);

        fileOpenRecent->addURL(url);
        setCaption(url.fileName(),doc->isModified());
    }

    fileSave->setEnabled(true);
    slotStatusMsg(i18n("Ready."));
}

void KalbumApp::slotFileExportHTML() {
    slotStatusMsg(i18n("Exporting Album as HTML..."));
    /*
     KURL url=KFileDialog::getSaveURL(QDir::currentDirPath(),
            i18n("*|All files"), this, i18n("Export as..."));
     
     
      if(!url.isEmpty())
     {
     		doc->exportHTML(url);
     }
    */
    doc->exportHTML();

    slotStatusMsg(i18n("Ready."));

}


void KalbumApp::slotEditMoveUp() {

    view->editMoveUp();

}

void KalbumApp::slotEditMoveDown() {

    view->editMoveDown();
}

void KalbumApp::slotFileClose() {
    slotStatusMsg(i18n("Closing file..."));

    close();

    slotStatusMsg(i18n("Ready."));
}

void KalbumApp::slotFilePrint() {
    slotStatusMsg(i18n("Printing..."));

    QPrinter printer;
    if (printer.setup(this)) {
        view->print(&printer);
    }

    slotStatusMsg(i18n("Ready."));
}

void KalbumApp::slotFileQuit() {
    slotStatusMsg(i18n("Exiting..."));
    saveOptions();
    // close the first window, the list makes the next one the first again.
    // This ensures that queryClose() is called on each window to ask for closing
    KMainWindow* w;
    if(memberList) {
        for(w=memberList->first(); w!=0; w=memberList->first()) {
            // only close the window if the closeEvent is accepted. If the user presses Cancel on the saveModified() dialog,
            // the window and the application stay open.
            if(!w->close())
                break;
        }
    }
}

void KalbumApp::slotEditCut() {
    slotStatusMsg(i18n("Cutting selection..."));

    view->cutPhotoItem();

    slotStatusMsg(i18n("Ready."));
}

void KalbumApp::slotEditCopy() {
    slotStatusMsg(i18n("Copying selection to clipboard..."));

    view->copyPhotoItem();

    slotStatusMsg(i18n("Ready."));
}

void KalbumApp::slotEditPaste() {
    slotStatusMsg(i18n("Inserting clipboard contents..."));

    view->pastePhotoItem();

    slotStatusMsg(i18n("Ready."));
}

void KalbumApp::slotEditPhoto() {
    slotStatusMsg(i18n("Editing Photo Properties..."));

    view->editPhotoItem();

    slotStatusMsg(i18n("Ready."));
}


void KalbumApp::slotViewToolBar() {
    slotStatusMsg(i18n("Toggling toolbar..."));
    ///////////////////////////////////////////////////////////////////
    // turn Toolbar on or off
    if(!viewToolBar->isChecked()) {
        toolBar("mainToolBar")->hide();
    } else {
        toolBar("mainToolBar")->show();
    }

    slotStatusMsg(i18n("Ready."));
}

void KalbumApp::slotViewStatusBar() {
    slotStatusMsg(i18n("Toggle the statusbar..."));
    ///////////////////////////////////////////////////////////////////
    //turn Statusbar on or off

    if(!viewStatusBar->isChecked()) {
        statusBar()->hide();
    } else {
        statusBar()->show();
    }

    slotStatusMsg(i18n("Ready."));
}

void KalbumApp::slotViewAlbumInfo() {

    dock_album_info->changeHideShowState();
    slotCheckDockWidgetStatus();


}

void KalbumApp::slotViewPhotoDetails() {

    dock_photo_details->changeHideShowState();
    slotCheckDockWidgetStatus();

}

void KalbumApp::slotViewPreviewImage() {

    dock_preview_image->changeHideShowState();
    slotCheckDockWidgetStatus();

}

void KalbumApp::slotViewThumbnails() {

    dock_thumbnails->changeHideShowState();
    slotCheckDockWidgetStatus();

}

void KalbumApp::slotCheckDockWidgetStatus() {

    viewAlbumInfo->setChecked(dock_album_info->isVisible());
    viewPhotoDetails->setChecked(dock_photo_details->isVisible());
    viewPreviewImage->setChecked(dock_preview_image->isVisible());
    viewThumbnails->setChecked(dock_thumbnails->isVisible());
    emit togglePreview(dock_preview_image->isVisible());


}

void KalbumApp::slotViewLayoutDefault() {


    dock_thumbnails->setDockSite(KDockWidget::DockFullSite);
    setCentralWidget(dock_thumbnails);
    setMainDockWidget(dock_thumbnails);

    dock_preview_image->manualDock(dock_thumbnails, KDockWidget::DockRight, 80);
    dock_preview_image->setDockSite(KDockWidget::DockFullSite);

    dock_album_info->manualDock(dock_photo_details, KDockWidget::DockCenter, 50);
    dock_album_info->setDockSite(KDockWidget::DockFullSite);

    dock_photo_details->manualDock(dock_preview_image, KDockWidget::DockTop, 50);
    dock_photo_details->setDockSite(KDockWidget::DockFullSite);



}

void KalbumApp::showEvent(QShowEvent *) {
    slotCheckDockWidgetStatus();
}

void KalbumApp::slotDockAlbumHidden() {
    viewAlbumInfo->setChecked(false);
}

void KalbumApp::slotDockPreviewHidden() {
    viewPreviewImage->setChecked(false);

    emit togglePreview(false);

}

void KalbumApp::slotDockDetailsHidden() {
    viewPhotoDetails->setChecked(false);
}

void KalbumApp::slotDockThumbnailsHidden() {
    viewThumbnails->setChecked(false);
}


void KalbumApp::slotStatusMsg(const QString &text) {
    ///////////////////////////////////////////////////////////////////
    // change status message permanently
    statusBar()->clear();
    statusBar()->changeItem(text, ID_STATUS_MSG);

    qApp->processEvents();
    
}

void KalbumApp::slotSortDateDec() {
    slotStatusMsg(i18n("Sorting photos by date..."));
    view->sortDateDec();
    slotStatusMsg(i18n("Ready."));
    doc->setModified(true);

}

void KalbumApp::slotSortDateInc() {
    slotStatusMsg(i18n("Sorting photos by date..."));
    view->sortDateInc();
    slotStatusMsg(i18n("Ready."));
    doc->setModified(true);

}

void KalbumApp::slotSortTitleDec() {
    slotStatusMsg(i18n("Sorting photos by their titles..."));
    view->sortTitleDec();
    slotStatusMsg(i18n("Ready."));
    doc->setModified(true);

}

void KalbumApp::slotSortTitleInc() {
    slotStatusMsg(i18n("Sorting photos by their titles..."));
    view->sortTitleInc();
    slotStatusMsg(i18n("Ready."));
    doc->setModified(true);

}

void KalbumApp::slotIncProgress() {

    int current=0;

    current = statProg->progress();
    statProg->setProgress(++current);


}

void KalbumApp::slotSetProgress(int current) {

    statProg->setProgress(current);

}


void KalbumApp::slotResetProgress(int current, int max) {

    if (max > 0) {
        statProg->setTotalSteps (max);
        statProg->setProgress(current);
    } else
        statProg->reset();

}

void KalbumApp::slotUpdateSelected(int selected){


  QString strInfo;

  if (selected == 0)
    {
      strInfo = i18n("0 Images Selected");
    }
  else if (selected != 1)
    {
      strInfo = i18n("%1 Images Selected")
        .arg(KGlobal::locale()->formatNumber(selected, 0));
    }
  else
    {
      strInfo = i18n("1 Image Selected");
    }

  statLabelSelect->setText(strInfo);

}


void KalbumApp::slotUpdateTotal(int total){


  QString strInfo;

  if (total == 0)
    {
      strInfo = i18n("Total: 0 Images");
    }
  else if (total != 1)
    {
      strInfo = i18n("Total: %1 Images")
        .arg(KGlobal::locale()->formatNumber(total, 0));
    }
  else
    {
      strInfo = i18n("Total: 1 Image");
    }

  statLabelTotal->setText(strInfo);

}




