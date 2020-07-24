#include "pokertable.h"
#include "ui_pokertable.h"
#include<QMessageBox>
PokerTable::PokerTable(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::PokerTable)
{

    QPixmap bkgnd(":/images/green-table.jpg");
    bkgnd = bkgnd.scaled(SCREEN_WIDTH*2 ,SCREEN_HIEGHT*1.2);
    QPalette palette;
    palette.setBrush(QPalette::Background, bkgnd);
    this->setPalette(palette);
    ui->setupUi(this);
    connect(ui->checkButton,SIGNAL(clicked(bool)),this,SLOT(check()));
    connect(ui->raise,SIGNAL(clicked(bool)),this,SLOT(raise()));
    connect(ui->fold,SIGNAL(clicked(bool)),this,SLOT(fold()));
    connect(ui->nextPlayer,SIGNAL(clicked(bool)),this,SLOT(nextPlayer()));
    connect(ui->startButton,SIGNAL(clicked(bool)),this,SLOT(start()));
    ///  карты на столе

    listLabels.push_back(ui->playerLab1_1);
    listLabels.push_back(ui->playerLab1_2);
    listLabels.push_back(ui->playerLab2_1);
    listLabels.push_back(ui->playerLab2_2);
    listLabels.push_back(ui->playerLab3_1);
    listLabels.push_back(ui->playerLab3_2);
    listLabels.push_back(ui->playerLab4_1);
    listLabels.push_back(ui->playerLab4_2);
    listLabels.push_back(ui->playerLab5_1);
    listLabels.push_back(ui->playerLab5_2);
    listLabels.push_back(ui->playerLab6_1);
    listLabels.push_back(ui->playerLab6_2);
    listLabels.push_back(ui->playerLab7_1);
    listLabels.push_back(ui->playerLab7_2);
    listLabels.push_back(ui->playerLab8_1);
    listLabels.push_back(ui->playerLab8_2);
    listLabels.push_back(ui->playerLab9_1);
    listLabels.push_back(ui->playerLab9_2);
    listLabels.push_back(ui->playerLab10_1);
    listLabels.push_back(ui->playerLab10_2);


}
void PokerTable::check(){

    if(ui->checkButton->text() == QString("Check")){
        cout<<"CHEKKKKKKK"<<endl;
        game->setPlayerChoose(4,game->getStavke());
    }
    if(ui->checkButton->text() == QString("Call")){
        cout<<"CALLlllllllllllllll"<<endl;
        game->setPlayerChoose(2,game->getStavke());
    }
    game->nextPlayer();
    //  preflop(4);
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
void PokerTable::begin(){
    while(game->getEndGame()==false){
        game->preflop();
        size_t players =game->getPlayers().size();
        size_t labels =players * 2;
        for( size_t p=0,l=0;p<players && l<labels;++p,++l){
            listLabels[l]->setPixmap(
                        QPixmap(
                            game->getPlayers().at(p)->getCards().at(0)->getPath().data()
                            ));
            listLabels[l]->setScaledContents(true);
            listLabels[++l]->setPixmap(
                        QPixmap(
                            game->getPlayers().at(p)->getCards().at(1)->getPath().data()
                            ));
            listLabels[l]->setScaledContents(true);
        }
        ui->spinBox->setValue(game->getStavke());
        ui->checkButton->setText(QString("Check"));
        game->flop();
        if(game->getCommomCards().size()==3){
            ui->comLabel1->setPixmap(
                        QPixmap(game->getCommomCards().at(0)->getPath().data()));
            ui->comLabel2->setPixmap(
                        QPixmap(game->getCommomCards().at(1)->getPath().data()));
            ui->comLabel3->setPixmap(
                        QPixmap(game->getCommomCards().at(2)->getPath().data()));
            ui->comLabel1->setScaledContents(true);
            ui->comLabel2->setScaledContents(true);
            ui->comLabel3->setScaledContents(true);

        }
        game->trade();
        game->turn();
        if(game->getCommomCards().size()==4){
            ui->comLabel4->setPixmap(
                        QPixmap(game->getCommomCards().at(3)->getPath().data()));
            ui->comLabel4->setScaledContents(true);
        }
        game->trade();
        game->river();
        if(game->getCommomCards().size()==5){
            ui->comLabel5->setPixmap(
                        QPixmap(game->getCommomCards().at(4)->getPath().data()));
            ui->comLabel5->setScaledContents(true);
        }
        ui->checkButton->setText(QString("Call"));
        game->trade();
        game->checkMoney();

        game->getCommomCards().clear();
        for(auto p: game->getPlayers())
            p->getCards().clear();
        for(auto c : listLabels)
              c->clear();
         ui->comLabel1->clear();
          ui->comLabel2->clear();
           ui->comLabel3->clear();
            ui->comLabel4->clear();
             ui->comLabel5->clear();
    }
}

PokerTable::~PokerTable()
{   delete game;
    delete ui;
}
