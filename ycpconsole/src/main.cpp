/***************************************************************************
                          main.cpp  -  description
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

#include <qapplication.h>
#include <qfont.h>
#include <qstring.h>
#include <qtextcodec.h>
#include <qtranslator.h>
#include "ycpdemo.h"


int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  
  a.setFont(QFont("helvetica", 12));
  
  // QTranslator tor( 0 );
  // set the location where your .qm files are in load() below as the last parameter instead of "."
  // for development, use "/" to use the english original as
  // .qm files are stored in the base project directory.
  //tor.load( QString("ycpdemo.") + QTextCodec::locale(), "." );
  //a.installTranslator( &tor );

  /* uncomment the following line, if you want a Windows 95 look*/
  // a.setStyle(WindowsStyle);

  YcpdemoApp *ycpdemo=new YcpdemoApp();
  a.setMainWidget(ycpdemo);

  ycpdemo->show();
  
  return a.exec();
}


