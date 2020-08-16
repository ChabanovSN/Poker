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
      void sendToClient(QTcpSocket* pSocket,const char *);
signals:
    void MainMenuWindow();
    void signalAllClient();
    void signalAnswerFromCLient(QString);
public slots:
    // slots for server
    void slotlAllClient(QString,QString);

    void showButtomForServer();   
    void sendTradeToUser(int descriptor,int stavka);    
    void slotNewConnection();
    void slotReadClient ();
    void gameOnServer();
    void slotProtocolTrade(string str,string color);
    // slots for client
    void slotReadyRead ();
    void slotError (QAbstractSocket::SocketError);
    void slotButtonWasPush(QString);

    void slotSendToServer(const char *);
    void slotConnected ();
    void slotDisconnectOnClient();
    //slot for service
    void hideButtomForClient();
    void on_starting_clicked();
    void on_stoping_clicked();
    void returnToMainMenu();
private:
    void on_starting_server();
    void on_starting_client();


private:
    deque<Player *> player;
    PokerTable * poker;
    PokerClient * pokerClient;
    Ui::NetServerWindow *ui;
    QTcpServer *tcpServer;

    QMap<int,QTcpSocket *> SClients;
    string passwd;
    string name;
    QTcpSocket * clientSocket;


};

#endif // NETSERVERWINDOW_H
