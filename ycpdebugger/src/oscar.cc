

/*
 *  Author: Arvin Schnell <arvin@suse.de>
 */


#include "oscar.h"

#include "oscar.moc.cc"


Oscar::Oscar (QWidget* parent, const char* name)
    : QVBox (parent, name)
{
    view = new QTextView (this);

    close = new QPushButton ("&Close", this);
    connect (close, SIGNAL (clicked ()), this, SLOT (hide ()));
}


void
Oscar::setText (const QString& str)
{
    view->setText ("<qt><pre>" + str + "</pre></qt>");
}


void
Oscar::setText (const YCPValue& value)
{
    QString tmp;
    dump (tmp, 0, value);
    setText (tmp);
}


void
Oscar::indent (QString& tmp, int level) const
{
    while (level-- > 0)
	tmp += "    ";
}


bool
Oscar::dump (QString& tmp, int level, const YCPValue& value) const
{
    if (value.isNull ()) {
	tmp += "nil";
	return false;
    }

    switch (value->valuetype ()) {

	case YT_LIST: {
	    YCPList l = value->asList ();
	    tmp += '[';
	    for (int i = 0; i < l->size (); i++) {
		tmp += '\n';
		indent (tmp, level + 1);
		if (!dump (tmp, level + 1, l->value (i)))
		    return false;
		if (i != l->size () - 1)
		    tmp += ',';
	    }
	    tmp += '\n';
	    indent (tmp, level);
	    tmp += ']';
	} break;

	case YT_MAP: {
	    YCPMap m = value->asMap ();
	    tmp += "$[";
	    for (YCPMapIterator i = m->begin (); i != m->end (); i++) {
		if (i != m->begin ())
		    tmp += ',';
		tmp += '\n';
		indent (tmp, level + 1);
		tmp += "<b>";
		if (!dump (tmp, level + 1, i.key ()))
		    return false;
		tmp += "</b>";
		tmp += " : ";
		if (!dump (tmp, level + 1, i.value ()))
		    return false;
	    }
	    tmp += '\n';
	    indent (tmp, level);
	    tmp += ']';
	} break;

	case YT_STRING: {
	    tmp += "<font color=\"red\">";
	    tmp += value->toString ().c_str ();
	    tmp += "</font>";
	} break;

	case YT_INTEGER:
	case YT_FLOAT: {
	    tmp += "<font color=\"blue\">";
	    tmp += value->toString ().c_str ();
	    tmp += "</font>";
	} break;

	default: {
	    tmp += value->toString ().c_str ();
	} break;

    }

    return true;
}

