
/***************************************************************************
                          ycpdemoview.cpp  -  description
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

#include <qmessagebox.h>
#include <qtimer.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "ycpdemoview.h"
#include "ycpdemo.h"

QString checkLogfile = "/tmp/parselog";
QString errorLogfile = "/tmp/errorlog";
QString nonameFile = "/tmp/noname.ycp";

const int   EXAMPLE_NO = 6;
const QString ycpExamples[] = { "/suse/gs/installation.ycp",
			 "/suse/gs/inst_sw_single.ycp",
			 "/usr/share/doc/packages/ex3.ycp",
			 "/usr/share/doc/packages/ex4.ycp",
			 "/usr/share/doc/packages/shell.ycp",
			 "/usr/share/doc/packages/perl.ycp" };
const char DEBUG_ON[] = "export Y2DEBUG=1; ";


YcpdemoView::YcpdemoView( QWidget* parent,  const char* name, WFlags fl, YcpdemoDoc *doc )
    : QWidget( parent, name, fl )
{
   // save pointer to ycp input file information
   document = doc;
   // Y2DEBUG: default is on
   y2debug = true;
   // create the 2 timer
   checkTimer = new QTimer( this );
   runTimer = new QTimer( this );

   connect( runTimer, SIGNAL(timeout()), this, SLOT(slotRunTimerDone()) );
   connect( checkTimer, SIGNAL(timeout()), this, SLOT(slotCheckTimerDone()) );

    if ( !name )
	setName( "YcpdemoView" );

    Form1Layout = new QGridLayout( this );
    Form1Layout->setSpacing( 6 );
    Form1Layout->setMargin( 11 );

    Layout10 = new QVBoxLayout;
    Layout10->setSpacing( 6 );
    Layout10->setMargin( 0 );

    Layout8 = new QHBoxLayout;
    Layout8->setSpacing( 6 );
    Layout8->setMargin( 0 );

    Layout4 = new QVBoxLayout;
    Layout4->setSpacing( 6 );
    Layout4->setMargin( 0 );

    ycpInput = new QMultiLineEdit( this, "MultiLineEdit1" );
    Layout4->addWidget( ycpInput );

    Layout3 = new QHBoxLayout;
    Layout3->setSpacing( 6 );
    Layout3->setMargin( 0 );


    runycp = new QPushButton( this, "RunButton" );
    runycp->setText( "Run" );
    QFont runycp_font(  runycp->font() );
    runycp_font.setBold( TRUE );
    runycp->setFont( runycp_font );
    Layout3->addWidget( runycp );

    checkycp = new QPushButton( this, "CheckButton" );
    checkycp->setText( "Check" );
    QFont checkycp_font(  checkycp->font() );
    checkycp_font.setBold( TRUE );
    checkycp->setFont( checkycp_font );
    Layout3->addWidget( checkycp );

    clearycp = new QPushButton( this, "ClearButton" );
    clearycp->setText( "Clear" );
    QFont clearycp_font(  clearycp->font() );
    clearycp_font.setBold( TRUE );
    clearycp->setFont( clearycp_font );
    Layout3->addWidget( clearycp );
    connect(clearycp,SIGNAL(clicked()), this, SLOT(slotClearClicked()) );

    Layout4->addLayout( Layout3 );
    Layout8->addLayout( Layout4 );

    Layout7 = new QVBoxLayout;
    Layout7->setSpacing( 6 );
    Layout7->setMargin( 0 );

    TextLabel2 = new QLabel( this, "TextLabel2" );
    TextLabel2->setText( "Load an Example:" );
    QFont TextLabel2_font(  TextLabel2->font() );
    TextLabel2_font.setBold( TRUE );
    TextLabel2->setFont( TextLabel2_font );
    Layout7->addWidget( TextLabel2 );

    for ( int i = 1; i <= EXAMPLE_NO; i++ )
    {
       QString number;
       number.setNum(i);
       QString label;

       if ( i == EXAMPLE_NO-1 )
       {
	  label = QString("Shell Example");
       }
       else if  ( i == EXAMPLE_NO )
       {
	  label = QString("Perl Example");
       }
       else
       {
	  label = QString("Example ")+number;
       }

       QPushButton *button= new QPushButton( label, this, number );

       Layout7->addWidget( button );
       connect(button, SIGNAL(clicked()), this, SLOT(slotButtonClicked()));
    }

    CheckBox2 = new QCheckBox( this, "CheckBox2" );
    CheckBox2->setText( "Maximum Debug" );
    CheckBox2->setChecked(true);	// default is debug ON
    Layout7->addWidget( CheckBox2 );
    connect(CheckBox2, SIGNAL(clicked()), this, SLOT(slotDebugClicked()));

    //PushButton6 = new QPushButton( this, "PushButton6" );
    //PushButton6->setText( "Decrease max. debug" );
    //Layout7->addWidget( PushButton6 );

    QSpacerItem* spacer = new QSpacerItem( 20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding );
    Layout7->addItem( spacer );
    Layout8->addLayout( Layout7 );
    Layout10->addLayout( Layout8 );

    Line1 = new QFrame( this, "Line1" );
    Line1->setFrameStyle( QFrame::HLine | QFrame::Sunken );
    Layout10->addWidget( Line1 );

    Layout9 = new QHBoxLayout;
    Layout9->setSpacing( 6 );
    Layout9->setMargin( 0 );

    Error_messages = new QLabel( this, "Error_messages" );
    Error_messages->setText(  "Error messages"  );
    QFont Error_messages_font(  Error_messages->font() );
    Error_messages_font.setBold( TRUE );
    Error_messages->setFont( Error_messages_font );
    Layout9->addWidget( Error_messages );
    QSpacerItem* spacer_2 = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    Layout9->addItem( spacer_2 );

    CheckBox1 = new QCheckBox( this, "CheckBox1" );
    CheckBox1->setText(  "Show warnings also"  );
    Layout9->addWidget( CheckBox1 );
    Layout10->addLayout( Layout9 );

    textView1 = new QTextView( this, "TextView1" );
    Layout10->addWidget( textView1 );

    Line2 = new QFrame( this, "Line2" );
    Line2->setFrameStyle( QFrame::HLine | QFrame::Sunken );
    Layout10->addWidget( Line2 );

    Error_messages_2 = new QLabel( this, "Error_messages_2" );
    Error_messages_2->setText(  "Debug messages"  );
    QFont Error_messages_2_font(  Error_messages_2->font() );
    Error_messages_2_font.setBold( TRUE );
    Error_messages_2->setFont( Error_messages_2_font );
    Layout10->addWidget( Error_messages_2 );

    textView2 = new QTextView( this, "TextView2" );
    Layout10->addWidget( textView2 );

    Form1Layout->addLayout( Layout10, 0, 0 );

    /** connect doc with the view*/
    connect(doc, SIGNAL( documentChanged() ), this, SLOT( slotDocumentChanged() ));

    /** connect textChanged from MultiLineEdit with doc */
    connect(ycpInput, SIGNAL( textChanged() ), doc, SLOT( slotTextChanged()));

    /** connect clicked from PushButton to private slot */
    connect(runycp, SIGNAL( clicked() ), this, SLOT( slotRunYcp() ));
    connect(checkycp, SIGNAL( clicked() ), this, SLOT( slotCheckYcp() ));

}

/*
 *  Main event handler. Reimplemented to handle application
 *  font changes
 */
bool YcpdemoView::event( QEvent* ev )
{
    bool ret = QWidget::event( ev );
    if ( ev->type() == QEvent::ApplicationFontChange ) {
	QFont runycp_font(  runycp->font() );
	runycp_font.setBold( TRUE );
	runycp->setFont( runycp_font );
	QFont checkycp_font(  checkycp->font() );
	checkycp_font.setBold( TRUE );
	checkycp->setFont( checkycp_font );
	QFont clearycp_font(  clearycp->font() );
	clearycp_font.setBold( TRUE );
	clearycp->setFont( clearycp_font );
	QFont TextLabel2_font(  TextLabel2->font() );
	TextLabel2_font.setBold( TRUE );
	TextLabel2->setFont( TextLabel2_font );
	QFont Error_messages_font(  Error_messages->font() );
	Error_messages_font.setBold( TRUE );
	Error_messages->setFont( Error_messages_font );
	QFont Error_messages_2_font(  Error_messages_2->font() );
	Error_messages_2_font.setBold( TRUE );
	Error_messages_2->setFont( Error_messages_2_font );
    }
    return ret;
}

YcpdemoView::~YcpdemoView()
{
}

//
// slot is called - when SIGNAL documentChanged is emitted from YcpdemoDoc
//		  - internal when loading an example
//
void YcpdemoView::slotDocumentChanged()
{
   // printf("%s\n", (const char *) doc->getContents());

   // disconnect the textChanged SIGNAL (new file does not mean document is modified)
   disconnect(ycpInput, SIGNAL( textChanged() ), document, SLOT( slotTextChanged()));

   // update the ycpInput
   ycpInput->setText(document->getContents());

   // clear logging
   textView1->setText("");
   textView2->setText("");

   connect(ycpInput, SIGNAL( textChanged() ), document, SLOT( slotTextChanged()));
}

//
// resets all text widgets, sets filename to unnamed
//
void YcpdemoView::slotClearClicked()
{
   textView1->setText("");
   textView2->setText("");

   deleteFile( nonameFile );
   document->load( nonameFile );

   ycpInput->setText("");

   emit setCaption( nonameFile );
}

//
// returns contents of multiline edit widget
//
const QString YcpdemoView::getActualInput()
{
   return ycpInput->text();
}

//
// sets flag debug on/off
//
void YcpdemoView::slotDebugClicked()
{
   if ( y2debug )
      y2debug = false;
   else
      y2debug = true;
}

//
// runs script "parseycp"
//
void YcpdemoView::slotCheckYcp()
{
   char systemCommand[300];
   // initialisations
   checkfileLength = 0;
   checkfileRetries = 0;
   textView1->setText("");
   textView2->setText("");
   deleteFile( checkLogfile );

   sprintf( systemCommand, "parseycp < %s > %s &",
	   (const char*)  document->getFilename(), (const char*)checkLogfile );

   if ( runYcpCommand( systemCommand) )
   {
      if( checkTimer->isActive() )
	 checkTimer->stop();
      // start timer (until it will be stopped)
      checkTimer->start( 500, FALSE );
   }
}

//
// check whether the ycp input is changed and runs the given command
//
bool YcpdemoView::runYcpCommand(char *command)
{
   bool ret = false;

   if ( document->isModified() )
   {
      // show Popup "Save file?"
      printf("Document has changed!!!\n");

      int exit=QMessageBox::information(this, "Save file?",
					"File has changed\nDo you want to save the changes?",
					QMessageBox::Ok, QMessageBox::Cancel);

      if (exit==1)
      {
	 // write ycpInput to file
	 document->save( ycpInput->text() );
      }
      else
      {
	 printf("File not saved.\n");
      };
   }

   if ( strlen(document->getFilename()) == 0 || strlen(document->getContents()) == 0 )
   {
      printf("Nothing to do!!!\n");
      ret = false;
   }
   else
   {
      system( command );
      ret = true;
   }
   return ret;
}

//
// slot is executed everytime CheckTimer timed out
//
void YcpdemoView::slotCheckTimerDone()
{
   if ( !showLogging( checkLogfile, &checkfileLength, &checkfileRetries, textView2 ) )
   {
      checkTimer->stop();
      // TODO: get logfile name to check y2log Logfile for errors!
      showErrorLog( checkLogfile, textView1);
   }
}

void YcpdemoView::showErrorLog( QString &logfile, QTextView *view )
{
   char systemCommand[300];
   QString fileContents;

   sprintf(systemCommand, "grep \"<3>\" %s > %s", (const char *)logfile, (const char*)errorLogfile );

   runYcpCommand(systemCommand);

   QFile f(errorLogfile);
   if ( f.open(IO_ReadOnly) )
   {    // file opened successfully
      QTextStream t( &f );
      while ( !t.eof() )
      {
	 fileContents = t.read();
      }
      f.close();
   }

   view->setText(fileContents);
}


//
// Runs script "runycp"
//
void YcpdemoView::slotRunYcp()
{
   char systemCommand[300];
   logfileLength = 0;
   logfileRetries = 0;
   textView1->setText("");
   textView2->setText("");

   QString logfile;
   char *login = getlogin();

   if ( getuid() == 0 )		// root
   {
      logfile = QString("/var/log/YaST2/y2log");
   }
   else
   {
      logfile = QString( "/suse/")+QString(login)+QString("/.y2log") ;
   }

   deleteFile(logfile);

   sprintf( systemCommand, "%s /usr/lib/YaST2/bin/y2qt %s qt -T -geometry 640x480 -style=platinum -font '-gnu-unifont-medium-r-normal--16-160-75-75-p-80-iso10646-1' &",
	    y2debug?DEBUG_ON:"",
	    (const char*) document->getFilename() );

   if ( runYcpCommand(systemCommand) )
   {
      if( runTimer->isActive() )
	 runTimer->stop();
      // start timer (until it will be stopped)
      runTimer->start( 500, FALSE );
   }
}

//
// slot is executed everytime RunTimer timed out
//
void YcpdemoView::slotRunTimerDone()
{
   QString logfile;
   char *login = getlogin();

   if ( getuid() == 0 )		// root
   {
      logfile = QString("/var/log/YaST2/y2log");
   }
   else
   {
      logfile = QString( "/suse/")+QString(login)+QString("/.y2log") ;
   }


   if ( !showLogging( logfile, &logfileLength, &logfileRetries, textView2 ) )
   {
      runTimer->stop();
      showErrorLog( logfile, textView1 );
   }
}

//
// deletes file
//
void YcpdemoView::deleteFile(QString &filename)
{
   char systemCommand[300];

   sprintf(systemCommand, "rm -f %s &", (const char *)filename);

   system(systemCommand);
}

//
// slot load example n clicked
//
void YcpdemoView::slotButtonClicked()
{
   const QObject *button = sender();
   printf( "Button %s clicked\n", button->name() );

   // set example filename
   const QString example_no = button->name();
   const QString example_file = ycpExamples[example_no.toInt()-1];

   printf( "Loading file: %s\n",  (const char *)example_file );

   // load example file
   document->load( example_file );
   // show file
   slotDocumentChanged();

   emit setCaption( example_file );
}

//
// reads logfile and shows logging
//
bool YcpdemoView::showLogging( QString &logfile, int *fileLength,
			       int *counter, QTextView *view )
{
   bool ret = true;
   QString fileContents;
   QFile f(logfile);
   struct stat stat_info;

   printf("LOGGING to: %s\n", (const char *)logfile);

   if ( stat( logfile, &stat_info ) == 0 )
   {
      if ( stat_info.st_size == *fileLength )
      {
	 // the file exists and filesize has not changed since last call
	 if ( f.open(IO_ReadOnly) )
	 {
	    QTextStream t( &f );        // use a text stream

	    while ( !t.eof() ) {        // until end of file...
	       fileContents = t.read();
	    }
	    f.close();
	 }
	 view->setText(fileContents);
	 // TODO Show errorlog here!!
	 ret = false;
      }
      else
      {
	 // store actual filelength and try it again
	 *fileLength = stat_info.st_size;
	 ret = true;
      }
   }
   else
   {
      // count the retries
      (*counter)++;
      if (*counter > 5)
      {
	 ret = false;
      }
      else
	 ret = true;
   }

   printf("Length of logfile: %d, return: %s\n", *fileLength, ret?"true":"false" );

   return ret;
}

#include <ycpdemoview.moc.cpp>
