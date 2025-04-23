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
#include <ctime>

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

    //初始化qtablewidget
    void InitializeTableSettings();

    //获取错误项
    bool IsRedColor(const QColor& color);

    //设置日志路径
    QString GetLogFilePath(const QString &appDirPath);

    //txt数据转存QList
    QList<QStringList> LoadDataFromFile(const QString &FilePath);

    //QList数据填充到qtablewidget
    void PopulateTable(const QList<QStringList> &TableData);

    //检验数据合法性
    bool ValidateCellContent(const QString &CellValue, int ColumnIndex);

    //历史故障码填充
    void PopulateHistoricalDTCs(bool &IsValid,const QString &HistoricalDTCsData);

    //当前故障码填充
    void PopulateActiveDTCs(bool &IsValid,const QString &ActiveDTCsData);

    //重启检测
    void CheckSystemReboot(bool &SystemReboot);

    //AOI车辆识别
    void IdentifyVehicleByAOI(bool &VehicleByAOI);

    //静态电流填充
    void PopulateQuiescentCurrent(float &QuiescentCurrent);

    // 日志输出
    void WriteLog(const QString &LogfilePath,const QString &UseTime);



protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void on_BeginpushButton_clicked();

private:
    Ui::MainWindow *ui;
    QTabWidget DatatableWidget;
};
#endif // MAINWINDOW_H
