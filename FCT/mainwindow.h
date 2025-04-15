#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QTextStream>
#include <QRegularExpression>
#include <QDebug>
#include <QCoreApplication>
#include <windows.h>
#include <QMessageBox>
#include <QWidget>
#include <QCloseEvent>
#include <iostream>
#include <direct.h>
#include <QDir>
#include <QFile>
#include <QFileDialog>


using namespace std;

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
class QMessageBox;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    /*
     * 1.读取数据函数，读取txt
     * 2.显示数据函数,显示在表格里面
     * 3.检测数据函数，表格内数据显示颜色
     * 4.log输出函数,输出数据保存到文件中
     * 5.log文件另存为函数，保存在另存的文件路径
     */

    // 进入会话模式
    void chat();

    // 检测软件版本号
    void CheckSoftwareVersion();

    //检测硬件版本号
    void CheckHardwareVersion();

    //检测母线电压
    void CheckBusVoltage();

    //检测母线电流
    void CheckBusCurrent();

    //检测项电压
    void CheckTermVoltage();

    //检测项电流
    void CheckTermCurrent();

    //获取错误项
    bool IsRedColor(const QColor& color);

    //默认日志路径
    QString GetLogFilePath();

    //另存为
    void CopyLogFileToSelectedDirectory();

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void on_BeginpushButton_clicked();

    void on_FinishpushButton_clicked();

    void on_BrowerspushButton_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
