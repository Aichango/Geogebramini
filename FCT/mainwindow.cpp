#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QWindow> // 用于窗口操作
#include <direct.h>
#include <filesystem>
#include <windows.h>
#include <io.h>
#include <direct.h>
#include <string>
#include <iostream>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 设置表格的大小策略
    ui->DatatableWidget->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);

    // 设置列宽自适应表格大小
    ui->DatatableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

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

void MainWindow::on_BeginpushButton_clicked()
{
    // 开始测试
    chat();
    CheckSoftwareVersion();
    CheckHardwareVersion();
    CheckBusVoltage();
    CheckBusCurrent();
    CheckTermVoltage();
    CheckTermCurrent();

}

// 进入会话模式
void MainWindow::chat()
{
    // Sleep(1000);
    QTableWidgetItem *pItem = new QTableWidgetItem("OK");
    pItem->setBackground(QBrush(Qt::green));        // 设置背景色
    ui->DatatableWidget->setItem(0,1,pItem);
};


// 检测软件版本号
void MainWindow::CheckSoftwareVersion()
{
    // Sleep(1000);
    QTableWidgetItem *pItem = new QTableWidgetItem("S1.0.0");
    pItem->setBackground(QBrush(Qt::green));        // 设置背景色
    ui->DatatableWidget->setItem(1,1,pItem);
};

// 检测硬件版本号
void MainWindow::CheckHardwareVersion()
{
    // Sleep(1000);
    QTableWidgetItem *pItem = new QTableWidgetItem("H1.0.0");
    pItem->setBackground(QBrush(Qt::green));        // 设置背景色
    ui->DatatableWidget->setItem(2,1,pItem);
};

//检测母线电压
void MainWindow::CheckBusVoltage()
{
    int BusVoltage=3;
    QString str = QString::number(BusVoltage);
    QTableWidgetItem *pItem = new QTableWidgetItem(str);
    if (3.3<BusVoltage && BusVoltage<5){
        pItem->setBackground(QBrush(Qt::green));
    }
    else{
        pItem->setBackground(QBrush(Qt::red));
    }    // 设置背景色
    ui->DatatableWidget->setItem(3,1,pItem);
}


//检测母线电流
void MainWindow::CheckBusCurrent()
{
    float BusCurrent=0.6;
    QString str = QString::number(BusCurrent);
    QTableWidgetItem *pItem = new QTableWidgetItem(str);
    if (1.2<BusCurrent && BusCurrent<1.8){
        pItem->setBackground(QBrush(Qt::green));
    }
    else{
        pItem->setBackground(QBrush(Qt::red));
    }    // 设置背景色
    ui->DatatableWidget->setItem(4,1,pItem);
}

//检测项电压
void MainWindow::CheckTermVoltage()
{
    float TermVoltage=2.2;
    QString str = QString::number(TermVoltage);
    QTableWidgetItem *pItem = new QTableWidgetItem(str);
    if (1.0<TermVoltage && TermVoltage<3.3){
        pItem->setBackground(QBrush(Qt::green));
    }
    else{
        pItem->setBackground(QBrush(Qt::red));
    }    // 设置背景色
    ui->DatatableWidget->setItem(5,1,pItem);
}

//检测项电流
void MainWindow::CheckTermCurrent()
{
    float TermCurrent=0.9;
    QString str = QString::number(TermCurrent);
    QTableWidgetItem *pItem = new QTableWidgetItem(str);
    if (0.8<TermCurrent && TermCurrent<1){
        pItem->setBackground(QBrush(Qt::green));
    }
    else{
        pItem->setBackground(QBrush(Qt::red));
    }    // 设置背景色
    ui->DatatableWidget->setItem(6,1,pItem);
}

/*
 * 1.点击后结束检测，表格显示最终结果（1.0单个文件不需要额外处理）
 * 2.结束log输出（定义log文件保存函数）
 */

// 创建日志目录和文件
QString MainWindow::GetLogFilePath()
{
    qDebug() << "应用程序目录：" << QCoreApplication::applicationDirPath();
    // 获取应用程序可执行文件所在目录
    const QString appDirPath = QCoreApplication::applicationDirPath();

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

//保存出错的日志
void MainWindow::on_FinishpushButton_clicked()
{
    qDebug() << "点击按钮时路径：" << GetLogFilePath();
    const QString fullPath = GetLogFilePath();
    if (fullPath.isEmpty()) return;

    QFile file(fullPath);
    if (!file.open(QIODevice::Append | QIODevice::Text)) {
        QMessageBox::critical(
            this,
            "错误",
            QString("文件创建失败：\n%1\n错误类型：%2").arg(fullPath).arg(file.errorString())
            );
        return;
    }

    QTextStream out(&file);
    #if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
            out.setCodec("UTF-8");
    #else
            out.setEncoding(QStringConverter::Utf8);
    #endif

    const int rowCount = ui->DatatableWidget->rowCount();
    const int colCount = ui->DatatableWidget->columnCount();

    bool hasError = false;
    for (int row = 0; row < rowCount; ++row) {
        QString headerText = ui->DatatableWidget->verticalHeaderItem(row)->text(); // 获取第1列的表头内容

        QTableWidgetItem* headerItem = ui->DatatableWidget->item(row, 0);
        QString headerrange = headerItem ? headerItem->text() : "未知项目";

        for (int col = 0; col < colCount; ++col) {
            if (QTableWidgetItem* item = ui->DatatableWidget->item(row, col)) {
                const QColor bgColor = item->background().color();

                if (IsRedColor(bgColor)) {
                    const QString entry = QString("error:[%1,%2],\"%3\",\"%4\",\"%5\"\n")
                    .arg(row + 1)
                    .arg(col + 1 )
                    .arg(headerText)
                    .arg(headerrange)
                    .arg(item->text());

                    out << entry;
                    hasError = true;
                }
            }
        }
    }

    file.close();
    QMessageBox::information(
        this,
        "操作完成",
        hasError ? "已记录错误信息" : "未发现错误单元格"
        );
}

void MainWindow::CopyLogFileToSelectedDirectory()
{
    // 1. 选择目标文件夹
    QString targetDir = QFileDialog::getExistingDirectory(
        this,
        tr("选择保存目录"),
        QDir::homePath(),
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks
        );

    // 用户取消选择
    if (targetDir.isEmpty()) return;

    // 2. 定义源文件路径（根据实际情况调整路径）
    QString sourcePath = GetLogFilePath();

    // 3. 验证源文件存在
    QFileInfo sourceFileInfo(sourcePath);
    if (!sourceFileInfo.exists() || !sourceFileInfo.isFile()) {
        QMessageBox::critical(
            this,
            tr("错误"),
            tr("源文件不存在：\n%1").arg(sourcePath)
            );
        return;
    }

    // 4. 构建目标路径
    QString targetPath = targetDir + "/" + sourceFileInfo.fileName();

    // 5. 检查目标文件是否已存在
    if (QFileInfo::exists(targetPath)) {
        QMessageBox::StandardButton reply = QMessageBox::question(
            this,
            tr("文件已存在"),
            tr("目标文件已存在，是否覆盖？\n%1").arg(targetPath),
            QMessageBox::Yes | QMessageBox::No
            );

        if (reply == QMessageBox::No) return;

        // 删除已有文件
        if (!QFile::remove(targetPath)) {
            QMessageBox::critical(
                this,
                tr("错误"),
                tr("无法删除旧文件：\n%1").arg(targetPath)
                );
            return;
        }
    }

    // 6. 执行文件复制
    QFile sourceFile(sourcePath);
    if (sourceFile.copy(targetPath)) {
        QMessageBox::information(
            this,
            tr("成功"),
            tr("文件已成功复制到：\n%1").arg(targetPath)
            );
    } else {
        QMessageBox::critical(
            this,
            tr("错误"),
            tr("文件复制失败！\n错误信息：%1").arg(sourceFile.errorString())
            );
    }
    ui->textBrowser->setText(targetPath);
}


/*
 * 1.跳转窗口选择文件路径
 * 2.点击确认后，修改目前显示的文件路径（定义log另存为函数）
 */
void MainWindow::on_BrowerspushButton_clicked()
{
    CopyLogFileToSelectedDirectory();
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

