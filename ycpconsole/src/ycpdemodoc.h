/***************************************************************************
                          ycpdemodoc.h  -  description
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
#ifndef YCPDEMODOC_H
#define YCPDEMODOC_H

// include files for QT
#include <qobject.h>
#include <qfile.h>
#include <qtextstream.h> 
#include <qstring.h>

// application specific includes

/**
  * the Document Class
  */

class YcpdemoDoc : public QObject
{
   Q_OBJECT

public:
   YcpdemoDoc();
   ~YcpdemoDoc();
   void newDoc();
   bool save(const QString &actualInput);
   bool saveAs(const QString &filename);
   bool load(const QString &filename);
   bool isModified() const;
   const QString& getContents();
   const QString& getFilename();
   
signals:
   void documentChanged() ;

protected slots:
   void slotTextChanged();
   
protected:
   bool modified;

private:
   QString ycpFilename;
   QString fileContents;

};

#endif
