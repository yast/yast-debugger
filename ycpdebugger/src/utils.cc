

/*
 *  Author: Arvin Schnell <arvin@suse.de>
 */


#include <qstring.h>


QString
untabify (const QString& in)
{
    const uint tabsize = 8;

    QString ret = in;

    for (uint i = 0; i < ret.length (); i++)
        if (ret[i] == '\t') {
	    const uint n = tabsize - (i % tabsize);
	    QString tmp;
	    for (uint j = 0; j <  n; j++)
		tmp += ' ';
	    ret.replace (i, 1, tmp);
	    i += n - 1;
	}

    return ret;
}
