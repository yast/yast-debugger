// -*- c++ -*-

/*
 *  Author: Arvin Schnell <arvin@suse.de>
 */


#ifndef BASE_H
#define BASE_H


#include <qmainwindow.h>
#include <qpopupmenu.h>
#include <qlistview.h>
#include <qlineedit.h>
#include <qsocket.h>
#include <qstring.h>
#include <qlist.h>

#include "oscar.h"


class Bibo : public QMainWindow
{
    Q_OBJECT

public:

    Bibo (QWidget* parent = 0, const char* name = 0, WFlags f = WType_TopLevel);
    ~Bibo ();

protected slots:

    void about ();

    void enter ();

    void ycpconnect ();
    void ycpdetach ();

    void sock_incomming ();
    void sock_error (int);
    void sock_connected ();
    void sock_closed ();

    void quit ();

    void send_step ();
    void send_single ();
    void send_next ();
    void send_up ();
    void send_continue ();
    void send_interrupt ();

    void ernie_pressed (QListViewItem*, const QPoint&, int);

    void bernd_clicked (QListViewItem*);

    void hide_toolbar ();

private:

    void send (const char*, bool);

    void inform (const char*);

    QPopupMenu* menu_file;
    QPopupMenu* menu_control;
    QPopupMenu* menu_options;
    QPopupMenu* menu_help;
    int menu_id_hide_toolbar;

    QToolBar* toolbar;

    QPopupMenu* menu_source;
    int menu_id_set_break, menu_id_clear_break;

    QListView* ernie;
    QListView* bernd;
    QLineEdit* input;

    QFont* font;

    QSocket* sock;

    bool insideblock;
    QList <QString> block;

    void handle_source ();
    void handle_ycpblock (int);
    void handle_breakpoints ();

    void mark_source ();

    void set_position (const char*);

    QPixmap* icon_arrow;
    QPixmap* icon_stop;
    QPixmap* icon_arrowstop;

    struct Position
    {
        QString file;
        int line;

        void setpos (const QString&, int);
    };

    Position current_pos;

    QList <Position> breakpoints;

    bool check_breakpoints (const QString&, int);

    Oscar* oscar[2];

    void reset ();

};


class ErnieItem : public QListViewItem
{

public:

    ErnieItem (QListView* parent, int line, const QString& text);

    int compare (QListViewItem* i, int col, bool ascending) const;

    int line () const { return _line; }

private:

    int _line;

};


class BerndItem : public QListViewItem
{

public:

    BerndItem (QListView* parent, int line, int what, const QString& text);

    int compare (QListViewItem* i, int col, bool ascending) const;

    void paintCell (QPainter*, const QColorGroup& cg, int column,
		    int width, int alignment);

    int what () const { return _what; }
    QString text () const { return QListViewItem::text (0); }

    static int linecnt;

    enum { INPUT, OUTPUT, ERROR };

private:

    int _line;
    int _what;

};


#endif
