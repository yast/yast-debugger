/***************************************************************************
                          ycpdemo.cpp  -  description
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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <qaccel.h>
#include "ycpdemo.h"

#include "filesave.xpm"
#include "fileopen.xpm"
#include "filenew.xpm"

YcpdemoApp::YcpdemoApp()
{
   resize( 880, 797 );
   setCaption("Ycpdemo " VERSION);

  ///////////////////////////////////////////////////////////////////
  // call inits to invoke all other construction parts
  initActions();
  initMenuBar();
  initToolBar();
  initStatusBar();

  initDoc();
  initView();

  viewToolBar->setOn(true);
  viewStatusBar->setOn(true);
}

YcpdemoApp::~YcpdemoApp()
{
}

/** initializes all QActions of the application */
void YcpdemoApp::initActions(){

   QPixmap openIcon, saveIcon, newIcon;
   newIcon = QPixmap(filenew);
   openIcon = QPixmap(fileopen);
   saveIcon = QPixmap(filesave);


  fileNew = new QAction("New File", newIcon, "&New", QAccel::stringToKey("Ctrl+N"), this);
  fileNew->setStatusTip("Creates a new document");
  fileNew->setWhatsThis("New File\n\nCreates a new document");
  connect(fileNew, SIGNAL(activated()), this, SLOT(slotFileNew()));
  
  fileOpen = new QAction("Open File", openIcon, "&Open...", 0, this);
  fileOpen->setStatusTip("Opens an existing document");
  fileOpen->setWhatsThis("Open File\n\nOpens an existing document");
  connect(fileOpen, SIGNAL(activated()), this, SLOT(slotFileOpen()));

  fileSave = new QAction("Save File", saveIcon, "&Save", QAccel::stringToKey("Ctrl+S"), this);
  fileSave->setStatusTip("Saves the actual document");
  fileSave->setWhatsThis("Save File.\n\nSaves the actual document");
  connect(fileSave, SIGNAL(activated()), this, SLOT(slotFileSave()));

  fileSaveAs = new QAction("Save File As", "Save &as...", 0, this);
  fileSaveAs->setStatusTip("Saves the actual document under a new filename");
  fileSaveAs->setWhatsThis("Save As\n\nSaves the actual document under a new filename");
  connect(fileSaveAs, SIGNAL(activated()), this, SLOT(slotFileSave()));

  fileClose = new QAction("Close File", "&Close", QAccel::stringToKey("Ctrl+W"), this);
  fileClose->setStatusTip("Closes the actual document");
  fileClose->setWhatsThis("Close File\n\nCloses the actual document");
  connect(fileClose, SIGNAL(activated()), this, SLOT(slotFileClose()));

  fileQuit = new QAction("Exit", "E&xit", QAccel::stringToKey("Ctrl+Q"), this);
  fileQuit->setStatusTip("Quits the application");
  fileQuit->setWhatsThis("Exit\n\nQuits the application");
  connect(fileQuit, SIGNAL(activated()), this, SLOT(slotFileQuit()));

  editCut = new QAction("Cut", "Cu&t", QAccel::stringToKey("Ctrl+X"), this);
  editCut->setStatusTip("Cuts the selected section and puts it to the clipboard");
  editCut->setWhatsThis("Cut\n\nCuts the selected section and puts it to the clipboard");
  connect(editCut, SIGNAL(activated()), this, SLOT(slotEditCut()));

  editCopy = new QAction("Copy", "&Copy", QAccel::stringToKey("Ctrl+C"), this);
  editCopy->setStatusTip("Copies the selected section to the clipboard");
  editCopy->setWhatsThis("Copy\n\nCopies the selected section to the clipboard");
  connect(editCopy, SIGNAL(activated()), this, SLOT(slotEditCopy()));

  editPaste = new QAction("Paste", "&Paste", QAccel::stringToKey("Ctrl+V"), this);
  editPaste->setStatusTip("Pastes the clipboard contents to actual position");
  editPaste->setWhatsThis("Paste\n\nPastes the clipboard contents to actual position");
  connect(editPaste, SIGNAL(activated()), this, SLOT(slotEditPaste()));

  viewToolBar = new QAction("Toolbar", "Tool&bar", 0, this, 0, true);
  viewToolBar->setStatusTip("Enables/disables the toolbar");
  viewToolBar->setWhatsThis("Toolbar\n\nEnables/disables the toolbar");
  connect(viewToolBar, SIGNAL(toggled(bool)), this, SLOT(slotViewToolBar(bool)));

  viewStatusBar = new QAction("Statusbar", "&Statusbar", 0, this, 0, true);
  viewStatusBar->setStatusTip("Enables/disables the statusbar");
  viewStatusBar->setWhatsThis("Statusbar\n\nEnables/disables the statusbar");
  connect(viewStatusBar, SIGNAL(toggled(bool)), this, SLOT(slotViewStatusBar(bool)));

  helpAboutApp = new QAction("About", "&About...", 0, this);
  helpAboutApp->setStatusTip("About the application");
  helpAboutApp->setWhatsThis("About\n\nAbout the application");
  connect(helpAboutApp, SIGNAL(activated()), this, SLOT(slotHelpAbout()));

}

void YcpdemoApp::initMenuBar()
{
  ///////////////////////////////////////////////////////////////////
  // MENUBAR

  ///////////////////////////////////////////////////////////////////
  // menuBar entry fileMenu
  
  fileMenu=new QPopupMenu();
  fileNew->addTo(fileMenu);
  fileOpen->addTo(fileMenu);
  fileClose->addTo(fileMenu);
  fileMenu->insertSeparator();
  fileSave->addTo(fileMenu);
  fileSaveAs->addTo(fileMenu);
  fileMenu->insertSeparator();
  fileMenu->insertSeparator();
  fileQuit->addTo(fileMenu);

  ///////////////////////////////////////////////////////////////////
  // menuBar entry editMenu
  editMenu=new QPopupMenu();
  editCut->addTo(editMenu);
  editCopy->addTo(editMenu);
  editPaste->addTo(editMenu);

  ///////////////////////////////////////////////////////////////////
  // menuBar entry viewMenu
  viewMenu=new QPopupMenu();
  viewMenu->setCheckable(true);
  viewToolBar->addTo(viewMenu);
  viewStatusBar->addTo(viewMenu);
  ///////////////////////////////////////////////////////////////////
  // EDIT YOUR APPLICATION SPECIFIC MENUENTRIES HERE

  ///////////////////////////////////////////////////////////////////
  // menuBar entry helpMenu
  helpMenu=new QPopupMenu();
  helpAboutApp->addTo(helpMenu);

  ///////////////////////////////////////////////////////////////////
  // MENUBAR CONFIGURATION
  menuBar()->insertItem("&File", fileMenu);
  menuBar()->insertItem("&Edit", editMenu);
  menuBar()->insertItem("&View", viewMenu);
  menuBar()->insertSeparator();
  menuBar()->insertItem("&Help", helpMenu);

}

void YcpdemoApp::initToolBar()
{
  ///////////////////////////////////////////////////////////////////
  // TOOLBAR
  fileToolbar = new QToolBar(this, "file operations");
  fileNew->addTo(fileToolbar);
  fileOpen->addTo(fileToolbar);
  fileSave->addTo(fileToolbar);
  fileToolbar->addSeparator();
  QWhatsThis::whatsThisButton(fileToolbar);

}

void YcpdemoApp::initStatusBar()
{
  ///////////////////////////////////////////////////////////////////
  //STATUSBAR
  statusBar()->message("Ready.", 2000);
}

void YcpdemoApp::initDoc()
{
   doc=new YcpdemoDoc();
}


void YcpdemoApp::slotSetCaption(const QString &caption)
{
   setCaption( caption );
}

void YcpdemoApp::initView()
{
  ////////////////////////////////////////////////////////////////////
  // set the main widget here
  view=new YcpdemoView(this, "YcpdemoView", 0, doc );
  connect( view, SIGNAL( setCaption(const QString &)), this, SLOT( slotSetCaption(const QString &)) );
  setCentralWidget(view);
}

bool YcpdemoApp::queryExit()
{
  int exit=QMessageBox::information(this, "Quit...",
                                    "Do your really want to quit?",
                                    QMessageBox::Ok, QMessageBox::Cancel);

  if (exit==1)
  {

  }
  else
  {

  };

  return (exit==1);
}

/////////////////////////////////////////////////////////////////////
// SLOT IMPLEMENTATION
/////////////////////////////////////////////////////////////////////


void YcpdemoApp::slotFileNew()
{
  statusBar()->message("Creating new file...");
  doc->newDoc();
  statusBar()->message("Ready.");
}

void YcpdemoApp::slotFileOpen()
{
  statusBar()->message("Opening file...");

  QString fileName = QFileDialog::getOpenFileName(0,0,this);
  if (!fileName.isEmpty())
  {
    doc->load(fileName);
    setCaption(fileName);
    QString message="Loaded document: "+fileName;
    statusBar()->message(message, 2000);
  }
  else
  {
    statusBar()->message("Opening aborted", 2000);
  }
}


void YcpdemoApp::slotFileSave()
{
  statusBar()->message("Saving file...");
  doc->save(view->getActualInput());
  statusBar()->message("Ready.");
}

void YcpdemoApp::slotFileSaveAs()
{
  statusBar()->message("Saving file under new filename...");
  QString fn = QFileDialog::getSaveFileName(0, 0, this);
  if (!fn.isEmpty())
  {
    doc->saveAs(fn);
  }
  else
  {
    statusBar()->message("Saving aborted", 2000);
  }

  statusBar()->message("Ready.");
}

void YcpdemoApp::slotFileClose()
{
  statusBar()->message("Closing file...");

  statusBar()->message("Ready.");
}


void YcpdemoApp::slotFilePrint()
{
  statusBar()->message("Printing...");
  QPrinter printer;
  if (printer.setup(this))
  {
    QPainter painter;
    painter.begin(&printer);
    ///////////////////////////////////////////////////////////////////
    // TODO: Define printing by using the QPainter methods here

    painter.end();
  };

  statusBar()->message("Ready.");
}

void YcpdemoApp::slotFileQuit()
{
  statusBar()->message("Exiting application...");
  ///////////////////////////////////////////////////////////////////
  // exits the Application
  if(doc->isModified())
  {
    if(queryExit())
    {
      qApp->quit();
    }
    else
    {

    };
  }
  else
  {
    qApp->quit();
  };

  statusBar()->message("Ready.");
}

void YcpdemoApp::slotEditCut()
{
  statusBar()->message("Cutting selection...");

  statusBar()->message("Ready.");
}

void YcpdemoApp::slotEditCopy()
{
  statusBar()->message("Copying selection to clipboard...");

  statusBar()->message("Ready.");
}

void YcpdemoApp::slotEditPaste()
{
  statusBar()->message("Inserting clipboard contents...");

  statusBar()->message("Ready.");
}


void YcpdemoApp::slotViewToolBar(bool toggle)
{
  statusBar()->message("Toggle toolbar...");
  ///////////////////////////////////////////////////////////////////
  // turn Toolbar on or off

  if (toggle== false)
  {
    fileToolbar->hide();
  }
  else
  {
    fileToolbar->show();
  };

  statusBar()->message("Ready.");
}

void YcpdemoApp::slotViewStatusBar(bool toggle)
{
  statusBar()->message("Toggle statusbar...");
  ///////////////////////////////////////////////////////////////////
  //turn Statusbar on or off

  if (toggle == false)
  {
    statusBar()->hide();
  }
  else
  {
    statusBar()->show();
  }

  statusBar()->message("Ready.");
}

void YcpdemoApp::slotHelpAbout()
{
  QMessageBox::about(this,"About...",
                      "Ycpdemo\nVersion " VERSION "\n(c) 2001 by Gabi Strattner" );
}

#include <ycpdemo.moc.cpp>
