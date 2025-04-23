#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QWindow> // 用于窗口操作
#include <direct.h>
#include <windows.h>
#include <io.h>
#include <direct.h>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    InitializeTableSettings();

    // 设置窗口置顶
    setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);

    // 强制刷新窗口状态（某些平台可能需要）
    if (windowHandle()) {
        windowHandle()->setFlags(windowHandle()->flags() | Qt::WindowStaysOnTopHint);
    }

    // 确保窗口显示（修改标志后可能需要重新显示）
    show();
}

MainWindow::~MainWindow()
{
    delete ui;
}

/*
 * 1.点击后开始检测txt文件内部数据，把数据显示在表格中（定义读取数据函数，显示数据函数）
 * 2.合格表格变绿，不合格变红（定义检测数据函数）
 * 3.数据有不合格的情况，计入log文件（定义log输出函数）
 */

void MainWindow::InitializeTableSettings()
{
    // 设置表格的大小策略
    ui->tableWidget->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);

    // 设置列宽自适应表格大小
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    // ui->tableWidget->setColumnWidth(0, 400);
    // ui->tableWidget->setColumnWidth(1, 100);

    // 设置表格全局属性
    ui->tableWidget->setStyleSheet(
        "QHeaderView::section {"
        "    background-color: lightgray;"       // 表头背景色
        "    qproperty-alignment: AlignCenter;"  // 表头文本居中
        "}"
        );

    //设置表格行列，填充固定值
    ui->tableWidget->setColumnCount(2);
    ui->tableWidget->setHorizontalHeaderLabels({"合格范围", "测试结果"});
    ui->tableWidget->setRowCount(16);
    ui->tableWidget->setVerticalHeaderLabels({
                                              "进入会话",
                                              "检测软件版本号",
                                              "检测硬件版本号",
                                              "母线电压",
                                              "母线电流",
                                              "6vADC电压（程控电源）",
                                              "6vADC电流（程控电源）",
                                              "9vADC电压（程控电源）",
                                              "9vADC电流（程控电源）",
                                              "12vADC电压（程控电源）",
                                              "12vADC电流（程控电源）",
                                              "历史故障码",
                                              "当前故障码",
                                              "重启检测",
                                              "AOI车辆识别",
                                              "静态电流（0~100uA万用表)"});

    //设置范围值
    //进入会话
    QTableWidgetItem *chat = new QTableWidgetItem("OK");         	// 新建一个项
    chat->setFlags(chat->flags() & (~Qt::ItemIsEditable));    	// 设置可选不可改
    ui->tableWidget->setItem(0, 0, chat);

    //检测软件版本号
    QTableWidgetItem *softwareversion = new QTableWidgetItem("S1.0.0");         	// 新建一个项
    softwareversion->setFlags(softwareversion->flags() & (~Qt::ItemIsEditable));    	// 设置可选不可改
    ui->tableWidget->setItem(1, 0, softwareversion);

    //检测硬件版本号
    QTableWidgetItem *hardwareversion = new QTableWidgetItem("H1.0.0");         	// 新建一个项
    hardwareversion->setFlags(hardwareversion->flags() & (~Qt::ItemIsEditable));    	// 设置可选不可改
    ui->tableWidget->setItem(2, 0, hardwareversion);

    //母线电压
    QTableWidgetItem *busvoltage = new QTableWidgetItem("[33v~39v(±10%)]");         	// 新建一个项
    busvoltage->setFlags(busvoltage->flags() & (~Qt::ItemIsEditable));    	// 设置可选不可改
    ui->tableWidget->setItem(3, 0, busvoltage);

    //母线电流
    QTableWidgetItem *buscurrent = new QTableWidgetItem("[0.9~1.1A]");         	// 新建一个项
    buscurrent->setFlags(buscurrent->flags() & (~Qt::ItemIsEditable));    	// 设置可选不可改
    ui->tableWidget->setItem(4, 0, buscurrent);

    //6vADC电压（程控电源）ADCvoltage6v
    QTableWidgetItem *ADCvoltage6v = new QTableWidgetItem("[5.7V~6.3V(±5%)]");         	// 新建一个项
    ADCvoltage6v->setFlags(ADCvoltage6v->flags() & (~Qt::ItemIsEditable));    	// 设置可选不可改
    ui->tableWidget->setItem(5, 0, ADCvoltage6v);

    //6vADC电流（程控电源）ADCcurrent6v
    QTableWidgetItem *ADCcurrent6v = new QTableWidgetItem("[95mA~105mA(±5%)]");         	// 新建一个项
    ADCcurrent6v->setFlags(ADCcurrent6v->flags() & (~Qt::ItemIsEditable));    	// 设置可选不可改
    ui->tableWidget->setItem(6, 0, ADCcurrent6v);

    //9vADC电压（程控电源）ADCvoltage9v
    QTableWidgetItem *ADCvoltage9v = new QTableWidgetItem("[8.55V~9.45V(±5%)]");         	// 新建一个项
    ADCvoltage9v->setFlags(ADCvoltage9v->flags() & (~Qt::ItemIsEditable));    	// 设置可选不可改
    ui->tableWidget->setItem(7, 0, ADCvoltage9v);

    //9vADC电流（程控电源）ADCcurrent9v
    QTableWidgetItem *ADCcurrent9v = new QTableWidgetItem("[142.5mA~157.5mA(±5%)]");         	// 新建一个项
    ADCcurrent9v->setFlags(ADCcurrent9v->flags() & (~Qt::ItemIsEditable));    	// 设置可选不可改
    ui->tableWidget->setItem(8, 0, ADCcurrent9v);

    //12vADC电压（程控电源）ADCvoltage12v
    QTableWidgetItem *ADCvoltage12v = new QTableWidgetItem("[11.4V~12.6V(±5%)]");         	// 新建一个项
    ADCvoltage12v->setFlags(ADCvoltage12v->flags() & (~Qt::ItemIsEditable));    	// 设置可选不可改
    ui->tableWidget->setItem(9, 0, ADCvoltage12v);

    //12vADC电流（程控电源）ADCcurrent12v
    QTableWidgetItem *ADCcurrent12v = new QTableWidgetItem("[190mA~210mA(±5%)]");         	// 新建一个项
    ADCcurrent12v->setFlags(ADCcurrent12v->flags() & (~Qt::ItemIsEditable));    	// 设置可选不可改
    ui->tableWidget->setItem(10, 0, ADCcurrent12v);

    // 历史故障码 historicalfaultcodes
    QTableWidgetItem *historicalfaultcodes = new QTableWidgetItem("NULL");         	// 新建一个项
    historicalfaultcodes->setFlags(historicalfaultcodes->flags() & (~Qt::ItemIsEditable));    	// 设置可选不可改
    ui->tableWidget->setItem(11, 0, historicalfaultcodes);

    // 当前故障码 currentfaultcode
    QTableWidgetItem *currentfaultcode = new QTableWidgetItem("NULL");         	// 新建一个项
    currentfaultcode->setFlags(currentfaultcode->flags() & (~Qt::ItemIsEditable));    	// 设置可选不可改
    ui->tableWidget->setItem(12, 0, currentfaultcode);

    // 重启检测 restartdetection
    QTableWidgetItem *restartdetection = new QTableWidgetItem("[0,1]");         	// 新建一个项
    restartdetection->setFlags(restartdetection->flags() & (~Qt::ItemIsEditable));    	// 设置可选不可改
    ui->tableWidget->setItem(13, 0, restartdetection);

    // AOI车辆识别 AOIvehicleidentification
    QTableWidgetItem *AOIvehicleidentification = new QTableWidgetItem("[0,1]");         	// 新建一个项
    AOIvehicleidentification->setFlags(AOIvehicleidentification->flags() & (~Qt::ItemIsEditable));    	// 设置可选不可改
    ui->tableWidget->setItem(14, 0, AOIvehicleidentification);

    // 静态电流（0~100uA万用表) staticcurrent
    QTableWidgetItem *staticcurrent = new QTableWidgetItem("[0.8uA~1.0uA]");         	// 新建一个项
    staticcurrent->setFlags(staticcurrent->flags() & (~Qt::ItemIsEditable));    	// 设置可选不可改
    ui->tableWidget->setItem(15, 0, staticcurrent);

}

//txt数据转存QList
QList<QStringList> MainWindow::LoadDataFromFile(const QString &FilePath)
{
    QList<QStringList> ParsedData;

    QFile DataFile(FilePath);
    if (!DataFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return ParsedData;
    }

    QTextStream DataStream(&DataFile);
    while (!DataStream.atEnd()) {
        const QString CurrentLine = DataStream.readLine().trimmed();
        if(!CurrentLine.isEmpty()) {
            ParsedData << CurrentLine.split(';');  // 修改分隔符
        }
    }

    DataFile.close();
    return ParsedData;
}

//QList数据填充到qtablewidget
void MainWindow::PopulateTable(const QList<QStringList> &TableData)
{
    for(int RowIndex = 0; RowIndex < TableData.size(); ++RowIndex) {
        const QStringList &CurrentRow = TableData[RowIndex];

        // 填充数据列（从第二列开始）
        for(int SourceCol = 0; SourceCol < CurrentRow.size(); ++SourceCol) {
            const int TargetCol = SourceCol + 1; // 数据列偏移

            // 创建单元格并设置值
            QTableWidgetItem *NewItem = new QTableWidgetItem(CurrentRow[SourceCol]);

            // 关键修改：第二个参数传递行号
            const bool IsValid = ValidateCellContent(CurrentRow[SourceCol], RowIndex);

            // 设置样式
            NewItem->setBackground(IsValid ? Qt::green : Qt::red);
            NewItem->setForeground(IsValid ? Qt::black : Qt::white);
            NewItem->setTextAlignment(Qt::AlignCenter);

            // 设置标记
            NewItem->setData(Qt::UserRole, IsValid); // 存储验证状态

            // 权限控制
            NewItem->setFlags(NewItem->flags() & ~Qt::ItemIsEditable);

            // 填充到目标列
            ui->tableWidget->setItem(RowIndex, TargetCol, NewItem);
        }
    }

    // 自动调整列宽（可选）
    // ui->DatatableWidget->resizeColumnsToContents();
}

//检验数据合法性
bool MainWindow::ValidateCellContent(const QString &CellValue, int ColumnIndex)
{
    switch(ColumnIndex) { 
    // 进入会话
    case 0:
        return !CellValue.trimmed().isEmpty();

    // 检测软件版本号
    case 1:
        return (CellValue=="S1.0.0");

    // 检测硬件版本号
    case 2:
        return (CellValue=="H1.0.0");

    // 母线电压
    case 3:
    {
        bool isNumber;
        double value = CellValue.toDouble(&isNumber);
        return isNumber && (value >= 0.0 && value <= 100.0);
    }

    // 母线电流
    case 4:
    {
        bool isNumber;
        double value = CellValue.toDouble(&isNumber);
        return isNumber && (value >= 0.0 && value <= 100.0);
    }

    // 6vADC电压（程控电源）
    case 5:
    {
        bool isNumber;
        double value = CellValue.toDouble(&isNumber);
        return isNumber && (value >= 0.0 && value <= 100.0);
    }
    // 6vADC电流（程控电源）
    case 6:
    {
        bool isNumber;
        double value = CellValue.toDouble(&isNumber);
        return isNumber && (value >= 0.0 && value <= 100.0);
    }

    // 9vADC电压（程控电源）
    case 7:
    {
        bool isNumber;
        double value = CellValue.toDouble(&isNumber);
        return isNumber && (value >= 0.0 && value <= 100.0);
    }

    // 9vADC电流（程控电源）
    case 8:
    {
        bool isNumber;
        double value = CellValue.toDouble(&isNumber);
        return isNumber && (value >= 0.0 && value <= 100.0);
    }

    // 12vADC电压（程控电源）
    case 9:
    {
        bool isNumber;
        double value = CellValue.toDouble(&isNumber);
        return isNumber && (value >= 0.0 && value <= 100.0);
    }

    // 12vADC电流（程控电源）
    case 10:
    {
        bool isNumber;
        double value = CellValue.toDouble(&isNumber);
        return isNumber && (value >= 0.0 && value <= 100.0);
    }

    // 历史故障码
    case 11:
    {
        bool isNumber;
        double value = CellValue.toDouble(&isNumber);
        return isNumber && (value >= 0.0 && value <= 100.0);
    }

    // 当前故障码
    case 12:
    {
        bool isNumber;
        double value = CellValue.toDouble(&isNumber);
        return isNumber && (value >= 0.0 && value <= 100.0);
    }

    // 重启检测
    case 13:
    {
        bool isNumber;
        double value = CellValue.toDouble(&isNumber);
        return isNumber && (value >= 0.0 && value <= 100.0);
    }

    // AOI车辆识别
    case 14:
    {
        bool isNumber;
        double value = CellValue.toDouble(&isNumber);
        return isNumber && (value >= 0.0 && value <= 100.0);
    }

    // 静态电流（0~100uA万用表)
    case 15:
    {
        bool isNumber;
        double value = CellValue.toDouble(&isNumber);
        return isNumber && (value >= 0.0 && value <= 100.0);
    }

    default:
        return false;
    }
}

//历史故障码填充
void MainWindow::PopulateHistoricalDTCs(bool &IsValid,const QString &HistoricalDTCsData)
{
    QString value = HistoricalDTCsData;
    // if (!IsValid){
    //     value = NULL;
    //     return;
    // }

    // 创建单元格并设置值
    QTableWidgetItem *NewItem = new QTableWidgetItem(value);
    NewItem->setBackground(IsValid ? Qt::green : Qt::red);
    NewItem->setForeground(IsValid ? Qt::black : Qt::white);
    NewItem->setTextAlignment(Qt::AlignCenter);
    // 设置标记
    NewItem->setData(Qt::UserRole, IsValid); // 存储验证状态

    // 权限控制
    NewItem->setFlags(NewItem->flags() & ~Qt::ItemIsEditable);

    // 填充到目标列
    ui->tableWidget->setItem(12,1, NewItem);
}

//当前故障码填充
void MainWindow::PopulateActiveDTCs(bool &IsValid,const QString &ActiveDTCsData )
{

    QString value = ActiveDTCsData;
    // if (!IsValid){
    //     value = NULL;
    //     return;
    // }

    // 创建单元格并设置值
    QTableWidgetItem *NewItem = new QTableWidgetItem(value);
    NewItem->setBackground(IsValid ? Qt::green : Qt::red);
    NewItem->setForeground(IsValid ? Qt::black : Qt::white);
    NewItem->setTextAlignment(Qt::AlignCenter);
    // 设置标记
    NewItem->setData(Qt::UserRole, IsValid); // 存储验证状态

    // 权限控制
    NewItem->setFlags(NewItem->flags() & ~Qt::ItemIsEditable);

    // 填充到目标列
    ui->tableWidget->setItem(12,1, NewItem);
}

//重启检测
void MainWindow::CheckSystemReboot(bool &SystemReboot)
{
    bool IsValid = (SystemReboot)?true:false;
    QString value = (SystemReboot)?"0":"1";
    // 创建单元格并设置值
    QTableWidgetItem *NewItem = new QTableWidgetItem(value);
    NewItem->setBackground(IsValid ? Qt::green : Qt::red);
    NewItem->setForeground(IsValid ? Qt::black : Qt::white);
    NewItem->setTextAlignment(Qt::AlignCenter);
    // 设置标记
    NewItem->setData(Qt::UserRole, IsValid); // 存储验证状态

    // 权限控制
    NewItem->setFlags(NewItem->flags() & ~Qt::ItemIsEditable);

    // 填充到目标列
    ui->tableWidget->setItem(13,1, NewItem);
}

//AOI车辆识别
void MainWindow::IdentifyVehicleByAOI(bool &VehicleByAOI)
{
    bool IsValid = (VehicleByAOI)?true:false;
    QString value = (VehicleByAOI)?"0":"1";
    // 创建单元格并设置值
    QTableWidgetItem *NewItem = new QTableWidgetItem(value);
    NewItem->setBackground(IsValid ? Qt::green : Qt::red);
    NewItem->setForeground(IsValid ? Qt::black : Qt::white);
    NewItem->setTextAlignment(Qt::AlignCenter);
    // 设置标记
    NewItem->setData(Qt::UserRole, IsValid); // 存储验证状态

    // 权限控制
    NewItem->setFlags(NewItem->flags() & ~Qt::ItemIsEditable);

    // 填充到目标列
    ui->tableWidget->setItem(14,1, NewItem);
}

//静态电流填充
void MainWindow::PopulateQuiescentCurrent(float &QuiescentCurrent)
{
    // 创建单元格并设置值
    QTableWidgetItem *NewItem = new QTableWidgetItem(QString::number(QuiescentCurrent, 'f', 2));
    bool IsValid = (0.8<QuiescentCurrent && QuiescentCurrent<1.0)?true:false;

    NewItem->setBackground(IsValid ? Qt::green : Qt::red);
    NewItem->setForeground(IsValid ? Qt::black : Qt::white);
    NewItem->setTextAlignment(Qt::AlignCenter);
    // 设置标记
    NewItem->setData(Qt::UserRole, IsValid); // 存储验证状态

    // 权限控制
    NewItem->setFlags(NewItem->flags() & ~Qt::ItemIsEditable);

    // 填充到目标列
    ui->tableWidget->setItem(15,1, NewItem);


}

// 日志输出
void MainWindow::WriteLog(const QString &LogfilePath,const QString &UseTime)
{
    GetLogFilePath(LogfilePath);
    ui-> textBrowser ->clear ();
    ui->textBrowser->insertPlainText(LogfilePath+"\n");
    ui->textBrowser->insertPlainText(UseTime+"ms");
}

void MainWindow::on_BeginpushButton_clicked()
{
    DWORD start = GetTickCount();


    // 开始测试
    QString FilePath = "D:\\Projects\\lf\\c_case\\workspace\\FCT\\test\\data.txt";
    QList<QStringList> TableData = LoadDataFromFile(FilePath);
    PopulateTable(TableData);

    //历史故障码
    bool IsValid = true;
    const QString HistoricalDTCsData = "";
    PopulateHistoricalDTCs(IsValid,HistoricalDTCsData);

    //当前故障码
    const QString ActiveDTCsData = "";
    PopulateActiveDTCs(IsValid,ActiveDTCsData );

    //重启检测
    bool SystemReboot = true;
    CheckSystemReboot(SystemReboot);

    //AOI车辆识别
    bool VehicleByAOI = true;
    IdentifyVehicleByAOI(VehicleByAOI);

    //静态电流
    float QuiescentCurrent = 0.9;
    PopulateQuiescentCurrent(QuiescentCurrent);

    // 日志输出
    // 需要计时的代码
    DWORD end = GetTickCount();
    DWORD time = end - start;

    const QString LogfilePath ="D:\\Projects\\lf\\c_case\\workspace\\FCT\\test\\test";
    const QString UseTime = QString::number(time, 'f', 6);
    WriteLog(LogfilePath,UseTime);
}


// 创建日志目录和文件
QString MainWindow::GetLogFilePath(const QString &appDirPath)
{
    // qDebug() << "应用程序目录：" << QCoreApplication::applicationDirPath();
    // 获取应用程序可执行文件所在目录
    // const QString appDirPath = QCoreApplication::applicationDirPath();

    // 构建test目录路径（自动处理路径分隔符）
    const QDir appDir(appDirPath);
    const QString testDirPath = appDir.filePath("test");

    // 创建QDir对象并验证目录
    QDir testDir(testDirPath);
    if (!testDir.exists()) {
        // 递归创建目录（包括所有父级目录）
        if (!testDir.mkpath(".")) {
            QMessageBox::critical(
                this,
                tr("目录创建失败"),
                tr("无法创建日志目录：\n%1\n请检查文件系统权限。").arg(testDirPath)
                );
            return QString(); // 返回空路径表示失败
        }
    }
    qDebug() << "最终日志路径：" << testDir.filePath("test.log");
    // 构建完整日志文件路径
    return testDir.filePath("test.log");
}

//获取出错的数据
bool MainWindow::IsRedColor(const QColor& color) {
    return color.red() > 200 &&
           color.green() < 50 &&
           color.blue() < 50;
}


// 在 mainwindow.cpp 中实现
void MainWindow::closeEvent(QCloseEvent *event)
{
    QMessageBox::StandardButton resBtn = QMessageBox::question(
        this, "退出", "确定要退出吗？",
        QMessageBox::Cancel | QMessageBox::Yes,
        QMessageBox::Cancel
        );

    if (resBtn == QMessageBox::Yes) {
        event->accept();
    } else {
        event->ignore(); // 忽略关闭事件
    }
}

