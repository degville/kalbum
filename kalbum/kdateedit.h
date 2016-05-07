// 	$Id: kdateedit.h,v 1.1.1.1 2003/03/04 15:27:45 grahammorrison Exp $	

#ifndef _KDATEEDIT_H
#define _KDATEEDIT_H

#include <qhbox.h>
#include <qvbox.h>
#include <qdatetime.h>

class QLineEdit;
class QPushButton;
class KDatePicker;
class KDateValidator;

class KDateEdit : public QHBox
{
    Q_OBJECT
  public:
    KDateEdit(QWidget *parent=0, const char *name=0);
    virtual ~KDateEdit();

    bool inputIsValid();

  signals:
    void dateChanged(QDate);

  public slots:
    void setDate(QDate date);
    QDate getDate() const;
    void setEnabled(bool on);

  protected slots:
    void toggleDatePicker();
    void lineEnterPressed();
 
  private:
    QPushButton *mDateButton;
    QLineEdit *mDateEdit;
    KDatePicker *mDatePicker;
    QVBox *mDateFrame;
};

#endif
