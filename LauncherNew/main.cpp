#include <QtGui/QApplication>
#include "serverbrowser.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName("Vice-Players");
    a.setApplicationVersion("1.0");
    ServerBrowser w;
    w.show();

    return a.exec();
}
