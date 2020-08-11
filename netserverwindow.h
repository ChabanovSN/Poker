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
    void signalAnswerFromCLient(QString);
public slots:
    // slots for server
    void slotlAllClient(QString,QString);
    void sendToClient(QTcpSocket* pSocket,const char *);
    void showButtomForServer();
    void madeTradeToUser();
    void sendTradeToUser(int descriptor,int stavka);    
    void slotNewConnection();
    void slotReadClient ();
    void gameOnServer();
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
    int server_status;
    QMap<int,QTcpSocket *> SClients;
    string passwd;
    string name;
    QTcpSocket * clientSocket; // for server
    //   QTcpSocket  clientSocketStatic; //for client
    bool complitlyGroup = false;
    QString tradeString="";
    int     tradeDiscripor=0;

     bool firstConnect = false;
     quint16 m_nNextBlockSize;

};

#endif // NETSERVERWINDOW_H
