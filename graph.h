#pragma once

#include <QQuickPaintedItem>

class Graph : public QQuickPaintedItem
{
    Q_OBJECT
    QML_ELEMENT
public:
    Graph();
    ~Graph();

    void paint(QPainter* painter) override;

    static void registerType();
};

