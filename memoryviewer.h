#ifndef MEMORYVIEWER_H
#define MEMORYVIEWER_H

#include <QWidget>
#include "defs.h"

class Mmu;

namespace Ui {
class MemoryViewer;
}

class MemoryViewer : public QWidget
{
    Q_OBJECT

public:
    MemoryViewer(QWidget *parent = nullptr);
    MemoryViewer(Mmu *pmmu, QWidget *parent = nullptr);
    ~MemoryViewer();
    void update();

private slots:
    void on_comboBox_currentIndexChanged(const int value);

private:
    Ui::MemoryViewer *ui;
    Mmu *mmu;
    int divide;
};

#endif // MEMORYVIEWER_H
