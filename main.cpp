#include <QApplication>
#include <QFile>

#include "devicemanagercontroller.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QFile File(":qss/stylesheet.qss");
    File.open(QFile::ReadOnly);
    QString StyleSheet = QLatin1String(File.readAll());
    a.setStyleSheet(StyleSheet);

    DeviceManagerController *controller = new DeviceManagerController;
    controller->launch();

    return a.exec();
}
