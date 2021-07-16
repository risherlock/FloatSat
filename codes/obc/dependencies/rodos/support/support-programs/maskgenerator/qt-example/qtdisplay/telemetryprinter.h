#ifndef TELEMETRYPRINTER_H
#define TELEMETRYPRINTER_H

#include <QMainWindow>
#include "QtNetwork/qudpsocket.h"


namespace Ui {
class TelemetryPrinter;
}

class TelemetryPrinter : public QMainWindow {
    Q_OBJECT

public:
    explicit TelemetryPrinter(QWidget *parent = 0);
    ~TelemetryPrinter();

    int signalCnt;

    void printAllContent();

signals:
    void periodicSignal(void);

public slots:
  void printOnSignal(void);
  void printOnUDP(void);

public:
    Ui::TelemetryPrinter *ui;
    QUdpSocket *tmSocket;
};

#endif // TELEMETRYPRINTER_H
