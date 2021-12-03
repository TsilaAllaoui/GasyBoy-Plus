#ifndef VRAMVIEWER_H
#define VRAMVIEWER_H

#include <QWidget>
#include <QPainter>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QVector>
#include "defs.h"

class Mmu;

namespace Ui {
class VramViewer;
}

class drawerThread : public QThread
{
    Q_OBJECT

public:
    drawerThread(Mmu *pmmu, QGraphicsRectItem ***p);
    ~drawerThread(){};
    void setAddr(uint16_t value);

private:
    Mmu *mmu;
    uint16_t addr;
    QGraphicsRectItem ***q;

protected:
    void run();

};


class VramViewer : public QWidget
{
    Q_OBJECT

    QColor white = QColor(255,255,255);
    QColor black = QColor(0,0,0);

public:
    VramViewer(QWidget *parent = nullptr);
    VramViewer(Mmu *pmmu, QWidget *parent);
    ~VramViewer();
    void update();

private:
    Ui::VramViewer *ui;
    Mmu *mmu;
    QPainter *painter;
    QGraphicsScene *scene;
    QGraphicsView *view;
    QGraphicsRectItem *pixel;
    //QGraphicsRectItem ***grid;
    QGraphicsRectItem ***q;//[0x40][0x80];
    drawerThread *drawer;

public slots:
    void onMmuWritten(uint16_t addr);
};

#endif // VRAMVIEWER_H
