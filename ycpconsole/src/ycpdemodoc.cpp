/***************************************************************************
                          ycpdemodoc.cpp  -  description
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

#include "ycpdemodoc.h"

YcpdemoDoc::YcpdemoDoc()
{
   ycpFilename = "";
   fileContents = "";
   modified = false;
}

YcpdemoDoc::~YcpdemoDoc()
{
}

void YcpdemoDoc::newDoc()
{

}

bool YcpdemoDoc::save(const QString & fileInput)
{
   QFile f(ycpFilename);

   if ( f.open(IO_WriteOnly) )
   {
      // file opened successfully
      f.writeBlock( fileInput, strlen(fileInput) );	// write file
      
      f.close();
   }
   
  modified = false;
  return true;
}

bool YcpdemoDoc::saveAs(const QString &filename)
{
   modified = false;
   ycpFilename = filename;
   return true;
} 

bool YcpdemoDoc::load(const QString &filename)
{
   ycpFilename = filename;

   // signal documentChanged
   emit documentChanged();
  
   return true;
}

void YcpdemoDoc::slotTextChanged()
{
   printf("Got signal modified.\n");
   modified = true;
}

bool YcpdemoDoc::isModified() const
{
  return modified;
}

const QString& YcpdemoDoc::getContents()
{
   QFile f(ycpFilename);

   fileContents = "";

   if ( f.open(IO_ReadOnly) ) {    // file opened successfully
      QTextStream t( &f );        // use a text stream
      
      while ( !t.eof() ) {       // until end of file...
	 fileContents = t.read();
      }
      f.close();
   }
   
   return fileContents;
}

const QString& YcpdemoDoc::getFilename()
{
   return ycpFilename;
}

#include <ycpdemodoc.moc.cpp>
