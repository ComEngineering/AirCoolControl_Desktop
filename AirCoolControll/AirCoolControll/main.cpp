#include "aircoolcontroll.h"
#include <QtWidgets/QApplication>
#include "Configurator.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Configurator conf;
    
    AirCoolControll w;
    QIcon mainIcon(":/Images/app_icon.png");
    a.setWindowIcon(mainIcon);
    
    w.show();
    return a.exec();
}
