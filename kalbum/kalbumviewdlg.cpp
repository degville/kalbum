#include <klocale.h>
/****************************************************************************
** Form implementation generated from reading ui file './kalbumviewdlg.ui'
**
** Created: Wed Jun 26 17:09:50 2002
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#include "kalbumviewdlg.h"

#include <qvariant.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>
#include <kiconview.h>
#include <qsplitter.h>

static const char* const image0_data[] = { 
"32 32 18 1",
". c None",
"l c #0058c0",
"m c #303030",
"c c #585858",
"i c #58a8ff",
"a c #800000",
"b c #808080",
"# c #a0a0a0",
"g c #a8dcff",
"e c #c00000",
"n c #c000c0",
"o c #c0ffc0",
"p c #c0ffff",
"d c #c3c3c3",
"j c #dcdcdc",
"f c #ff0000",
"k c #ffc0ff",
"h c #ffffff",
"................................",
"...............#aaa.............",
"...........#aaaabbbbc...........",
"........#aaabbbbdddddbc.........",
".....#aaabbbddddbbbbbbcee.......",
".....aabdddbbbbccfffeeeee.......",
".....fefbbccfffeeeeeeeeea.......",
".....fafffeeeeeeebgbeeeea.......",
".....eafeeeeeeeehgggcbeea.......",
".....eafeeeeeeecggigigija.......",
".....eafeeeeeeeggggigiija.......",
".....eafeeeeeeehhkkkigila.......",
".....eaecimeeeejhkkknnila.......",
".....ecchgimeeeechhhnnlca.......",
"....cdhdddgimeeeeaacjgiaa.......",
"....dhbbdgghimeeeaaaaaaaa.......",
"...digbb##gghmaeeaaaaaaaa.......",
"...diggbb##dmaaaeeaaaaaaa.......",
"...cdgg#dddcaaaeeeeeeeaaa.......",
"....cdghdccaaaaeeeeeeeeaa.......",
".....emccaaaaaeeeeeeeeeea.......",
".....eaaaaaaaeeeeeeeeeeea.......",
".....eaaaaaaeeeeeeeeddcea.......",
".....eaaaaaaeeeeeeegggdcc.......",
".....eafaaaeeeeeeeoggggiidcc....",
".....eaeaaeeeeeeehhooopgiiid....",
".....eafeeeeeeeecdhoophgiiic....",
".....#afeeeeeeaaacdhhphhiic.....",
"......#eeeaa##....ccdpphhc......",
"....................ccddcc......",
".......................cc.......",
"................................"};



/* 
 *  Constructs a KalbumViewDlg which is a child of 'parent', with the 
 *  name 'name' and widget flags set to 'f'.
 */
KalbumViewDlg::KalbumViewDlg( QWidget* parent,  const char* name, WFlags fl )
    : QWidget( parent, name, fl )
{
    QPixmap image0( ( const char** ) image0_data );
    if ( !name )
	    setName( "KalbumViewDlg" );
    resize( 668, 673 ); 
    
    setAcceptDrops( TRUE );
    KalbumViewDlgLayout = new QGridLayout( this, 2, 1, 0, 4, "KalbumViewDlgLayout");
    QSpacerItem* spacer = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    KalbumViewDlgLayout->addItem( spacer, 0, 0 );

    KIconView1 = new KalbumIconView( this, "KIconView1" );

    KIconView1->setBackgroundOrigin( KIconView::WidgetOrigin );
    KIconView1->setFrameShape( KIconView::Panel );
    KIconView1->setFrameShadow( KIconView::Sunken );
    KIconView1->setLineWidth( 2 );
    KIconView1->setMargin( 1 );
    KIconView1->setSelectionMode( KIconView::Extended );
    KIconView1->setGridX( -1 );
    KIconView1->setGridY( -1 );
    KIconView1->setSpacing( 20 );
    KIconView1->setItemTextPos( KIconView::Bottom );
    KIconView1->setArrangement( KIconView::LeftToRight );
    KIconView1->setResizeMode( KIconView::Adjust );
    KIconView1->setMaxItemWidth( 120 );
    KIconView1->setMaxItemTextLength( 128 );
    KIconView1->setAutoArrange( TRUE );
    KIconView1->setItemsMovable( FALSE );
    KIconView1->setWordWrapIconText( TRUE );
    KIconView1->setMode( KIconView::Select );

    KalbumViewDlgLayout->addWidget( KIconView1, 0, 0 );

    // signals and slots connections
    connect( KIconView1, SIGNAL( dropped(QDropEvent*, int ) ), this, SLOT( slotDropEvent(QDropEvent *, int)));
//    connect( KIconView1, SIGNAL( moved() ), this, SLOT( slotDragIcon() ) );
    connect( KIconView1, SIGNAL( rightButtonClicked(QIconViewItem*,const QPoint&) ), this, SLOT( slotRHClick() ) );
    connect( KIconView1, SIGNAL( selectionChanged() ), this, SLOT( slotSelection() ) );
    connect( KIconView1, SIGNAL( executed(QIconViewItem*, const QPoint&) ), this, SLOT( slotPhotoProp(QIconViewItem *)) );
}

/*  
 *  Destroys the object and frees any allocated resources
 */
KalbumViewDlg::~KalbumViewDlg()
{
    // no need to delete child widgets, Qt does it all for us
}

void KalbumViewDlg::slotPrevProp()
{
    qWarning( "KalbumViewDlg::slotPrevProp(): Not implemented yet!" );
}

void KalbumViewDlg::slotDragIcon()
{
    qWarning( "KalbumViewDlg::slotDragIcon(): Not implemented yet!" );
}

void KalbumViewDlg::slotDropEvent( QDropEvent *event, int drop_index )
{
    qWarning( "KalbumViewDlg::slotDropEvent(QDropEvent *): Not implemented yet!" );
}

void KalbumViewDlg::slotNextProp()
{
    qWarning( "KalbumViewDlg::slotNextProp(): Not implemented yet!" );
}

void KalbumViewDlg::slotPhotoProp(QIconViewItem *)
{
    qWarning( "KalbumViewDlg::slotPhotoProp(KalbumViewItem *): Not implemented yet!" );
}

void KalbumViewDlg::slotRHClick()
{
    qWarning( "KalbumViewDlg::slotRHClick(): Not implemented yet!" );
}

void KalbumViewDlg::slotSelection()
{
    qWarning( "KalbumViewDlg::slotSelection(): Not implemented yet!" );
}

void KalbumViewDlg::slotCut()
{

}

void KalbumViewDlg::slotCopy()
{

}

void KalbumViewDlg::slotPaste()
{

}

void KalbumViewDlg::slotMoveLeft()
{

}

void KalbumViewDlg::slotMoveRight()
{

}

void KalbumViewDlg::slotEditPhoto()
{

}




#include "kalbumviewdlg.moc"





