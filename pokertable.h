#ifndef POKERTABLE_H
#define POKERTABLE_H

#include <QMainWindow>
#include<QLabel>
#include<QLineEdit>
#define SCREEN_WIDTH  1000
#define SCREEN_HIEGHT 600
#include "logic/Card.h"
#include "logic/Game.h"
QT_BEGIN_NAMESPACE
namespace Ui { class PokerTable; }
QT_END_NAMESPACE

class PokerTable : public QMainWindow
{
    Q_OBJECT

public:
    PokerTable(QWidget *parent = nullptr);
    ~PokerTable();

public slots:
    void All_in();
    void start();
    void check();
    void begin();
    void raise();
    void fold();
    void nextGame();
    void writeProtocolGameBlack(string str);
    void writeProtocolGameRed(string str);
    void writeProtocolGameGreen(string str);
private:
    void writeMoney();
   Game * game = new Game(8);
   QList<QLabel*> listCards;
   QList<QLabel*> listMoney;
   QList<QLineEdit*>listNames;
   void preflop(int n);
    Ui::PokerTable *ui;
    int counter = 1;// считать партии
};
#endif // POKERTABLE_H
