#include "pokertable.h"

#include <QApplication>

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    PokerTable w;
    w.show();
    return a.exec();
}
