#ifndef MEMORYVIEWER_H
#define MEMORYVIEWER_H

#include <QWidget>
#include <QTableView>
#include <QComboBox>
#include <QThread>
#include "defs.h"

class Mmu;

namespace Ui {
class MemoryViewer;
}

class memoryDrawerThread : public QThread
{
    Q_OBJECT

public:
    memoryDrawerThread(Mmu *pmmu);
    ~memoryDrawerThread(){};
    void setAddr(uint16_t value);

private:
    Mmu *mmu;
    int divide;

protected:
    void run();

};

class MemoryViewer : public QWidget
{
    Q_OBJECT

public:
    MemoryViewer(QWidget *parent = nullptr);
    MemoryViewer(Mmu *pmmu, QWidget *parent = nullptr);
    ~MemoryViewer();
    void setView();

public slots:
    void on_comboBox_currentIndexChanged(const int value);
    void onMmuWritten(uint16_t);

private:
    memoryDrawerThread *drawer;
    Ui::MemoryViewer *ui;
    Mmu *mmu;
    int divide;
};

#endif // MEMORYVIEWER_H
