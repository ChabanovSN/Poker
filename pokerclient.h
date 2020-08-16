#ifndef POKERCLIENT_H
#define POKERCLIENT_H
#define SCREEN_WIDTH  1000
#define SCREEN_HIEGHT 600
#include <QMainWindow>
#include<QLabel>
#include<QLineEdit>
#include<logic/GameNet.h>
namespace Ui {
class PokerClient;
}

class PokerClient : public QMainWindow
{
    Q_OBJECT

public:
    explicit PokerClient(QWidget *parent = nullptr);
    ~PokerClient();
    void  init();
signals:
    void signalAllClient(QString);
    void signalAllClientCommomCards(QString);
    void signalYourStep();
    void signalButtonWasPush(QString);
public slots:
    void activeAllButton();
    void writeProtocolGameBlack(string str);
    void writeProtocolGameRed(string str);
    void writeProtocolGameGreen(string str);
    void writeMoney();
    void writePlayersCardNames();
    void writeCommonCards();

    void All_in();
  //  void start();
    void check();
   // void begin();
   void raise();
    void fold();
public:
    void disactiveAllButton();
    void setIdCurrenClient(int n){
        idCurrentClient=n;
    }
    GameNet * getGame(){
        return game;
    }
private:
    GameNet * game;
    Ui::PokerClient *ui;
    QList<QLabel*> listCards;
    QList<QLabel*> listMoney;
    QList<QLineEdit*>listNames;
    void preflop(int n);
    int counter = 1;// считать партии
    int idCurrentClient = 0;
 //   QString stringPlayerChoose="";

};

#endif // POKERCLIENT_H
