#include "mainwindow.h"
#include "ui_mainwindow.h"

// MainWindow.cpp
#include "MainWindow.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include <QMessageBox>
#include <QListWidgetItem>
#include <QFormLayout>
#include <QDockWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QMouseEvent>
#include <QComboBox>
#include <QWheelEvent>
#include <QResizeEvent>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    plotWidget = new PlotWidget;
    setCentralWidget(plotWidget);

    createControls();
}

void MainWindow::createControls() {
    QDockWidget *dock = new QDockWidget("Functions", this);
    addDockWidget(Qt::RightDockWidgetArea, dock);

    QWidget *controls = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout;

    // Function type selection
    functionTypeCombo = new QComboBox;
    functionTypeCombo->addItems({"Linear", "Quadratic", "Logarithmic"});
    connect(functionTypeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::onFunctionTypeChanged);

    // Parameter inputs
    QGroupBox *paramGroup = new QGroupBox("Parameters");
    paramGroup->setLayout(new QVBoxLayout);
    paramGroup->layout()->addWidget(createParamInputs());

    // Buttons
    addButton = new QPushButton("Add Function");
    connect(addButton, &QPushButton::clicked, this, &MainWindow::onAddFunction);

    removeButton = new QPushButton("Remove Selected");
    connect(removeButton, &QPushButton::clicked, this, &MainWindow::onRemoveFunction);

    // Function list
    functionList = new QListWidget;
    connect(functionList, &QListWidget::itemSelectionChanged,
            [this](){ removeButton->setEnabled(functionList->currentRow() != -1); });

    layout->addWidget(new QLabel("Function Type:"));
    layout->addWidget(functionTypeCombo);
    layout->addWidget(paramGroup);
    layout->addWidget(addButton);
    layout->addWidget(removeButton);
    layout->addWidget(new QLabel("Added Functions:"));
    layout->addWidget(functionList);

    controls->setLayout(layout);
    dock->setWidget(controls);
}

QWidget* MainWindow::createParamInputs() {
    QWidget *widget = new QWidget;
    QFormLayout *layout = new QFormLayout;

    paramEdits.clear();
    for(int i=0; i<3; ++i) {
        QLineEdit *edit = new QLineEdit;
        edit->setPlaceholderText("0");
        paramEdits.append(edit);
        layout->addRow(QString("Parameter %1:").arg(i+1), edit);
    }

    widget->setLayout(layout);
    return widget;
}

void MainWindow::onFunctionTypeChanged(int index) {
    // Update parameter labels based on function type
    QFormLayout *layout = qobject_cast<QFormLayout*>(paramEdits[0]->parentWidget()->layout());

    QStringList labels;
    switch(index) {
    case 0: // Linear
        labels = {"Slope (a)", "Intercept (b)"};
        break;
    case 1: // Quadratic
        labels = {"a (x²)", "b (x)", "c (const)"};
        break;
    case 2: // Logarithmic
        labels = {"Base", "Coefficient", "Vertical Shift"};
        break;
    }

    for(int i=0; i<labels.size(); ++i) {
        layout->itemAt(i, QFormLayout::LabelRole)->widget()->setVisible(true);
        paramEdits[i]->setVisible(true);
        qobject_cast<QLabel*>(layout->itemAt(i, QFormLayout::LabelRole)->widget())->setText(labels[i]);
    }
    for(int i=labels.size(); i<3; ++i) {
        layout->itemAt(i, QFormLayout::LabelRole)->widget()->setVisible(false);
        paramEdits[i]->setVisible(false);
    }
}

void MainWindow::onAddFunction() {
    try {
        std::unique_ptr<Function> func;
        int typeIndex = functionTypeCombo->currentIndex();

        switch(typeIndex) {
        case 0: {
            auto f = std::make_unique<LinearFunction>();
            f->a = paramEdits[0]->text().toFloat();
            f->b = paramEdits[1]->text().toFloat();
            func = std::move(f);
            break;
        }
        case 1: {
            auto f = std::make_unique<QuadraticFunction>();
            f->a = paramEdits[0]->text().toFloat();
            f->b = paramEdits[1]->text().toFloat();
            f->c = paramEdits[2]->text().toFloat();
            func = std::move(f);
            break;
        }
        case 2: {
            auto f = std::make_unique<LogFunction>();
            f->base = paramEdits[0]->text().toFloat();
            f->coeff = paramEdits[1]->text().toFloat();
            f->verticalShift = paramEdits[2]->text().toFloat();
            func = std::move(f);
            break;
        }
        }

        plotWidget->addFunction(std::move(func));
        updateFunctionList();
    }
    catch(...) {
        QMessageBox::warning(this, "Input Error", "Invalid parameter values");
    }
}

void MainWindow::onRemoveFunction() {
    int row = functionList->currentRow();
    if(row >= 0 && row < plotWidget->functions.size()) {
        plotWidget->functions.erase(plotWidget->functions.begin() + row);
        updateFunctionList();
        plotWidget->update();
    }
}

void MainWindow::updateFunctionList() {
    functionList->clear();
    for(const auto& func : plotWidget->functions) {
        QListWidgetItem *item = new QListWidgetItem(
            QString("%1: %2").arg(func->type()).arg(func->expression()));
        item->setForeground(func->color);
        functionList->addItem(item);
    }
}
