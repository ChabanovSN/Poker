#ifndef ALONESETTING_H
#define ALONESETTING_H
#include"pokertable.h"
//#include "menustart.h"
#include <QMainWindow>

namespace Ui {
class AloneSetting;
}

class AloneSetting :  public QMainWindow
{
    Q_OBJECT

public:
    explicit AloneSetting(QWidget *parent = nullptr);
    ~AloneSetting();
signals:
    void MainMenuWindow();
public slots:
    void game();
    void returnToMainMenu();
private:
    PokerTable * poker;
    // MenuStart menuStart;
    Ui::AloneSetting *ui;
};

#endif // ALONESETTING_H
