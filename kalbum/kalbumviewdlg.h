/****************************************************************************
** Form interface generated from reading ui file './kalbumviewdlg.ui'
**
** Created: Wed Jun 26 16:43:14 2002
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#ifndef KALBUMVIEWDLG_H
#define KALBUMVIEWDLG_H

#include <qvariant.h>
#include <qwidget.h>
#include <klistview.h>

#include "kalbumiconview.h"


class QVBoxLayout; 
class QHBoxLayout; 
class QGridLayout; 
class KIconView;
class KalbumViewItem;

class KalbumViewDlg : public QWidget
{ 
    Q_OBJECT

public:
    KalbumViewDlg( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
    ~KalbumViewDlg();

    KalbumIconView* KIconView1;

public slots:
    virtual void slotPrevProp();
    virtual void slotDragIcon();
    virtual void slotDropEvent(QDropEvent *event, int drop_index);
    virtual void slotNextProp();
    virtual void slotPhotoProp(QIconViewItem *);
    virtual void slotRHClick();
    virtual void slotSelection();
    virtual void slotCut();
    virtual void slotCopy();
    virtual void slotPaste();
    virtual void slotMoveLeft();
    virtual void slotMoveRight();
    virtual void slotEditPhoto();

protected:
    QGridLayout* KalbumViewDlgLayout;

};

#endif // KALBUMVIEWDLG_H
