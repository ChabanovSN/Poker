#include "menustart.h"
#include "ui_menustart.h"

MenuStart::MenuStart(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MenuStart)
{
    ui->setupUi(this);
    this->setWindowTitle(QString("Poker"));
    QPixmap bkgnd(":/images/menu.jpg");
    bkgnd = bkgnd.scaled(520 ,400);
    QPalette palette;
    palette.setBrush(QPalette::Background, bkgnd);
    this->setPalette(palette);
    aloneSetting = new AloneSetting();

     // подключаем к слоту запуска главного окна по кнопке во втором окне
     connect(aloneSetting, &AloneSetting::MainMenuWindow, this, &MenuStart::show);
     connect(ui->AloneButton,SIGNAL(clicked(bool)),this,SLOT(startAloneSetting()));

}

void MenuStart::startAloneSetting(){


    aloneSetting->show();
    this->close();
}

MenuStart::~MenuStart()
{
    delete ui;
    delete aloneSetting;
}
