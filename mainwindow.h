#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_connect_clicked();
    void on_connected();
    void on_disconnected();
    void on_readyRead();
    void on_error();

    void on_pushButton_pvinv_01_00_clicked();
    void on_pushButton_pvinv_02_00_clicked();
    void on_pushButton_pvinv_03_00_clicked();
    void on_pushButton_pvinv_04_00_clicked();
    void on_pushButton_pvinv_05_00_clicked();
    void on_pushButton_pvinv_06_00_clicked();
    void on_pushButton_pvinv_07_00_clicked();
    void on_pushButton_pvinv_08_00_clicked();
    void on_pushButton_pvinv_09_clicked();
    void on_pushButton_pvinv_09_00_clicked();
    void on_pushButton_pvinv_09_01_clicked();
    void on_pushButton_pvinv_0A_00_clicked();
    void on_pushButton_pvinv_0A_01_clicked();
    void on_pushButton_pvinv_0A_02_clicked();
    void on_pushButton_pvinv_0A_03_clicked();
    void on_pushButton_pvinv_0A_04_clicked();
    void on_pushButton_pvinv_0A_05_clicked();
    void on_pushButton_pvinv_0A_06_clicked();
    void on_pushButton_pvinv_0A_07_clicked();
    void on_pushButton_pvinv_0A_08_clicked();
    void on_pushButton_pvinv_0A_09_clicked();
    void on_pushButton_pvinv_0A_10_clicked();
    void on_pushButton_pvinv_0A_11_clicked();
    void on_pushButton_pvinv_0A_12_clicked();
    void on_pushButton_pvinv_0A_13_clicked();
    void on_pushButton_pvinv_0A_14_clicked();
    void on_pushButton_pvinv_0A_15_clicked();
    void on_pushButton_pvinv_0A_16_clicked();
    void on_pushButton_pvinv_0A_17_clicked();
    void on_pushButton_pvinv_0A_18_clicked();
    void on_pushButton_pvinv_0A_19_clicked();
    void on_pushButton_pvinv_0A_20_clicked();
    void on_pushButton_pvinv_0A_21_clicked();
    void on_pushButton_pvinv_0A_22_clicked();
    void on_pushButton_pvinv_0A_23_clicked();
    void on_pushButton_pvinv_0A_24_clicked();
    void on_pushButton_pvinv_0A_setAll_clicked();
    void on_pushButton_pvinv_0A_clearAll_clicked();
    void on_pushButton_pvinv_0B_set_clicked();
    void on_pushButton_pvinv_0B_clear_clicked();
    void on_pushButton_pvinv_0C_set_clicked();
    void on_pushButton_pvinv_0C_clear_clicked();

    void on_pushButton_charger_01_00_clicked();
    void on_pushButton_charger_02_00_clicked();
    void on_pushButton_charger_03_00_clicked();
    void on_pushButton_charger_04_00_clicked();
    void on_pushButton_charger_05_00_clicked();

private:
    void parsePvInv(const QJsonObject &obj);
    void parsePvInv_80(const QJsonObject &obj);
    void parsePvInv_81(const QJsonObject &obj);
    void parsePvInv_82(const QJsonObject &obj);

    void parseCharger(const QJsonObject &obj);
    void parseCharger_80(const QJsonObject &obj);
    void parseCharger_81(const QJsonObject &obj);
    void parseCharger_82(const QJsonObject &obj);

    Ui::MainWindow *ui;
    QTcpSocket *tcpSocket;
};

#endif // MAINWINDOW_H
