// -*- c++ -*-

/*
 *  Author: Arvin Schnell <arvin@suse.de>
 */


#ifndef OSCAR_H
#define OSCAR_H


#include <qvbox.h>
#include <qtextview.h>
#include <qpushbutton.h>

#include <ycp/YCPParser.h>


class Oscar : public QVBox
{
    Q_OBJECT

public:

    Oscar (QWidget* parent = 0, const char* name = 0);

    void setText (const QString&);
    void setText (const YCPValue&);

private:

    QTextView* view;
    QPushButton* close;

    void indent (QString&, int level) const;
    bool dump (QString&, int level, const YCPValue& value) const;

};


#endif

