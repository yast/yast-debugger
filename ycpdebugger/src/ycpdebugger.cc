

/*
 *  Author: Arvin Schnell <arvin@suse.de>
 */


#include <qapplication.h>
#include <qwindowsstyle.h>

#include "base.h"


int
main (int argc, char** argv)
{
    QApplication app (argc, argv);

    Bibo* bibo = new Bibo;
    bibo->setCaption ("YCP Debugger");
    bibo->resize (640, 480);
    app.setMainWidget (bibo);
    bibo->show ();

    return app.exec ();
}
