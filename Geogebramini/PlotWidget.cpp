// PlotWidget.cpp
#include "PlotWidget.h"
#include <QPainter>
#include <QFontMetrics>
#include <cmath>
#include <QMouseEvent>


PlotWidget::PlotWidget(QWidget *parent) : QWidget(parent) {
    setMouseTracking(true);
}

void PlotWidget::addFunction(std::unique_ptr<Function> func) {
    functions.push_back(std::move(func));
    update();
}

QPointF PlotWidget::toScreen(const QPointF &logic) const {
    return QPointF(
        width()/2.0 + offset.x() + logic.x() * zoom,
        height()/2.0 + offset.y() - logic.y() * zoom
        );
}

QPointF PlotWidget::toLogic(const QPointF &screen) const {
    return QPointF(
        (screen.x() - width()/2.0 - offset.x()) / zoom,
        (height()/2.0 + offset.y() - screen.y()) / zoom
        );
}

void PlotWidget::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.fillRect(rect(), Qt::white);

    painter.setRenderHint(QPainter::Antialiasing);

    if(showGrid) drawGrid(painter);
    if(showAxes) drawAxes(painter);
    drawLabels(painter);

    // Draw functions
    for(const auto& func : functions) {
        if(!func->visible) continue;

        QPainterPath path = func->createPath(
            toLogic(QPointF(0, 0)).x(),
            toLogic(QPointF(width(), 0)).x(),
            0.1f
            );

        QTransform transform;
        transform.translate(width()/2.0 + offset.x(), height()/2.0 + offset.y());
        transform.scale(zoom, -zoom);

        // 修改这里：设置细线宽
        QPen curvePen(func->color);
        curvePen.setWidthF(0.5);  // 设置线宽为0.5逻辑单位
        curvePen.setCosmetic(true); // 保持屏幕像素宽度不受缩放影响

        painter.setPen(curvePen);
        painter.setTransform(transform);
        painter.drawPath(path);
    }
}

void PlotWidget::drawGrid(QPainter &painter) {
    const float logicWidth = width() / zoom;
    const float logicHeight = height() / zoom;

    QPointF center = toLogic(rect().center());
    float left = center.x() - logicWidth/2;
    float right = center.x() + logicWidth/2;
    float bottom = center.y() - logicHeight/2;
    float top = center.y() + logicHeight/2;

    // Minor grid
    painter.setPen(QPen(gridSettings.gridColor, 0.5));
    for(float x = std::floor(left/gridSettings.minorStep)*gridSettings.minorStep;
         x <= right; x += gridSettings.minorStep) {
        QPointF p1 = toScreen(QPointF(x, bottom));
        QPointF p2 = toScreen(QPointF(x, top));
        painter.drawLine(p1, p2);
    }

    // Major grid
    painter.setPen(QPen(gridSettings.gridColor, 1));
    for(float x = std::floor(left/gridSettings.majorStep)*gridSettings.majorStep;
         x <= right; x += gridSettings.majorStep) {
        QPointF p1 = toScreen(QPointF(x, bottom));
        QPointF p2 = toScreen(QPointF(x, top));
        painter.drawLine(p1, p2);
    }
}

void PlotWidget::drawAxes(QPainter &painter) {
    painter.setPen(QPen(gridSettings.axisColor, 2));

    // X axis
    QPointF xStart = toScreen(QPointF(toLogic(QPointF(0, height()/2)).x(), 0));
    QPointF xEnd = toScreen(QPointF(toLogic(QPointF(width(), height()/2)).x(), 0));
    painter.drawLine(xStart, xEnd);

    // Y axis
    QPointF yStart = toScreen(QPointF(0, toLogic(QPointF(width()/2, 0)).y()));
    QPointF yEnd = toScreen(QPointF(0, toLogic(QPointF(width()/2, height())).y()));
    painter.drawLine(yStart, yEnd);
}

void PlotWidget::drawLabels(QPainter &painter) {
    // Implement axis labels and scale markers
}

// 鼠标交互实现
void PlotWidget::mousePressEvent(QMouseEvent *event) {
    lastMousePos = event->pos();
}

void PlotWidget::mouseMoveEvent(QMouseEvent *event) {
    if(event->buttons() & Qt::LeftButton) {
        QPoint delta = event->pos() - lastMousePos;
        offset += delta;
        lastMousePos = event->pos();
        update();
    }
}

void PlotWidget::wheelEvent(QWheelEvent *event) {
    QPointF mouseLogicBefore = toLogic(event->position());
    float zoomFactor = event->angleDelta().y() > 0 ? 1.1f : 1/1.1f;
    zoom *= zoomFactor;
    QPointF mouseLogicAfter = toLogic(event->position());
    offset += (mouseLogicBefore - mouseLogicAfter) * zoom;
    update();
}

// PlotWidget.cpp
void PlotWidget::resizeEvent(QResizeEvent* event) {
    // 计算窗口尺寸变化量
    if (event->oldSize().isValid()) {
        const QSize oldSize = event->oldSize();
        const QSize newSize = event->size();

        // 计算尺寸变化量（考虑可能的双向变化）
        const int deltaWidth = newSize.width() - oldSize.width();
        const int deltaHeight = newSize.height() - oldSize.height();

        // 调整偏移量以保持内容在视图中的相对位置
        offset.rx() -= deltaWidth / 2;  // 补偿水平中心变化
        offset.ry() -= deltaHeight / 2; // 补偿垂直中心变化

        // 限制最小偏移量（可选）
        offset.setX(qMax(-width()/2, qMin(static_cast<int>(offset.x()), width()/2)));
        offset.setY(qMax(-height()/2, qMin(static_cast<int>(offset.y()), height()/2)));
    }

    // 保持默认行为
    QWidget::resizeEvent(event);

    // 立即重绘（可选）
    update();
}
