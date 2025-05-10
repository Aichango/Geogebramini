#ifndef FUNCTION_H
#define FUNCTION_H
// Function.h
#pragma once
#include <QPainterPath>
#include <QColor>
#include <memory>

class Function {
public:
    QColor color = Qt::blue;
    bool visible = true;

    virtual ~Function() = default;
    virtual QPainterPath createPath(float xMin, float xMax, float step) const = 0;
    virtual QString expression() const = 0;
    virtual QString type() const = 0;
};

class LinearFunction : public Function {
public:
    float a = 1.0f, b = 0.0f;

    QPainterPath createPath(float xMin, float xMax, float step) const override;
    QString expression() const override { return QString("y = %1x + %2").arg(a).arg(b); }
    QString type() const override { return "Linear"; }
};

class QuadraticFunction : public Function {
public:
    float a = 1.0f, b = 0.0f, c = 0.0f;

    QPainterPath createPath(float xMin, float xMax, float step) const override;
    QString expression() const override { return QString("y = %1x² + %2x + %3").arg(a).arg(b).arg(c); }
    QString type() const override { return "Quadratic"; }
};

class LogFunction : public Function {
public:
    float base = 10.0f, coeff = 1.0f, verticalShift = 0.0f;

    QPainterPath createPath(float xMin, float xMax, float step) const override;
    QString expression() const override {
        return QString("y = %1log%2(x) + %3").arg(coeff).arg(base).arg(verticalShift);
    }
    QString type() const override { return "Logarithmic"; }
};
#endif // FUNCTION_H
