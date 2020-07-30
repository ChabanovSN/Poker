#ifndef MENUSTART_H
#define MENUSTART_H
#include"alonesetting.h"
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
private:
    Ui::MenuStart *ui;
    AloneSetting * aloneSetting;
};

#endif // MENUSTART_H
