#ifndef NETSERVERWINDOW_H
#define NETSERVERWINDOW_H

#include <QMainWindow>
#include <QtNetwork>
#include <QTcpSocket>
#include <QObject>
#include <QByteArray>
#include <QDebug>
#include"pokertable.h"
#include"pokerclient.h"
namespace Ui {
class NetServerWindow;
}

class NetServerWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit NetServerWindow(QWidget *parent = nullptr);
    ~NetServerWindow();
signals:
    void MainMenuWindow();
    void signalAllClient();
private slots:
    // slots for server
    void on_starting_clicked();
    void on_stoping_clicked();
    void newuser();
    void showButtomForServer();
    void on_starting_server();
    void madeTradeToUser();
    void sendTradeToUser(int descriptor,int stavka);
    // slots for client

    void sendPlayerChooseButton(QString);
    void slotlAllClient(QString,QString);
//    void     slotlAllClientCom(QString,QString);
    void onSokConnectedForClient();
    void onSokDisconnectedForClient();
    void hideButtomForClient();

     void on_starting_client();
   ///

private:
    deque<Player *> player;
    PokerTable * poker;
    PokerClient * pokerClient;
    Ui::NetServerWindow *ui;
    QTcpServer *tcpServer;
    int server_status;
    QMap<int,QTcpSocket *> SClients;
    QString passwd;
    string name;
    QTcpSocket * clientSocket; // for server
     QTcpSocket  clientSocketStatic; //for client
     bool complitlyGroup = false;

public slots:
    //slot for server
     void slotReadClientOnServers();
    //slot for client
    void onSokReadyReadForClient();
     //////
     void game();
     void returnToMainMenu();
public:
//    void setMyFild(Field  *field = 0);
//    void setEnemyFild(Field  *field = 0);
//    Field *MyField;
//    Field *EnemyField;

private:
     QString tradeString="";
     int     tradeDiscripor=0;
};

#endif // NETSERVERWINDOW_H
