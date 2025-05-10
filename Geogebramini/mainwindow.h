#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include "PlotWidget.h"

QT_BEGIN_NAMESPACE
class QComboBox;
class QLineEdit;
class QPushButton;
class QListWidget;
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);

private slots:
    void onFunctionTypeChanged(int index);
    void onAddFunction();
    void onRemoveFunction();
    void updateFunctionList();

private:
    void createControls();
    QWidget* createParamInputs();

    PlotWidget *plotWidget;
    QComboBox *functionTypeCombo;
    QListWidget *functionList;
    QVector<QLineEdit*> paramEdits;
    QPushButton *addButton;
    QPushButton *removeButton;
};
#endif // MAINWINDOW_H
