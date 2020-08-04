#include "pokerclient.h"
#include "ui_pokerclient.h"

PokerClient::PokerClient(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PokerClient)
{


    init();

}

void PokerClient::writePlayersCardNames(){
    writeProtocolGameRed("Game ~~ "+to_string(counter++)+" ~~");
  //  game->preflop(); /// PREFLOP
    for( size_t p=0;p<game->getPlayers().size();p++){
        int id = game->getPlayers().at(p)->getId()-1;
        listNames[id]->setText(
                    QString::fromStdString( game->getPlayers().at(p)->getName()));
        listCards[id*2]->setPixmap(
                    QPixmap(game->getPlayers().at(p)->getCards().at(0)->getPath().data()));
        listCards[id*2+1]->setPixmap(
                    QPixmap(game->getPlayers().at(p)->getCards().at(1)->getPath().data()));
    }



    writeMoney();
    ui->spinBox->setValue(game->getStavke());

}
void PokerClient::writeCommonCards(){
    if(game->getCommomCards().size()==3){
        ui->comLabel1->setPixmap(
                    QPixmap(game->getCommomCards().at(0)->getPath().data()));
        ui->comLabel2->setPixmap(
                    QPixmap(game->getCommomCards().at(1)->getPath().data()));
        ui->comLabel3->setPixmap(
                    QPixmap(game->getCommomCards().at(2)->getPath().data()));
}
}
void PokerClient::writeMoney(){
    for( size_t p=0;p<game->getPlayers().size();p++){
        int id = game->getPlayers().at(p)->getId()-1;
        int money = game->getPlayers().at(p)->getMoney();
        listMoney[id]->setText(QString::number(money));
    }
    ui->labelBank->setText(QString::number(game->getBank()));
}



void PokerClient::init(){
     game = new GameNet();
    ui->setupUi(this);
    QPixmap bkgnd(":/images/green-table.jpg");
    bkgnd = bkgnd.scaled(SCREEN_WIDTH*2 ,SCREEN_HIEGHT*1.2);
    QPalette palette;
    palette.setBrush(QPalette::Background, bkgnd);
    this->setPalette(palette);

    this->setWindowTitle(QString("Poker"));
    ui->combo->setPixmap( QPixmap(":/images/combo.png"));
 //   ui->combo->setScaledContents(true);

      ui->checkButton->setDisabled(true);
      ui->raise->setDisabled(true);
      ui->fold->setDisabled(true);
      ui->All_in->setDisabled(true);

    connect(ui->checkButton,SIGNAL(clicked(bool)),this,SLOT(check()));
  //  connect(ui->raise,SIGNAL(clicked(bool)),this,SLOT(raise()));
  //  connect(ui->fold,SIGNAL(clicked(bool)),this,SLOT(fold()));
  //  connect(ui->All_in,SIGNAL(clicked(bool)),this,SLOT(All_in()));
//    connect(ui->startButton,SIGNAL(clicked(bool)),this,SLOT(start()));
    connect(this,SIGNAL(signalAllClient(QString)),game,SLOT(stringToGame(QString)));
    connect(this,SIGNAL(signalAllClientCommomCards(QString)),game,SLOT(commonCardsFromString(QString)));


     connect(this,SIGNAL(signalYourStep()),this,SLOT(actuveAllButton()));

    connect(game,SIGNAL(signalwriteCommonCards()),this,SLOT(writeCommonCards()));
    connect(game,SIGNAL(signalwritePlayersCardNames()),this,SLOT(writePlayersCardNames()));
    connect(game,SIGNAL(protocolTradeBlack(string)),this,SLOT(writeProtocolGameBlack(string)));
    connect(game,SIGNAL(protocolTradeGreen(string)),this,SLOT(writeProtocolGameGreen(string)));
    connect(game,SIGNAL(protocolTradeRed(string)),this,SLOT(writeProtocolGameRed(string)));
    ///  карты на столе

    listCards ={ui->playerLab1_1,ui->playerLab1_2,ui->playerLab2_1,
                ui->playerLab2_2,ui->playerLab3_1,ui->playerLab3_2,
                ui->playerLab4_1,ui->playerLab4_2,ui->playerLab5_1,
                ui->playerLab5_2,ui->playerLab6_1,ui->playerLab6_2,
                ui->playerLab7_1,ui->playerLab7_2,ui->playerLab8_1,
                ui->playerLab8_2,ui->playerLab9_1,ui->playerLab9_2,
                ui->playerLab10_1,ui->playerLab10_2};
    for(auto c : listCards) c->clear();
    //имена играков
    listNames ={ui->name1,ui->name2,ui->name3,
                ui->name4,ui->name5,ui->name6,ui->name7,ui->name8,ui->name9,ui->name10};
    // деньги
    listMoney={ui->labelMoney1,ui->labelMoney2,ui->labelMoney3,ui->labelMoney4,ui->labelMoney5,
               ui->labelMoney6,ui->labelMoney7,ui->labelMoney8,ui->labelMoney9,ui->labelMoney10};

}

void PokerClient::check(){

    if(ui->checkButton->text() == QString("Check")){
        game->setPlayerChoose(4,game->getStavke());
    }
    if(ui->checkButton->text() == QString("Call")){
        game->setPlayerChoose(2,game->getStavke());
    }
    stringPlayerChoose = game->getPlayerChoose();
  //  qDebug()<<"PokerClient::check() "<<stringPlayerChoose;
     emit signalButtonWasPush(stringPlayerChoose);

}

void PokerClient::actuveAllButton(){
    ui->checkButton->setDisabled(false);
    ui->raise->setDisabled(false);
    ui->fold->setDisabled(false);
    ui->All_in->setDisabled(false);

}
void PokerClient::writeProtocolGameRed(string str){
    ui->protocolGame->setTextColor(QColor(255,0,0));
    ui->protocolGame->append(QString::fromStdString(str));
}
void PokerClient::writeProtocolGameBlack(string str){
    ui->protocolGame->setTextColor(QColor(0,0,0));
    ui->protocolGame->append(QString::fromStdString(str));
}
void PokerClient::writeProtocolGameGreen(string str){
    ui->protocolGame->setTextColor(QColor(0,100,0));
    ui->protocolGame->append(QString::fromStdString(str));
}

PokerClient::~PokerClient()
{   delete game;
    delete ui;
}
