/***************************************************************************
                          ycpdemoview.h  -  description
                             -------------------
    begin                : Die Feb  6 16:04:40 MET 2001
    copyright            : (C) 2001 by Gabi Strattner
    email                : gs@suse.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef YCPDEMOVIEW_H
#define YCPDEMOVIEW_H

// include files for QT
#include <qwidget.h>

#include <qvbox.h>
#include <qcheckbox.h>
#include <qframe.h>
#include <qlabel.h>
#include <qmultilineedit.h>
#include <qpushbutton.h>
#include <qtextview.h>
#include <qlayout.h>
#include <qvariant.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qsplitter.h>
#include <qwidget.h>

#include <stdio.h>
#include <string>
#include <stdlib.h>

// application specific includes
#include "ycpdemodoc.h"

/**
 * This class provides an incomplete base for your application view. 
 */

class YcpdemoView : public QWidget
{
   Q_OBJECT
   
public:
   YcpdemoView(QWidget *parent=0, const char *name=0, WFlags fl=0, YcpdemoDoc *doc=0);
   ~YcpdemoView();
   const QString getActualInput();
   
protected:
   bool showLogging(  QString &logfile, int *length, int *retryCounter, QTextView *view );
   void showErrorLog(  QString &logfile,  QTextView *view );
   bool runYcpCommand(char *command);
   void deleteFile(QString &filename);

signals:
   void setCaption(const QString &caption);
   
protected slots:
   void slotDocumentChanged();
   void slotCheckYcp();
   void slotRunYcp();
   void slotCheckTimerDone();
   void slotRunTimerDone();
   void slotButtonClicked();
   void slotDebugClicked();
   void slotClearClicked();
   
   
private:
   // save the document pointer (parameter of constructor)
   YcpdemoDoc*  document;
   // ycp input file
   QMultiLineEdit* ycpInput;
   // Output log file
   QTextView* textView1;
   // Error Messages
   QTextView* textView2;
   // RUN button
   QPushButton* runycp;
   // CHECK button
   QPushButton* checkycp;
   QPushButton* clearycp;
   
   // Timer (needed for reading the logfiles)
   QTimer *checkTimer;
   QTimer *runTimer;
   
   int	checkfileLength;
   int	logfileLength;
   int  checkfileRetries;
   int  logfileRetries;
   bool y2debug;
   
   QLabel* TextLabel2;

   QCheckBox* CheckBox2;
   QPushButton* PushButton6;
   QFrame* Line1;
   QLabel* Error_messages;
   QCheckBox* CheckBox1;
   QFrame* Line2;
   QLabel* Error_messages_2;

protected:
    QGridLayout* Form1Layout;
    QVBoxLayout* Layout10;
    QHBoxLayout* Layout8;
    QVBoxLayout* Layout4;
    QHBoxLayout* Layout3;
    QVBoxLayout* Layout7;
    QHBoxLayout* Layout9;
    bool event( QEvent* );

};

#endif
