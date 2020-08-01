#ifndef MENUSTART_H
#define MENUSTART_H
#include"alonesetting.h"
#include"netserverwindow.h"
#include <QMainWindow>

namespace Ui {
class MenuStart;
}

class MenuStart :      public QMainWindow
{
    Q_OBJECT

public:
    explicit MenuStart(QWidget *parent = nullptr);
    ~MenuStart();

public slots:
    void startAloneSetting();
    void startNetSetting();
private:
    Ui::MenuStart *ui;
    AloneSetting * aloneSetting;
    NetServerWindow *  netServerWindow;
};

#endif // MENUSTART_H
