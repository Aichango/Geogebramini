#ifndef PLOTWIDGET_H
#define PLOTWIDGET_H
// PlotWidget.h
#pragma once
#include <QWidget>
#include <vector>
#include "Function.h"

class PlotWidget : public QWidget {
    friend class MainWindow;
    Q_OBJECT
public:
    explicit PlotWidget(QWidget *parent = nullptr);

    void addFunction(std::unique_ptr<Function> func);
    void clearFunctions() { functions.clear(); update(); }

    void setGridVisible(bool visible) { showGrid = visible; update(); }
    void setAxesVisible(bool visible) { showAxes = visible; update(); }

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    struct GridSettings {
        float majorStep = 1.0f;
        float minorStep = 0.2f;
        QColor axisColor = Qt::black;
        QColor gridColor = Qt::lightGray;
    } gridSettings;

    std::vector<std::unique_ptr<Function>> functions;
    QPointF offset;
    float zoom = 50.0f;
    bool showGrid = true;
    bool showAxes = true;
    QPoint lastMousePos;

    QPointF toScreen(const QPointF &logic) const;
    QPointF toLogic(const QPointF &screen) const;
    void drawGrid(QPainter &painter);
    void drawAxes(QPainter &painter);
    void drawLabels(QPainter &painter);
};
#endif // PLOTWIDGET_H
