

/*
 *  Author: Arvin Schnell <arvin@suse.de>
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <qpopupmenu.h>
#include <qmenubar.h>
#include <qsplitter.h>
#include <qvbox.h>
#include <qmessagebox.h>
#include <qheader.h>
#include <qstatusbar.h>
#include <qinputdialog.h>
#include <qtoolbar.h>
#include <qtoolbutton.h>

#include <ycp/Parser.h>

#include "base.h"
#include "utils.h"

#include "arrow.xpm"
#include "stop.xpm"
#include "arrowstop.xpm"

#include "base.moc.cc"


Bibo::Bibo (QWidget* parent, const char* name, WFlags f)
    : QMainWindow (parent, name, f)
{
    /* Main Menu */

    menu_file = new QPopupMenu (this);
    menuBar ()->insertSeparator ();
    menuBar ()->insertItem ("&File", menu_file);
    menu_file->insertItem ("&Connect", this, SLOT (ycpconnect ()));
    menu_file->insertItem ("&Detach", this, SLOT (ycpdetach ()));
    menu_file->insertSeparator ();
    menu_file->insertItem ("&Quit", this, SLOT (quit ()));

    menu_control = new QPopupMenu (this);
    menuBar ()->insertSeparator ();
    menuBar ()->insertItem ("&Control", menu_control);
    menu_control->insertItem ("Single", this, SLOT (send_single ()));
    menu_control->insertItem ("&Step", this, SLOT (send_step ()),
			      CTRL + Key_S);
    menu_control->insertItem ("&Next", this, SLOT (send_next ()));
    menu_control->insertItem ("&Up", this, SLOT (send_up ()));
    menu_control->insertItem ("&Continue", this, SLOT (send_continue ()));
    menu_control->insertSeparator ();
    menu_control->insertItem ("&Interrupt", this, SLOT (send_interrupt ()),
			      CTRL + Key_I);

    menu_options = new QPopupMenu (this);
    menuBar ()->insertSeparator ();
    menuBar ()->insertItem ("&Options", menu_options);
    menu_id_hide_toolbar = menu_options->insertItem ("&Hide Toolbar",
						     this, SLOT (hide_toolbar ()));

    menu_help = new QPopupMenu (this);
    menuBar ()->insertSeparator ();
    menuBar ()->insertItem ("&Help", menu_help);
    menu_help->insertItem ("&About", this, SLOT (about ()));

    /* Toolbar */

    toolbar = new QToolBar (this, "toolbar");

    QToolButton* tmp1;

    tmp1 = new QToolButton (QPixmap (), "Single", "Single", this,
			    SLOT (send_single ()), toolbar);
    tmp1->setText ("Single");

    toolbar->addSeparator ();

    tmp1 = new QToolButton (QPixmap (), "Step", "Step", this,
			    SLOT (send_step ()), toolbar);
    tmp1->setText ("Step");

    toolbar->addSeparator ();

    tmp1 = new QToolButton (QPixmap (), "Continue", "Continue", this,
			    SLOT (send_continue ()), toolbar);
    tmp1->setText ("Continue");

    toolbar->addSeparator ();

    tmp1 = new QToolButton (QPixmap (), "Interrupt", "Interrupt", this,
			    SLOT (send_interrupt ()), toolbar);
    tmp1->setText ("Interrupt");

    addToolBar (toolbar, "Toolbar", Top, FALSE);

    /* Other Widgets */

    QSplitter* splitter = new QSplitter (this);
    splitter->setOrientation (Vertical);

    QVBox* box1 = new QVBox (splitter);

    ernie = new QListView (box1);
    ernie->header ()->hide ();	// hide header
    ernie->addColumn ("Line");
    ernie->setColumnAlignment (0, Qt::AlignRight);
    ernie->addColumn ("Source");
    ernie->setColumnAlignment (1, Qt::AlignLeft);
    ernie->setAllColumnsShowFocus (true);
    ernie->setSelectionMode (QListView::NoSelection);
    connect (ernie, SIGNAL (rightButtonPressed (QListViewItem*, const QPoint&, int)),
	     this, SLOT (ernie_pressed (QListViewItem*, const QPoint&, int)));

    QVBox* box2 = new QVBox (splitter);

    bernd = new QListView (box2);
    bernd->header ()->hide ();	// hide header
    bernd->addColumn ("Output");
    bernd->setColumnAlignment (0, Qt::AlignLeft);
    bernd->setAllColumnsShowFocus (true);
    bernd->setSelectionMode (QListView::NoSelection);
    connect (bernd, SIGNAL (clicked (QListViewItem*)), this,
	     SLOT (bernd_clicked (QListViewItem*)));

    input = new QLineEdit (box2);
    connect (input, SIGNAL (returnPressed ()), this, SLOT (enter ()));
    input->setFocus ();

    setCentralWidget (splitter);

    /* Source Menu */

    menu_source = new QPopupMenu (ernie);
    menu_id_set_break = menu_source->insertItem ("Set Breakpoint");
    menu_id_clear_break = menu_source->insertItem ("Clear Breakpoint");

    /* Socket */

    sock = new QSocket;
    connect (sock, SIGNAL (readyRead ()), this, SLOT (sock_incomming ()));
    connect (sock, SIGNAL (error (int)), this, SLOT (sock_error (int)));
    connect (sock, SIGNAL (connected ()), this, SLOT (sock_connected ()));
    connect (sock, SIGNAL (connectionClosed ()), this, SLOT (sock_closed ()));
    connect (sock, SIGNAL (delayedCloseFinished ()), this, SLOT (sock_closed ()));

    /* Font */

    font = new QFont;
    font->setRawName ("-etl-fixed-medium-r---16--iso8859-1");

    ernie->setFont (*font);
    bernd->setFont (*font);
    input->setFont (*font);

    /* Misc Stuff */

    statusBar ()->message ("disconnected");

    oscar[0] = oscar[1] = 0;

    icon_arrow = new QPixmap (arrow_xpm);
    icon_stop = new QPixmap (stop_xpm);
    icon_arrowstop = new QPixmap (arrowstop_xpm);

    block.setAutoDelete (true);
    breakpoints.setAutoDelete (true);

    reset ();
}


Bibo::~Bibo ()
{
    delete sock;
    delete font;

    if (oscar[0])
	delete oscar[0];
    if (oscar[1])
	delete oscar[1];
}


void
Bibo::reset ()
{
    current_pos.setpos ("", -1);

    breakpoints.clear ();

    block.clear ();
    insideblock = false;

    mark_source ();
}


void
Bibo::about ()
{
    QMessageBox* about =
	new QMessageBox ("YCP Debugger",
			 "<center><b>The YCP Debugger</b></center>\n"
			 "<hr/>\n"
			 "<p>Author: Arvin Schnell &lt;arvin@suse.de&gt;</p>\n"
			 "<p>Copyright: SuSE Linux AG Nuernberg, Germany</p>",
			 QMessageBox::NoIcon,
			 QMessageBox::Ok + QMessageBox::Default,
			 0, 0, this);

    about->setMinimumSize (350, 150);
    about->exec ();

    delete about;
}


void
Bibo::enter ()
{
    QString command = input->text ();
    input->setText ("");

    if (command.isEmpty ()) {
	BerndItem* item = dynamic_cast<BerndItem*> (bernd->firstChild ());
	while (item) {
	    if (item->what () == BerndItem::INPUT)
		command = item->text ();
	    item = dynamic_cast<BerndItem*> (item->nextSibling ());
	}
    }

    if (!command.isEmpty ())
	send ((const char*) command, true);
}


ErnieItem::ErnieItem (QListView* parent, int line, const QString& text)
    : QListViewItem (parent)
{
    ErnieItem::_line = line;

    QString tmp;
    tmp.setNum (line);
    setText (0, tmp);

    setText (1, text);
}


int
ErnieItem::compare (QListViewItem* i, int col, bool ascending) const
{
    ErnieItem* ii = dynamic_cast <ErnieItem*> (i);
    return ascending ? _line - ii->_line : ii->_line - _line;
}


BerndItem::BerndItem (QListView* parent, int line, int what,
		      const QString& text)
    : QListViewItem (parent, text)
{
    BerndItem::_line = line;
    BerndItem::_what = what;
}


int
BerndItem::compare (QListViewItem* i, int col, bool ascending) const
{
    BerndItem* ii = dynamic_cast <BerndItem*> (i);
    return ascending ? _line - ii->_line : ii->_line - _line;
}


void
BerndItem::paintCell (QPainter* painter, const QColorGroup& cg, int column,
		      int width, int alignment)
{
    QColorGroup tmp (cg);
    switch (_what) {
	case OUTPUT:
	    tmp.setColor (QColorGroup::Text, Qt::black);
	    break;
	case INPUT:
	    tmp.setColor (QColorGroup::Text, Qt::blue);
	    break;
	case ERROR:
	    tmp.setColor (QColorGroup::Text, Qt::red);
	    break;
    }
    QListViewItem::paintCell (painter, tmp, column, width, alignment);
}


int BerndItem::linecnt = 1;


void
Bibo::ycpconnect ()
{
    if (sock->state () != QSocket::Idle)
	return;

    bool ok = false;
    QString host = QInputDialog::getText ("YCP Debugger", "Please enter host:",
					  QLineEdit::Normal, "localhost", &ok,
					  this);
    if (!ok || host.isEmpty ())
	return;

    sock->connectToHost (host, 19473);
    statusBar ()->message ("connecting");
}


void
Bibo::ycpdetach ()
{
    if (sock->state () == QSocket::Idle)
	return;

    sock->close ();
    statusBar ()->message ("disconnecting");
}


void
Bibo::sock_error (int num)
{
    QMessageBox::warning (0, "YCP Debugger", "Connection failed.");
}


void
Bibo::sock_connected ()
{
    statusBar ()->message ("connected");
}


void
Bibo::sock_closed ()
{
    statusBar ()->message ("disconnected");

    reset ();
}


void
Bibo::sock_incomming ()
{
    while (sock->canReadLine ()) {

	QString str = sock->readLine ();

	while (str.length () > 0 && (str[str.length () - 1] == '\n' ||
				     str[str.length () - 1] == '\r'))
	    str.remove (str.length () - 1, 1);

	if (str.left (5) == "BEGIN") {
	    insideblock = true;
	    block.clear ();
	    continue;
	}

	if (str.left (3) == "END") {
	    insideblock = false;

	    if (str == "END SOURCE") {
		handle_source ();
		continue;
	    }

	    if (str == "END VARIABLE") {
		handle_ycpblock (0);
		continue;
	    }

	    if (str == "END TOKEN") {
		handle_ycpblock (1);
		continue;
	    }

	    if (str == "END BREAKPOINTS") {
		handle_breakpoints ();
		continue;
	    }

	    for (QString* cur = block.first (); cur != 0;
		 cur = block.next ()) {
		BerndItem* item = new BerndItem (bernd, BerndItem::linecnt++,
						 BerndItem::OUTPUT, *cur);
		bernd->ensureItemVisible (item);
	    }

	    continue;
	}

	if (insideblock) {

	    block.append (new QString (str));

	} else {

	    BerndItem* item = new BerndItem (bernd, BerndItem::linecnt++,
					     BerndItem::OUTPUT, str);
	    bernd->ensureItemVisible (item);

	    if (str.left (9) == "position:")
		set_position (str);

	}

    }
}


void
Bibo::send_step ()
{
    send ("step", true);
}


void
Bibo::send_single ()
{
    send ("single", true);
}


void
Bibo::send_next ()
{
    send ("next", true);
}


void
Bibo::send_up ()
{
    send ("up", true);
}


void
Bibo::send_continue ()
{
    send ("continue", true);
}


void
Bibo::send_interrupt ()
{
    send ("interrupt", true);
}


void
Bibo::send (const char* command, bool visible)
{
    if (sock->state () != QSocket::Connection)
	return;

    if (visible) {
	BerndItem* item = new BerndItem (bernd, BerndItem::linecnt++,
					 BerndItem::INPUT, command);
	bernd->ensureItemVisible (item);
    }

    const int size = strlen (command) + 3;
    char* buffer = new char [size];
    strcpy (buffer, (const char*) command);
    strcat (buffer, "\r\n");
    sock->writeBlock (buffer, size - 1);
    delete[] buffer;
}


void
Bibo::inform (const char* command)
{
    if (sock->state () != QSocket::Connection)
	return;

    BerndItem* item = new BerndItem (bernd, BerndItem::linecnt++,
				     BerndItem::ERROR, command);
    bernd->ensureItemVisible (item);
}


void
Bibo::quit ()
{
    exit (EXIT_SUCCESS);
}


void
Bibo::handle_source ()
{
    ernie->clear ();

    int linecnt = 1;		// line numbers start at 1

    for (QString* cur = block.first (); cur != 0; cur = block.next ()) {
	QString tmp = untabify (*cur);
	new ErnieItem (ernie, linecnt++, (const char*) tmp);
    }

    mark_source ();
}


void
Bibo::mark_source ()
{
    ErnieItem* item = dynamic_cast <ErnieItem*> (ernie->firstChild ());
    while (item) {

	QPixmap* tmp = 0;

	if (item->line () == current_pos.line) {
	    tmp = icon_arrow;
	    ernie->ensureItemVisible (item);
	}

	if (check_breakpoints (current_pos.file, item->line ()))
	    tmp = tmp ? icon_arrowstop : icon_stop;

	item->setPixmap (0, tmp ? *tmp : 0);

	item = dynamic_cast <ErnieItem*> (item->nextSibling ());
    }
}


void
Bibo::set_position (const char* str)
{
    char file[250];
    int line;

    if (sscanf ((const char*) str, "position: %245s %u", file, &line) != 2) {
	inform ("parse error in backend output");
	return;
    }

    bool doit = false;

    if (current_pos.file != file) {
	current_pos.file = file;
	ernie->clear ();
	send ("list source", false);
    	doit = true;
    }

    if (current_pos.line != line) {
	current_pos.line = line;
	doit = true;
    }

    if (doit)
	mark_source ();
}


void
Bibo::ernie_pressed (QListViewItem* tmpitem, const QPoint& point, int)
{
    ErnieItem* item = dynamic_cast <ErnieItem*> (tmpitem);

    if (!item)
	return;

    bool isbreak = check_breakpoints (current_pos.file, item->line ());
    menu_source->setItemEnabled (menu_id_set_break, !isbreak);
    menu_source->setItemEnabled (menu_id_clear_break, isbreak);

    int id = menu_source->exec (point);

    if (id == menu_id_set_break) {
	QString tmp;
	tmp.sprintf ("break %s %d", (const char*) current_pos.file,
		     item->line ());
	send (tmp, false);
	return;
    }

    if (id == menu_id_clear_break) {
	QString tmp;
	tmp.sprintf ("clear %s %d", (const char*) current_pos.file,
		     item->line ());
	send (tmp, false);
	return;
    }
}


void
Bibo::bernd_clicked (QListViewItem* tmpitem)
{
    BerndItem* item = dynamic_cast <BerndItem*> (tmpitem);

    if (item && item->what () == BerndItem::INPUT)
	send (item->text (), true);
}


void
Bibo::handle_ycpblock (int num)
{
    QString tmp;

    for (QString* cur = block.first (); cur != 0; cur = block.next ())
	tmp += *cur;

    if (!oscar[num]) {
	oscar[num] = new Oscar;
	oscar[num]->setMinimumSize (500, 400);

	switch (num) {
	    case 0:
		oscar[num]->setCaption ("YCP Debugger - Variable");
		break;
	    case 1:
		oscar[num]->setCaption ("YCP Debugger - Token");
		break;
	}
    }

    Parser parser ((const char*) tmp);
    YCodePtr value = parser.parse ();

    if (!value || value->isError ())
	oscar[num]->setText (tmp);
    else
	oscar[num]->setText (value->evaluate ());

    oscar[num]->show ();
}


void
Bibo::handle_breakpoints ()
{
    breakpoints.clear ();

    for (QString* cur = block.first (); cur != 0; cur = block.next ()) {

	char file[250];
	int line;

	if (sscanf ((const char*) *cur, "%245s %d", file, &line) != 2) {
	    inform ("parse error in backend output");
	    continue;
	}

	Position* tmp = new Position;
	tmp->setpos (file, line);
	breakpoints.append (tmp);

    }

    mark_source ();
}


bool
Bibo::check_breakpoints (const QString& check_file, int check_line)
{
    for (Position* cur = breakpoints.first (); cur != 0;
	 cur = breakpoints.next ())
	if ((check_line == (*cur).line) && (check_file == (*cur).file))
            return true;

    return false;
}


void
Bibo::Position::setpos (const QString& set_file, int set_line)
{
    file = set_file;
    line = set_line;
}


void
Bibo::hide_toolbar ()
{
    if (toolbar->isHidden ()) {
	toolbar->show ();
	menu_options->changeItem (menu_id_hide_toolbar, "Hide Toolbar");
    } else {
	toolbar->hide ();
	menu_options->changeItem (menu_id_hide_toolbar, "Show Toolbar");
    }
}
