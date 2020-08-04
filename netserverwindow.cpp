#include "netserverwindow.h"
#include "ui_netserverwindow.h"
#include<QMessageBox>
#include<QtDebug>
NetServerWindow::NetServerWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::NetServerWindow)
{     pokerClient = new PokerClient();

      connect(pokerClient,SIGNAL(signalButtonWasPush(QString)),this,SLOT(sendPlayerChooseButton(QString)));




        ui->setupUi(this);
          this->setWindowTitle(QString("Poker"));
          QPixmap bkgnd(":/images/blue-card.jpg");
            bkgnd = bkgnd.scaled(620 ,420);
              QPalette palette;
                palette.setBrush(QPalette::Background, bkgnd);
                  this->setPalette(palette);
                  connect(ui->returnMainMenu,SIGNAL(clicked(bool)),this,SLOT(returnToMainMenu()));
                    connect(ui->game,SIGNAL(clicked(bool)),this,SLOT(game()));

                      //client
                      connect(ui->radioButtonServer,SIGNAL(clicked(bool)),this,SLOT(showButtomForServer()));
                        // connect(&clientSocketStatic, SIGNAL(error(QAbstractSocket::SocketError)),this, SLOT(onSokDisplayError(QAbstractSocket::SocketError)));
                        connect(ui->radioButtonCliennt,SIGNAL(clicked(bool)),this,SLOT(hideButtomForClient()));
                          //connect(this,SIGNAL(signalAllClient()),this,SLOT(slotlAllClient()));


                          connect(ui->start,SIGNAL(clicked(bool)),this,SLOT(on_starting_clicked()));
                          connect(ui->stop,SIGNAL(clicked(bool)),this,SLOT(on_stoping_clicked()));
                              hideButtomForClient();
}
/// BEGIN PART CLIENT




/// END PART CLIENT

//  QHostAddress::LocalHost
void NetServerWindow::on_starting_clicked()
{  if(ui->radioButtonServer->isChecked()){
        on_starting_server();
    }
    else{
        on_starting_client();
    }

}

void NetServerWindow::on_stoping_clicked()
{  if(ui->radioButtonServer->isChecked()){
        if(server_status==1){
            foreach(int i,SClients.keys()){
                QTextStream os(SClients[i]);
                os.setAutoDetectUnicode(true);
                os << QTime::currentTime().toString() << "\n";
                SClients[i]->close();
                SClients.remove(i);
            }
            tcpServer->close();
            ui->textinfo->append(QString::fromUtf8("Сервер остановлен!"));
            qDebug() << QString::fromUtf8("Сервер остановлен!");
            server_status=0;
            ui->game->setDisabled(true);
            ui->radioButtonCliennt->setDisabled(false);
        }
    }else{
        onSokDisconnectedForClient();
    }
}


void NetServerWindow::newuser()
{
    if(server_status==1){
        // qDebug() << QString::fromUtf8("У нас новое соединение!");
        ui->textinfo->append(QString::fromUtf8("У нас новое соединение!"));
        clientSocket=tcpServer->nextPendingConnection();

        int idusersocs=clientSocket->socketDescriptor();
        SClients[idusersocs]=clientSocket;
        connect(SClients[idusersocs],SIGNAL(readyRead()),this, SLOT(slotReadClientOnServers()));

       // connect(SClients[idusersocs],SIGNAL(ready()),this, SLOT(madeTradeToUser()));
        ui->textinfo->append("Ip: " +  clientSocket->peerAddress().toString());
        //  ui->textinfo->append("SClients[idusersocs]" + QString::number( idusersocs));
    }
}


void NetServerWindow::slotReadClientOnServers()
{
      int idusersocs=0;
    if (clientSocket == 0)
        clientSocket = (QTcpSocket*)sender(); // ловлю сокет клиента
    idusersocs=clientSocket->socketDescriptor();
    QTextStream os(clientSocket);
    os.setAutoDetectUnicode(true);

    QString info = QString::fromUtf8(clientSocket->readAll());
    QStringList list =  info.split(QRegExp("~"));
    qDebug()<<" on server "<<info<<endl;
    if(list.at(1).toInt()==0){
        Player * pl = new Player();
        pl->setInfoPlaeyr(list.at(2));
        pl->setId(player.size()+1);
        pl->setUserDescriptor(idusersocs);
        if(player.size()<=10){
            player.push_back(pl);
            info=QString::fromStdString( "Присоединился игрок " + pl->getName() +" сумма на счете "+to_string(pl->getMoney()));
            //
            ui->textinfo->append(info);
            os <<passwd<<"~"<<"1"<<"~"
              <<pl->getInfoPlayer();
            // << QTime::currentTime().toString() << "\n";
            // os.flush();
        }
        else{
            info =QString::fromUtf8("Количество играков превысило 10");
            os<<info;
            clientSocket->close();
            SClients.remove(idusersocs);
        }
    }

}
void NetServerWindow::game(){
    complitlyGroup = true;
    int blind   = ui->spinBoxBlind->value();
    poker = new PokerTable(player,blind);
    // emit signalAllClient();
    connect(poker->getGame(),SIGNAL(signalAllClient(QString,QString)),this,SLOT(slotlAllClient(QString,QString)));
    connect(poker->getGame(),SIGNAL(signalSendTradeToUser(int,int)),this,SLOT(sendTradeToUser(int,int)));

    this->close();
    poker->show();


}

void NetServerWindow::on_starting_server(){

    if( ui->textName->text().length()>0)
        name =  ui->textName->text().toUtf8().toStdString();
    else{
        QMessageBox::information(this, QString::fromUtf8("Внимание!"), QString::fromUtf8("Имя не должен быть пустым"));
        return;
    }

    if( ui->textpasswd->text().length()>0)
        passwd =  ui->textpasswd->text();
    else{
        QMessageBox::information(this, QString::fromUtf8("Внимание!"), QString::fromUtf8("Пароль не должен быть пустым"));
        return;
    }

    tcpServer = new QTcpServer(this);
    connect(tcpServer, SIGNAL(newConnection()), this, SLOT(newuser()));

    if (!tcpServer->listen(QHostAddress(ui->textip->text()), ui->textport->text().toInt()) && server_status==0) {
        // qDebug() <<  QObject::tr("Unable to start the server: %1.").arg(tcpServer->errorString());
        ui->textinfo->append(tcpServer->errorString());
    } else {
        server_status=1;
        int money   =  ui->spinBoxMoney->value();
        player.push_back(new Player(name ,1,money )); // first diller
        // qDebug() << tcpServer->isListening() << "TCPSocket listen on port";
        // qDebug() <<tcpServer->serverAddress() <<QString::fromUtf8("Адресс сервера!\n");
        // qDebug() <<tcpServer->serverPort() <<QString::fromUtf8("Port сервера!\n");
        ui->textinfo->append(QString::fromUtf8("Сервер запущен!"));
        string str = "Вы игрок - "+player[0]->getName()+
                ", сумма на счете: "+to_string( player[0]->getMoney());
        ui->textinfo->append(QString::fromUtf8(str.data()));
        //  qDebug() << QString::fromUtf8("Сервер запущен!");
        ui->game->setDisabled(false);
        ui->radioButtonCliennt->setDisabled(true);
    }


}


void NetServerWindow::sendTradeToUser(int descriptor,int stavka){
 //   qDebug()<<"sendTradeToUser "<<descriptor <<endl;
     QString spt("~");

          tradeString
            .append(QString::number(stavka));
           tradeDiscripor=descriptor;
slotlAllClient( tradeString,"777");
//           qDebug()<<"sendTradeToUser "<<tradeString <<endl;
//          SClients[tradeDiscripor]->write(tradeString.toUtf8().constData());
          tradeDiscripor=0;
          tradeString.clear();

}
void NetServerWindow::madeTradeToUser(){
    qDebug()<<"sendTradeToUser "<<tradeString <<endl;
   SClients[tradeDiscripor]->write(tradeString.toUtf8().constData());
   tradeDiscripor=0;
   tradeString.clear();
//    QTextStream os(SClients[descriptor]);
//    os.setAutoDetectUnicode(true);
//    os<<passwd+"~"+"777"+"~"+QString::number(stavka);
//    os.flush();
}
void NetServerWindow::slotlAllClient(QString gameString,QString code){
    //if( complitlyGroup){
    // complitlyGroup = false;

    qDebug()<<"slotlAllClient "<< passwd+"~"+code+"~"+gameString<<endl;;
    foreach(int i,SClients.keys()){
        QTextStream os(SClients[i]);
        os.setAutoDetectUnicode(true);
        os<<passwd+"~"+code+"~"+gameString;
        os.flush();

    }


    //}
}
////////////////////////////////////////////////////////////////////////////////////
////////// CLIENT   CLIENT CLIENT ///////////////////////////////////////////////
void NetServerWindow::on_starting_client(){
    onSokConnectedForClient();
}

void NetServerWindow::onSokConnectedForClient(){

    if( ui->textName->text().length()>0)
        name =  ui->textName->text().toUtf8().toStdString();
    else{
        QMessageBox::information(this, QString::fromUtf8("Внимание!"), QString::fromUtf8("Имя не должен быть пустым"));
        return;
    }

    if( ui->textpasswd->text().length()>0)
        passwd =  ui->textpasswd->text();
    else{
        QMessageBox::information(this, QString::fromUtf8("Внимание!"), QString::fromUtf8("Пароль не должен быть пустым"));
        return;
    }




    clientSocketStatic.connectToHost(QHostAddress(ui->textip->text()), ui->textport->text().toInt());
    if(clientSocketStatic.isOpen())
        ui->textinfo->append(QString::fromUtf8("Клиент запущен!"));
    else
        ui->textinfo->append(QString::fromUtf8("Ошибка соединения!"));



    ui->game->setDisabled(false);
    ui->radioButtonServer->setDisabled(true);

    connect(&clientSocketStatic,SIGNAL(connected()), this, SLOT(onSokReadyReadForClient()));

    connect(&clientSocketStatic,SIGNAL(readyRead()), this, SLOT(onSokReadyReadForClient()));

    //connect(&clientSocketStatic,SIGNAL(signalButtonWasPush(QString)), this, SLOT(sendPlayerChooseButton(QString)));


}

void NetServerWindow::onSokDisconnectedForClient(){
    clientSocketStatic.disconnectFromHost();
    ui->game->setDisabled(true);
    ui->radioButtonServer->setDisabled(false);
    ui->textinfo->append("Соединение разованно "+
                         clientSocketStatic.peerAddress().toString()+
                         ":"+QString::number(clientSocketStatic.peerPort()));
}
void NetServerWindow::onSokReadyReadForClient(){
    QTextStream os(&clientSocketStatic);
    os.setAutoDetectUnicode(true);
    if(player.size()==0) {
        int money   =  ui->spinBoxMoney->value();
        player.push_back(new Player(name ,0, money));
        os<<passwd<<"~"<<"0"<<"~"
         <<player[0]->getInfoPlayer();
    }
    else{
        QString info = QString::fromUtf8(clientSocketStatic.readLine());
        info.remove(QRegExp("\n"));
        qDebug()<<" on clinet info "<<info<<endl;
        QStringList list =  info.split(QRegExp("~"));

        QString info2 = QString::fromUtf8(clientSocketStatic.readLine());
        info2.remove(QRegExp("\n"));
        if(info2.size()>0){
            QStringList   list =  info2.split(QRegExp("~"));
            if(list.at(1).toInt()==888){// код только для общих карт
                qDebug()<<" on clinet 888 "<<info2<<endl
                       <<list.at(2)<<endl;
                if(pokerClient !=nullptr)
                    emit pokerClient->signalAllClientCommomCards(list.at(2));
                else
                    qDebug()<<" on clinet 888 FUCK "<<endl;
            }
        }
        if(list.at(1).toInt()==999){ // код до флопа

            emit pokerClient->signalAllClient(list.at(2));

            // poker->getGame()->stringToGame(list.at(2));
            this->close();
            pokerClient->show();
        }
        else if(list.at(1).toInt()==777){
            qDebug()<<" on clinet 777 "<< " stavka" <<list.at(2).toInt()<<endl;
            emit pokerClient->signalYourStep();
            /// ответ sendPlayerChooseButton(QString);
        }

        else{

            if(list.at(1).toInt()==1){
                info= list.at(2);
                player[0]->setInfoPlaeyr(info);
                info = QString::fromStdString(player[0]->getName()+" вы "+
                        to_string(player[0]->getId()) +"-й игрок\nОжидаете формирование команды");
                ui->textinfo->append(info);
            }
        }
    }
}



//from client

void NetServerWindow::sendPlayerChooseButton(QString str){
    // отправить строку выбранной кнопки
    //вывафыв
    qDebug()<<"sendPlayerChooseButtonax "<<str;
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
