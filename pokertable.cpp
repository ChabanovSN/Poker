#include "pokertable.h"
#include "ui_pokertable.h"
#include<QMessageBox>
PokerTable::PokerTable(int players,int money, int blind,QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::PokerTable)
{
       game = new Game(players,money,blind);

       init();
}
PokerTable::PokerTable(deque<Player *> players, int blind,QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::PokerTable)
{
       game = new Game(players,blind);
       init();
}

void PokerTable::check(){

    if(ui->checkButton->text() == QString("Check")){
        game->setPlayerChoose(4,game->getStavke());
    }
    if(ui->checkButton->text() == QString("Call")){
        game->setPlayerChoose(2,game->getStavke());
    }
    game->nextPlayer();

}
void PokerTable::raise(){

    int stavka = ui->spinBox->value();
    if(stavka>game->getStavke()){
        game->setPlayerChoose(1,stavka);
        game->nextPlayer();
    }
    else
        QMessageBox::information(this, QString::fromUtf8("Внимание!"), QString::fromUtf8("Ваша ставка должна быть больше!"));

}
void PokerTable::fold(){

    game->setPlayerChoose(3,game->getStavke());
    game->nextPlayer();
}
void PokerTable::start(){


    if(ui->startButton->text() == QString("Start game")){
        ui->startButton->setText("Next party");
        ui->startButton->setStyleSheet("background-color: red");
        begin();
    }else{
        game->checkMoney();
        // QMessageBox::information(this, QString::fromUtf8("ХРЕНЬ"), QString::fromUtf8("Вы победили!"));
        if(game->getEndGame()){
            for(auto p : game->getPlayers()){
                if(p->getWinner())
                    QMessageBox::information(this,
                                             QString::fromUtf8("Поздравляю!!!"),
                                             QString::fromStdString( p->getName()+ " вы победили!"));
            }
        }else{
            nextGame();
            if(!game->getEndGame())
                            begin();
        }
    }
}
void PokerTable::begin(){
    ui->startButton->hide();

    if (!game->getEndGame()){
        writeProtocolGameRed("Game ~~ "+to_string(counter++)+" ~~");
        game->preflop(); /// PREFLOP
        QPixmap pix(":/images/rubashka.jpg");

        for( size_t p=0;p<game->getPlayers().size();p++){
            int id = game->getPlayers().at(p)->getId()-1;
            listNames[id]->setText(
                        QString::fromStdString( game->getPlayers().at(p)->getName()));
                 if(game->getPlayers().at(p)->getIsComputer()==false){
            listCards[id*2]->setPixmap(
                        QPixmap(game->getPlayers().at(p)->getCards().at(0)->getPath().data()));
            listCards[id*2+1]->setPixmap(
                        QPixmap(game->getPlayers().at(p)->getCards().at(1)->getPath().data()));

                 }else{
                     listCards[id*2]->setPixmap(pix);
                     listCards[id*2]->setScaledContents(true);
                     listCards[id*2+1]->setPixmap(pix);
                     listCards[id*2+1]->setScaledContents(true);
                 }

        }
        writeMoney();
        ui->spinBox->setValue(game->getStavke());
    }

    if(game->getPlayers().size()>2) // если играком больше 3 то они делают ставки
        if (!game->getEndGame())
            game->trade();
    writeMoney();


    ui->checkButton->setText(QString("Check"));
    if (!game->getEndGame()){
        game->flop();
        if(game->getCommomCards().size()==3){
            ui->comLabel1->setPixmap(
                        QPixmap(game->getCommomCards().at(0)->getPath().data()));
            ui->comLabel2->setPixmap(
                        QPixmap(game->getCommomCards().at(1)->getPath().data()));
            ui->comLabel3->setPixmap(
                        QPixmap(game->getCommomCards().at(2)->getPath().data()));


            game->trade();

            writeMoney();
        }

        if (!game->getEndGame()){
            game->turn();
            if(game->getCommomCards().size()==4)
                ui->comLabel4->setPixmap(
                            QPixmap(game->getCommomCards().at(3)->getPath().data()));


            game->trade();
            writeMoney();
        }

        if (!game->getEndGame()){
            game->river();
            if(game->getCommomCards().size()==5)
                ui->comLabel5->setPixmap(
                            QPixmap(game->getCommomCards().at(4)->getPath().data()));


            ui->checkButton->setText(QString("Call"));
            game->trade();
            writeMoney();
        }
        game->howWinner();

        for( size_t p=0;p<game->getPlayers().size();p++){
            int id = game->getPlayers().at(p)->getId()-1;
            listNames[id]->setText(
                        QString::fromStdString( game->getPlayers().at(p)->getName()));

            listCards[id*2]->setPixmap(
                        QPixmap(game->getPlayers().at(p)->getCards().at(0)->getPath().data()));
            listCards[id*2+1]->setPixmap(
                        QPixmap(game->getPlayers().at(p)->getCards().at(1)->getPath().data()));



        }

        game->checkMoney();
        writeMoney();
          ui->startButton->show();


    }
}
void PokerTable::nextGame(){
    game->setEndGame(false);
    game->getCommomCards().clear();
    for(auto p: game->getPlayers())
        p->clearCard();
    for(auto c : listCards)
        c->clear();
    ui->comLabel1->clear();
    ui->comLabel2->clear();
    ui->comLabel3->clear();
    ui->comLabel4->clear();
    ui->comLabel5->clear();
    game->newStack();
    game->setDiller();
}
void PokerTable::writeMoney(){
    for( size_t p=0;p<game->getPlayers().size();p++){
        int id = game->getPlayers().at(p)->getId()-1;
        int money = game->getPlayers().at(p)->getMoney();
        listMoney[id]->setText(QString::number(money));
    }
    ui->labelBank->setText(QString::number(game->getBank()));
}
void PokerTable::writeProtocolGameRed(string str){
    ui->protocolGame->setTextColor(QColor(255,0,0));
    ui->protocolGame->append(QString::fromStdString(str));
}
void PokerTable::writeProtocolGameBlack(string str){
    ui->protocolGame->setTextColor(QColor(0,0,0));
    ui->protocolGame->append(QString::fromStdString(str));
}
void PokerTable::writeProtocolGameGreen(string str){
    ui->protocolGame->setTextColor(QColor(0,100,0));
    ui->protocolGame->append(QString::fromStdString(str));
}
void PokerTable::All_in(){
    for( size_t p=0;p<game->getPlayers().size();p++)
        if(game->getPlayers().at(p)->getId()==1){
            int money = game->getPlayers().at(p)->getMoney();
            ui->spinBox->setValue(money);
            break;

        }

    // writeMoney();
}
void PokerTable::init(){
    ui->setupUi(this);
    QPixmap bkgnd(":/images/green-table.jpg");
    bkgnd = bkgnd.scaled(SCREEN_WIDTH*2 ,SCREEN_HIEGHT*1.2);
    QPalette palette;
    palette.setBrush(QPalette::Background, bkgnd);
    this->setPalette(palette);

    this->setWindowTitle(QString("Poker"));
    ui->combo->setPixmap( QPixmap(":/images/combo.png"));
 //   ui->combo->setScaledContents(true);


    connect(ui->checkButton,SIGNAL(clicked(bool)),this,SLOT(check()));
    connect(ui->raise,SIGNAL(clicked(bool)),this,SLOT(raise()));
    connect(ui->fold,SIGNAL(clicked(bool)),this,SLOT(fold()));
    connect(ui->All_in,SIGNAL(clicked(bool)),this,SLOT(All_in()));
    connect(ui->startButton,SIGNAL(clicked(bool)),this,SLOT(start()));
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
PokerTable::~PokerTable()
{   delete game;
    delete ui;
}

