#include "gui_eod.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    gui_eod w;
    w.show();
    return a.exec();
}
