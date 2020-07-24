#ifndef POKERTABLE_H
#define POKERTABLE_H

#include <QMainWindow>
#include<QLabel>
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
    void nextPlayer(){
         game->nextPlayer();
    }
    void start(){
          begin();
    }
    void check();
    void begin();
    void raise();
    void fold();
  // void call();
private:
   Game * game = new Game(3);
   QList<QLabel*> listLabels;
   void preflop(int n);
    Ui::PokerTable *ui;
};
#endif // POKERTABLE_H
