#include "netserverwindow.h"
#include "ui_netserverwindow.h"
#include<QMessageBox>
#include<QtDebug>
/*
протокол:
пароль~0~..... от клиента регистрация игрока на сервере
пароль~1~..... ответ сервера что игрок зарегистрирован
пароль~2~.....  код до флопа, запускает окно покера на клиентах
пароль~3~.....  рассылка общих карт клиентам
пароль~4~.....  отправка предложения торгов клиенту и ответ клиента
пароль~5~.....
пароль~6~.....  отправка клиентам текста протокола игры
*/
NetServerWindow::NetServerWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::NetServerWindow)
{
    ui->setupUi(this);

    this->setWindowTitle(QString("Poker"));
    QPixmap bkgnd(":/images/blue-card.jpg");
    bkgnd = bkgnd.scaled(620 ,420);
    QPalette palette;
    palette.setBrush(QPalette::Background, bkgnd);
    this->setPalette(palette);

    connect(ui->returnMainMenu,SIGNAL(clicked(bool)),this,SLOT(returnToMainMenu()));
    connect(ui->game,SIGNAL(clicked(bool)),this,SLOT(gameOnServer()));

    connect(ui->radioButtonServer,SIGNAL(clicked(bool)),this,SLOT(showButtomForServer()));
    connect(ui->radioButtonCliennt,SIGNAL(clicked(bool)),this,SLOT(hideButtomForClient()));

    connect(ui->start,SIGNAL(clicked()),this,SLOT(on_starting_clicked()));
    connect(ui->stop,SIGNAL(clicked()),this,SLOT(on_stoping_clicked()));

    hideButtomForClient();
}

//  QHostAddress::LocalHost
void NetServerWindow::on_starting_clicked()
{  if(ui->radioButtonServer->isChecked()){
        on_starting_server();
    }
    else{
        on_starting_client();
    }

}

void NetServerWindow::on_stoping_clicked(){  
    if(ui->radioButtonServer->isChecked()){
        foreach(int i,SClients.keys()){
            SClients[i]->close();
            SClients.remove(i);
        }
        tcpServer->close();
        ui->textinfo->append(QString::fromUtf8("Сервер остановлен!"));
        qDebug() << QString::fromUtf8("Сервер остановлен!");

        ui->game->setDisabled(true);
        ui->radioButtonCliennt->setDisabled(false);

    }else{
        slotDisconnectOnClient();
    }
}
///////////////SERVER////////////////////////////////
void NetServerWindow::on_starting_server(){

    if( ui->textName->text().length()>0)
        name =  ui->textName->text().toUtf8().toStdString();
    else{
        QMessageBox::information(this, QString::fromUtf8("Внимание!"), QString::fromUtf8("Имя не должен быть пустым"));
        return;
    }
    if( ui->textpasswd->text().length()>0)
        passwd =  ui->textpasswd->text().toUtf8().toStdString();
    else{
        QMessageBox::information(this, QString::fromUtf8("Внимание!"), QString::fromUtf8("Пароль не должен быть пустым"));
        return;
    }

    tcpServer = new QTcpServer(this);
    connect(tcpServer, SIGNAL(newConnection()), this, SLOT(slotNewConnection()));

    if (!tcpServer->listen(QHostAddress(ui->textip->text()), ui->textport->text().toInt())) {
        ui->textinfo->append(tcpServer->errorString());
        tcpServer->close();
        return;
    }
    else {
        player.push_back(new Player(name ,1,ui->spinBoxMoney->value())); // first diller
        ui->textinfo->append(QString::fromUtf8("Сервер запущен!"));
        string str = "Вы игрок - "+player[0]->getName()+
                ", сумма на счете: "+to_string( player[0]->getMoney());

        ui->textinfo->append(QString::fromUtf8(str.data()));
        ui->game->setDisabled(false);
        ui->radioButtonCliennt->setDisabled(true);
    }
}


void NetServerWindow::slotNewConnection(){

    clientSocket=tcpServer->nextPendingConnection();
    int idusersocs=clientSocket->socketDescriptor();
    SClients[idusersocs]=clientSocket;


    connect(SClients[idusersocs], SIGNAL(disconnected()),SClients[idusersocs], SLOT(deleteLater()));
    connect(SClients[idusersocs], SIGNAL(readyRead()),this, SLOT(slotReadClient()));




    ui->textinfo->append(QString::fromUtf8("У нас новое соединение!"));
    ui->textinfo->append("Ip: " +  clientSocket->peerAddress().toString());
}


void NetServerWindow::slotReadClient(){
    Player * pl;
    char *buf =new char[passwd.size()+2];
 //   qDebug()<<"slotReadClient() server ";
    string sendString="";
    int idusersocs=0;


    if (clientSocket == 0)
        clientSocket = (QTcpSocket*)sender(); // ловлю сокет клиента
    idusersocs=clientSocket->socketDescriptor();


    clientSocket->peek(buf, passwd.size()+2);
  //  qDebug()<<" peek "<<buf[passwd.size()+1]<<endl;


    QString info = QString::fromUtf8(clientSocket->readAll());
    QStringList list =  info.split(QRegExp("~"));
  //  qDebug()<<" on server info "<<info<<endl;

    if(list.size()>1 && list.at(0).toStdString() == passwd){
        char ch = buf[passwd.size()+1];
        switch (ch) {
        case '0':
            pl = new Player();
            pl->setInfoPlaeyr(list.at(2));
            pl->setId(player.size()+1);
            pl->setUserDescriptor(idusersocs);
            if(player.size()<=10){
                sendString.append(passwd).append("~1~")
                        .append(pl->getInfoPlayer().toStdString());
                sendToClient(clientSocket,sendString.c_str());sendString.clear();
                player.push_back(pl);
                ui->textinfo->append(QString::fromStdString( "Присоединился игрок " + pl->getName() +
                                                             " сумма на счете "+to_string(pl->getMoney())));
            }
            else{
                info =QString::fromUtf8("Количество играков превысило 10");
                sendToClient(clientSocket,info.toUtf8().constData());
                clientSocket->close();
                SClients.remove(idusersocs);
            }
            break;
        case '4': // отправка предложения торгов клиенту и ответ клиента
           // qDebug()<<" on server 4 "<<list.at(2)<<endl;
            poker->activeAllButton();
            emit signalAnswerFromCLient(list.at(2));
            break;
        default:
            qDebug()<<" on server ERROR "<<info<<endl;
        }

    }
}
void NetServerWindow::gameOnServer(){

    int blind   = ui->spinBoxBlind->value();
    poker = new PokerTable(player,blind);
    connect(poker->getGame(),SIGNAL(signalAllClient(QString,QString)),this,SLOT(slotlAllClient(QString,QString)),Qt::DirectConnection);
    connect(poker->getGame(),SIGNAL(signalSendTradeToUser(int,int)),this,SLOT(sendTradeToUser(int,int)),Qt::DirectConnection);
    connect(this,SIGNAL(signalAnswerFromCLient(QString)),poker->getGame(),SLOT(slotAnswerFromCLient(QString)),Qt::DirectConnection);
    connect(poker->getGame(),SIGNAL(signalProtocolTrade(string,string)),this,SLOT( slotProtocolTrade(string,string)));



    this->close();
    poker->show();


}


void NetServerWindow::sendToClient(QTcpSocket* pSocket,const char * str){
    qDebug()<<"sendToClient "<<str <<endl;
    pSocket->write(str);
     pSocket->flush();



}

void NetServerWindow::sendTradeToUser(int descriptor,int stavka){    
    string send ="";
    send.append(passwd+"~4~").append(to_string(stavka));
    qDebug()<<"sendTradeToUser "<<descriptor << " " <<QString::fromStdString(send)<<endl;

    poker->disactiveAllButton();
    sendToClient(SClients[descriptor],send.c_str());
}
void NetServerWindow::slotProtocolTrade(string str,string color){
    string send ="";
    send.append(passwd+"~6~").append(color+"~").append(str);
    qDebug()<<"slotProtocolTrade "<<QString::fromStdString(send)<<endl;

    foreach(int i,SClients.keys())
        sendToClient(SClients[i],send.c_str());

}

void NetServerWindow::slotlAllClient(QString gameString,QString code){
    qDebug()<<"slotlAllClient "<<QString::fromStdString( passwd)<<"~"<<code<<"~"<<gameString<<endl;
    string send;
    send.append(passwd+"~").append(code.toStdString()+"~")
            .append(gameString.toStdString());

    foreach(int i,SClients.keys())
        sendToClient(SClients[i],send.c_str());

}



////////////////////////////////////////////////////////////////////////////////////
////////// CLIENT   CLIENT CLIENT ///////////////////////////////////////////////
void NetServerWindow::on_starting_client(){
    if( ui->textName->text().length()>0)
        name =  ui->textName->text().toUtf8().toStdString();
    else{
        QMessageBox::information(this, QString::fromUtf8("Внимание!"), QString::fromUtf8("Имя не должен быть пустым"));
        return;
    }

    if( ui->textpasswd->text().length()>0)
        passwd =  ui->textpasswd->text().toUtf8().toStdString();
    else{
        QMessageBox::information(this, QString::fromUtf8("Внимание!"), QString::fromUtf8("Пароль не должен быть пустым"));
        return;
    }
    pokerClient = new PokerClient();

    clientSocket = new QTcpSocket(this);
    clientSocket->connectToHost(QHostAddress(ui->textip->text()), ui->textport->text().toInt());

    connect(clientSocket, SIGNAL(connected()), SLOT(slotConnected()));
    connect(clientSocket, SIGNAL(readyRead()), SLOT(slotReadyRead()));
    connect(clientSocket, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(slotError(QAbstractSocket::SocketError)));                   //sendPlayerChooseButton(QString)
    connect(pokerClient,SIGNAL(signalButtonWasPush(QString)),this,SLOT(slotButtonWasPush(QString)));

    ui->game->setDisabled(false);
    ui->radioButtonServer->setDisabled(true);
}
void NetServerWindow::slotConnected(){

    string sendStr="";
    if(player.size()==0) {
        player.push_back(new Player(name ,0, ui->spinBoxMoney->value()));
        sendStr.append(passwd).append("~0~")
                .append(player[0]->getInfoPlayer().toStdString());
        slotSendToServer(sendStr.c_str());
    }
    ui->textinfo->append(QString::fromUtf8("Соединение с сервером установленно"));
}
//client

void NetServerWindow::slotReadyRead(){
    char *buf =new char[passwd.size()+2];
    clientSocket->peek(buf, passwd.size()+2);
    qDebug()<<"slotReadyRead client peek "<<buf[passwd.size()+1]<<endl;

    string sendStr="";QString info;QStringList list;

    info = QString::fromUtf8(clientSocket->readAll());

 //   qDebug()<<"slotReadyRead client"<<info<<endl;
    list =  info.split(QRegExp("~"));

    if(list.size()>1 && list.at(0).toStdString() == passwd){
        switch (buf[passwd.size()+1]) {
        case '1':  //пароль~1~..... ответ сервера что игрок за регистрирован
            info= list.at(2);
            player[0]->setInfoPlaeyr(info);
            pokerClient->setIdCurrenClient(player[0]->getId());
            info = QString::fromStdString(player[0]->getName()+" вы "+
                    to_string(player[0]->getId()) +"-й игрок\nОжидаете формирование команды");
            ui->textinfo->append(info);
            break;
        case '2': // код до флопа, запускает окно покера на клиентах
            emit pokerClient->signalAllClient(list.at(2));
            this->close();
            pokerClient->show();
            break;
        case '3':// рассылка общих карт клиентам
            if(pokerClient !=nullptr){
            //    qDebug()<<" on clinet 3 FUCK "<<list.at(2)<<endl;
//                QString st; st.rep
//                list = list.at(2).split("@");
//                 qDebug()<<" on clinet 3 FUCK "<<list.at(0)<<endl;
                emit pokerClient->signalAllClientCommomCards(list.at(2));
            }
            else
                qDebug()<<" on clinet 3 FUCK "<<endl;
            break;
        case '4':// отправка предложения торгов клиенту
           // qDebug()<<" on clinet 4 "<< " stavka" <<list.at(2)<<endl;
            emit pokerClient->signalYourStep();
            break;

        case '6':// отправка клиентам текста протокола игры
          //  qDebug()<<" on clinet 6 "<< " color protokol"
                 //  <<list.at(2)<<" "<<list.at(3)<<endl;

                  if(list.at(2)==QString("R"))
                      emit pokerClient->getGame()->protocolTradeRed(list.at(3).toStdString());
                  if(list.at(2)==QString("G"))
                      emit pokerClient->getGame()->protocolTradeGreen(list.at(3).toStdString());
                  if(list.at(2)==QString("B"))
                      emit pokerClient->getGame()->protocolTradeBlack(list.at(3).toStdString());
            break;
        default:
            qDebug()<<" on clinet info ERROR "<<info<<endl;
            return;
        }

    }

}

void NetServerWindow::slotSendToServer(const char *str){
    qDebug()<<"slotSendToServer "<<str <<endl;

    clientSocket->write(str);
    clientSocket->flush();
}
void NetServerWindow::slotButtonWasPush(QString str){
    string send= "";
    send.append(passwd+"~4~").append(str.toStdString());
    slotSendToServer(send.c_str());
}
void NetServerWindow::slotError(QAbstractSocket::SocketError err)
{
    QString strError =
            "Error: " + (err == QAbstractSocket::HostNotFoundError ?
                             "The host was not found." :
                             err == QAbstractSocket::RemoteHostClosedError ?
                                 "The remote host is closed." :
                                 err == QAbstractSocket::ConnectionRefusedError ?
                                     "The connection was refused." :
                                     QString(clientSocket->errorString())
                                     );
    ui->textinfo->append(strError);
}


void NetServerWindow::slotDisconnectOnClient(){
    clientSocket->disconnectFromHost();
    ui->game->setDisabled(true);
    ui->radioButtonServer->setDisabled(false);
    ui->textinfo->append("Соединение разованно "+
                         clientSocket->peerAddress().toString()+
                         ":"+QString::number(clientSocket->peerPort()));
}




///////////////////////////////////////////////////////////////
/// SERVICE
void NetServerWindow::returnToMainMenu(){
    this->close();      // Закрываем окно
    emit MainMenuWindow(); // И вызываем сигнал на открытие главного окна
}

void NetServerWindow::showButtomForServer(){
    ui->spinBoxBlind->show();
    ui->labelBlind->show();
    ui->game->show();
}
void NetServerWindow::hideButtomForClient(){
    ui->spinBoxBlind->hide();
    ui->game->hide();
    ui->labelBlind->hide();
}
NetServerWindow::~NetServerWindow()
{
    delete ui;
}



