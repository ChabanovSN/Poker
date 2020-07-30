#include"menustart.h"
#include <QApplication>

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
  //  PokerTable w;
      MenuStart w;
      w.show();
   return a.exec();
}
