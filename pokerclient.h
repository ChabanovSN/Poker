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
public slots:
    void writeProtocolGameBlack(string str);
    void writeProtocolGameRed(string str);
    void writeProtocolGameGreen(string str);
    void writeMoney();
    void writePlayersCardNames();
    void writeCommonCards();
private:
    GameNet * game;
    Ui::PokerClient *ui;
    QList<QLabel*> listCards;
    QList<QLabel*> listMoney;
    QList<QLineEdit*>listNames;
    void preflop(int n);
    int counter = 1;// считать партии
};

#endif // POKERCLIENT_H
