#include "telemetryprinter.h"
#include "ui_telemetryprinter.h"
#include "QtNetwork/qudpsocket.h"


#include "QTimer"
#include "QTime"
#include "QDebug"

#include "standarttm.h"


StandardTelemetry standardTm;


TelemetryPrinter::TelemetryPrinter(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::TelemetryPrinter) {

    ui->setupUi(this);

    qDebug() << "netcat -u localhost 5030";

    signalCnt = 0;
    QTimer *timer = new QTimer(this);
    timer->start(1000);
    connect(timer,SIGNAL(timeout()),this,SLOT(printOnSignal()));

    tmSocket = new QUdpSocket(this);
    tmSocket->bind(5030, QUdpSocket::ShareAddress);
    connect(tmSocket, SIGNAL(readyRead()), this, SLOT(printOnUDP()));

   QString danger = "QProgressBar::chunk {background-color: rgb(0,0,255)}";
   ui->CMD_InmdiatCmdCnt->setStyleSheet(danger); 

}

TelemetryPrinter::~TelemetryPrinter() {
    delete ui;
}


void TelemetryPrinter::printOnSignal() {

   signalCnt++;

   qDebug()<<"signalCnt="<< signalCnt <<endl;

}

void TelemetryPrinter::printOnUDP() {

    QByteArray datagram;
    datagram.resize(tmSocket->pendingDatagramSize());
    tmSocket->readDatagram(datagram.data(), datagram.size());
    memcpy(&standardTm, datagram.data(), sizeof(standardTm) );
    //ui->speed->setText(QString(datagram));

    printAllContent();

   qDebug()<<" UDP Message" << endl;

}

// data source standardTm.

void TelemetryPrinter::printAllContent() {
    ui->TIM_BoardTime_Secs->display(QString::number(standardTm.TIM_BoardTime_Secs));
    ui->TIM_UTC_Secs->display(QString::number(standardTm.TIM_UTC_Secs));
    ui->CMM_NodeId->display(QString::number(standardTm.CMM_NodeId));
    ui->CMM_AmIWorker->setChecked(standardTm.CMM_AmIWorker != 0);
    ui->HKE_Downlink_Running->setChecked(standardTm.HKE_Downlink_Running != 0);
    ui->WAT_TimingChritical->setChecked(standardTm.WAT_TimingChritical != 0);
    ui->WAT_ResetEnabled->setChecked(standardTm.WAT_ResetEnabled != 0);
    ui->HKE_HistoryRate->setText(QString::number(standardTm.HKE_HistoryRate));
    ui->CMD_InmdiatCmdCnt->setValue(standardTm.CMD_InmdiatCmdCnt);
    ui->ANM_AnomalyCnt->setValue(standardTm.ANM_AnomalyCnt);
    ui->CMM_BootCnt->display(QString::number(standardTm.CMM_BootCnt));
    ui->CMD_RejectedCmdCnt->setText(QString::number(standardTm.CMD_RejectedCmdCnt));
}



