// Function.cpp
#include "Function.h"
#include <cmath>

QPainterPath LinearFunction::createPath(float xMin, float xMax, float step) const {
    QPainterPath path;
    bool valid = false;

    for(float x = xMin; x <= xMax; x += step) {
        float y = a * x + b;
        if(!valid) {
            path.moveTo(x, y);
            valid = true;
        } else {
            path.lineTo(x, y);
        }
    }
    return path;
}

QPainterPath QuadraticFunction::createPath(float xMin, float xMax, float step) const {
    QPainterPath path;
    bool valid = false;

    for(float x = xMin; x <= xMax; x += step) {
        float y = a * x * x + b * x + c;
        if(!valid) {
            path.moveTo(x, y);
            valid = true;
        } else {
            path.lineTo(x, y);
        }
    }
    return path;
}

QPainterPath LogFunction::createPath(float xMin, float xMax, float step) const {
    QPainterPath path;
    bool valid = false;

    for(float x = xMin; x <= xMax; x += step) {
        if(x <= 0) continue; // Skip invalid domain

        float y = coeff * std::log(x)/std::log(base) + verticalShift;
        if(!valid) {
            path.moveTo(x, y);
            valid = true;
        } else {
            path.lineTo(x, y);
        }
    }
    return path;
}
