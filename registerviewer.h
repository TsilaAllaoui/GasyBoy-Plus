#ifndef REGISTERVIEWER_H
#define REGISTERVIEWER_H

#include <QWidget>
#include "defs.h"

class Cpu;
class Mmu;

namespace Ui {
class RegisterViewer;
}

class RegisterViewer : public QWidget
{
    Q_OBJECT

public:
    RegisterViewer(QWidget *parent = nullptr);
    RegisterViewer(Cpu *pcpu, Mmu *pmmu, QWidget *parent = nullptr);
    ~RegisterViewer();
    void update();

private:
    Ui::RegisterViewer *ui;
    Cpu *cpu;
    Mmu *mmu;
    bool loop;

public slots:
    void onCpuStepped();
};

#endif // REGISTERVIEWER_H
