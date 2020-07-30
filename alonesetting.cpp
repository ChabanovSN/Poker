#include "alonesetting.h"
#include "ui_alonesetting.h"

AloneSetting::AloneSetting(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AloneSetting)
{
    ui->setupUi(this);
     this->setWindowTitle(QString("Poker"));
    QPixmap bkgnd(":/images/red-card.jpg");
    bkgnd = bkgnd.scaled(455 ,400);
    QPalette palette;
    palette.setBrush(QPalette::Background, bkgnd);
    this->setPalette(palette);
    connect(ui->returnMainMenu,SIGNAL(clicked(bool)),this,SLOT(returnToMainMenu()));
    connect(ui->game,SIGNAL(clicked(bool)),this,SLOT(game()));

}

void AloneSetting::game(){
    int players =  ui->spinBoxPlayers->value();
    int money   =  ui->spinBoxMoney->value();
    int blind   = ui->spinBoxBlind->value();
    poker = new PokerTable(players,money,blind);
    this->close();
    poker->show();


}
void AloneSetting::returnToMainMenu(){
    this->close();      // Закрываем окно
       emit MainMenuWindow(); // И вызываем сигнал на открытие главного окна
}
AloneSetting::~AloneSetting()
{   delete poker;
    delete ui;
}
