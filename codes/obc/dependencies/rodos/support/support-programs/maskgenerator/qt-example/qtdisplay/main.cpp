#include "telemetryprinter.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TelemetryPrinter w;
    w.show();
    //w.ui->temperatur->setText("Ende");

    return a.exec();
}
