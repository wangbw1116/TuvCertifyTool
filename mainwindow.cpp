#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , tcpSocket(nullptr)
{
    ui->setupUi(this);

    tcpSocket = new QTcpSocket();
    connect(tcpSocket, &QTcpSocket::connected, this, &MainWindow::on_connected);
    connect(tcpSocket, &QTcpSocket::disconnected, this, &MainWindow::on_disconnected);
    connect(tcpSocket, &QTcpSocket::readyRead, this, &MainWindow::on_readyRead);
    connect(tcpSocket, &QTcpSocket::errorOccurred, this, &MainWindow::on_error);

    for (int i = 0; i < 10; i++) {
        ui->tableWidget_pvinv_0B->setItem(i, 0, new QTableWidgetItem(""));
        ui->tableWidget_pvinv_0B->setItem(i, 1, new QTableWidgetItem(""));
    }
    for (int i = 0; i < 10; i++) {
        ui->tableWidget_pvinv_0C->setItem(i, 0, new QTableWidgetItem(""));
        ui->tableWidget_pvinv_0C->setItem(i, 1, new QTableWidgetItem(""));
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_connect_clicked()
{
    ui->pushButton_connect->setEnabled(false);
    if (ui->pushButton_connect->text() == "连接") {
        qDebug() << "Ip: " << ui->lineEdit_ip->text();
        qDebug() << "Port: " << ui->lineEdit_port->text();
        tcpSocket->connectToHost(ui->lineEdit_ip->text(), ui->lineEdit_port->text().toUShort());
        if (tcpSocket->waitForConnected(1000)) {
            qDebug() << "connect ok";
        } else {
            qDebug() << "connect failed";
        }
    } else {
        tcpSocket->disconnectFromHost();
    }
    ui->pushButton_connect->setEnabled(true);
}

void MainWindow::on_connected()
{
    ui->pushButton_connect->setText("断开连接");
}

void MainWindow::on_disconnected()
{
    ui->pushButton_connect->setText("连接");
}

/*
格式：
{
    "PvInv":{
        "80":{
            "00":1,
            "01":2,
            ...
        },
        ...
    },
    ...
}
*/
void MainWindow::on_readyRead()
{
    QByteArray data = tcpSocket->readAll();
    QJsonObject obj = QJsonDocument::fromJson(data).object();
    if (obj.contains("PvInv")) {
        parsePvInv(obj.value("PvInv").toObject());
    }
    if (obj.contains("Charger")) {
        parseCharger(obj.value("Charger").toObject());
    }
}

void MainWindow::on_error()
{
    qDebug() << "socket error: " << tcpSocket->errorString();
}

void MainWindow::parsePvInv(const QJsonObject &obj)
{
    if (obj.contains("80")) {
        parsePvInv_80(obj.value("80").toObject());
    }
    if (obj.contains("81")) {
        parsePvInv_81(obj.value("81").toObject());
    }
    if (obj.contains("82")) {
        parsePvInv_82(obj.value("82").toObject());
    }
}

void MainWindow::parsePvInv_80(const QJsonObject &obj)
{
    if (obj.contains("00")) {
        uint32_t val = obj.value("00").toInt();
        QString info;
        switch (val) {
        case 0x0:
            info = "NULL";
            break;
        case 0x1:
            info = "初始化";
            break;
        case 0x2:
            info = "停机";
            break;
        case 0x3:
            info = "运行";
            break;
        case 0xa:
            info = "开环调试";
            break;
        case 0xf:
            info = "故障";
            break;
        default:
            info = "非法数值";
            break;
        }
        ui->lineEdit_pvinv_80_00->setText("0x"+QString::number(val, 16)+":"+info);
    }
    if (obj.contains("01")) {
        uint32_t val = obj.value("01").toInt();
        QString info;
        switch (val) {
        case 0x0:
            info = "NULL";
            break;
        case 0x1:
            info = "待机";
            break;
        case 0x2:
            info = "并网预充";
            break;
        case 0x3:
            info = "并网软起";
            break;
        case 0x4:
            info = "并网运行";
            break;
        case 0x5:
            info = "离网启动";
            break;
        case 0x6:
            info = "离网运行";
            break;
        default:
            info = "非法数值";
            break;
        }
        ui->lineEdit_pvinv_80_01->setText("0x"+QString::number(val, 16)+":"+info);
    }
    if (obj.contains("02")) {
        uint32_t val = obj.value("02").toInt();
        QString info;
        switch (val) {
        case 0x0:
            info = "NULL";
            break;
        case 0x4:
            info = "请求并网运行";
            break;
        case 0x6:
            info = "请求离网运行";
            break;
        default:
            info = "非法数值";
            break;
        }
        ui->lineEdit_pvinv_80_02->setText("0x"+QString::number(val, 16)+":"+info);
    }
    if (obj.contains("03")) {
        uint32_t val = obj.value("03").toInt();
        QString info;
        switch (val) {
        case 0x0:
            info = "NULL";
            break;
        case 0x1:
            info = "正常";
            break;
        case 0x2:
            info = "故障断开连接";
            break;
        case 0x3:
            info = "故障恢复重连";
            break;
        case 0x4:
            info = "故障穿越";
            break;
        case 0x5:
            info = "故障穿越恢复";
            break;
        case 0x6:
            info = "功-频调节";
            break;
        default:
            info = "非法数值";
            break;
        }
        ui->lineEdit_pvinv_80_03->setText("0x"+QString::number(val, 16)+":"+info);
    }
    if (obj.contains("04")) {
        uint32_t val = obj.value("04").toInt();
        QString info;
        switch (val) {
        case 0x0:
            info = "NULL";
            break;
        case 0x1:
            info = "初始化";
            break;
        case 0x2:
            info = "停机";
            break;
        case 0x3:
            info = "运行";
            break;
        case 0xa:
            info = "开环调试";
            break;
        case 0xf:
            info = "故障";
            break;
        default:
            info = "非法数值";
            break;
        }
        ui->lineEdit_pvinv_80_04->setText("0x"+QString::number(val, 16)+":"+info);
    }
    if (obj.contains("05")) {
        uint32_t val = obj.value("05").toInt();
        QString info;
        switch (val) {
        case 0x0:
            info = "NULL";
            break;
        case 0x1:
            info = "初始化";
            break;
        case 0x2:
            info = "停机";
            break;
        case 0x3:
            info = "运行";
            break;
        case 0xa:
            info = "开环调试";
            break;
        case 0xf:
            info = "故障";
            break;
        default:
            info = "非法数值";
            break;
        }
        ui->lineEdit_pvinv_80_05->setText("0x"+QString::number(val, 16)+":"+info);
    }
}

void MainWindow::parsePvInv_81(const QJsonObject &obj)
{
    if (obj.contains("00")) {
        ui->lineEdit_pvinv_81_00->setText(QString::number(obj.value("00").toDouble(), 'f', 3));
    }
    if (obj.contains("01")) {
        ui->lineEdit_pvinv_81_01->setText(QString::number(obj.value("01").toDouble(), 'f', 3));
    }
    if (obj.contains("02")) {
        ui->lineEdit_pvinv_81_02->setText(QString::number(obj.value("02").toDouble(), 'f', 3));
    }
    if (obj.contains("03")) {
        ui->lineEdit_pvinv_81_03->setText(QString::number(obj.value("03").toDouble(), 'f', 3));
    }
    if (obj.contains("04")) {
        ui->lineEdit_pvinv_81_04->setText(QString::number(obj.value("04").toDouble(), 'f', 3));
    }
    if (obj.contains("05")) {
        ui->lineEdit_pvinv_81_05->setText(QString::number(obj.value("05").toDouble(), 'f', 3));
    }
    if (obj.contains("06")) {
        ui->lineEdit_pvinv_81_06->setText(QString::number(obj.value("06").toDouble(), 'f', 3));
    }
    if (obj.contains("07")) {
        ui->lineEdit_pvinv_81_07->setText(QString::number(obj.value("07").toDouble(), 'f', 3));
    }
    if (obj.contains("08")) {
        ui->lineEdit_pvinv_81_08->setText(QString::number(obj.value("08").toDouble(), 'f', 3));
    }
    if (obj.contains("09")) {
        ui->lineEdit_pvinv_81_09->setText(QString::number(obj.value("09").toDouble(), 'f', 3));
    }
    if (obj.contains("10")) {
        ui->lineEdit_pvinv_81_10->setText(QString::number(obj.value("10").toDouble(), 'f', 3));
    }
    if (obj.contains("11")) {
        ui->lineEdit_pvinv_81_11->setText(QString::number(obj.value("11").toDouble(), 'f', 3));
    }
    if (obj.contains("12")) {
        ui->lineEdit_pvinv_81_12->setText(QString::number(obj.value("12").toDouble(), 'f', 3));
    }
    if (obj.contains("13")) {
        ui->lineEdit_pvinv_81_13->setText(QString::number(obj.value("13").toDouble(), 'f', 3));
    }
    if (obj.contains("14")) {
        ui->lineEdit_pvinv_81_14->setText(QString::number(obj.value("14").toDouble(), 'f', 3));
    }
    if (obj.contains("15")) {
        ui->lineEdit_pvinv_81_15->setText(QString::number(obj.value("15").toDouble(), 'f', 3));
    }
    if (obj.contains("16")) {
        ui->lineEdit_pvinv_81_16->setText(QString::number(obj.value("16").toDouble(), 'f', 3));
    }
    if (obj.contains("17")) {
        ui->lineEdit_pvinv_81_17->setText(QString::number(obj.value("17").toDouble(), 'f', 3));
    }
    if (obj.contains("18")) {
        ui->lineEdit_pvinv_81_18->setText(QString::number(obj.value("18").toDouble(), 'f', 3));
    }
    if (obj.contains("19")) {
        ui->lineEdit_pvinv_81_19->setText(QString::number(obj.value("19").toDouble(), 'f', 3));
    }
    if (obj.contains("20")) {
        ui->lineEdit_pvinv_81_20->setText(QString::number(obj.value("20").toDouble(), 'f', 3));
    }
    if (obj.contains("21")) {
        ui->lineEdit_pvinv_81_21->setText(QString::number(obj.value("21").toDouble(), 'f', 3));
    }
    if (obj.contains("22")) {
        ui->lineEdit_pvinv_81_22->setText(QString::number(obj.value("22").toDouble(), 'f', 3));
    }
    if (obj.contains("23")) {
        ui->lineEdit_pvinv_81_23->setText(QString::number(obj.value("23").toDouble(), 'f', 3));
    }
    if (obj.contains("24")) {
        ui->lineEdit_pvinv_81_24->setText(QString::number(obj.value("24").toDouble(), 'f', 3));
    }
    if (obj.contains("25")) {
        ui->lineEdit_pvinv_81_25->setText(QString::number(obj.value("25").toDouble(), 'f', 3));
    }
}

void MainWindow::parsePvInv_82(const QJsonObject &obj)
{
    if (obj.contains("00")) {
        ui->lineEdit_pvinv_82_00->setText("0x"+QString::number(obj.value("00").toInt(), 16));
    }
    if (obj.contains("01")) {
        ui->lineEdit_pvinv_82_01->setText("0x"+QString::number(obj.value("01").toInt(), 16));
    }
    if (obj.contains("02")) {
        ui->lineEdit_pvinv_82_02->setText("0x"+QString::number(obj.value("02").toInt(), 16));
    }
    if (obj.contains("03")) {
        ui->lineEdit_pvinv_82_03->setText("0x"+QString::number(obj.value("03").toInt(), 16));
    }
    if (obj.contains("04")) {
        ui->lineEdit_pvinv_82_04->setText("0x"+QString::number(obj.value("04").toInt(), 16));
    }
}

void MainWindow::parseCharger(const QJsonObject &obj)
{
    if (obj.contains("80")) {
        parseCharger_80(obj.value("80").toObject());
    }
    if (obj.contains("81")) {
        parseCharger_81(obj.value("81").toObject());
    }
    if (obj.contains("82")) {
        parseCharger_82(obj.value("82").toObject());
    }
}

void MainWindow::parseCharger_80(const QJsonObject &obj)
{
    if (obj.contains("00")) {
        uint32_t val = obj.value("00").toInt();
        QString info;
        switch (val) {
        case 0x0:
            info = "NULL";
            break;
        case 0x1:
            info = "初始化";
            break;
        case 0x2:
            info = "停机";
            break;
        case 0x3:
            info = "运行";
            break;
        case 0xa:
            info = "开环调试";
            break;
        case 0xf:
            info = "故障";
            break;
        default:
            info = "非法数值";
            break;
        }
        ui->lineEdit_charger_80_00->setText("0x"+QString::number(val, 16)+":"+info);
    }
    if (obj.contains("01")) {
        uint32_t val = obj.value("01").toInt();
        QString info;
        switch (val) {
        case 0x0:
            info = "NULL";
            break;
        case 0x1:
            info = "待机";
            break;
        case 0x2:
            info = "母线PC预充";
            break;
        case 0x3:
            info = "母线DAB预充";
            break;
        case 0x4:
            info = "母线FBC软起";
            break;
        case 0x5:
            info = "母线电压控制";
            break;
        case 0x6:
            info = "电池电流控制";
            break;
        default:
            info = "非法数值";
            break;
        }
        ui->lineEdit_charger_80_01->setText("0x"+QString::number(val, 16)+":"+info);
    }
}

void MainWindow::parseCharger_81(const QJsonObject &obj)
{
    if (obj.contains("00")) {
        ui->lineEdit_charger_81_00->setText(QString::number(obj.value("00").toDouble(), 'f', 3));
    }
    if (obj.contains("01")) {
        ui->lineEdit_charger_81_01->setText(QString::number(obj.value("01").toDouble(), 'f', 3));
    }
    if (obj.contains("02")) {
        ui->lineEdit_charger_81_02->setText(QString::number(obj.value("02").toDouble(), 'f', 3));
    }
    if (obj.contains("03")) {
        ui->lineEdit_charger_81_03->setText(QString::number(obj.value("03").toDouble(), 'f', 3));
    }
    if (obj.contains("04")) {
        ui->lineEdit_charger_81_04->setText(QString::number(obj.value("04").toDouble(), 'f', 3));
    }
    if (obj.contains("05")) {
        ui->lineEdit_charger_81_05->setText(QString::number(obj.value("05").toDouble(), 'f', 3));
    }
    if (obj.contains("06")) {
        ui->lineEdit_charger_81_06->setText(QString::number(obj.value("06").toDouble(), 'f', 3));
    }
    if (obj.contains("07")) {
        ui->lineEdit_charger_81_07->setText(QString::number(obj.value("07").toDouble(), 'f', 3));
    }
    if (obj.contains("08")) {
        ui->lineEdit_charger_81_08->setText(QString::number(obj.value("08").toDouble(), 'f', 3));
    }
    if (obj.contains("09")) {
        ui->lineEdit_charger_81_09->setText(QString::number(obj.value("09").toDouble(), 'f', 3));
    }
}

void MainWindow::parseCharger_82(const QJsonObject &obj)
{
    if (obj.contains("00")) {
        ui->lineEdit_charger_82_00->setText("0x"+QString::number(obj.value("00").toInt(), 16));
    }
}

void MainWindow::on_pushButton_pvinv_01_00_clicked()
{
    int val = ui->comboBox_pvinv_01_00->currentText().split(':').at(0).toUInt(nullptr, 16);
    QJsonObject devObj, msgObj, dataObj;
    dataObj.insert("00", QJsonValue(val));
    msgObj.insert("01", QJsonValue(dataObj));
    devObj.insert("PvInv", QJsonValue(msgObj));
    QByteArray data = QJsonDocument(devObj).toJson();
    tcpSocket->write(data);
}

void MainWindow::on_pushButton_pvinv_02_00_clicked()
{
    int val = ui->comboBox_pvinv_02_00->currentText().split(':').at(0).toUInt(nullptr, 16);
    QJsonObject devObj, msgObj, dataObj;
    dataObj.insert("00", QJsonValue(val));
    msgObj.insert("02", QJsonValue(dataObj));
    devObj.insert("PvInv", QJsonValue(msgObj));
    QByteArray data = QJsonDocument(devObj).toJson();
    tcpSocket->write(data);
}

void MainWindow::on_pushButton_pvinv_03_00_clicked()
{
    int val = ui->comboBox_pvinv_03_00->currentText().split(':').at(0).toUInt(nullptr, 16);
    QJsonObject devObj, msgObj, dataObj;
    dataObj.insert("00", QJsonValue(val));
    msgObj.insert("03", QJsonValue(dataObj));
    devObj.insert("PvInv", QJsonValue(msgObj));
    QByteArray data = QJsonDocument(devObj).toJson();
    tcpSocket->write(data);
}

void MainWindow::on_pushButton_pvinv_04_00_clicked()
{
    int val = ui->lineEdit_pvinv_04_00->text().toUInt();
    QJsonObject devObj, msgObj, dataObj;
    dataObj.insert("00", QJsonValue(val));
    msgObj.insert("04", QJsonValue(dataObj));
    devObj.insert("PvInv", QJsonValue(msgObj));
    QByteArray data = QJsonDocument(devObj).toJson();
    tcpSocket->write(data);
}

void MainWindow::on_pushButton_pvinv_05_00_clicked()
{
    double val = ui->lineEdit_pvinv_05_00->text().toDouble();
    QJsonObject devObj, msgObj, dataObj;
    dataObj.insert("00", QJsonValue(val));
    msgObj.insert("05", QJsonValue(dataObj));
    devObj.insert("PvInv", QJsonValue(msgObj));
    QByteArray data = QJsonDocument(devObj).toJson();
    tcpSocket->write(data);
}

void MainWindow::on_pushButton_pvinv_06_00_clicked()
{
    int val = ui->comboBox_pvinv_06_00->currentText().split(':').at(0).toUInt(nullptr, 16);
    QJsonObject devObj, msgObj, dataObj;
    dataObj.insert("00", QJsonValue(val));
    msgObj.insert("06", QJsonValue(dataObj));
    devObj.insert("PvInv", QJsonValue(msgObj));
    QByteArray data = QJsonDocument(devObj).toJson();
    tcpSocket->write(data);
}

void MainWindow::on_pushButton_pvinv_07_00_clicked()
{
    int val = ui->comboBox_pvinv_07_00->currentText().split(':').at(0).toUInt(nullptr, 16);
    QJsonObject devObj, msgObj, dataObj;
    dataObj.insert("00", QJsonValue(val));
    msgObj.insert("07", QJsonValue(dataObj));
    devObj.insert("PvInv", QJsonValue(msgObj));
    QByteArray data = QJsonDocument(devObj).toJson();
    tcpSocket->write(data);
}

void MainWindow::on_pushButton_pvinv_08_00_clicked()
{
    double val = ui->lineEdit_pvinv_08_00->text().toDouble();
    QJsonObject devObj, msgObj, dataObj;
    dataObj.insert("00", QJsonValue(val));
    msgObj.insert("08", QJsonValue(dataObj));
    devObj.insert("PvInv", QJsonValue(msgObj));
    QByteArray data = QJsonDocument(devObj).toJson();
    tcpSocket->write(data);
}

void MainWindow::on_pushButton_pvinv_09_clicked()
{
    double val00 = ui->lineEdit_pvinv_09_00->text().toDouble();
    double val01 = ui->lineEdit_pvinv_09_01->text().toDouble();
    QJsonObject devObj, msgObj, dataObj;
    dataObj.insert("00", QJsonValue(val00));
    dataObj.insert("01", QJsonValue(val01));
    msgObj.insert("09", QJsonValue(dataObj));
    devObj.insert("PvInv", QJsonValue(msgObj));
    QByteArray data = QJsonDocument(devObj).toJson();
    tcpSocket->write(data);
}

void MainWindow::on_pushButton_pvinv_09_00_clicked()
{
    double val = ui->lineEdit_pvinv_09_00->text().toDouble();
    QJsonObject devObj, msgObj, dataObj;
    dataObj.insert("00", QJsonValue(val));
    msgObj.insert("09", QJsonValue(dataObj));
    devObj.insert("PvInv", QJsonValue(msgObj));
    QByteArray data = QJsonDocument(devObj).toJson();
    tcpSocket->write(data);
}

void MainWindow::on_pushButton_pvinv_09_01_clicked()
{
    double val = ui->lineEdit_pvinv_09_01->text().toDouble();
    QJsonObject devObj, msgObj, dataObj;
    dataObj.insert("01", QJsonValue(val));
    msgObj.insert("09", QJsonValue(dataObj));
    devObj.insert("PvInv", QJsonValue(msgObj));
    QByteArray data = QJsonDocument(devObj).toJson();
    tcpSocket->write(data);
}

void MainWindow::on_pushButton_pvinv_0A_00_clicked()
{
    double val = ui->lineEdit_pvinv_0A_00->text().toDouble();
    QJsonObject devObj, msgObj, dataObj;
    dataObj.insert("00", QJsonValue(val));
    msgObj.insert("0A", QJsonValue(dataObj));
    devObj.insert("PvInv", QJsonValue(msgObj));
    QByteArray data = QJsonDocument(devObj).toJson();
    tcpSocket->write(data);
}

void MainWindow::on_pushButton_pvinv_0A_01_clicked()
{
    double val = ui->lineEdit_pvinv_0A_01->text().toDouble();
    QJsonObject devObj, msgObj, dataObj;
    dataObj.insert("01", QJsonValue(val));
    msgObj.insert("0A", QJsonValue(dataObj));
    devObj.insert("PvInv", QJsonValue(msgObj));
    QByteArray data = QJsonDocument(devObj).toJson();
    tcpSocket->write(data);
}

void MainWindow::on_pushButton_pvinv_0A_02_clicked()
{
    double val = ui->lineEdit_pvinv_0A_02->text().toDouble();
    QJsonObject devObj, msgObj, dataObj;
    dataObj.insert("02", QJsonValue(val));
    msgObj.insert("0A", QJsonValue(dataObj));
    devObj.insert("PvInv", QJsonValue(msgObj));
    QByteArray data = QJsonDocument(devObj).toJson();
    tcpSocket->write(data);
}

void MainWindow::on_pushButton_pvinv_0A_03_clicked()
{
    int val = ui->lineEdit_pvinv_0A_03->text().toUInt();
    QJsonObject devObj, msgObj, dataObj;
    dataObj.insert("03", QJsonValue(val));
    msgObj.insert("0A", QJsonValue(dataObj));
    devObj.insert("PvInv", QJsonValue(msgObj));
    QByteArray data = QJsonDocument(devObj).toJson();
    tcpSocket->write(data);
}

void MainWindow::on_pushButton_pvinv_0A_04_clicked()
{
    int val = ui->lineEdit_pvinv_0A_04->text().toUInt();
    QJsonObject devObj, msgObj, dataObj;
    dataObj.insert("04", QJsonValue(val));
    msgObj.insert("0A", QJsonValue(dataObj));
    devObj.insert("PvInv", QJsonValue(msgObj));
    QByteArray data = QJsonDocument(devObj).toJson();
    tcpSocket->write(data);
}

void MainWindow::on_pushButton_pvinv_0A_05_clicked()
{
    double val = ui->lineEdit_pvinv_0A_05->text().toDouble();
    QJsonObject devObj, msgObj, dataObj;
    dataObj.insert("05", QJsonValue(val));
    msgObj.insert("0A", QJsonValue(dataObj));
    devObj.insert("PvInv", QJsonValue(msgObj));
    QByteArray data = QJsonDocument(devObj).toJson();
    tcpSocket->write(data);
}

void MainWindow::on_pushButton_pvinv_0A_06_clicked()
{
    double val = ui->lineEdit_pvinv_0A_06->text().toDouble();
    QJsonObject devObj, msgObj, dataObj;
    dataObj.insert("06", QJsonValue(val));
    msgObj.insert("0A", QJsonValue(dataObj));
    devObj.insert("PvInv", QJsonValue(msgObj));
    QByteArray data = QJsonDocument(devObj).toJson();
    tcpSocket->write(data);
}

void MainWindow::on_pushButton_pvinv_0A_07_clicked()
{
    double val = ui->lineEdit_pvinv_0A_07->text().toDouble();
    QJsonObject devObj, msgObj, dataObj;
    dataObj.insert("07", QJsonValue(val));
    msgObj.insert("0A", QJsonValue(dataObj));
    devObj.insert("PvInv", QJsonValue(msgObj));
    QByteArray data = QJsonDocument(devObj).toJson();
    tcpSocket->write(data);
}

void MainWindow::on_pushButton_pvinv_0A_08_clicked()
{
    int val = ui->lineEdit_pvinv_0A_08->text().toUInt();
    QJsonObject devObj, msgObj, dataObj;
    dataObj.insert("08", QJsonValue(val));
    msgObj.insert("0A", QJsonValue(dataObj));
    devObj.insert("PvInv", QJsonValue(msgObj));
    QByteArray data = QJsonDocument(devObj).toJson();
    tcpSocket->write(data);
}

void MainWindow::on_pushButton_pvinv_0A_09_clicked()
{
    int val = ui->lineEdit_pvinv_0A_09->text().toUInt();
    QJsonObject devObj, msgObj, dataObj;
    dataObj.insert("09", QJsonValue(val));
    msgObj.insert("0A", QJsonValue(dataObj));
    devObj.insert("PvInv", QJsonValue(msgObj));
    QByteArray data = QJsonDocument(devObj).toJson();
    tcpSocket->write(data);
}

void MainWindow::on_pushButton_pvinv_0A_10_clicked()
{
    double val = ui->lineEdit_pvinv_0A_10->text().toDouble();
    QJsonObject devObj, msgObj, dataObj;
    dataObj.insert("10", QJsonValue(val));
    msgObj.insert("0A", QJsonValue(dataObj));
    devObj.insert("PvInv", QJsonValue(msgObj));
    QByteArray data = QJsonDocument(devObj).toJson();
    tcpSocket->write(data);
}

void MainWindow::on_pushButton_pvinv_0A_11_clicked()
{
    double val = ui->lineEdit_pvinv_0A_11->text().toDouble();
    QJsonObject devObj, msgObj, dataObj;
    dataObj.insert("11", QJsonValue(val));
    msgObj.insert("0A", QJsonValue(dataObj));
    devObj.insert("PvInv", QJsonValue(msgObj));
    QByteArray data = QJsonDocument(devObj).toJson();
    tcpSocket->write(data);
}

void MainWindow::on_pushButton_pvinv_0A_12_clicked()
{
    double val = ui->lineEdit_pvinv_0A_12->text().toDouble();
    QJsonObject devObj, msgObj, dataObj;
    dataObj.insert("12", QJsonValue(val));
    msgObj.insert("0A", QJsonValue(dataObj));
    devObj.insert("PvInv", QJsonValue(msgObj));
    QByteArray data = QJsonDocument(devObj).toJson();
    tcpSocket->write(data);
}

void MainWindow::on_pushButton_pvinv_0A_13_clicked()
{
    int val = ui->lineEdit_pvinv_0A_13->text().toUInt();
    QJsonObject devObj, msgObj, dataObj;
    dataObj.insert("13", QJsonValue(val));
    msgObj.insert("0A", QJsonValue(dataObj));
    devObj.insert("PvInv", QJsonValue(msgObj));
    QByteArray data = QJsonDocument(devObj).toJson();
    tcpSocket->write(data);
}

void MainWindow::on_pushButton_pvinv_0A_14_clicked()
{
    int val = ui->lineEdit_pvinv_0A_14->text().toUInt();
    QJsonObject devObj, msgObj, dataObj;
    dataObj.insert("14", QJsonValue(val));
    msgObj.insert("0A", QJsonValue(dataObj));
    devObj.insert("PvInv", QJsonValue(msgObj));
    QByteArray data = QJsonDocument(devObj).toJson();
    tcpSocket->write(data);
}

void MainWindow::on_pushButton_pvinv_0A_15_clicked()
{
    double val = ui->lineEdit_pvinv_0A_15->text().toDouble();
    QJsonObject devObj, msgObj, dataObj;
    dataObj.insert("15", QJsonValue(val));
    msgObj.insert("0A", QJsonValue(dataObj));
    devObj.insert("PvInv", QJsonValue(msgObj));
    QByteArray data = QJsonDocument(devObj).toJson();
    tcpSocket->write(data);
}

void MainWindow::on_pushButton_pvinv_0A_16_clicked()
{
    double val = ui->lineEdit_pvinv_0A_16->text().toDouble();
    QJsonObject devObj, msgObj, dataObj;
    dataObj.insert("16", QJsonValue(val));
    msgObj.insert("0A", QJsonValue(dataObj));
    devObj.insert("PvInv", QJsonValue(msgObj));
    QByteArray data = QJsonDocument(devObj).toJson();
    tcpSocket->write(data);
}

void MainWindow::on_pushButton_pvinv_0A_17_clicked()
{
    double val = ui->lineEdit_pvinv_0A_17->text().toDouble();
    QJsonObject devObj, msgObj, dataObj;
    dataObj.insert("17", QJsonValue(val));
    msgObj.insert("0A", QJsonValue(dataObj));
    devObj.insert("PvInv", QJsonValue(msgObj));
    QByteArray data = QJsonDocument(devObj).toJson();
    tcpSocket->write(data);
}

void MainWindow::on_pushButton_pvinv_0A_18_clicked()
{
    int val = ui->lineEdit_pvinv_0A_18->text().toUInt();
    QJsonObject devObj, msgObj, dataObj;
    dataObj.insert("18", QJsonValue(val));
    msgObj.insert("0A", QJsonValue(dataObj));
    devObj.insert("PvInv", QJsonValue(msgObj));
    QByteArray data = QJsonDocument(devObj).toJson();
    tcpSocket->write(data);
}

void MainWindow::on_pushButton_pvinv_0A_19_clicked()
{
    int val = ui->lineEdit_pvinv_0A_19->text().toUInt();
    QJsonObject devObj, msgObj, dataObj;
    dataObj.insert("19", QJsonValue(val));
    msgObj.insert("0A", QJsonValue(dataObj));
    devObj.insert("PvInv", QJsonValue(msgObj));
    QByteArray data = QJsonDocument(devObj).toJson();
    tcpSocket->write(data);
}

void MainWindow::on_pushButton_pvinv_0A_20_clicked()
{
    double val = ui->lineEdit_pvinv_0A_20->text().toDouble();
    QJsonObject devObj, msgObj, dataObj;
    dataObj.insert("20", QJsonValue(val));
    msgObj.insert("0A", QJsonValue(dataObj));
    devObj.insert("PvInv", QJsonValue(msgObj));
    QByteArray data = QJsonDocument(devObj).toJson();
    tcpSocket->write(data);
}

void MainWindow::on_pushButton_pvinv_0A_21_clicked()
{
    double val = ui->lineEdit_pvinv_0A_21->text().toDouble();
    QJsonObject devObj, msgObj, dataObj;
    dataObj.insert("21", QJsonValue(val));
    msgObj.insert("0A", QJsonValue(dataObj));
    devObj.insert("PvInv", QJsonValue(msgObj));
    QByteArray data = QJsonDocument(devObj).toJson();
    tcpSocket->write(data);
}

void MainWindow::on_pushButton_pvinv_0A_22_clicked()
{
    int val = ui->lineEdit_pvinv_0A_22->text().toUInt();
    QJsonObject devObj, msgObj, dataObj;
    dataObj.insert("22", QJsonValue(val));
    msgObj.insert("0A", QJsonValue(dataObj));
    devObj.insert("PvInv", QJsonValue(msgObj));
    QByteArray data = QJsonDocument(devObj).toJson();
    tcpSocket->write(data);
}

void MainWindow::on_pushButton_pvinv_0A_23_clicked()
{
    int val = ui->lineEdit_pvinv_0A_23->text().toUInt();
    QJsonObject devObj, msgObj, dataObj;
    dataObj.insert("23", QJsonValue(val));
    msgObj.insert("0A", QJsonValue(dataObj));
    devObj.insert("PvInv", QJsonValue(msgObj));
    QByteArray data = QJsonDocument(devObj).toJson();
    tcpSocket->write(data);
}

void MainWindow::on_pushButton_pvinv_0A_24_clicked()
{
    int val = ui->comboBox_pvinv_0A_24->currentIndex();
    QJsonObject devObj, msgObj, dataObj;
    dataObj.insert("24", QJsonValue(val));
    msgObj.insert("0A", QJsonValue(dataObj));
    devObj.insert("PvInv", QJsonValue(msgObj));
    QByteArray data = QJsonDocument(devObj).toJson();
    tcpSocket->write(data);
}

void MainWindow::on_pushButton_pvinv_0A_setAll_clicked()
{
    QJsonObject devObj, msgObj, dataObj;

    dataObj.insert("00", QJsonValue(ui->lineEdit_pvinv_0A_00->text().toDouble()));
    dataObj.insert("01", QJsonValue(ui->lineEdit_pvinv_0A_01->text().toDouble()));
    dataObj.insert("02", QJsonValue(ui->lineEdit_pvinv_0A_02->text().toDouble()));
    dataObj.insert("03", QJsonValue((int)ui->lineEdit_pvinv_0A_03->text().toUInt()));
    dataObj.insert("04", QJsonValue((int)ui->lineEdit_pvinv_0A_04->text().toUInt()));
    dataObj.insert("05", QJsonValue(ui->lineEdit_pvinv_0A_05->text().toDouble()));
    dataObj.insert("06", QJsonValue(ui->lineEdit_pvinv_0A_06->text().toDouble()));
    dataObj.insert("07", QJsonValue(ui->lineEdit_pvinv_0A_07->text().toDouble()));
    dataObj.insert("08", QJsonValue((int)ui->lineEdit_pvinv_0A_08->text().toUInt()));
    dataObj.insert("09", QJsonValue((int)ui->lineEdit_pvinv_0A_09->text().toUInt()));
    dataObj.insert("10", QJsonValue(ui->lineEdit_pvinv_0A_10->text().toDouble()));
    dataObj.insert("11", QJsonValue(ui->lineEdit_pvinv_0A_11->text().toDouble()));
    dataObj.insert("12", QJsonValue(ui->lineEdit_pvinv_0A_12->text().toDouble()));
    dataObj.insert("13", QJsonValue((int)ui->lineEdit_pvinv_0A_13->text().toUInt()));
    dataObj.insert("14", QJsonValue((int)ui->lineEdit_pvinv_0A_14->text().toUInt()));
    dataObj.insert("15", QJsonValue(ui->lineEdit_pvinv_0A_15->text().toDouble()));
    dataObj.insert("16", QJsonValue(ui->lineEdit_pvinv_0A_16->text().toDouble()));
    dataObj.insert("17", QJsonValue(ui->lineEdit_pvinv_0A_17->text().toDouble()));
    dataObj.insert("18", QJsonValue((int)ui->lineEdit_pvinv_0A_18->text().toUInt()));
    dataObj.insert("19", QJsonValue((int)ui->lineEdit_pvinv_0A_19->text().toUInt()));
    dataObj.insert("20", QJsonValue(ui->lineEdit_pvinv_0A_20->text().toDouble()));
    dataObj.insert("21", QJsonValue(ui->lineEdit_pvinv_0A_21->text().toDouble()));
    dataObj.insert("22", QJsonValue((int)ui->lineEdit_pvinv_0A_22->text().toUInt()));
    dataObj.insert("23", QJsonValue((int)ui->lineEdit_pvinv_0A_23->text().toUInt()));
    dataObj.insert("24", QJsonValue((int)ui->comboBox_pvinv_0A_24->currentIndex()));

    msgObj.insert("0A", QJsonValue(dataObj));
    devObj.insert("PvInv", QJsonValue(msgObj));
    QByteArray data = QJsonDocument(devObj).toJson();
    tcpSocket->write(data);
}

void MainWindow::on_pushButton_pvinv_0A_clearAll_clicked()
{
    ui->lineEdit_pvinv_0A_00->clear();
    ui->lineEdit_pvinv_0A_01->clear();
    ui->lineEdit_pvinv_0A_02->clear();
    ui->lineEdit_pvinv_0A_03->clear();
    ui->lineEdit_pvinv_0A_04->clear();
    ui->lineEdit_pvinv_0A_05->clear();
    ui->lineEdit_pvinv_0A_06->clear();
    ui->lineEdit_pvinv_0A_07->clear();
    ui->lineEdit_pvinv_0A_08->clear();
    ui->lineEdit_pvinv_0A_09->clear();
    ui->lineEdit_pvinv_0A_10->clear();
    ui->lineEdit_pvinv_0A_11->clear();
    ui->lineEdit_pvinv_0A_12->clear();
    ui->lineEdit_pvinv_0A_13->clear();
    ui->lineEdit_pvinv_0A_14->clear();
    ui->lineEdit_pvinv_0A_15->clear();
    ui->lineEdit_pvinv_0A_16->clear();
    ui->lineEdit_pvinv_0A_17->clear();
    ui->lineEdit_pvinv_0A_18->clear();
    ui->lineEdit_pvinv_0A_19->clear();
    ui->lineEdit_pvinv_0A_20->clear();
    ui->lineEdit_pvinv_0A_21->clear();
    ui->lineEdit_pvinv_0A_22->clear();
    ui->lineEdit_pvinv_0A_23->clear();
    ui->comboBox_pvinv_0A_24->setCurrentIndex(0);
}

void MainWindow::on_pushButton_pvinv_0B_set_clicked()
{
    QJsonObject devObj, msgObj, dataObj;
    int num = ui->lineEdit_pvinv_0B_01->text().toUInt();
    if (num < 0 || num > 10) {
        QMessageBox::warning(NULL, tr("提示"), tr("个数必须在[0, 10]内"), QMessageBox::Yes);
        return;
    }

    dataObj.insert("00", QJsonValue(ui->comboBox_pvinv_0B_00->currentIndex()));
    dataObj.insert("01", QJsonValue(num));
    QJsonArray array1, array2;
    for (int i = 0; i < num; i++) {
        array1.append(QJsonValue((int)ui->tableWidget_pvinv_0B->item(i, 0)->text().toUInt()));
    }
    dataObj.insert("02", QJsonValue(array1));

    for (int i = 0; i < num; i++) {
        array2.append(QJsonValue((int)ui->tableWidget_pvinv_0B->item(i, 1)->text().toUInt()));
    }
    dataObj.insert("03", QJsonValue(array2));

    msgObj.insert("0B", QJsonValue(dataObj));
    devObj.insert("PvInv", QJsonValue(msgObj));
    QByteArray data = QJsonDocument(devObj).toJson();
    tcpSocket->write(data);
}

void MainWindow::on_pushButton_pvinv_0B_clear_clicked()
{
    ui->comboBox_pvinv_0B_00->setCurrentIndex(0);
    ui->lineEdit_pvinv_0B_01->clear();
    for (int i = 0; i < 10; i++) {
        ui->tableWidget_pvinv_0B->item(i, 0)->setText("");
        ui->tableWidget_pvinv_0B->item(i, 1)->setText("");
    }
}

void MainWindow::on_pushButton_pvinv_0C_set_clicked()
{
    QJsonObject devObj, msgObj, dataObj;
    int num = ui->lineEdit_pvinv_0C_01->text().toUInt();
    if (num < 0 || num > 10) {
        QMessageBox::warning(NULL, tr("提示"), tr("个数必须在[0, 10]内"), QMessageBox::Yes);
        return;
    }

    dataObj.insert("00", QJsonValue(ui->comboBox_pvinv_0C_00->currentIndex()));
    dataObj.insert("01", QJsonValue(num));
    QJsonArray array1, array2;
    for (int i = 0; i < num; i++) {
        array1.append(QJsonValue((int)ui->tableWidget_pvinv_0C->item(i, 0)->text().toUInt()));
    }
    dataObj.insert("02", QJsonValue(array1));

    for (int i = 0; i < num; i++) {
        array2.append(QJsonValue((int)ui->tableWidget_pvinv_0C->item(i, 1)->text().toUInt()));
    }
    dataObj.insert("03", QJsonValue(array2));

    msgObj.insert("0C", QJsonValue(dataObj));
    devObj.insert("PvInv", QJsonValue(msgObj));
    QByteArray data = QJsonDocument(devObj).toJson();
    tcpSocket->write(data);
}

void MainWindow::on_pushButton_pvinv_0C_clear_clicked()
{
    ui->comboBox_pvinv_0C_00->setCurrentIndex(0);
    ui->lineEdit_pvinv_0C_01->clear();
    for (int i = 0; i < 10; i++) {
        ui->tableWidget_pvinv_0C->item(i, 0)->setText("");
        ui->tableWidget_pvinv_0C->item(i, 1)->setText("");
    }
}

void MainWindow::on_pushButton_charger_01_00_clicked()
{
    int val = ui->comboBox_charger_01_00->currentText().split(':').at(0).toUInt(nullptr, 16);
    QJsonObject devObj, msgObj, dataObj;
    dataObj.insert("00", QJsonValue(val));
    msgObj.insert("01", QJsonValue(dataObj));
    devObj.insert("Charger", QJsonValue(msgObj));
    QByteArray data = QJsonDocument(devObj).toJson();
    tcpSocket->write(data);
}

void MainWindow::on_pushButton_charger_02_00_clicked()
{
    int val = ui->comboBox_charger_02_00->currentText().split(':').at(0).toUInt(nullptr, 16);
    QJsonObject devObj, msgObj, dataObj;
    dataObj.insert("00", QJsonValue(val));
    msgObj.insert("02", QJsonValue(dataObj));
    devObj.insert("Charger", QJsonValue(msgObj));
    QByteArray data = QJsonDocument(devObj).toJson();
    tcpSocket->write(data);
}

void MainWindow::on_pushButton_charger_03_00_clicked()
{
    double val = ui->lineEdit_charger_03_00->text().toDouble();
    QJsonObject devObj, msgObj, dataObj;
    dataObj.insert("00", QJsonValue(val));
    msgObj.insert("03", QJsonValue(dataObj));
    devObj.insert("Charger", QJsonValue(msgObj));
    QByteArray data = QJsonDocument(devObj).toJson();
    tcpSocket->write(data);
}

void MainWindow::on_pushButton_charger_04_00_clicked()
{
    double val = ui->lineEdit_charger_04_00->text().toDouble();
    QJsonObject devObj, msgObj, dataObj;
    dataObj.insert("00", QJsonValue(val));
    msgObj.insert("04", QJsonValue(dataObj));
    devObj.insert("Charger", QJsonValue(msgObj));
    QByteArray data = QJsonDocument(devObj).toJson();
    tcpSocket->write(data);
}

void MainWindow::on_pushButton_charger_05_00_clicked()
{
    double val = ui->lineEdit_charger_05_00->text().toDouble();
    QJsonObject devObj, msgObj, dataObj;
    dataObj.insert("00", QJsonValue(val));
    msgObj.insert("05", QJsonValue(dataObj));
    devObj.insert("Charger", QJsonValue(msgObj));
    QByteArray data = QJsonDocument(devObj).toJson();
    tcpSocket->write(data);
}

